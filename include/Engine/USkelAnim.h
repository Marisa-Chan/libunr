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
 * USkelAnim.h - Skeletal Animation Classes
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/UObject.h"
#include "Core/UMath.h"
#include "Engine/UMesh.h"

class AActor;
class USkeletalMesh;

/*-----------------------------------------------------------------------------
 * FChunkHeader
 * Header for different sectiosn of PSK and PSA files
-----------------------------------------------------------------------------*/
#define PSK_PSA_TYPE_FLAGS 0x1e83b9 
struct FChunkHeaderOut
{
  char  ChunkId[20];
  int   TypeFlags;
  int   DataSize;
  int   DataCount;

  friend FFileArchiveOut& operator<<( FFileArchiveOut& Out, FChunkHeaderOut& Hdr );
};

/*-----------------------------------------------------------------------------
 * FNamedBone
 * Describes a bone within an animation object
-----------------------------------------------------------------------------*/
struct LIBUNR_API FNamedBone
{
  FName Name;
  u32 Flags;
  int ParentIndex;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FNamedBone& NB );
};

/*-----------------------------------------------------------------------------
 * FAnalogTrack
 * Describes animation for a single bone
-----------------------------------------------------------------------------*/
struct LIBUNR_API FAnalogTrack
{
  u32 Flags;
  TArray<FPlane>  KeyQuats;
  TArray<FVector> KeyPositions;
  TArray<float>   KeyTimes;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FAnalogTrack& AAT );
};

/*-----------------------------------------------------------------------------
 * FMotionChunk
 * A group of analog chunks to describe motion for a single frame (?)
-----------------------------------------------------------------------------*/
struct LIBUNR_API FMotionChunk
{
  FVector RootSpeed3D;
  float TrackTime;
  int StartBone;
  u32 Flags;

  TArray<int> BoneIndices;
  TArray<FAnalogTrack> AnalogTracks;
  FAnalogTrack RootTrack;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FMotionChunk& AM );
};

/*-----------------------------------------------------------------------------
 * UAnimation
 * An object for skeletal animations
-----------------------------------------------------------------------------*/
class LIBUNR_API UAnimation : public UObject
{
  DECLARE_NATIVE_CLASS( UAnimation, UObject, CLASS_NoExport, Engine )

  UAnimation();
  virtual void Load();
  virtual bool ExportToFile( const char* Path, const char* Dir );

  TArray<FNamedBone> RefBones;
  TArray<FMotionChunk> Moves;
  TArray<FMeshAnimSeq> AnimSeqs;

  // Purpose currently unknown
  //TArray<FMotionChunkDigestInfo> MovesInfo;
  //int RawNumFrames;
  //TArray<FQuatAnimKey> RawAnimKeys;
  //TArray<FAnimInfoBinary> RawAnimSeqInfo;
  //float CompFactor;
};

class LIBUNR_API USkeletalMeshInstance : public UObject
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

  TArray<FCoords>*    SpaceBases; // Last computed skeleton coordinates.
  TArray<int>*        CachedLinks; // Initialized links with animation and skeleton
  u8                       bHasUpdated; // Has this mesh been updated this tick yet?
  USkeletalMesh*           LastDrawnMesh; // Last mesh used on this render actor.
  UAnimation*              CachedAnim; // Last used animation on the render actor.
  TArray<FCoords>*    CachedOrientations; // Cached data (for tweening)
  TArray<FVector>*    CachedPositions; // Cached data (for tweening)
  float                    TweenStartFrame; // Starting frame for tweening
  Matrix                   Base; // Temp data
  u8                       bHasCachedFrame, bWasTweening; // Temp data
  idx                      CachedTweenSeq; // Temp data
  TArray<MeshModifierType>* Modifiers; // Bone rotation/scale modifiers
  TArray<AnimationChannel>* Channels; // Animation channels effected on this actor.
  int                      TChannelPtr; // Temp data

  TArray<AActor*>* AttachedActors; // Actors attached to this skeletal mesh.
  int AttachedBoneIndex; // Bone where this actor is attached to.
  idx AttachedBoneName; // Cached attached bone name (in case mesh changes).
  AActor* MyAttachment; // Actor this actor is currently being attached to.
  u8 HardAttachFlags;
};

