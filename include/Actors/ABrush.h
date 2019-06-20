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
 * ABrush.h - Brush actor class
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Actors/AActor.h"

class DLL_EXPORT ABrush : public AActor
{
  DECLARE_NATIVE_CLASS( ABrush, AActor, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ABrush();
  void ExportToLevelText( FileStreamOut* Out );
  virtual bool ExportToFile( const char* Dir, const char* Type );

  enum ECsgOper
  {
    CSG_Active,
    CSG_Add,
    CSG_Subtract,
    CSG_Intersect,
    CSG_Deintersect
  } CsgOper;

  UObject* UnusedLightMesh;
  FVector  PostPivot;

  FScale   MainScale;
  FScale   PostScale;
  FScale   TempScale;

  FColor BrushColor;
  int    PolyFlags;
  bool   bColored;
};

class DLL_EXPORT AMover : public ABrush 
{
  DECLARE_NATIVE_CLASS( AMover, ABrush, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AMover();

  enum EMoverEncroachType
  {
    ME_StopWhenEncroach,
    ME_ReturnWhenEncroach,
    ME_CrushWhenEncroach,
    ME_IgnoreWhenEncroach
  };
  EMoverEncroachType MoverEncroachType;

  enum EMoverGlideType
  {
    MV_MoveByTime,
    MV_GlideByTime,
    MV_AccelOverTime,
    MV_DeAccelOverTime
  };
  EMoverGlideType MoverGlideType;

  bool bUseShortestRotation;
  
  enum EBumpType
  {
    BT_PlayerBump,
    BT_PawnBump,
    BT_AnyBump
  };
  EBumpType BumpType;

  u8 KeyNum;
  u8 PrevKeyNum;
  u8 NumKeys;
  u8 WorldRaytraceKey;
  u8 BrushRaytraceKey;

  float MoveTime[16];
  float StayOpenTime;
  float OtherTime;
  int   EncroachDamage;

  bool bTriggerOnceOnly;
  bool bSlave;
  bool bUseTriggered;
  bool bDamageTriggered;
  bool bDynamicLightMover;

  bool bDirectionalPushOff;
  bool bAdvancedCamUpdate;
  bool bUseGoodCollision;

  idx     PlayerBumpEvent;
  idx     BumpEvent;
  AActor* SavedTrigger;
  float   DamageThreshold;
  int     NumTriggerEvents;
  AMover* Leader;
  AMover* Follower;
  idx     ReturnGroup;
  float   DelayTime;

  USound* OpeningSound;
  USound* OpenedSound;
  USound* ClosingSound;
  USound* ClosedSound;
  USound* MoveAmbientSound;

  FVector  KeyPos[16];
  FRotator KeyRot[16];
  FVector  BasePos, OldPos, OldPrePivot, SavedPos;
  FRotator BaseRot, OldRot, SavedRot;
  Array<int> NotifyLightMaps;

  ANavigationPoint* MyMarker;
  AActor*   TriggerActor;
  AActor*   TriggerActor2;
  APawn*    WaitingPawn;
  bool      bOpening, bDelaying;
  bool      bPlayerOnly;
  ATrigger* RecommendedTrigger;

  FVector  SimOldPos;
  int      SimOldRotPitch, SimOldRotYaw, SimOldRotRoll;
  FVector  SimInterpolate;
  FVector  RealPosition;
  FRotator RealRotation;
  int      ServerUpdate;
  int      ClientUpdate;
  int      LNextKeyNum;

  bool bClientPause;

  // Rune Variables
  FName InterpolateEvent[16];
};

class DLL_EXPORT APolyObj : public ABrush
{
  DECLARE_NATIVE_CLASS( APolyObj, ABrush, 0, Engine )
  EXPOSE_TO_USCRIPT()

  APolyObj();

  enum EPolyObjEncroachType
  {
    PET_StopWhenEncroach,
    PET_ReturnWhenEncroach,
    PET_CrushWhenEncroach,
    PET_IgnoreWhenEncroach
  } PolyObjEncroachType;

  enum EPolyGlideType
  {
    PGT_MoveByTime,
    PGT_GlideByTime,
    PGT_AccelOverTime,
    PGT_DeAccelOverTime
  } PolyObjGlideType;

  enum EPolyBumpType
  {
    BT_PlayerBump,
    BT_PawnBump,
    BT_CreatureBump,
    BT_AnyBump
  } PolyObjBumpType;

  u8 KeyNum;
  u8 PrevKeyNum;
  u8 NumKeys;
  u8 WorldRaytraceKey;
  u8 BrushRaytraceKey;

  float MoveTime[16];
  float StayOpenTime;
  float OtherTime;
  int   EncroachDamage;
  FName InterpolateEvent[16];
  int BumpDamage;

  bool bTriggerOnceOnly;
  bool bSlave;
  bool bUseTriggered;
  bool bDamageTriggered;
  bool bDynamicLightMover;
  idx     PlayerBumpEvent;
  idx     BumpEvent;
  AActor* SavedTrigger;
  float   DamageThreshold;
  int     NumTriggerEvents;
  AMover* Leader;
  AMover* Follower;
  idx     ReturnGroup;
  float   DelayTime;

  USound* OpeningSound;
  USound* OpenedSound;
  USound* ClosingSound;
  USound* ClosedSound;
  USound* MoveAmbientSound;

  FVector  KeyPos[16];
  FRotator KeyRot[16];
  FVector  BasePos, OldPos, OldPrePivot, SavedPos;
  FRotator BaseRot, OldRot, SavedRot;

  ANavigationPoint* MyMarker;
  AActor*   TriggerActor;
  AActor*   TriggerActor2;
  APawn*    WaitingPawn;
  bool      bOpening, bDelaying, bClientPause;
  bool      bPlayerOnly;
  ATrigger* RecommendedTrigger;

  FVector  SimOldPos;
  int      SimOldRotPitch, SimOldRotYaw, SimOldRotRoll;
  FVector  SimInterpolate;
  FVector  RealPosition;
  FRotator RealRotation;
  int      ClientUpdate;

  bool bCanRender;
  bool bDynamicLightPolyObj;

  FVector  CStartLocation;
  FVector  CHeightVect;
  FRotator CRotator;
  float    CTimeCounter;
  float    CPrevV;
  float    CPulse;
  float    CDelay;
  AActor*  CBase;

  float   CCycleTime;
  float   CPhase;
  float   CHeight;
  float   CSize;
  float   CSpeed;
  USound* CSound;
  bool    Flag1;
  bool    Flag2;
  FName   CBaseName;

#define CCUTOFF = 0.3f;

  int OnMeCount;
  APawn* OnMeList[4];
  bool   SinkResting;
  bool   bInitiallyOn;
  FVector ThrustVector;

  EMatterType Matter;
  UClass*     DebrisType;
  bool        bDestroyable;
  int         NumDebrisChunks;
  float       DebrisSpawnRadius;
  UTexture*   DebrisTexture;
};

