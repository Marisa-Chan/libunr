/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU General Public License as published by     *|
|*  the Free Software Foundation, either version 3 of the License, or        *|
|*  (at your option) any later version.                                      *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU General Public License for more details.                             *|
|*                                                                           *|
|*  You should have received a copy of the GNU General Public License        *|
|*  along with this program. If not, see <https://www.gnu.org/licenses/>.    *|
\*===========================================================================*/

/*========================================================================
 * UTexture.h - Native texture object type
 * See the following in in UT-Package-File-Format.pdf
 *   Class 'Texture'
 *   Class 'Palette'
 *   Class 'Font'
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/UObject.h"
#include "Core/UPackage.h"
#include "Core/UMath.h"
#include "Engine/USound.h"

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

// Rune matter type
enum EMatterType
{
  MATTER_NONE,
  MATTER_WOOD,
  MATTER_METAL,
  MATTER_STONE,
  MATTER_FLESH,
  MATTER_ICE,
  MATTER_WATER,
  MATTER_EARTH,
  MATTER_SNOW,
  MATTER_BREAKABLEWOOD,
  MATTER_BREAKABLESTONE,
  MATTER_SHIELD,
  MATTER_WEAPON,
  MATTER_MUD,
  MATTER_LAVA
};

class LIBUNR_API FColor
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

class LIBUNR_API FMipmap
{
public:  
  FMipmap() 
  {
    USize = 0;
    VSize = 0;
  }
  
  FMipmap( int InUSize, int InVSize )
  : USize( InUSize ), VSize( InVSize )
  , DataArray( ((size_t)InUSize * (size_t)InVSize), 0 )
  {}
  
  void Clear()
  {
    memset( DataArray.data(), 0, (size_t)USize * (size_t)VSize );
  }
  
  int USize, VSize;
  TArray<u8> DataArray;
};

class LIBUNR_API UPalette : public UObject
{
  DECLARE_NATIVE_CLASS( UPalette, UObject, CLASS_SafeReplace, Engine )
  EXPOSE_TO_USCRIPT()

  UPalette();
  virtual void Load();
  
  FColor Colors[256];
};

class LIBUNR_API UBitmap : public UObject
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

class LIBUNR_API UTexture : public UBitmap
{
  DECLARE_NATIVE_CLASS( UTexture, UBitmap, CLASS_SafeReplace, Engine )
  EXPOSE_TO_USCRIPT()
  UTexture();

  virtual void Tick( float DeltaTime );
  virtual FMipmap& GetMips( size_t MipLevel );

  virtual void Load();
  virtual bool ExportToFile( const char* Dir, const char* Type );
  //virtual bool ExportToBmp( const char* Dir );
  //virtual bool ExportToPcx( const char* Dir );
  //virtual bool ExportToPng( const char* Dir );

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

  // Surface flags
  bool bInvisible;
  bool bMasked;
  bool bTransparent;
  bool bNotSolid;
  bool bEnvironment;
  bool bSemisolid;
  bool bModulate;
  bool bFakeBackdrop;
  bool bTwoSided;
  bool bAutoUPan;
  bool bAutoVPan;
  bool bNoSmooth;
  bool bBigWavy;
  bool bSmallWavy;
  bool bWaterWavy;
  bool bCloudWavy;
  bool bLowShadowDetail;
  bool bNoMerge;
  bool bAlphaBlend;
  bool bUseHeightMap;
  bool bDirtyShadows;
  bool bHighLedge;
  bool bSpecialLit;
  bool bGouraud;
  bool bUnlit;
  bool bHighShadowDetail;
  bool bPortal;
  bool bMirrored;
  bool bX2, bX3, bX4, bX5, bX6, bX7;

  // Texture flags
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
  u32   CurrentTick;
  
  TArray<FMipmap> Mips;
  TArray<FMipmap> DecompMips;
  ETextureFormat DecompFormat;

  ESurfaceType SurfaceType;
  EUClampMode  UClampMode;
  EVClampMode  VClampMode;

  FColor PaletteTransform;
  void*  TextureHandle;
  u8     TextureMaterial;
};

class LIBUNR_API UFont : public UObject
{
  DECLARE_NATIVE_CLASS( UFont, UObject, CLASS_NoExport, Engine )
  UFont();

  struct FFontCharInfo
  {
    FFontCharInfo()
    {
      X = 0;
      Y = 0;
      Width = 0;
      Height = 0;
    }

    u32 X;
    u32 Y;
    u32 Width;
    u32 Height;
  };

  struct FFontTexture
  {
    ~FFontTexture()
    {
      Texture->DelRef();
      delete Characters;
    }

    UTexture* Texture;
    TArray<FFontCharInfo>* Characters;
  };

  TArray<FFontTexture>* FontTextures;

  virtual void Load();
};

class LIBUNR_API UScriptedTexture : public UTexture
{
  DECLARE_NATIVE_CLASS( UScriptedTexture, UTexture, CLASS_SafeReplace, Engine )
  EXPOSE_TO_USCRIPT()

  UScriptedTexture();

  bool bUse32BitFormat;
  bool bConstantRender;
  bool bClearScreen;
  bool bDrawFrame;

  u8 DrawStyle, DrawOpacity;
  class AActor* NotifyActor;
  UTexture* SourceTexture;

  UBitmap*  LocalSourceBitmap;
  UPalette* PaletteMap;
  float     LocalTime;

  void* PortalPtr;

  struct PortalRenderInfo
  {
    float  FOV;
    u8     RendMap;
    FPlane ColorScale;
    int    PixelOffset;
    FPlane NearClip;
    bool   bUseNearClip;
    bool   bWrapPixels;
  };

  PortalRenderInfo PortalInfo;

  enum ETexCompressType
  {
    COMP_None,
    COMP_P8,
    COMP_Mono,
  };

  // TODO: Make a dummy property in Object that can be pointed to crap like
  // this to instead of wasting bytes on variables that never get touched
  //
  int Junk1, Junk2, Junk3;
};

// This is deprecated and will never be implemented. Real dynamic shadows will
// be implemented in the renderer. This class is here simply to allow the export
// of the ShadowBitMap.uc class in 227
class LIBUNR_API UShadowBitMap : public UTexture
{
  DECLARE_NATIVE_CLASS( UShadowBitMap, UTexture, 0, Engine )
  EXPOSE_TO_USCRIPT()

  UShadowBitMap();

  AActor* ProjectingActor;
  FRotator ProjectDirection;
  u8 Gradience;
  u8 Softness;
  u8 StaticLevel;
  float ShadowScale;
  bool bIsValid;
};

