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

#include "Engine/USkelAnim.h"

UAnimation::UAnimation()
  : UObject()
{
}

UAnimation::~UAnimation()
{
}

FPackageFileIn& operator>>( FPackageFileIn& In, FRefBone& RB )
{
  In >> RB.Name;
  In >> RB.Flags;
  In >> RB.ParentIndex;

  return In;
}

FPackageFileIn& operator>>( FPackageFileIn& In, FAnalogTrack& AT )
{
  In >> AT.Flags;
  In >> AT.KeyQuats;
  In >> AT.KeyPositions;
  In >> AT.KeyTimes;
  
  return In;
}

FPackageFileIn& operator>>( FPackageFileIn& In, FAnimAnalogTrack& AAT )
{
  In >> AAT.Track;
  In >> AAT.Root;

  return In;
}

FPackageFileIn& operator>>( FPackageFileIn& In, FAnimMove& AM )
{
  In >> AM.RootSpeed3D;
  In >> AM.TrackTime;
  In >> AM.StartBone;
  In >> AM.BoneIndices;
  In >> AM.AnalogTracks;

  return In;
}

void UAnimation::Load()
{
  Super::Load();

  FPackageFileIn& In = *PkgFile;
  In >> RefBones;
  In >> AnimMoves;
}

bool UAnimation::ExportToFile( const char* Path, const char* Dir )
{
  return true;
}

USkeletalMeshInstance::USkeletalMeshInstance()
  : UObject()
{
}

USkeletalMeshInstance::~USkeletalMeshInstance()
{
}

#include "Core/UClass.h"
IMPLEMENT_NATIVE_CLASS( UAnimation );
IMPLEMENT_NATIVE_CLASS( USkeletalMeshInstance );

BEGIN_PROPERTY_LINK( USkeletalMeshInstance, 20 )
  LINK_NATIVE_PROPERTY( SpaceBases );
  LINK_NATIVE_PROPERTY( CachedLinks );
  LINK_NATIVE_PROPERTY( bHasUpdated );
  LINK_NATIVE_PROPERTY( LastDrawnMesh );
  LINK_NATIVE_PROPERTY( CachedAnim );
  LINK_NATIVE_PROPERTY( CachedOrientations );
  LINK_NATIVE_PROPERTY( CachedPositions );
  LINK_NATIVE_PROPERTY( TweenStartFrame );
  LINK_NATIVE_PROPERTY( Base );
  LINK_NATIVE_PROPERTY( bHasCachedFrame );
  LINK_NATIVE_PROPERTY( bWasTweening );
  LINK_NATIVE_PROPERTY( CachedTweenSeq );
  LINK_NATIVE_PROPERTY( Modifiers );
  LINK_NATIVE_PROPERTY( Channels );
  LINK_NATIVE_PROPERTY( TChannelPtr );
  LINK_NATIVE_PROPERTY( AttachedActors );
  LINK_NATIVE_PROPERTY( AttachedBoneIndex );
  LINK_NATIVE_PROPERTY( AttachedBoneName );
  LINK_NATIVE_PROPERTY( MyAttachment );
  LINK_NATIVE_PROPERTY( HardAttachFlags );
END_PROPERTY_LINK()

