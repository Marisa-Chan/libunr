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

void UPalette::LoadFromPackage( FPackageFileIn* In )
{
  // Read the none property (this will never have any other property types)
  idx None;
  *In >> CINDEX( None );
  
  idx PaletteSize;
  *In >> CINDEX( PaletteSize );
  
  if ( PaletteSize != 256 )
    Logf( LOG_WARN, "Palette does not have 256 entries!" );
  
  // Load colors
  for (int i = 0; i < PaletteSize; i++) {
    *In >> Colors[i];
  }
}

bool UPalette::StaticLinkNativeProperties()
{
  if ( StaticInitNativePropList( 1 ) )
  {
    LINK_NATIVE_ARRAY( UPalette, Colors ); 
    return true;
  }

  return false;
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

void UBitmap::LoadFromPackage( FPackageFileIn* In )
{
}

bool UBitmap::StaticLinkNativeProperties()
{
  FNativeProp

  if ( StaticInitNativePropList( 11 ) )
  {
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
    return true;
  }

  return false;
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

void UTexture::LoadFromPackage( FPackageFileIn* In )
{
  ReadDefaultProperties( In );
}

// TODO: Export textures besides TEXF_P8
bool UTexture::ExportToFile()
{
/*
  String* Filename = new String( Pkg->ResolveNameFromIdx( NameIdx ) );
  Filename->Append( ".bmp" );
  
  FileStreamOut* Out = new FileStreamOut();
  Out->Open( *Filename );
  
  u32 bfSize = 0;
  u32 bfOffBits = 0;
  
  // Write BMP header
  Out->Write( (char*)"BM", 2 ); // magic number
  *Out << (u32)0;        // come back to write the file size later
  *Out << (u32)0;        // reserved
  *Out << (u32)0;        // come back to write the pixel data
  
  // 8-bit paletted textures (standard format for UE1 games)
  if ( Format == TEXF_P8 )
  {
    *Out << (u32)40; // biSize
    *Out << USize;   // biWidth
    *Out << VSize;   // biHeight
    *Out << (u16)1;  // biPlanes
    *Out << (u16)8;  // biBitCount
    *Out << (u32)0;  // biCompression
    *Out << (u32)0;  // biSizeImage
    *Out << (u32)0;  // biXPelsPerMeter
    *Out << (u32)0;  // biYPelsPerMeter
    *Out << (u32)0;  // biClrUsed
    *Out << (u32)0;  // biClrImportant
    
    // Write color table
    for (int i = 0; i < 256; i++)
    {
      *Out << Palette->Colors[i].B;
      *Out << Palette->Colors[i].G;
      *Out << Palette->Colors[i].R;
      *Out << (u8)0;
    }
    
    bfOffBits = Out->Tell();
    
    // Just export the first mipmap since that's probably what the user is going to want anyway
    // if there's some need for exporting choice mipmaps, implement it later
    u8* Bitmap = Mips[0].DataArray.Data();
    for (int i = 0; i < Mips[0].DataArray.Size(); i++)
      *Out << Bitmap[i];
    
    bfSize = Out->Tell();
    
    // Go back and write header info
    Out->Seek( 2, Begin );
    *Out << bfSize;
    
    Out->Seek( 8, Begin );
    *Out << bfOffBits;
    
    Out->Close();
*/  
    return true;
//	}

}

bool UTexture::StaticLinkNativeProperties()
{
  FNativePropertyList* SuperPropList = UBitmap::StaticNativePropList;
  if ( SuperPropList == NULL )
    return false;

  // Double check this
  // this is gonna get REAL out of hand with Engine.Actor...
  if ( StaticInitNativePropList( 32 + SuperPropList->Num ) )
  {
    xstl::Copy( StaticNativePropList->Properties, 
                SuperPropList->Properties,
                sizeof( FNativePropertyLink ) * SuperPropList->Num );
    StaticNativePropList->Added = SuperPropList->Added;
         
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
    LINK_NATIVE_PROPERTY( UTexture, bHighColorQuality );
    LINK_NATIVE_PROPERTY( UTexture, bHighTextureQuality );
    LINK_NATIVE_PROPERTY( UTexture, bRealtime );
    LINK_NATIVE_PROPERTY( UTexture, bParametric );
    LINK_NATIVE_PROPERTY( UTexture, bRealtimeChanged );
    LINK_NATIVE_PROPERTY( UTexture, bHasComp );
    LINK_NATIVE_PROPERTY( UTexture, bFractical );
    LINK_NATIVE_PROPERTY( UTexture, AnimNext );
    LINK_NATIVE_PROPERTY( UTexture, AnimCurrent );
    LINK_NATIVE_PROPERTY( UTexture, PrimeCount );
    LINK_NATIVE_PROPERTY( UTexture, PrimeCurrent );
    LINK_NATIVE_PROPERTY( UTexture, MinFrameRate );
    LINK_NATIVE_PROPERTY( UTexture, MaxFrameRate );
    LINK_NATIVE_PROPERTY( UTexture, Accumulator );
    LINK_NATIVE_PROPERTY( UTexture, Mips );
    LINK_NATIVE_PROPERTY( UTexture, CompMips );
    LINK_NATIVE_PROPERTY( UTexture, CompFormat );
    LINK_NATIVE_PROPERTY( UTexture, SurfaceType );
    LINK_NATIVE_PROPERTY( UTexture, UClampMode );
    LINK_NATIVE_PROPERTY( UTexture, VClampMode );
    LINK_NATIVE_PROPERTY( UTexture, PaletteTransform );
    return true;
  }
  return false;
}

