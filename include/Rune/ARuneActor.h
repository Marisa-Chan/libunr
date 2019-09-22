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
 * ARuneActor.h - Various actor classes specific to Rune
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Engine/UModel.h"
#include "Actors/AActor.h"
#include "Actors/AHUD.h"
#include "Actors/ANavigationPoint.h"

class DLL_EXPORT AAnimationProxy : public AKeypoint
{
  DECLARE_NATIVE_CLASS( AAnimationProxy, AKeypoint, 0, Engine ) 
  EXPOSE_TO_USCRIPT()

  AAnimationProxy();

  class AWeapon* curWeapon;
  class AWeapon* NewWeapon;
  class AShield* curShield;
  class AShield* newShield;
};

class DLL_EXPORT AAutoLink : public ANavigationPoint
{
  DECLARE_NATIVE_CLASS( AAutoLink, ANavigationPoint, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AAutoLink();

  FName LinkTag;
};

class DLL_EXPORT ADebugHUD : public AHUD
{
  DECLARE_NATIVE_CLASS( ADebugHUD, AHUD, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ADebugHUD();

  FName ModeTable[11];
  AActor* Watch;
  int DebugMode;
  int DebugHudMode;
};

class DLL_EXPORT AParticleSystem : public AActor
{
  DECLARE_NATIVE_CLASS( AParticleSystem, AActor, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AParticleSystem();

  struct FParticle
  {
	  FVector Location;
	  FVector Velocity;
	  float LifeSpan;
	  float XScale;
	  float YScale;
	  float ScaleStartX;
	  float ScaleStartY;
	  FVector Alpha;
	  float U0, V0;
	  float U1, V1;
	  FVector Points[4];
	  u8 Style;
	  u8 TextureIndex;
	  bool Valid;
  };

  bool IsLoaded;
  bool bSpriteInEditor;
  bool bSystemOneShot;
  bool bSystemTicks;
  bool bRelativeToSystem;
  bool bEventDeath;
  u8 ParticleCount;
  UTexture* ParticleTexture[4];
  bool bRandomTexture;
  u8 ParticleType;
  u8 ParticleSpriteType;
  u8 SpawnShape;
  float RandomDelay;
  float SystemLifeSpan;
  float CurrentDelay;
  u8 OldParticleCount;
  FCoords SystemCoords;
  bool HasValidCoords;
  float LastTime;
  float CurrentTime;
  FParticle ParticleArray[64];
  FVector OriginOffset;
  FVector ShapeVector;
  FVector VelocityMin;
  FVector VelocityMax;
  float ScaleMin;
  float ScaleMax;
  float ScaleDeltaX;
  float ScaleDeltaY;
  float LifeSpanMin;
  float LifeSpanMax;
  u8 AlphaStart;
  u8 AlphaEnd;
  u8 PercentOffset;
  bool bAlphaFade;
  bool bApplyGravity;
  float GravityScale;
  bool bApplyZoneVelocity;
  float ZoneVelocityScale;
  bool bWaterOnly;
  bool bOneShot;
  bool bConvergeX;
  bool bConvergeY;
  bool bConvergeZ;
  bool bConstrainToBounds;
  float SpawnDelay;
  float SpawnOverTime;
  float TextureChangeTime;
  u8 BaseJointIndex;
  u8 OffsetJointIndex;
  FVector OldBaseLocation;
  FVector OldOffsetLocation;
  float SwipeSpeed;
  u8 NumConPts;
  float BeamThickness;
  float BeamTextureScale;
  int TargetJointIndex;
  FVector ConnectionPoint[32];
  FVector ConnectionOffset[32];
  bool bUseTargetLocation;
  FVector TargetLocation;
  bool bEventSystemInit;
  bool bEventSystemTick;
  bool bEventParticleTick;
  bool bTaperStartPoint;
  bool bTaperEndPoint;
};

class DLL_EXPORT AScriptDispatcher : public AKeypoint
{
  DECLARE_NATIVE_CLASS( AScriptDispatcher, AKeypoint, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AScriptDispatcher();

  struct SAction
  {
    float   Delay;
    FName   AnimToPlay;
    FName   EventToFire;
    USound* SoundToPlay;
    bool    bTaskLocked;
  };

  FName LookTarget[12];
  FString* ControlMouth[12];
  FString* ControlHead[12];
  float ControlTimeGranularity;
  SAction Actions[12];
  FName NextOrder;
  FName NextOrderTag;
  bool bWaitToBeTriggered;
  APawn* WaitingScripter;
};

class DLL_EXPORT AScriptAction : public AKeypoint
{
  DECLARE_NATIVE_CLASS( AScriptAction, AKeypoint, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AScriptAction();

  FName AnimToPlay;
  float PauseBeforeSound;
  USound* SoundToPlay;
  float AnimTimeToLoop;
  bool bReleaseUponCompletion;
  bool bFireEventImmediately;
  bool bTurnToRotation;
  FString* ControlTorso;
  FName NextOrder;
  FName NextOrderTag;
  bool bWaitToBeTriggered;
  FString* ControlMouth;
  FString* ControlHead;
  float ControlTimeGranularity;
  FName LookTarget;
  APawn* WaitingScripter;
};

class DLL_EXPORT ASoundPlayer : public AKeypoint
{
  DECLARE_NATIVE_CLASS( ASoundPlayer, AKeypoint, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ASoundPlayer();

  bool bPlayerSound;
  bool bAutoContinuous;
  bool bRandomPosition;
  float RndPosHeight;
  float RndPosRadius;
  u8 RandomDelayMin;
  u8 RandomDelayMax;
  u8 RandomPercentPitch;
  u8 RandomPercentVolume;
  USound* TSound[4];
  u8 TSoundPitch[4];
  float TSoundProbability[4];
  float TSoundRadius;
  u8 TSoundVolume[4];
  int TriggerCountdown;
  u8 TriggerBehavior;
  u8 SelectMode;
  int SoundCount;
  int SoundXLat[4];
  float AdjustedProbability[4];
  int CSnd;
  float CSndDuration;
  FVector StartOffsetLocation;
  FVector RandomSize;
  int SPMaxSize;
  int CycleSound;
};

