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
 * APawn.h - Pawn class, base class of all creature types
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Actors/AActor.h"
#include "Actors/AReplicationInfo.h"
#include "Actors/AWeapon.h"

class DLL_EXPORT APawn : public AActor
{
  DECLARE_NATIVE_CLASS( APawn, AActor, 0, Engine )
  EXPOSE_TO_USCRIPT()

  APawn();
  virtual void PostDefaultLoad();

  bool bBehindView;
  bool bIsPlayer;
  bool bJustLanded;
  bool bUpAndOut;
  bool bIsWalking;
  bool bHitSlopedWall;
  bool bNeverSwitchOnPickup;
  bool bWarping;
  bool bUpdatingDisplay;
  bool bClientSimFall;
  bool bClientTick;
  bool bUseNoWalkInAir;
  bool bNoStopAtLedge;
  bool bPostRender2D;

  bool bCanStrafe;
  bool bFixedStart;
  bool bReducedSpeed;
  bool bCanJump;
  bool bCanWalk;
  bool bCanSwim;
  bool bCanFly;
  bool bCanOpenDoors;
  bool bCanDoSpecial;
  bool bDrowning;
  bool bLOSFlag;
  bool bFromWall;
  bool bHunting;
  bool bAvoidLedges;
  bool bStopAtLedges;
  bool bJumpOffPawn;
  bool bShootSpecial;
  bool bAutoActivate;
  bool bIsHuman;
  bool bIsFemale;
  bool bIsMultiSkinned;
  bool bCountJumps;
  bool bAdvancedTactics;
  bool bViewTarget;
  FString* SelectionMesh;
  FString* SpecialMesh;

  // 227 flags
  bool bEnhancedSightCheck;
  bool bRepHealth;
  bool bDoAutoSerpentine;
  bool bIsCrawler;
  bool bIsBleeding;
  bool bIsAmbientCreature;
  ANavigationPoint* LastAnchor;

  float SightCounter;
  float PainTime;
  float SpeechTime;

  float AvgPhysicsTime;
  PointRegion FootRegion;
  PointRegion HeadRegion;

  float MoveTimer;
  AActor* MoveTarget;
  AActor* FaceTarget;
  FVector Destination;
  FVector Focus;
  float   DesiredSpeed;
  float   MaxDesiredSpeed;
  float   MeleeRange;
  float   SerpentineDist;
  float   SerpentineTime;
  FVector MovementStart;

  float GroundSpeed;
  float WaterSpeed;
  float AirSpeed;
  float AccelRate;
  float JumpZ;
  float MaxStepHeight;
  float AirControl;

  float MinHitWall;
  u8    Visibility;
  float Alertness;
  float Stimulus;
  float SightRadius;
  float PeripheralVision;
  float HearingThreshold;
  float HuntOffDistance;
  FVector LastSeenPos;
  FVector LastSeeingPos;
  float   LastSeenTime;
  APawn*  Enemy;

  AWeapon* Weapon;
  AWeapon* PendingWeapon;
  AInventory* SelectedItem;

  APawn*  LastDamageInstigator;
  FVector LastDamageHitLocation;
  FVector LastDamageMomentum;
  idx     LastDamageType;
  float   LastDamageTime;
  bool    bLastDamageSpawnedBlood;

  FRotator ViewRotation;
  FVector  WalkBob;
  float    BaseEyeHeight;
  float    EyeHeight;
  FVector  Floor;
  float    SplashTime;

  float OrthoZoom;
  float FovAngle;

  int DieCount;
  int ItemCount;
  int KillCount;
  int SecretCount;
  int Spree;

  int Health;

  idx ReducedDamageType;
  float ReducedDamagePct;

  UClass* DropWhenKilled;

  float UnderWaterTime;

  enum EAttitude
  {
    ATTITUDE_Fear,
    ATTITUDE_Hate,
    ATTITUDE_Frenzy,
    ATTITUDE_Threaten,
    ATTITUDE_Ignore,
    ATTITUDE_Friendly,
    ATTITUDE_Follow
  };

  EAttitude AttitudeToPlayer;

  enum EIntelligence
  {
    BRAINS_None,
    BRAINS_Reptile,
    BRAINS_Mammal,
    BRAINS_Human
  };

  EIntelligence Intelligence;

  float Skill;
  AActor* SpecialGoal;
  float   SpecialPause;

  // Sorry, what are structs again? /s
  FVector Noise1Spot;
  float   Noise1Time;
  APawn*  Noise1Other;
  float   Noise1Loudness;

  FVector Noise2Spot;
  float   Noise2Time;
  APawn*  Noise2Other;
  float   Noise2Loudness;

  float LastPainSound;

  APawn* NextPawn;

  USound* HitSound1;
  USound* HitSound2;
  USound* Land;
  USound* Die;
  USound* WaterStep;

  u8 bZoom, bRun, bLook, bDuck, bSnapLevel,
     bStrafe, bFire, bAltFire, bFreeLook,
     bExtra0, bExtra1, bExtra2, bExtra3;

  float CombatStyle;
  ANavigationPoint* Home;

  idx NextState;
  idx NextLabel;

  float SoundDampening;
  float DamageScaling;

  idx AlarmTag;
  idx SharedAlarmTag;
  ADecoration* CarriedDecoration;

  idx PlayerRestartState;

  FString* MenuName;
  FString* MenuNameDative;
  FString* NameArticle;

  u8 VoicePitch;
  UClass* VoiceType;
  float OldMessageTime;

  ANavigationPoint* RouteCache[16];
  UClass* PlayerReplicationInfoClass;
  APlayerReplicationInfo* PlayerReplicationInfo;

  // 227 enhancements
  AActor* BleedingActor;
  float   SightDistanceMulti;

  // Probably gonna go unused with dynamic shadows being a possibility
  AActor* Shadow;

  UClass* Pawn_BloodsprayClass;
  UClass* Pawn_BleedingClass;

  bool bIsSpeaking;
  bool bWasSpeaking;
  FString* LastPhoneme;
  FString* NextPhoneme;

  enum EPawnSightCheck
  {
    SEE_PlayersOnly,
    SEE_All,
    SEE_None
  };

  EPawnSightCheck SightCheckType;

  float BeaconOffset;
  float MaxFrobDistance;
  AActor* FrobTarget;

  // Deus Ex variables
  bool bCanGlide;
  int HealthHead;
  int HealthTorso;
  int HealthLegLeft;
  int HealthLegRight;
  int HealthArmLeft;
  int HealthArmRight;
  float AnimTimer[4];
  bool bOnFire;
  float BurnTimer;
  float AIHorizontalFov;
  float AspectRatio;
  float AngularResolution;
  float MinAngularSize;
  float VisibilityThreshold;
  float SmellThreshold;
  FName Alliance;
  FRotator AIAddViewRotation;

  // Rune Variables
  UClass* CarcassType;
  bool bCanGrabEdges;
  bool bAlignToFloor;
  float AmbientSoundTime;
  float CombatRange;
  bool bStopMoveIfCombatRange;
  float WalkingSpeed;
  float MovementSpeed;
  bool bHurrying;
  u8 SpeedScale;
  int ClassID;
  class AShield* Shield;
  FName StartEvent;
  int CurrentSkin;
  FVector ViewLocation;
  FVector GrabLocationUp;
  FVector GrabLocationIn;
  int Strength;
  int RunePower;
  int MaxHealth;
  int MaxStrength;
  int MaxPower;
  int BodyPartHealth[15];
  int GibCount;
  UClass* GibClass;
  float PainDelay;
  bool bGibbable;
  bool bInvisible;
  USound* HitSound3;
  USound* Die2;
  USound* Die3;
  USound* GibSound;
  float FootstepVolume;
  USound* LandGrunt;
  USound* FootStepWood[3];
  USound* FootStepMetal[3];
  USound* FootStepStone[3];
  USound* FootStepFlesh[3];
  USound* FootStepIce[3];
  USound* FootStepEarth[3];
  USound* FootStepSnow[3];
  USound* FootStepBreakableWood[3];
  USound* FootStepBreakableStone[3];
  USound* FootStepWater[3];
  USound* FootStepMud[3];
  USound* FootStepLava[3];
  USound* LandSoundWood;
  USound* LandSoundMetal;
  USound* LandSoundStone;
  USound* LandSoundFlesh;
  USound* LandSoundIce;
  USound* LandSoundSnow;
  USound* LandSoundEarth;
  USound* LandSoundBreakableWood;
  USound* LandSoundBreakableStone;
  USound* LandSoundWater;
  USound* LandSoundMud;
  USound* LandSoundLava;
  FName NextStateAfterPain;
  FName WeaponJoint;
  FName ShieldJoint;
  FName StabJoint;
  bool bCanLook;
  FRotator MaxBodyAngle;
  FRotator MaxHeadAngle;
  bool bRotateHead;
  bool bRotateTorso;
  bool bHeadLookUpDouble;
  float LookDegPerSec;
  AActor* LookTarget;
  FVector LookSpot;
  FRotator LookAngle;
  FRotator targetangle;
  bool bOverrideLookTarget;
  int MouthRot;
  int DesiredMouthRot;
  int MouthRotRate;
  int MaxMouthRot;
  int MaxMouthRotRate;
  UClass* FootprintClass;
  UClass* WetFootprintClass;
  UClass* BloodyFootprintClass;
  int LFootJoint;
  int RFootJoint;
  bool bFootsteps;
  int WaterSteps;
  int BloodSteps;
  bool bSwingingHigh;
  bool bSwingingLow;
  float DeathRadius;
  float DeathHeight;
  bool bAllowStandOn;
  bool bLeadEnemy;
  FName UninterruptedAnim;
  AActor* UseActor;
  FString* SkinDefaultText;
};

