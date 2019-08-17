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
 * USkelMesh.h - Skeletal Mesh Classes
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Engine/ULodMesh.h"
#include "Engine/USkelAnim.h"

struct DLL_EXPORT FWedgeChunk
{
  FWedgeChunk( FLodWedge& LodWedge );

  u16 VertexIndex;
  float U;
  float V;
  byte MaterialIndex;
  byte _pad0;

  friend FFileArchiveOut& operator<<( FFileArchiveOut& Out, FWedgeChunk& Wedge );
};

struct DLL_EXPORT FFaceChunk
{
  FFaceChunk( FLodFace& LodFace );

  u16 WedgeIndex[3];
  u8  MatIndex;
  u8  AuxMatIndex;
  int SmoothingGroups;
};

struct DLL_EXPORT FMaterialChunk
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

struct DLL_EXPORT FSkeletalWedge
{
  u16 VertexIndex;
  u16 Flags;
  float U;
  float V;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FSkeletalWedge& SW );
};

struct DLL_EXPORT FBonePos
{
  FPlane Orientation;
  FVector Position;
  float Length;
  FVector Size;
  
  friend FPackageFileIn& operator>>( FPackageFileIn& In, FBonePos& BP );
};

struct DLL_EXPORT FRefSkeleton
{
  FName Name;
  u32 Flags;
  FBonePos BonePos;
  u32 NumChildren;
  u32 ParentIndex;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FRefSkeleton& RS );
};

struct DLL_EXPORT FBoneWeightIndex
{
  u16 WeightIndex;
  u16 Number;
  u16 DetailA;
  u16 DetailB;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FBoneWeightIndex& BWI );
};

struct DLL_EXPORT FBoneWeight
{
  u16 PointIndex;
  u16 BoneWeight;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FBoneWeight& BW );
};

class DLL_EXPORT USkeletalMesh : public ULodMesh
{
  DECLARE_NATIVE_CLASS( USkeletalMesh, ULodMesh, CLASS_NoExport, Engine )

  USkeletalMesh();
  virtual void Load();
  virtual bool ExportToFile( const char* Dir, const char* Type );

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

