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
 * UFire.h - Fire texture related classes
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Engine/UTexture.h"

/*-----------------------------------------------------------------------------
 * UFractalTexture
 * The base of all procedurally generated textures
-----------------------------------------------------------------------------*/
class LIBUNR_API UFractalTexture : public UTexture
{
  DECLARE_NATIVE_CLASS( UFractalTexture, UTexture, CLASS_Abstract, Fire )
  EXPOSE_TO_USCRIPT();

  UFractalTexture();
  virtual void Tick( float DeltaTime );

  int UMask;
  int VMask;
  int LightOutput;
  int SoundOutput;
  int GlobalPhase;
  u8  DrawPhase;
  u8  AuxPhase;
  u8  bHasInit;
  u8  bHardwareAccelerated; // TODO

  // Software accelerated features
  FMipmap* Buffer;
};

/*-----------------------------------------------------------------------------
 * UFireTexture
 * An animated fire texture 
-----------------------------------------------------------------------------*/
class LIBUNR_API UFireTexture : public UFractalTexture
{
  DECLARE_NATIVE_CLASS( UFireTexture, UFractalTexture, 0, Fire )
  EXPOSE_TO_USCRIPT();

  UFireTexture();
  virtual void Tick( float DeltaTime );
  virtual void Load();
  virtual bool ExportToFile( const char* Dir, const char* Type );

  enum ESpark
  {
    SPARK_Burn,
    SPARK_Sparkle,
    SPARK_Pulse,
    SPARK_Signal,
    SPARK_Blaze,
    SPARK_OzHasSpoken,
    SPARK_Cone,
    SPARK_BlazeRight,
    SPARK_BlazeLeft,
    SPARK_Cylinder,
    SPARK_Cylinder3D,
    SPARK_Lissajous,
    SPARK_Jugglers,
    SPARK_Emit,
    SPARK_Fountain,
    SPARK_FLocks,
    SPARK_Eels,
    SPARK_Organic,
    SPARK_WanderOrganic,
    SPARK_RandomCloud,
    SPARK_CustomCloud,
    SPARK_LocalCloud,
    SPARK_Stars,
    SPARK_LineLightning,
    SPARK_RampLightning,
    SPARK_SphereLightning,
    SPARK_Wheel,
    SPARK_Gametes,
    SPARK_Sprinkler
  };

  enum DMode
  {
    DRAW_Normal,
    DRAW_Lathe,
    DRAW_Lathe_2,
    DRAW_Lathe_3,
    DRAW_Lathe_4
  };

  struct Spark
  {
    u8 Type;
    u8 Heat;
    u8 X;
    u8 Y;

    u8 ByteA;
    u8 ByteB;
    u8 ByteC;
    u8 ByteD;

    friend FPackageFileIn& operator>>( FPackageFileIn& In, Spark& S )
    {
      In >> S.Type;
      In >> S.Heat;
      In >> S.X;
      In >> S.Y;
      In >> S.ByteA;
      In >> S.ByteB;
      In >> S.ByteC;
      In >> S.ByteD;
      return In;
    }

    friend FPackageFileOut& operator>>( FPackageFileOut& Out, Spark& S )
    {
      Out << S.Type;
      Out << S.Heat;
      Out << S.X;
      Out << S.Y;
      Out << S.ByteA;
      Out << S.ByteB;
      Out << S.ByteC;
      Out << S.ByteD;
      return Out;
    }
  };

  struct Particle
  {
    Particle()
    {
      memset( this, 0, sizeof( Particle ) );
    }

    Particle( u8 InX, u8 InY, u8 InType, u8 InHeat )
      : X(InX), Y(InY), Type(InType), Heat(InHeat) {}

    Particle( const Particle& Copy )
      : X(Copy.X), Y(Copy.Y), Type(Copy.Type), Heat(Copy.Heat) {}

    u8 X;
    u8 Y;
    u8 Type;
    i16 Heat;
  };

  ESpark SparkType;
  u8     RenderHeat;
  bool   bRising;

  u8 FX_Heat;
  u8 FX_Size;
  u8 FX_AuxSize;
  u8 FX_Area;
  u8 FX_Frequency;
  u8 FX_Phase;
  u8 FX_HorizSpeed;
  u8 FX_VertSpeed;

  DMode DrawMode;
  int   SparksLimit;

  int NumSparks;
  TArray<Spark>* Sparks;
  TArray<Particle>* Particles;

  int OldRenderHeat;
  u8  RenderTable[1028];
  u8  StarStatus;
  u8  PenDownX;
  u8  PenDownY;
};

/*-----------------------------------------------------------------------------
 * UWaterTexture
 * A base class for procedurally generated water textures
-----------------------------------------------------------------------------*/
class LIBUNR_API UWaterTexture : public UFractalTexture
{
  DECLARE_NATIVE_CLASS( UWaterTexture, UFractalTexture, CLASS_Abstract, Fire )
  EXPOSE_TO_USCRIPT()

  UWaterTexture();

  enum WDrop
  {
    DROP_FixedDepth,
    DROP_PhaseSpot,
    DROP_ShallowSpot,
    DROP_HalfAmpl,
    DROP_RandomMover,
    DROP_FixedRandomSpot,
    DROP_WhirlyThing,
    DROP_BigWhirly,
    DROP_HorizontalLine,
    DROP_VerticalLine,
    DROP_DiagonalLine1,
    DROP_DiagonalLine2,
    DROP_HorizontalOsc,
    DROP_VerticalOsc,
    DROP_DiagonalOsc1,
    DROP_DiagonalOsc2,
    DROP_RainDrops,
    DROP_AreaClamp,
    DROP_LeakyTap,
    DROP_DrippyTap,
  };

  struct ADrop
  {
    u8 Type;
    u8 Depth;
    u8 X;
    u8 Y;

    u8 ByteA;
    u8 ByteB;
    u8 ByteC;
    u8 ByteD;
  };

  u8 DropType;
  u8 WaveAmp;

  u8 FX_Frequency;
  u8 FX_Phase;
  u8 FX_Amplitude;
  u8 FX_Speed;
  u8 FX_Radius;
  u8 FX_Size;
  u8 FX_Depth;
  u8 FX_Time;

  int NumDrops;
  ADrop Drops[256];

  int SourceFields;
  u8  RenderTable[1028];
  u8  WaterTable[1536];
  u8  WaterParity;
  int OldWaveAmp;
};

/*-----------------------------------------------------------------------------
 * UWaveTexture
 * A wave texture with bump map and phong shading characteristics
-----------------------------------------------------------------------------*/
class LIBUNR_API UWaveTexture : public UWaterTexture
{
  DECLARE_NATIVE_CLASS( UWaveTexture, UWaterTexture, 0, Fire )
  EXPOSE_TO_USCRIPT()

  UWaveTexture();

  u8 BumpMapLight;
  u8 BumpMapAngle;
  u8 PhongRange;
  u8 PhongSize;
};

/*-----------------------------------------------------------------------------
 * UWetTexture
 * A wet texture with a specified, external background texture
-----------------------------------------------------------------------------*/
class LIBUNR_API UWetTexture : public UWaterTexture
{
  DECLARE_NATIVE_CLASS( UWetTexture, UWaterTexture, 0, Fire )
  EXPOSE_TO_USCRIPT()

  UWetTexture();

  UTexture* SourceTexture;
  UTexture* OldSourceTex;
  int       LocalSourceBitmap;
};

/*-----------------------------------------------------------------------------
 * UIceTexture
 * An ice texture that uses two textures to produce an ice effect
-----------------------------------------------------------------------------*/
class LIBUNR_API UIceTexture : public UFractalTexture
{
  DECLARE_NATIVE_CLASS( UIceTexture, UFractalTexture, 0, Fire )
  EXPOSE_TO_USCRIPT()

  UIceTexture();

  enum PanningType
  {
    SLIDE_Linear,
    SLIDE_Circular,
    SLIDE_Gestation,
    SLIDE_WavyX,
    SLIDE_WavyY
  };

  enum TimingType
  {
    TIME_FrameRateSync,
    TIME_RealTimeScroll
  };

  UTexture* GlassTexture;
  UTexture* SourceTexture;

  PanningType PanningStyle;
  TimingType  TimeMethod;

  u8 HorizPanSpeed;
  u8 VertPanSpeed;
  u8 Frequency;
  u8 Amplitude;

  bool  MoveIce;
  float MasterCount;
  float UDisplace;
  float VDisplace;
  float UPosition;
  float VPosition;

  float TickAccu;
  int   OldUDisplace;
  int   OldVDisplace;
  UTexture* OldGlassTex;
  UTexture* OldSourceTex;
  int LocalSource;
  int ForceRefresh;
};

