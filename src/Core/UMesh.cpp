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
 * UMesh.cpp - Mesh functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UMesh.h"

FPackageFileIn& operator>>( FPackageFileIn& In, FMeshVert& MV )
{
  if ( MV.bDeusEx )
  {
    u64 XYZ;
    In >> XYZ;

    MV.X = (XYZ & 0xFFFF) / 256;
    MV.Y = ((XYZ >> 16) & 0xFFFF) / 256;
    MV.Z = ((XYZ >> 32) & 0xFFFF) / 256;
  }
  else
  {
    u32 XYZ;
    In >> XYZ;

    MV.X = (XYZ & 0x7FF) / 8;
    MV.Y = ((XYZ >> 11) & 0x7FF) / 8;
    MV.Z = ((XYZ >> 22) & 0x3FF) / 4;
  }
    
  if ( MV.X > 128 )
    MV.X -= 256;
  MV.X = -MV.X;

  if ( MV.Y > 128 )
    MV.Y -= 256;
  MV.Y = -MV.Y;

  if ( MV.Z > 128 )
    MV.Z -= 256;

  return In;
}

FPackageFileIn& operator>>( FPackageFileIn& In, FMeshTri& MT )
{
  In >> MT.Vertex[0] >> MT.Vertex[1] >> MT.Vertex[2];
  In >> MT.VertexUV[0][0] >> MT.VertexUV[1][0];
  In >> MT.VertexUV[0][1] >> MT.VertexUV[1][1];
  In >> MT.VertexUV[0][2] >> MT.VertexUV[1][2];
  In >> MT.Flags;
  In >> MT.TextureIndex;
  return In;
}

FPackageFileIn& operator>>( FPackageFileIn& In, FMeshAnim& MA )
{
  return In;  
}

FPackageFileIn& operator>>( FPackageFileIn& In, FMeshVertConnect& MVC )
{
  return In;  
}

UMesh::UMesh()
  : UPrimitive()
{
  Verts = new Array<FMeshVert>();
  Tris  = new Array<FMeshTri>();
  Anims = new Array<FMeshAnim>();
  bDeusExMesh = false;
}

UMesh::~UMesh()
{
  delete Verts;
  delete Tris;
  delete Anims;
}

void UMesh::Load()
{
/*  Super::Load();

  u32 VertsJump = 0;
  u32 VertLinksJump = 0;
  u32 TrisJump = 0;
  u32 ConnectsJump = 0;

  idx VertsCount;
  idx TrisCount;
  idx AnimsCount;
  idx ConnectsCount;
  idx VertLinksCount;
  idx TexturesCount;
  idx BoundBoxCount;
  idx BoundSphereCount;
  

  if ( PkgFile->Ver > PKG_VER_UN_200 )
    *PkgFile >> VertsJump;

  *PkgFile >> CINDEX( VertsCount );
  Verts->Resize( VertsCount );

  // Deus Ex mesh detection
  // I'm going to assume that this works since UTPT does the same exact thing
  if ( VertsCount > 0 && VertsJump > 0 && ( (VertsJump-PkgFile->Tell())/VertsCount ) == 8 )
    bDeusExMesh = true;

  for ( int i = 0; i < VertsCount; i++ )
  {
    (*Verts)[i].bDeusEx = bDeusExMesh;
    *PkgFile >> (*Verts)[i];
  }

  if ( PkgFile->Ver > PKG_VER_UN_200 )
    *PkgFile >> TrisJump;

  *PkgFile >> CINDEX( TrisCount );
  Tris->Resize( TrisCount );
  for ( int i = 0; i < TrisCount; i++ )  
    *PkgFile >> (*Tris)[i];

  *PkgFile >> CINDEX( AnimsCount );
  Anims->Resize( AnimsCount );
  for ( int i = 0; i < AnimsCount; i++ )
    *PkgFile >> (*Anims)[i];

  if ( PkgFile->Ver > PKG_VER_UN_200 )
    *PkgFile >> ConnectsJump;

  *PkgFile >> CINDEX( ConnectsCount );
  Connects->Resize( ConnectsCount );
  for ( int i = 0; i < ConnectsCount; i++ )
    *PkgFile >> (*Connects)[i];

  // Why are these stored twice???
  FBox BBox2;
  FSphere Sphere2;

  *PkgFile >> BBox2;
  *PkgFile >> Sphere2; 
*/
}

UAnimationNotify::UAnimationNotify()
  : UObject()
{
}

UAnimationNotify::~UAnimationNotify()
{
}

#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( UAnimationNotify );
IMPLEMENT_NATIVE_CLASS( UMesh );

BEGIN_PROPERTY_LINK( UAnimationNotify, 5 )
  LINK_NATIVE_ARRAY   ( AnimationNotify );
  LINK_NATIVE_PROPERTY( NumNotifies );
  LINK_NATIVE_PROPERTY( Owner );
  LINK_NATIVE_PROPERTY( bInitialized );
  LINK_NATIVE_PROPERTY( bErrorOccured );
END_PROPERTY_LINK()

