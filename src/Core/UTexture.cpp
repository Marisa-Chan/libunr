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
  FString Filename( Dir );
#if defined LIBUNR_WIN32
  Filename.ReplaceChars( '\\', '/' );
#endif
  if ( Filename.Back() != '/' )
    Filename += '/';

  Filename += Name;
  Filename += ".";
  if ( strnicmp( Type, "bmp", 3 ) != 0 )
  {
    Logf( LOG_WARN, "Can't export texture to file type '%s'", Type );
    return false;
  }
  Filename += Type;
  
  FileStreamOut* Out = new FileStreamOut();
  if ( Out->Open( Filename ) != 0 )
  {
    Logf( LOG_WARN, "Failed to export texture to bitmap file '%s'", Filename.Data() );
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
  LINK_NATIVE_ARRAY( Colors ); 
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UBitmap, 11 )
  LINK_NATIVE_PROPERTY( Format );
  LINK_NATIVE_PROPERTY( Palette );
  LINK_NATIVE_PROPERTY( UBits );
  LINK_NATIVE_PROPERTY( VBits );
  LINK_NATIVE_PROPERTY( USize );
  LINK_NATIVE_PROPERTY( VSize );
  LINK_NATIVE_PROPERTY( UClamp );
  LINK_NATIVE_PROPERTY( VClamp );
  LINK_NATIVE_PROPERTY( MipZero );
  LINK_NATIVE_PROPERTY( MaxColor );
  LINK_NATIVE_ARRAY( InternalTime );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UTexture, 70 )
  LINK_NATIVE_PROPERTY( BumpMap );
  LINK_NATIVE_PROPERTY( DetailTexture );
  LINK_NATIVE_PROPERTY( MacroTexture );
  LINK_NATIVE_PROPERTY( Diffuse );
  LINK_NATIVE_PROPERTY( Specular );
  LINK_NATIVE_PROPERTY( Alpha );
  LINK_NATIVE_PROPERTY( DrawScale );
  LINK_NATIVE_PROPERTY( Friction );
  LINK_NATIVE_PROPERTY( MipMult );
  LINK_NATIVE_ARRAY   ( FootstepSound );
  LINK_NATIVE_PROPERTY( HitSound );
  LINK_NATIVE_PROPERTY( bInvisible );
  LINK_NATIVE_PROPERTY( bMasked );
  LINK_NATIVE_PROPERTY( bTransparent );
  LINK_NATIVE_PROPERTY( bNotSolid );
  LINK_NATIVE_PROPERTY( bEnvironment );
  LINK_NATIVE_PROPERTY( bSemisolid );
  LINK_NATIVE_PROPERTY( bModulate );
  LINK_NATIVE_PROPERTY( bFakeBackdrop );
  LINK_NATIVE_PROPERTY( bTwoSided );
  LINK_NATIVE_PROPERTY( bAutoUPan );
  LINK_NATIVE_PROPERTY( bAutoVPan );
  LINK_NATIVE_PROPERTY( bNoSmooth );
  LINK_NATIVE_PROPERTY( bBigWavy );
  LINK_NATIVE_PROPERTY( bSmallWavy );
  LINK_NATIVE_PROPERTY( bWaterWavy );
  LINK_NATIVE_PROPERTY( bCloudWavy );
  LINK_NATIVE_PROPERTY( bLowShadowDetail );
  LINK_NATIVE_PROPERTY( bNoMerge );
  LINK_NATIVE_PROPERTY( bAlphaBlend );
  LINK_NATIVE_PROPERTY( bUseHeightMap );
  LINK_NATIVE_PROPERTY( bDirtyShadows );
  LINK_NATIVE_PROPERTY( bHighLedge );
  LINK_NATIVE_PROPERTY( bSpecialLit );
  LINK_NATIVE_PROPERTY( bGouraud );
  LINK_NATIVE_PROPERTY( bUnlit );
  LINK_NATIVE_PROPERTY( bHighShadowDetail );
  LINK_NATIVE_PROPERTY( bPortal );
  LINK_NATIVE_PROPERTY( bMirrored );
  LINK_NATIVE_PROPERTY( bX2 );
  LINK_NATIVE_PROPERTY( bX3 );
  LINK_NATIVE_PROPERTY( bX4 );
  LINK_NATIVE_PROPERTY( bX5 );
  LINK_NATIVE_PROPERTY( bX6 );
  LINK_NATIVE_PROPERTY( bX7 );
  LINK_NATIVE_PROPERTY( bHighColorQuality );
  LINK_NATIVE_PROPERTY( bHighTextureQuality );
  LINK_NATIVE_PROPERTY( bRealtime );
  LINK_NATIVE_PROPERTY( bParametric );
  LINK_NATIVE_PROPERTY( bRealtimeChanged );
  LINK_NATIVE_PROPERTY( bHasComp );
  LINK_NATIVE_PROPERTY( bFractical );
  LINK_NATIVE_PROPERTY( LODSet );
  LINK_NATIVE_PROPERTY( AnimNext );
  LINK_NATIVE_PROPERTY( AnimCurrent );
  LINK_NATIVE_PROPERTY( PrimeCount );
  LINK_NATIVE_PROPERTY( PrimeCurrent );
  LINK_NATIVE_PROPERTY( MinFrameRate );
  LINK_NATIVE_PROPERTY( MaxFrameRate );
  LINK_NATIVE_PROPERTY( Accumulator );
  LINK_NATIVE_PROPERTY( Mips );
  LINK_NATIVE_PROPERTY( DecompMips );
  LINK_NATIVE_PROPERTY( DecompFormat );
  LINK_NATIVE_PROPERTY( SurfaceType );
  LINK_NATIVE_PROPERTY( UClampMode );
  LINK_NATIVE_PROPERTY( VClampMode );
  LINK_NATIVE_PROPERTY( PaletteTransform );
  LINK_NATIVE_PROPERTY( TextureHandle );
  LINK_NATIVE_PROPERTY_ALIASED( CompMips, DecompMips );
  LINK_NATIVE_PROPERTY_ALIASED( CompFormat, DecompFormat );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UScriptedTexture, 16 )
  LINK_NATIVE_PROPERTY( bUse32BitFormat );
  LINK_NATIVE_PROPERTY( bConstantRender );
  LINK_NATIVE_PROPERTY( bClearScreen );
  LINK_NATIVE_PROPERTY( bDrawFrame );
  LINK_NATIVE_PROPERTY( DrawStyle );
  LINK_NATIVE_PROPERTY( DrawOpacity );
  LINK_NATIVE_PROPERTY( NotifyActor );
  LINK_NATIVE_PROPERTY( SourceTexture );
  LINK_NATIVE_PROPERTY( LocalSourceBitmap );
  LINK_NATIVE_PROPERTY( PaletteMap );
  LINK_NATIVE_PROPERTY( LocalTime );
  LINK_NATIVE_PROPERTY( PortalPtr );
  LINK_NATIVE_PROPERTY( PortalInfo );
  LINK_USELESS_PROPERTY( Junk1 );
  LINK_USELESS_PROPERTY( Junk2 );
  LINK_USELESS_PROPERTY( Junk3 );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UShadowBitMap, 7 )
  LINK_NATIVE_PROPERTY( ProjectingActor );
  LINK_NATIVE_PROPERTY( ProjectDirection );
  LINK_NATIVE_PROPERTY( Gradience );
  LINK_NATIVE_PROPERTY( Softness );
  LINK_NATIVE_PROPERTY( StaticLevel );
  LINK_NATIVE_PROPERTY( ShadowScale );
  LINK_NATIVE_PROPERTY( bIsValid );
END_PROPERTY_LINK()

