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

#pragma once

#include "XMemory.h"
#include "UObject.h"
#include "UPackage.h"
#include "USound.h"

// Order is dictated by Engine.Bitmap.ETextureFormat
// if this order is broken for some reason, that game
// will probably have strange texture format issues
enum ETextureFormat
{
  TEXF_P8,
  TEXF_RGB32,
  TEXF_RGB64,
  TEXF_DXT1,
  TEXF_RGB24,
  TEXF_RGBA8,
  TEXF_DXT3,
  TEXF_DXT5,
};

class DLL_EXPORT FColor
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

class DLL_EXPORT FMipmap
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

class DLL_EXPORT UPalette : public UObject
{
  DECLARE_NATIVE_CLASS( UPalette, UObject, CLASS_SafeReplace, Engine )
  EXPOSE_TO_USCRIPT()

  UPalette();
  virtual void Load();
  
  FColor Colors[256];
};

class DLL_EXPORT UBitmap : public UObject
{
  DECLARE_NATIVE_CLASS( UBitmap, UObject, CLASS_SafeReplace, Engine )
  EXPOSE_TO_USCRIPT()
  UBitmap(); 

  ETextureFormat Format;
  UPalette* Palette;
  u8  UBits,  VBits;
  u32 USize,  VSize;
  u32 UClamp, VClamp;
  FColor MipZero;
  FColor MaxColor;
  int InternalTime[2];

  virtual void Load();
};

enum ELODSet
{
  LODSET_None,
  LODSET_World,
  LODSET_Skin,
};

enum ESurfaceType
{
  EST_Default,
  EST_Rock,
  EST_Dirt, // Footprints
  EST_Metal,
  EST_Wood,
  EST_Plant,
  EST_Flesh,
  EST_Ice,
  EST_Snow, // Footprints
  EST_Water,
  EST_Glass,
  EST_Carpet,
  EST_Custom00,
  EST_Custom01,
  EST_Custom02,
  EST_Custom03,
  EST_Custom04,
  EST_Custom05,
  EST_Custom06, // Footprints
  EST_Custom07, // Footprints
  EST_Custom08, // Footprints
  EST_Custom09, // Footprints
  EST_Custom10 // Footprints
};

enum EUClampMode
{
  UWrap,
  UClamp,
};

enum EVClampMode
{
  VWrap,
  VClamp,
};

class DLL_EXPORT UTexture : public UBitmap
{
  DECLARE_NATIVE_CLASS( UTexture, UBitmap, CLASS_SafeReplace, Engine )
  EXPOSE_TO_USCRIPT()
  EXPORTABLE()
  UTexture();

  UTexture* BumpMap;
  UTexture* DetailTexture;
  UTexture* MacroTexture;
  
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

  ELODSet LODSet;
  UTexture* AnimNext;
  UTexture* AnimCurrent;
  u8 PrimeCount;
  u8 PrimeCurrent;
  float MinFrameRate, MaxFrameRate;
  float Accumulator;
  
  Array<FMipmap> Mips;
  Array<FMipmap> CompMips;
  ETextureFormat CompFormat;

  ESurfaceType SurfaceType;
  EUClampMode  UClampMode;
  EVClampMode  VClampMode;

  FColor PaletteTransform;
  
  virtual void Load();
};

