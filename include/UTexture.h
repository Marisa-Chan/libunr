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
 * UTexture.h - Native texture object type
 * See the 'Class Texture' and 'Class Palette' in UT-Package-File-Format.pdf
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#ifndef __UTEXTURE_H__
#define __UTEXTURE_H__

#include "FMemory.h"
#include "UObject.h"

enum ETextureFormat
{
  TEXF_P8,
  TEXF_RGB32,
  TEXF_DXT1,
  TEXF_RGB24,
  TEXF_RGBA8,
  TEXF_DXT3,
  TEXF_DXT5,
};

class FColor
{
public:
  u32 Data;
  
  friend FArchive& operator<< ( FArchive& Ar, FColor& Color )
  {
    u8 Channels[4];
    Ar >> Channels[0];
    Ar >> Channels[1];
    Ar >> Channels[2];
    Ar >> Channels[3];
    
    #ifdef LIBUNR_BIG_ENDIAN
      Color.Data = (Channels[3] << 24) | (Channels[2] << 16) | (Channels[1] << 8) | Channels[0];
    #else
      Color.Data = (Channels[0] << 24) | (Channels[1] << 16) | (Channels[2] << 8) | Channels[3];
    #endif
    
    return Ar;
  }
};

class FMipmap
{
public:
  int USize, VSize;
  TArray<u8> DataArray;
  
  FMipmap() 
  {}
  
  FMipmap( int InUSize, int InVSize )
  : USize( InUSize ), VSize( InVSize )
  , DataArray( InUSize * InVSize, 0 )
  {}
  
  void Clear()
  {
    FMemory::Set( DataArray.Data(), 0, USize * VSize );
  }
};

class UPalette : public UObject
{
  DECLARE_CLASS( UPalette, UObject, CLASS_SafeReplace )
  
  UPalette();
  
  TArray<FColor> Colors;
};

class UBitmap : public UObject
{
  DECLARE_CLASS( UBitmap, UObject, CLASS_SafeReplace )
  
  ETextureFormat Format;
  u8  UBits,  VBits;
  u32 USize,  VSize;
  u32 UClamp, VClamp;
  FColor MipZero;
  FColor MaxColor;
  int InternalTime[2];
  
  UBitmap();
};

class UTexture : public UBitmap
{
  DECLARE_CLASS( UTexture, UBitmap, CLASS_SafeReplace )
  
  UTexture* BumpMap;
  UTexture* DetailTexture;
  UTexture* MacroTexture;
  
  float Diffuse;
  float Specular;
  float Alpha;
  float DrawScale;
  float Friction;
  float MipMult;
  
  // USound* FootstepSound[6]
  // USound* HitSound;
  
  bool bHighColorQuality;
  bool bHighTextureQuality;
  bool bRealtime;
  bool bParametric;
  bool bRealtimeChanged;
  bool bHasComp;
  bool bFractical;
  
  UTexture* AnimNext;
  UTexture* AnimCurrent;
  u8 PrimeCount;
  u8 PrimeCurrent;
  float MinFrameRate, MaxFrameRate;
  float Accumulator;
  
  TArray<FMipmap> Mips;
  TArray<FMipmap> CompMips;
  ETextureFormat  CompFormat;
  
  UTexture();
  virtual bool ExportToFile();
  virtual void LoadFromPackage( FArchive& Ar );

  static UClass* ConstructNativeClass( u32 Flags );
};

#endif 

