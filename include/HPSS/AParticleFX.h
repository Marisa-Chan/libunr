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
 * AParticleFX.h - Harry Potter particle fx stub
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Engine/UGesture.h"
#include "Actors/AActor.h"
#include "HPSS/UParticleList.h"

class DLL_EXPORT AParticleFX : public AActor
{
  DECLARE_NATIVE_CLASS( AParticleFX, AActor, 0, Engine );
  EXPOSE_TO_USCRIPT();

  AParticleFX();

  enum EParticlePrimitive
  {
    PPRIM_Line,
    PPRIM_Billboard,
    PPRIM_Liquid,
    PPRIM_Shard,
    PPRIM_TriTube,
  };

  enum EDistributionType
  {
    DIST_Random, 
    DIST_Uniform,
    DIST_OwnerMesh
  };

  struct FFloatParams
  {
    float Base;
    float Rand;
  };

  struct FColorParams
  {
    FColor Base;
    FColor Rand;
  };

  struct FParticleParams
  {
    FVector Position;
    FVector Velocity;  
    float  Lifetime; 
    float  Alpha;    
    FColor  Color;   
    float  Width;    
    float  Length;   
    float  DripTimer;
    float  SpinRate; 
  };

  FFloatParams ParticlesPerSec;
  FFloatParams SourceWidth;
  FFloatParams SourceHeight;
  FFloatParams SourceDepth;
  FFloatParams Period;
  FFloatParams Decay;
  FFloatParams AngularSpreadWidth;
  FFloatParams AngularSpreadHeight;
  bool bSteadyState;
  bool bPrime;
  FFloatParams Speed;
  FFloatParams Lifetime;
  FColorParams ColorStart;
  FColorParams ColorEnd;
  FFloatParams AlphaStart;
  FFloatParams AlphaEnd;
  FFloatParams SizeWidth;
  FFloatParams SizeLength;
  FFloatParams SizeEndScale;
  FFloatParams SpinRate;
  FFloatParams DripTime;
  bool bUpdate;
  bool bVelocityRelative;
  bool bSystemRelative;
  float ParentBlend;
  float LOD;
  float AlphaDelay;
  float ColorDelay;
  float SizeDelay;
  float AlphaGrowPeriod;
  float SizeGrowPeriod;
  float Chaos;
  float ChaosDelay;
  float Elasticity;
  FVector Attraction;
  float Damping;
  u8 Distribution;
  UGesture* Pattern;
  float WindModifier;
  bool bWindPerParticle;
  float GravityModifier;
  FVector Gravity;
  int ParticlesAlive;
  int ParticlesMax;
  UTexture* Textures[5];
  UTexture* ColorPalette;
  u8 RenderPrimitive;
  UParticleList* ParticleList;
  float EmitDelay;
  FVector LastUpdateLocation;
  FVector LastEmitLocation;
  FRotator LastUpdateRotation;
  float EmissionResidue;
  float Age;
  float ElapsedTime;
  int ParticlesEmitted;
  FPlane LightColor;
  int CurrentPriorityTag;
  bool bShellOnly;
  bool bEmit;
};

