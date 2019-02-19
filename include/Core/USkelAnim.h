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
 * USkelAnim.h - Skeletal Animation Classes
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/UObject.h"
#include "Core/UMath.h"
#include "Core/USkelMesh.h"

class AActor;

class UAnimation : public UObject
{
  DECLARE_NATIVE_CLASS( UAnimation, UObject, CLASS_NoExport, Engine )

  UAnimation();

  // TODO: Fill out variables when we get to implementing this
};

class USkeletalMeshInstance : public UObject
{
  DECLARE_NATIVE_CLASS( USkeletalMeshInstance, UObject, 0, Engine )
  EXPOSE_TO_USCRIPT()

  USkeletalMeshInstance();

  struct Matrix
  {
    FPlane XPlane;
    FPlane YPlane;
    FPlane ZPlane;
    FPlane WPlane;
  };

  struct MeshModifierType
  {
    FVector BoneScale;
    FCoords CoordsMod, OrgCoords;
    int     RootB;
    u8      InUse;
  };

  struct AnimationChannel
  {
    int     RootBoneIndex;
    int     MChunkPtr, ASeqPtr;
    float   Rate, Frame, FrameStart, TweenOut, Alpha;
    FQuat   LastAnimQuat;
    FVector LastAnimTrack;
    u8      bLoop, bTweenIn, bNotifies;
    idx     AnimName;
  };

  Array<FCoords>*          SpaceBases; // Last computed skeleton coordinates.
  Array<int>*              CachedLinks; // Initialized links with animation and skeleton
  u8                       bHasUpdated; // Has this mesh been updated this tick yet?
  USkeletalMesh*           LastDrawnMesh; // Last mesh used on this render actor.
  UAnimation*              CachedAnim; // Last used animation on the render actor.
  Array<FCoords>*          CachedOrientations; // Cached data (for tweening)
  Array<FVector>*          CachedPositions; // Cached data (for tweening)
  float                    TweenStartFrame; // Starting frame for tweening
  Matrix                   Base; // Temp data
  u8                       bHasCachedFrame, bWasTweening; // Temp data
  idx                      CachedTweenSeq; // Temp data
  Array<MeshModifierType>* Modifiers; // Bone rotation/scale modifiers
  Array<AnimationChannel>* Channels; // Animation channels effected on this actor.
  int                      TChannelPtr; // Temp data

  Array<AActor*>* AttachedActors; // Actors attached to this skeletal mesh.
  int AttachedBoneIndex; // Bone where this actor is attached to.
  idx AttachedBoneName; // Cached attached bone name (in case mesh changes).
  AActor* MyAttachment; // Actor this actor is currently being attached to.
  u8 HardAttachFlags;
};

