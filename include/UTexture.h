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

#include "Memory.h"
#include "UObject.h"
#include "UPackage.h"
#include "USound.h"

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
  union 
  {
    struct
    {
      u8 R, G, B, A;
    };
    
    u32 Data;
  };
  
  friend FPackageFileIn& operator>>( FPackageFileIn& In, FColor& Color )
  {
    In >> Color.R;
    In >> Color.G;
    In >> Color.B;
    In >> Color.A;
  
    return In;
  }
};

class FMipmap
{
public:  
  FMipmap() 
  {}
  
  FMipmap( int InUSize, int InVSize )
  : USize( InUSize ), VSize( InVSize )
  , DataArray( (size_t)(InUSize * InVSize), 0 )
  {}
  
  void Clear()
  {
    xstl::Set( DataArray.Data(), 0, USize * VSize );
  }
  
  int USize, VSize;
  Array<u8> DataArray;
};

class UPalette : public UObject
{
  DECLARE_CLASS( UPalette, UObject, CLASS_SafeReplace, Engine )
  
  UPalette();
  virtual void LoadFromPackage( FPackageFileIn& Ar );
  
  FColor Colors[256];
};

class UBitmap : public UObject
{
  DECLARE_CLASS( UBitmap, UObject, CLASS_SafeReplace, Engine )
  
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
  DECLARE_CLASS( UTexture, UBitmap, CLASS_SafeReplace, Engine )
  UTexture();

  UTexture* BumpMap;
  UTexture* DetailTexture;
  UTexture* MacroTexture;
  UPalette* Palette;
  
  float Diffuse;
  float Specular;
  float Alpha;
  float DrawScale;
  float Friction;
  float MipMult;
  
  USound* FootstepSound[6];
  USound* HitSound;
  
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
  
  Array<FMipmap> Mips;
  Array<FMipmap> CompMips;
  ETextureFormat CompFormat;
  
  virtual bool ExportToFile();
  virtual void LoadFromPackage( FPackageFileIn& Ar );
};

#endif 

