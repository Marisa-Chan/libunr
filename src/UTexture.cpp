/*========================================================================*\
|*  This file is part of libunr.                                          *|
|*                                                                        *|
|*  libunr is free software: you can redistribute it and/or modify        *|
|*  it under the terms of the GNU General Public License as published by  *|
|*  the Free Software Foundation, either version 3 of the License, or     *|
|*  (at your option) any later version.                                   *|
|*                                                                        *|
|*  libunr is distributed in the hope that it will be useful,             *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of        *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *|
|*  GNU General Public License for more details.                          *|
|*                                                                        *|
|*  You should have received a copy of the GNU General Public License     *|
|*  along with libunr.  If not, see <http://www.gnu.org/licenses/>.       *|
|*                                                                        *|
\*========================================================================*/

/*========================================================================
 * UTexture.cpp - Native texture object type
 * See the 'Class Texture' in UT-Package-File-Format.pdf
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

//#include "FLog.h"
#include "UMusic.h"
#include "UPackage.h"
#include "UProperty.h"
#include "UTexture.h"

UPalette::UPalette()
  : UObject()
{
}

UPalette::~UPalette()
{
}

void UPalette::Load()
{
  ReadDefaultProperties();

  idx PaletteSize;
  *PkgFile >> CINDEX( PaletteSize );
  
  if ( PaletteSize != 256 )
    Logf( LOG_WARN, "Palette does not have 256 entries!" );
  
  // Load colors
  for (int i = 0; i < PaletteSize; i++) {
    *PkgFile >> Colors[i];
  }
}

UBitmap::UBitmap()
  : UObject()
{
  Format = TEXF_P8;
  UBits = 0;
  VBits = 0;
  USize = 0;
  VSize = 0;
  UClamp = 0;
  VClamp = 0;
  InternalTime[0] = 0;
  InternalTime[1] = 0;
}

UBitmap::~UBitmap()
{
}

void UBitmap::Load()
{
}

UTexture::UTexture()
  : UBitmap()
{
  BumpMap = NULL;
  DetailTexture = NULL;
  MacroTexture = NULL;
  
  Diffuse   = 1.f;
  Specular  = 0.f;
  Alpha     = 0.f;
  DrawScale = 1.f;
  Friction  = 0.f;
  MipMult   = 1.f;
  
  // USound* FootstepSound[6]
  // USound* HitSound;
  
  bHighColorQuality = true;
  bHighTextureQuality = true;
  bRealtime = false;
  bParametric = false;
  bRealtimeChanged = false;
  bHasComp = false;
  bFractical = false;
  
  AnimNext = NULL;
  AnimCurrent = NULL;
  PrimeCount = 0;
  PrimeCurrent = 0;
  MinFrameRate = 0.f;
  MaxFrameRate = 0.f;
  Accumulator = 0.f;
  
  CompFormat = TEXF_P8;
}

UTexture::~UTexture()
{
}

void UTexture::Load()
{
  ReadDefaultProperties();

  u8 MipCount = 0;
  *PkgFile >> MipCount;
  Mips.Resize( MipCount );

  for ( int i = 0; i < MipCount; i++ )
  {
    u32 WidthOffset = 0;
    idx MipMapSize;

    // Get the width offset if we need it (but I never see it used?)
    if ( PkgFile->Ver >= PKG_VER_UN_220 )
      *PkgFile >> WidthOffset;

    // Read mip map data
    *PkgFile >> CINDEX( MipMapSize );
    Mips[i].DataArray.Resize( MipMapSize );
    PkgFile->Read( Mips[i].DataArray.Data(), MipMapSize );

    // Read in width and height data
    *PkgFile >> Mips[i].USize;
    *PkgFile >> Mips[i].VSize;

    // Unused width and height in bits
    u8 Unused[2];
    *PkgFile >> Unused[0];
    *PkgFile >> Unused[1];
  }
}

// TODO: Export textures besides TEXF_P8
// TODO: Export to formats besides .bmp
bool UTexture::ExportToFile( const char* Dir, const char* Type )
{
  String* Filename = new String( Dir );
#if defined LIBUNR_WIN32
  Filename->ReplaceChars( '\\', '/' );
#endif
  if ( Filename->Back() != '/' )
    Filename->Append( "/" );

  Filename->Append( Pkg->ResolveNameFromIdx( NameIdx ) );
  Filename->Append( "." );
  if ( strnicmp( Type, "bmp", 3 ) != 0 )
  {
    Logf( LOG_WARN, "Can't export texture to file type '%s'", Type );
    return false;
  }
  Filename->Append( Type );
  
  FileStreamOut* Out = new FileStreamOut();
  if ( Out->Open( *Filename ) != 0 )
  {
    Logf( LOG_WARN, "Failed to export texture to bitmap file '%s'", Filename->Data() );
    return false;
  }
  
  u32 Zero32 = 0; 
  u32 bfSize = 0;
  u32 bfOffBits = 0;

  // Write BMP header
  Out->Write( (char*)"BM", 2 ); // magic number
  *Out << Zero32;        // come back to write the file size later
  *Out << Zero32;        // reserved
  *Out << Zero32;        // come back to write the pixel data
  
  // 8-bit paletted textures (standard format for UE1 games)
  if ( Format == TEXF_P8 )
  {
    u32 biSize = 40;
    u16 biPlanes = 1;
    u16 biBitCount = 8;

    *Out << biSize;     // biSize
    *Out << USize;      // biWidth
    *Out << VSize;      // biHeight
    *Out << biPlanes;   // biPlanes
    *Out << biBitCount; // biBitCount
    *Out << Zero32;     // biCompression
    *Out << Zero32;     // biSizeImage
    *Out << Zero32;     // biXPelsPerMeter
    *Out << Zero32;     // biYPelsPerMeter
    *Out << Zero32;     // biClrUsed
    *Out << Zero32;     // biClrImportant
    
    // Write color table
    u8 Alpha = 0;
    for ( int i = 0; i < 256; i++ )
    {
      *Out << Palette->Colors[i].B;
      *Out << Palette->Colors[i].G;
      *Out << Palette->Colors[i].R;
      *Out << Alpha;
    }
    
    bfOffBits = Out->Tell();
    
    // Just export the first mipmap since that's probably what the user is going to want anyway
    // if there's some need for exporting choice mipmaps, implement it later
    u8* Bitmap = Mips[0].DataArray.Data();

    // We need to write each row from bottom to top
    for ( int i = VSize-1; i >= 0; i-- )
      for ( int j = 0; j < USize; j++ ) 
        *Out << Bitmap[(i*USize) + j];
    
    bfSize = Out->Tell();
    
    // Go back and write header info
    Out->Seek( 2, Begin );
    *Out << bfSize;
    
    Out->Seek( 10, Begin );
    *Out << bfOffBits;
    
    Out->Close();  
    return true;
  }
}

UFont::UFont()
  : UObject()
{
  FontTextures = new Array<FFontTexture>();
  FontTextures->Reserve( 1 ); // there's not even a way to access subfonts???
}

UFont::~UFont()
{
  delete FontTextures;
}

void UFont::Load()
{
  ReadDefaultProperties();

  u8 TextureCount = MAX_UINT8;
  *PkgFile >> TextureCount;
  FontTextures->Reserve( TextureCount );

  for ( int i = 0; i < TextureCount; i++ )
  {
    FFontTexture* FontTexture = &(*FontTextures)[i];

    idx TextureIdx = MAX_UINT32;
    idx CharCount  = MAX_UINT32;

    *PkgFile >> CINDEX( TextureIdx );
    *PkgFile >> CINDEX( CharCount );

    FontTexture->Texture = (UTexture*)LoadObject( TextureIdx, UTexture::StaticClass(), this );
    FontTexture->Characters->Reserve( CharCount );

    for ( int j = 0; j < CharCount; j++ )
    {
      FFontCharInfo* Character = &(*FontTexture->Characters)[j];
      *PkgFile >> Character->X;
      *PkgFile >> Character->Y;
      *PkgFile >> Character->Width;
      *PkgFile >> Character->Height;
    }
  }
}

