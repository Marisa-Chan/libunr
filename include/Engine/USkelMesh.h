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
 * USkelMesh.h - Skeletal Mesh Classes
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Engine/ULodMesh.h"
#include "Engine/USkelAnim.h"

struct LIBUNR_API FWedgeChunk
{
  FWedgeChunk( FLodWedge& LodWedge );

  u16 VertexIndex;
  float U;
  float V;
  u8 MaterialIndex;
  u8 _pad0;

  friend FFileArchiveOut& operator<<( FFileArchiveOut& Out, FWedgeChunk& Wedge );
};

struct LIBUNR_API FFaceChunk
{
  FFaceChunk( FLodFace& LodFace );

  u16 WedgeIndex[3];
  u8  MatIndex;
  u8  AuxMatIndex;
  int SmoothingGroups;
};

struct LIBUNR_API FMaterialChunk
{
  FMaterialChunk( FLodMaterial& LodMaterial );

  char Name[64];
  int  TextureIndex;
  int  PolyFlags;
  int  AuxMaterialIndex;
  int  AuxFlags;
  int  LodBias;
  int  LodStyle;
};

struct LIBUNR_API FSkeletalWedge
{
  u16 VertexIndex;
  u16 Flags;
  float U;
  float V;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FSkeletalWedge& SW );
};

struct LIBUNR_API FBonePos
{
  FPlane Orientation;
  FVector Position;
  float Length;
  FVector Size;
  
  friend FPackageFileIn& operator>>( FPackageFileIn& In, FBonePos& BP );
};

struct LIBUNR_API FRefSkeleton
{
  FName Name;
  u32 Flags;
  FBonePos BonePos;
  u32 NumChildren;
  u32 ParentIndex;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FRefSkeleton& RS );
};

struct LIBUNR_API FBoneWeightIndex
{
  u16 WeightIndex;
  u16 Number;
  u16 DetailA;
  u16 DetailB;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FBoneWeightIndex& BWI );
};

struct LIBUNR_API FBoneWeight
{
  float Weight;
  u16 PointIndex;
  u16 BoneWeight;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FBoneWeight& BW );
  friend FFileArchiveOut& operator<<( FFileArchiveOut& Out, FBoneWeight& BW );
};

class LIBUNR_API USkeletalMesh : public ULodMesh
{
  DECLARE_NATIVE_CLASS( USkeletalMesh, ULodMesh, CLASS_NoExport, Engine )

  USkeletalMesh();
  virtual void Load();

  TArray<FSkeletalWedge> ExtWedges;
  TArray<FVector> Points;
  TArray<FRefSkeleton> Refs;
  TArray<FBoneWeightIndex> WeightIndices;
  TArray<FBoneWeight> Weights;
  TArray<FVector> LocalPoints;
  u32 SkeletalDepth;
  UAnimation* DefaultAnimation;
  u32 WeaponBoneIndex;
  FCoords WeaponAdjust;
};

