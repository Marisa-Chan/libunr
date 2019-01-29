/*========================================================================*\
|*  This file is part of libunr.                                          *|
|*                                                                        *|
|*  libunr is free software: you can redistribute it and/or modify        *|
|*  it under the terms of the GNU General Public License as published by  *|
|*  the Free Software Foundation, either version 3 of the License, or     *|
|*  (at your option) any later version.                                   *|
|*                                                                        *|
|*  libunr is distributed in the hope that it will be useful,             *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of        *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *|
|*  GNU General Public License for more details.                          *|
|*                                                                        *|
|*  You should have received a copy of the GNU General Public License     *|
|*  along with libunr.  If not, see <http://www.gnu.org/licenses/>.       *|
|*                                                                        *|
\*========================================================================*/

/*========================================================================
 * UModel.h - Model class, used for brushes and levels
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/UPrimitive.h"
#include "Core/UTexture.h"

// 227i limits
#define MAX_NODES  131072
#define MAX_POINTS 131072
#define MAX_VERTICES 32
#define VERTEX_THRESHOLD MAX_VERTICES-2
#define MAX_NODE_VERTICES 16
#define MAX_FINAL_VERTICES 24
#define MAX_ZONES 64

/*-----------------------------------------------------------------------------
 * Model data for package version 68 or higher
-----------------------------------------------------------------------------*/
struct FProjector
{
  class AProjector* Projector;
  Array<int> Nodes;
};

struct FDecal
{
  FVector Vertices[4];
  class ADecal* Decal;
  Array<int> Nodes;
};

struct DLL_EXPORT FPoly
{
  friend FPackageFileIn& operator>>( FPackageFileIn& Ar, FPoly& P );

  FVector Base;
  FVector Normal;
  FVector TextureU;
  FVector TextureV;
  FVector Vertex[MAX_VERTICES];
  u32     PolyFlags;
  class ABrush* Brush;
  UTexture* Texture;
  idx     ItemName;
  int     NumVertices;
  int     iLink;
  int     iBrushPoly;
  i16     PanU, PanV;
};

class DLL_EXPORT UPolys : public UObject
{
  DECLARE_NATIVE_CLASS( UPolys, UObject, CLASS_RuntimeStatic | CLASS_NoExport, Engine )
  UPolys();

  virtual void Load();

  Array<FPoly> Element;
};

struct DLL_EXPORT FBspNode
{
  friend FPackageFileIn& operator>>( FPackageFileIn& In, FBspNode& BN );

  FPlane Plane;
  u64    ZoneMask;
  int    iVertPool;
  int    iSurf;

  union
  {
    int iBack;
    int iChild;
  };
  int iFront;
  int iPlane;

  int iCollisionBound;
  int iRenderBound;
  u8  iZone[2];
  u8  NumVertices;
  u8  NodeFlags;
  int iLeaf[2];
};

struct DLL_EXPORT FBspSurf
{
  friend FPackageFileIn& operator>>( FPackageFileIn& In, FBspSurf& BS );

  UTexture* Texture;
  u32 PolyFlags;
  int pBase;
  int vNormal;
  int vTextureU;
  int vTextureV;
  int iLightMap;
  int iBrushPoly;
  i16 PanU;
  i16 PanV;
  class ABrush* Brush;
  Array<FDecal> Decals;
  Array<FProjector> Projectors;
  Array<int> Nodes;
};

struct DLL_EXPORT FLightMapIndex
{
  int DataOffset;
  int iLightActors;
  FVector Pan;
  float UScale, VScale;
  int   UClamp, VClamp;
  u8    UBits,  VBits;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FLightMapIndex& LMI )
  {
    In >> LMI.DataOffset;
    In >> LMI.Pan;
    In >> CINDEX( LMI.UClamp );
    In >> CINDEX( LMI.VClamp );
    In >> LMI.UScale;
    In >> LMI.VScale;
    In >> LMI.iLightActors;
    return In;
  }

};

struct FVert
{
  idx pVertex;
  idx iSide;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FVert& V )
  {
    In >> CINDEX( V.pVertex );
    In >> CINDEX( V.iSide );
    return In;
  }
};

struct FLeaf
{
  idx iZone;
  idx iPermeating;
  idx iVolumetric;
  u64 VisibleZones;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FLeaf& L )
  {
    In >> CINDEX( L.iZone );
    In >> CINDEX( L.iPermeating );
    In >> CINDEX( L.iVolumetric );
    In >> L.VisibleZones;
  }
};

struct FZoneProperties
{
  class AZoneInfo* ZoneInfo;
  float LastRenderTime;
  u64   Connectivity;
  u64   Visibility;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FZoneProperties& ZP );
};

/*-----------------------------------------------------------------------------
 * UVectors
 * Old Vectors object
-----------------------------------------------------------------------------*/
class DLL_EXPORT UVectors : public UObject
{
  DECLARE_NATIVE_CLASS( UVectors, UObject, CLASS_NoExport, Engine )
  UVectors();

  virtual void Load();

  Array<FVector>* Data;
};

/*-----------------------------------------------------------------------------
 * UBspNodes
 * Old BspNodes object
-----------------------------------------------------------------------------*/
class DLL_EXPORT UBspNodes : public UObject
{
  DECLARE_NATIVE_CLASS( UBspNodes, UObject, CLASS_NoExport, Engine )
  UBspNodes();

  virtual void Load();

  Array<FBspNode>* Data;
};

/*-----------------------------------------------------------------------------
 * UBspSurfs
 * Old BspSurfs object
-----------------------------------------------------------------------------*/
class DLL_EXPORT UBspSurfs : public UObject
{
  DECLARE_NATIVE_CLASS( UBspSurfs, UObject, CLASS_NoExport, Engine )
  UBspSurfs();

  virtual void Load();

  Array<FBspSurf>* Data;
};

/*-----------------------------------------------------------------------------
 * UVerts
 * Old Verts object
-----------------------------------------------------------------------------*/
class DLL_EXPORT UVerts : public UObject
{
  DECLARE_NATIVE_CLASS( UVerts, UObject, CLASS_NoExport, Engine )
  UVerts();

  virtual void Load();

  Array<FVert>* Data;
};

/*-----------------------------------------------------------------------------
 * UModel
 * Holds polygon and BSP information for a level object
-----------------------------------------------------------------------------*/
class DLL_EXPORT UModel : public UPrimitive
{
  DECLARE_NATIVE_CLASS( UModel, UPrimitive, CLASS_NoExport, Engine )
  UModel();

  virtual void Load();

  UPolys* Polys;
  Array<FBspNode> Nodes;
  Array<FVert>    Verts;
  Array<FVector>  Vectors;
  Array<FVector>  Points;
  Array<FBspSurf> Surfs;
  Array<FLightMapIndex> LightMap;
  Array<u8> LightBits;
  Array<FBox>  Bounds;
  Array<int>   LeafHulls;
  Array<FLeaf> Leaves;
  Array<AActor*> Lights;

  int NumSharedSides;
  int NumZones;
  FZoneProperties Zones[MAX_ZONES];
};

