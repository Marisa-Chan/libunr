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
 * UMesh.cpp - Mesh functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Util/FString.h"
#include "Core/USystem.h"
#include "Engine/UMesh.h"
#include "Engine/UModel.h"

FPackageFileIn& operator>>( FPackageFileIn& In, FMeshVert& MV )
{
  if ( MV.bDeusEx )
  {
    u64 XYZ;
    In >> XYZ;

    MV.X = (XYZ & 0xFFFF) / 256.0f;
    MV.Y = ((XYZ >> 16) & 0xFFFF) / 256.0f;
    MV.Z = ((XYZ >> 32) & 0xFFFF) / 256.0f;
  }
  else
  {
    u32 XYZ;
    In >> XYZ;

    MV.X = (XYZ & 0x7FF) / 8.0f;
    MV.Y = ((XYZ >> 11) & 0x7FF) / 8.0f;
    MV.Z = ((XYZ >> 22) & 0x3FF) / 4.0f;
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
  In.Read( &MT, sizeof(FMeshTri) );
  return In;
}

FPackageFileIn& operator>>( FPackageFileIn& In, FMeshAnimFunc& MAF )
{
  In >> MAF.Time;

  // We'll get the actual function object if we need it later
  // Chances are, if we need it then the function will already be loaded
  // since the mesh would've been loaded from a defaultproperty list.
  In >> CINDEX( MAF.FuncIdx );

  return In; 
}

FPackageFileIn& operator>>( FPackageFileIn& In, FMeshAnimSeq& MA )
{
  In >> MA.Name;
  In >> MA.Group;
  In >> MA.StartFrame;
  In >> MA.NumFrames;

  idx FunctionCount;
  In >> CINDEX( FunctionCount );
  MA.Functions.Resize( FunctionCount );

  for ( int i = 0; i < FunctionCount; i++ )
    In >> MA.Functions[i];

  In >> MA.Rate;

  return In;
}

FPackageFileIn& operator>>( FPackageFileIn& In, FMeshVertConnect& MVC )
{
  In.Read( &MVC, sizeof(FMeshVertConnect) );
  return In;  
}

UMesh::UMesh()
  : UPrimitive()
{
  bDeusExMesh = false;
}

UMesh::~UMesh()
{
}

void UMesh::Load()
{
  Super::Load();

  FPackageFileIn& In = *PkgFile;

  u32 VertsJump = 0;
  u32 VertLinksJump = 0;
  u32 TrisJump = 0;
  u32 ConnectsJump = 0;

  idx VertsCount;
  //idx TrisCount;
  //idx AnimsCount;
  //idx ConnectsCount;
  //idx VertLinksCount;
  idx TexturesCount;
  //idx BoundBoxCount;
  //idx BoundSphereCount;
  idx TexLODCount;

  if ( In.Ver > PKG_VER_UN_200 )
    In >> VertsJump;

  In >> CINDEX( VertsCount );
  Verts.Resize( VertsCount );

  // Deus Ex mesh detection
  if ( VertsCount > 0 && VertsJump > 0 && ( (VertsJump - In.Tell())/VertsCount ) == 8 )
    bDeusExMesh = true;

  for ( int i = 0; i < VertsCount; i++ )
  {
    Verts[i].bDeusEx = bDeusExMesh;
    In >> Verts[i];
  }

  if ( PkgFile->Ver > PKG_VER_UN_200 )
    In >> TrisJump;

  In >> Tris;
  In >> Anims;

  if ( PkgFile->Ver > PKG_VER_UN_200 )
    In >> ConnectsJump;

  In >> Connects;

  // Why are these stored twice???
  FBox BBox2;
  FSphere Sphere2;

  In >> BBox2 >> Sphere2; 

  In >> VertLinksJump;
  In >> VertLinks;

  In >> CINDEX( TexturesCount );
  Textures.Resize( TexturesCount );
  for ( int i = 0; i < TexturesCount; i++ )
  {
    idx ObjRef;
    In >> CINDEX( ObjRef );
    Textures[i] = (UTexture*)LoadObject( ObjRef, UTexture::StaticClass(), NULL );
  }

  In >> BoundingBoxes;
  In >> BoundingSpheres;
  In >> FrameVerts >> AnimFrames;

  u32 ANDFlags, ORFlags; // ???
  In >> ANDFlags >> ORFlags;

  In >> Scale >> Origin >> RotOrigin;
  
  u32 CurPoly, CurVertex; // ???
  In >> CurPoly >> CurVertex;

  if ( In.Ver == PKG_VER_UN_222 )
  {
    TextureLODs.Resize( 1 );
    In >> TextureLODs[0];
  }
  else if ( In.Ver > PKG_VER_UN_222 )
  {
    In >> CINDEX( TexLODCount );
    TextureLODs.Resize( TexLODCount );
    for ( int i = 0; i < TexLODCount; i++ )
      In >> TextureLODs[i];
  }
  else
  {
    TextureLODs.PushBack( 1.00f );
  }
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

