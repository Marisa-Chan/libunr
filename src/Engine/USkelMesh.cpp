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
 * USkelMesh.cpp - Skeletal Mesh functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Engine/USkelMesh.h"

USkeletalMesh::USkeletalMesh()
  : ULodMesh()
{
}

USkeletalMesh::~USkeletalMesh()
{
}

FPackageFileIn& operator>>( FPackageFileIn& In, FSkeletalWedge& SW )
{
  In >> SW.VertexIndex; 
  In >> SW.Flags;
  In >> SW.U;
  In >> SW.V;
  return In;
}

FPackageFileIn& operator>>( FPackageFileIn& In, FBonePos& BP )
{
  In >> BP.Orientation;
  In >> BP.Position; 
  In >> BP.Length;
  In >> BP.Size;
  return In;
}

FPackageFileIn& operator>>( FPackageFileIn& In, FRefSkeleton& RS )
{
  In >> RS.Name;
  In >> RS.Flags;
  In >> RS.BonePos;
  In >> RS.NumChildren;
  In >> RS.ParentIndex;
  return In;
}

FPackageFileIn& operator>>( FPackageFileIn& In, FBoneWeightIndex& BWI )
{
  In >> BWI.WeightIndex;
  In >> BWI.Number;
  In >> BWI.DetailA;
  In >> BWI.DetailB;
  return In;
}

FPackageFileIn& operator>>( FPackageFileIn& In, FBoneWeight& BW )
{
  In >> BW.PointIndex;
  In >> BW.BoneWeight;
  return In;
}

void USkeletalMesh::Load()
{
  Super::Load();
  FPackageFileIn& In = *PkgFile;

  In >> ExtWedges; 
  In >> Points;
  In >> Refs;
  In >> WeightIndices;
  In >> Weights;
  In >> LocalPoints;

  In >> SkeletalDepth;

  idx DefAnimIdx; 
  In >> CINDEX( DefAnimIdx );
  DefaultAnimation = (UAnimation*)LoadObject( DefAnimIdx, UAnimation::StaticClass(), NULL );

  In >> WeaponBoneIndex;
  In >> WeaponAdjust;
}

bool USkeletalMesh::ExportToFile( const char* Dir, const char* Type )
{
  return true;
}

#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( USkeletalMesh );

