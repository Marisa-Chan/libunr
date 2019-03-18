/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU Affero General Public License as           *|
|*  published by the Free Software Foundation, either version 3 of the       *|
|*  License, or (at your option) any later version.                          *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU Affero General Public License for more details.                      *|
|*                                                                           *|
|*  You should have received a copy of the GNU Affero General Public License *|
|*  along with this program.  If not, see <https://www.gnu.org/licenses/>.   *|
\*===========================================================================*/

/*========================================================================
 * UTexture.cpp - Native texture object type
 * See the 'Class Texture' in UT-Package-File-Format.pdf
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

//#include "FLog.h"
#include "Core/UMusic.h"
#include "Core/UPackage.h"
#include "Core/UProperty.h"
#include "Core/UTexture.h"

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

// TODO: Export textures besides TEXF_P8 (S3TC, etc etc)
// TODO: Export to formats besides .bmp
bool UTexture::ExportToFile( const char* Dir, const char* Type )
{
  String* Filename = new String( Dir );
#if defined LIBUNR_WIN32
  Filename->ReplaceChars( '\\', '/' );
#endif
  if ( Filename->Back() != '/' )
    Filename->Append( "/" );

  Filename->Append( Name );
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
    FontTexture->Characters = new Array<FFontCharInfo>();
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

UScriptedTexture::UScriptedTexture()
  : UTexture()
{
}

UScriptedTexture::~UScriptedTexture()
{
}

UShadowBitMap::UShadowBitMap()
  : UTexture()
{
}

UShadowBitMap::~UShadowBitMap()
{
}

IMPLEMENT_NATIVE_CLASS( UPalette );
IMPLEMENT_NATIVE_CLASS( UBitmap );
IMPLEMENT_NATIVE_CLASS( UFont );
IMPLEMENT_NATIVE_CLASS( UTexture );
IMPLEMENT_NATIVE_CLASS( UScriptedTexture );
IMPLEMENT_NATIVE_CLASS( UShadowBitMap );

BEGIN_PROPERTY_LINK( UPalette, 1 )
  LINK_NATIVE_ARRAY( UPalette, Colors ); 
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UBitmap, 11 )
  LINK_NATIVE_PROPERTY( UBitmap, Format );
  LINK_NATIVE_PROPERTY( UBitmap, Palette );
  LINK_NATIVE_PROPERTY( UBitmap, UBits );
  LINK_NATIVE_PROPERTY( UBitmap, VBits );
  LINK_NATIVE_PROPERTY( UBitmap, USize );
  LINK_NATIVE_PROPERTY( UBitmap, VSize );
  LINK_NATIVE_PROPERTY( UBitmap, UClamp );
  LINK_NATIVE_PROPERTY( UBitmap, VClamp );
  LINK_NATIVE_PROPERTY( UBitmap, MipZero );
  LINK_NATIVE_PROPERTY( UBitmap, MaxColor );
  LINK_NATIVE_ARRAY( UBitmap, InternalTime );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UTexture, 67 )
  LINK_NATIVE_PROPERTY( UTexture, BumpMap );
  LINK_NATIVE_PROPERTY( UTexture, DetailTexture );
  LINK_NATIVE_PROPERTY( UTexture, MacroTexture );
  LINK_NATIVE_PROPERTY( UTexture, Diffuse );
  LINK_NATIVE_PROPERTY( UTexture, Specular );
  LINK_NATIVE_PROPERTY( UTexture, Alpha );
  LINK_NATIVE_PROPERTY( UTexture, DrawScale );
  LINK_NATIVE_PROPERTY( UTexture, Friction );
  LINK_NATIVE_PROPERTY( UTexture, MipMult );
  LINK_NATIVE_ARRAY   ( UTexture, FootstepSound );
  LINK_NATIVE_PROPERTY( UTexture, HitSound );
  LINK_NATIVE_PROPERTY( UTexture, bInvisible );
  LINK_NATIVE_PROPERTY( UTexture, bMasked );
  LINK_NATIVE_PROPERTY( UTexture, bTransparent );
  LINK_NATIVE_PROPERTY( UTexture, bNotSolid );
  LINK_NATIVE_PROPERTY( UTexture, bEnvironment );
  LINK_NATIVE_PROPERTY( UTexture, bSemisolid );
  LINK_NATIVE_PROPERTY( UTexture, bModulate );
  LINK_NATIVE_PROPERTY( UTexture, bFakeBackdrop );
  LINK_NATIVE_PROPERTY( UTexture, bTwoSided );
  LINK_NATIVE_PROPERTY( UTexture, bAutoUPan );
  LINK_NATIVE_PROPERTY( UTexture, bAutoVPan );
  LINK_NATIVE_PROPERTY( UTexture, bNoSmooth );
  LINK_NATIVE_PROPERTY( UTexture, bBigWavy );
  LINK_NATIVE_PROPERTY( UTexture, bSmallWavy );
  LINK_NATIVE_PROPERTY( UTexture, bWaterWavy );
  LINK_NATIVE_PROPERTY( UTexture, bLowShadowDetail );
  LINK_NATIVE_PROPERTY( UTexture, bNoMerge );
  LINK_NATIVE_PROPERTY( UTexture, bAlphaBlend );
  LINK_NATIVE_PROPERTY( UTexture, bUseHeightMap );
  LINK_NATIVE_PROPERTY( UTexture, bDirtyShadows );
  LINK_NATIVE_PROPERTY( UTexture, bHighLedge );
  LINK_NATIVE_PROPERTY( UTexture, bSpecialLit );
  LINK_NATIVE_PROPERTY( UTexture, bGouraud );
  LINK_NATIVE_PROPERTY( UTexture, bUnlit );
  LINK_NATIVE_PROPERTY( UTexture, bHighShadowDetail );
  LINK_NATIVE_PROPERTY( UTexture, bPortal );
  LINK_NATIVE_PROPERTY( UTexture, bMirrored );
  LINK_NATIVE_PROPERTY( UTexture, bX2 );
  LINK_NATIVE_PROPERTY( UTexture, bX3 );
  LINK_NATIVE_PROPERTY( UTexture, bX4 );
  LINK_NATIVE_PROPERTY( UTexture, bX5 );
  LINK_NATIVE_PROPERTY( UTexture, bX6 );
  LINK_NATIVE_PROPERTY( UTexture, bX7 );
  LINK_NATIVE_PROPERTY( UTexture, bHighColorQuality );
  LINK_NATIVE_PROPERTY( UTexture, bHighTextureQuality );
  LINK_NATIVE_PROPERTY( UTexture, bRealtime );
  LINK_NATIVE_PROPERTY( UTexture, bParametric );
  LINK_NATIVE_PROPERTY( UTexture, bRealtimeChanged );
  LINK_NATIVE_PROPERTY( UTexture, bHasComp );
  LINK_NATIVE_PROPERTY( UTexture, bFractical );
  LINK_NATIVE_PROPERTY( UTexture, LODSet );
  LINK_NATIVE_PROPERTY( UTexture, AnimNext );
  LINK_NATIVE_PROPERTY( UTexture, AnimCurrent );
  LINK_NATIVE_PROPERTY( UTexture, PrimeCount );
  LINK_NATIVE_PROPERTY( UTexture, PrimeCurrent );
  LINK_NATIVE_PROPERTY( UTexture, MinFrameRate );
  LINK_NATIVE_PROPERTY( UTexture, MaxFrameRate );
  LINK_NATIVE_PROPERTY( UTexture, Accumulator );
  LINK_NATIVE_PROPERTY( UTexture, Mips );
  LINK_NATIVE_PROPERTY( UTexture, DecompMips );
  LINK_NATIVE_PROPERTY( UTexture, DecompFormat );
  LINK_NATIVE_PROPERTY( UTexture, SurfaceType );
  LINK_NATIVE_PROPERTY( UTexture, UClampMode );
  LINK_NATIVE_PROPERTY( UTexture, VClampMode );
  LINK_NATIVE_PROPERTY( UTexture, PaletteTransform );
  LINK_NATIVE_PROPERTY( UTexture, TextureHandle );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UScriptedTexture, 16 )
  LINK_NATIVE_PROPERTY( UScriptedTexture, bUse32BitFormat );
  LINK_NATIVE_PROPERTY( UScriptedTexture, bConstantRender );
  LINK_NATIVE_PROPERTY( UScriptedTexture, bClearScreen );
  LINK_NATIVE_PROPERTY( UScriptedTexture, bDrawFrame );
  LINK_NATIVE_PROPERTY( UScriptedTexture, DrawStyle );
  LINK_NATIVE_PROPERTY( UScriptedTexture, DrawOpacity );
  LINK_NATIVE_PROPERTY( UScriptedTexture, NotifyActor );
  LINK_NATIVE_PROPERTY( UScriptedTexture, SourceTexture );
  LINK_NATIVE_PROPERTY( UScriptedTexture, LocalSourceBitmap );
  LINK_NATIVE_PROPERTY( UScriptedTexture, PaletteMap );
  LINK_NATIVE_PROPERTY( UScriptedTexture, LocalTime );
  LINK_NATIVE_PROPERTY( UScriptedTexture, PortalPtr );
  LINK_NATIVE_PROPERTY( UScriptedTexture, PortalInfo );
  LINK_NATIVE_PROPERTY( UScriptedTexture, Junk1 );
  LINK_NATIVE_PROPERTY( UScriptedTexture, Junk2 );
  LINK_NATIVE_PROPERTY( UScriptedTexture, Junk3 );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UShadowBitMap, 7 )
  LINK_NATIVE_PROPERTY( UShadowBitMap, ProjectingActor );
  LINK_NATIVE_PROPERTY( UShadowBitMap, ProjectDirection );
  LINK_NATIVE_PROPERTY( UShadowBitMap, Gradience );
  LINK_NATIVE_PROPERTY( UShadowBitMap, Softness );
  LINK_NATIVE_PROPERTY( UShadowBitMap, StaticLevel );
  LINK_NATIVE_PROPERTY( UShadowBitMap, ShadowScale );
  LINK_NATIVE_PROPERTY( UShadowBitMap, bIsValid );
END_PROPERTY_LINK()

