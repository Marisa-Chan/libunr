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

FWedgeChunk::FWedgeChunk( FLodWedge& LodWedge )
{
  VertexIndex = LodWedge.VertexIndex;
  U = (float)LodWedge.S / 255.f;
  V = (255.f - (float)LodWedge.T) / 255.f;
  MaterialIndex = 0; // Can this go unused?
  _pad0 = 0;
}

FFileArchiveOut& operator<<( FFileArchiveOut& Out, FWedgeChunk& Wedge )
{
  Out << Wedge.VertexIndex << Wedge.U << Wedge.V << Wedge.MaterialIndex << Wedge._pad0;
  return Out;
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
  if ( stricmp( Type, "psk" ) != 0 )
  {
    GLogf( LOG_ERR, "Can't export skeletal mesh to file type '%s'", Type );
    return false;
  }

  // Open psk file
  FStringFilePath Filename( Dir, Name.Data(), Type );
  FFileArchiveOut Out();
  if ( Out.Open( Filename ) != 0 )
  {
    GLogf( LOG_WARN, "Failed to export skeletal mesh to file '%s'", Filename.Data() );
    return false;
  }

  FChunkHeaderOut ChunkHdr;

  // Write PSK header
  strcpy( ChunkHdr.ChunkId, "ACTRHEAD" );
  ChunkHdr.TypeFlags = PSK_PSA_TYPE_FLAGS;
  ChunkHdr.DataSize = 0;
  ChunkHdr.DataCount = 0;

  Out << ChunkHdr;

  // Write points
  strcpy( ChunkHdr.ChunkId, "PNTS0000" );
  ChunkHdr.DataSize = sizeof(FVector);
  ChunkHdr.DataCount = Points.Size();
  
  Out << ChunkHdr;
  Out << Points;

  // Write wedges
  strcpy( ChunkHdr.ChunkId, "VTXW0000" );
  ChunkHdr.DataSize = sizeof(FWedgeChunk);
  ChunkHdr.DataCount = Wedges.Size();

  Out << ChunkHdr;

  for ( int i = 0; i < ExtWedges.Size(); i++ )
  {
    FWedgeChunk Wedge( Wedges[i] );
    Out << Wedge;
  }

  // Write faces
  strcpy( ChunkHdr.ChunkId, "FACE0000" );
  ChunkHdr.DataSize = sizeof(FFaceChunk);
  ChunkHdr.DataCount = Faces.Size();

  Out << ChunkHdr;

  for ( int i = 0; i < Faces.Size(); i++ )
  {
    FFaceChunk Face( Faces[i] );
    Out << Face;
  }

  // Write materials
  strcpy( ChunkHdr.ChunkId, "MATT0000" );
  ChunkHdr.DataSize = sizeof(FMaterialChunk);
  ChunkHdr.DataCount = Materials.Size();

  Out << ChunkHdr;

  for ( int i = 0; i < Materials.Size(); i++ )
  {
    FMaterialChunk Mat( Materials[i] );
    Out << Mat;
  }

  // Write bone data
  strcpy( ChunkHdr.ChunkId, "REFSKELT" );
  ChunkHdr.DataSize = sizeof(FBoneChunk);
  ChunkHdr.DataCount = Refs.Size();

  Out << ChunkHdr;

  for ( int i = 0; i < Refs.Size(); i++ )
  {
    FBoneChunk Bone( Refs[i] );
    Out << Bone;
  }

  // Write weight data
  strcpy( ChunkHdr.ChunkId, "RAWWEIGHTS" );
  ChunkHdr.DataSize = sizeof(FWeightChunk);
  ChunkHdr.DataCount = Weights.Size();

  Out << ChunkHdr;

  for ( int i = 0; i < Weights.Size(); i++ )
  {
    Out << Weights[i];
  }

  // Close file
  Out.Close();
  return true;
}

#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( USkeletalMesh );

