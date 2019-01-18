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

// 227i limits
#define MAX_NODES  131072
#define MAX_POINTS 131072
#define MAX_VERTICES 32
#define VERTEX_THRESHOLD MAX_VERTICES-2
#define MAX_NODE_VERTICES 16
#define MAX_FINAL_VERTICES 24
#define MAX_ZONES 64

class DLL_EXPORT FPoly
{
  FPoly();
  friend FPackageFileIn& operator>>( FPackageFileIn& Ar, FPoly& Poly );

  FVector Base;
  FVector Normal;
  FVector TextureU;
  FVector TextureV;
  FVector Vertex[MAX_VERTICES];
  u32     PolyFlags;
  class ABrush* Brush;
  UTexture* Texture;
  idx     Item;
  int     NumVertices;
  int     iLink;
  int     iBrushPoly;
  i16     PanU, PanV;
};

class DLL_EXPORT UPolys : public UObject
{
  DECLARE_NATIVE_CLASS( UPolys, UObject, CLASS_RuntimeStatic, Engine )
  UPolys();

  virtual void Load();

  Array<FPoly> Element;
};

class DLL_EXPORT FBspNode
{
public:
  FBspNode();
  friend FPackageFileIn& operator>>( FPackageFileIn& In, FBspNode& BspNode )
  {
    In >> Plane >> ZoneMask >> NodeFlags >> CINDEX( iVertPool );
    In >> CINDEX( iSurf ) >> CINDEX( iBack ) >> CINDEX( iFront ) >> CINDEX( iPlane );
    In >> CINDEX( iCollisionBound ) >> CINDEX( iRenderBound ) >> iZone[0] >> iZone[1];
    In >> NumVertices >> iLeaf[0] >> iLeaf[1];
    return In;
  }

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

class DLL_EXPORT FBspSurf
{
public:
  FBspSurf();
  friend FPackageFileIn& operator>>( FPackageFileIn& Ar, FBspSurf& BspSurf );

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

class DLL_EXPORT FLightMapIndex
{
public:
  FLightMapIndex();
  friend FPackageFileIn& operator>>( FPackageFileIn& Ar, FLightMapIndex& LightMapIndex );

  int DataOffset;
  int iLightActors;
  FVector Pan;
  float UScale, VScale;
  int   UClamp, VClamp;
  u8    UBits,  VBits;
};

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
};

