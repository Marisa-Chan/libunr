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
 * ARuneActor.h - Actor class replacement for Rune
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Actors/AActor.h"
#include "Core/UModel.h"

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

class DLL_EXPORT ARuneActor : public AActor
{
  DECLARE_ALIASED_CLASS( ARuneActor, AActor, CLASS_NativeReplication, Engine )
  EXPOSE_TO_USCRIPT()

  ARuneActor();

  bool bReleaseLock;
  bool bRenderedLastFrame;
  bool bSpecialRender;
  bool bFrameNotifies;
  bool bLookFocusPlayer;
  bool bLookFocusCreature;
  bool bForceRender;
  u8 BaseJoint;
  u8 BaseMatterType;
  FVector BaseScrollDir;
  bool bHasShadow;
  class UShadowTexture* ShadowTexture;
  FVector ShadowVector;
  FVector ColorAdjust;
  FVector DesiredColorAdjust;
  u8 DesiredFatness;
  float AlphaScale;
  int LODPolyCount;
  float LODDistMax;
  float LODDistMin;
  float LODPercentMin;
  float LODPercentMax;
  u8 LODCurve;
  bool bPointLight;
  bool bMirrored;
  bool bPreLight;
  bool bComplexOcclusion;
  AActor* AttachParent;
  u8 AttachParentJoint;
  u8 IncrementStat;
  bool bJointsBlock;
  bool bJointsTouch;
  bool bSweepable;
  bool bSpecialLit2;
  bool bSpecialLit3;
  bool bNegativeLight;
  bool bAffectWorld;
  bool bAffectActors;
  bool bNoSurfaceBob;
  bool bDrawSkel;
  bool bDrawJoints;
  bool bDrawAxes;
  bool bApplyLagToAccelerators;
  u8 SkelMesh;
  USkelModel* Skeletal;
  USkelModel* SubstituteMesh;
  float BlendAnimAlpha;
  class AAnimationProxy* AnimProxy;
  UTexture* SkelGroupSkins[16];
  int SkelGroupFlags[16];
  u8 JointFlags[50];
  AActor* JointChild[50];
};

class AScriptDispatcher : public AKeypoint
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

