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
 * UModel.h - Model class, used for brushes and levels
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/UPrimitive.h"
#include "Engine/UTexture.h"

// 227i limits
#define MAX_NODES  131072
#define MAX_POINTS 131072
#define MAX_VERTICES 32
#define VERTEX_THRESHOLD MAX_VERTICES-2
#define MAX_NODE_VERTICES 16
#define MAX_FINAL_VERTICES 24
#define MAX_ZONES 64

/*-----------------------------------------------------------------------------
 * Poly flags
 * See Engine/Inc/UnObj.h in any set of public headers
-----------------------------------------------------------------------------*/
enum EPolyFlags
{
  // Regular in-game flags.
  PF_Invisible        = 0x00000001,  // Poly is invisible.
  PF_Masked           = 0x00000002,  // Poly should be drawn masked.
  PF_Translucent      = 0x00000004,  // Poly is transparent.
  PF_NotSolid         = 0x00000008,  // Poly is not solid, doesn't block.
  PF_Environment      = 0x00000010,  // Poly should be drawn environment mapped.
  PF_Semisolid        = 0x00000020,  // Poly is semi-solid = collision solid, Csg nonsolid.
  PF_Modulated        = 0x00000040,  // Modulation transparency.
  PF_FakeBackdrop     = 0x00000080,  // Poly looks exactly like backdrop.
  PF_TwoSided         = 0x00000100,  // Poly is visible from both sides.
  PF_AutoUPan         = 0x00000200,  // Automatically pans in U direction.
  PF_AutoVPan         = 0x00000400,  // Automatically pans in V direction.
  PF_NoSmooth         = 0x00000800,  // Don't smooth textures.
  PF_BigWavy          = 0x00001000,  // Poly has a big wavy pattern in it.
  PF_SpecialPoly      = 0x00001000,  // Game-specific poly-level render control (reuse BigWavy flag)
  PF_SmallWavy        = 0x00002000,  // Small wavy pattern (for water/enviro reflection).
  PF_Flat             = 0x00004000,  // Flat surface.
  PF_ForceViewZone    = 0x00004000,  // Force current iViewZone in OccludeBSP (reuse Flat flag)
  PF_LowShadowDetail  = 0x00008000,  // Low detail shadows.
  PF_NoMerge          = 0x00010000,  // Don't merge poly's nodes before lighting when rendering.
  PF_AlphaBlend       = 0x00020000,  // This poly should be alpha blended
  PF_DirtyShadows     = 0x00040000,  // Dirty shadows.
  PF_BrightCorners    = 0x00080000,  // Brighten convex corners.
  PF_SpecialLit       = 0x00100000,  // Only speciallit lights apply to this poly.
  PF_Gouraud          = 0x00200000,  // Gouraud shaded.
  PF_NoBoundRejection = 0x00200000,  // Disable bound rejection in OccludeBSP (reuse Gourard flag)
  PF_Unlit            = 0x00400000,  // Unlit.
  PF_HighShadowDetail = 0x00800000,  // High detail shadows.
  PF_CustomShader     = 0x01000000,  // Use a custom shader specified in the texture
  PF_Portal           = 0x04000000,  // Portal between iZones.
  PF_Mirrored         = 0x08000000,  // Reflective surface.

  // Editor flags.
  PF_Memorized   = 0x01000000,  // Editor: Poly is remembered.
  PF_Selected    = 0x02000000,  // Editor: Poly is selected.
  PF_Highlighted = 0x10000000,  // Editor: Poly is highlighted.
  PF_FlatShaded  = 0x40000000,  // FPoly has been split by SplitPolyWithPlane.

  // Internal.
  PF_Detail       = 0x20000000,  // Polygon should blend detail texture with main texture
  PF_EdProcessed  = 0x40000000,  // FPoly was already processed in editorBuildFPolys.
  PF_EdCut        = 0x80000000,  // FPoly has been split by SplitPolyWithPlane.
  PF_RenderFog    = 0x40000000,  // Render with fogmapping.
  PF_Occlude      = 0x80000000,  // Occludes even if PF_NoOcclude.
  PF_RenderHint   = 0x01000000,  // Rendering optimization hint.

  // Combinations of flags.
  PF_NoOcclude  = PF_Masked | PF_Translucent | PF_Invisible | PF_Modulated | PF_AlphaBlend,
  PF_NoEdit     = PF_Memorized | PF_Selected | PF_EdProcessed | PF_NoMerge | PF_EdCut,
  PF_NoImport   = PF_NoEdit | PF_NoMerge | PF_Memorized | PF_Selected | PF_EdProcessed | PF_EdCut,
  PF_AddLast    = PF_Semisolid | PF_NotSolid,
  PF_NoAddToBSP = PF_EdCut | PF_EdProcessed | PF_Selected | PF_Memorized,
  PF_NoShadows  = PF_Unlit | PF_Invisible | PF_FakeBackdrop,
  PF_Transient  = PF_Highlighted,
};

/*-----------------------------------------------------------------------------
 * Model data for package version 68 or higher
-----------------------------------------------------------------------------*/
struct FProjector
{
  class AProjector* Projector;
  TArray<int> Nodes;
};

struct FDecal
{
  FVector Vertices[4];
  class ADecal* Decal;
  TArray<int> Nodes;
};

struct LIBUNR_API FPoly
{
  friend LIBUNR_API FPackageFileIn& operator>>( FPackageFileIn& Ar, FPoly& P );
  void ExportToLevelText( FFileArchiveOut* Out );

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

class LIBUNR_API UPolys : public UObject
{
  DECLARE_NATIVE_CLASS( UPolys, UObject, CLASS_RuntimeStatic | CLASS_NoExport, Engine )
  UPolys();

  virtual void Load();
  void ExportToLevelText( FFileArchiveOut* Out );

  TArray<FPoly> Element;
};

struct LIBUNR_API FBspNode
{
  friend LIBUNR_API FPackageFileIn& operator>>( FPackageFileIn& In, FBspNode& BN );

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

struct LIBUNR_API FBspSurf
{
  friend LIBUNR_API FPackageFileIn& operator>>( FPackageFileIn& In, FBspSurf& BS );

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
  TArray<FDecal> Decals;
  TArray<FProjector> Projectors;
  TArray<int> Nodes;
};

struct LIBUNR_API FLightMapIndex
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

struct LIBUNR_API FVert
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

struct LIBUNR_API FLeaf
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
    return In;
  }
};

struct LIBUNR_API FZoneProperties
{
  class AZoneInfo* ZoneInfo;
  float LastRenderTime;
  u64   Connectivity;
  u64   Visibility;

  friend LIBUNR_API FPackageFileIn& operator>>( FPackageFileIn& In, FZoneProperties& ZP );
};

/*-----------------------------------------------------------------------------
 * UVectors
 * Old Vectors object
-----------------------------------------------------------------------------*/
class LIBUNR_API UVectors : public UObject
{
  DECLARE_NATIVE_CLASS( UVectors, UObject, CLASS_NoExport, Engine )
  UVectors();

  virtual void Load();

  TArray<FVector>* Data;
};

/*-----------------------------------------------------------------------------
 * UBspNodes
 * Old BspNodes object
-----------------------------------------------------------------------------*/
class LIBUNR_API UBspNodes : public UObject
{
  DECLARE_NATIVE_CLASS( UBspNodes, UObject, CLASS_NoExport, Engine )
  UBspNodes();

  virtual void Load();

  TArray<FBspNode>* Data;
};

/*-----------------------------------------------------------------------------
 * UBspSurfs
 * Old BspSurfs object
-----------------------------------------------------------------------------*/
class LIBUNR_API UBspSurfs : public UObject
{
  DECLARE_NATIVE_CLASS( UBspSurfs, UObject, CLASS_NoExport, Engine )
  UBspSurfs();

  virtual void Load();

  TArray<FBspSurf>* Data;
};

/*-----------------------------------------------------------------------------
 * UVerts
 * Old Verts object
-----------------------------------------------------------------------------*/
class LIBUNR_API UVerts : public UObject
{
  DECLARE_NATIVE_CLASS( UVerts, UObject, CLASS_NoExport, Engine )
  UVerts();

  virtual void Load();

  TArray<FVert>* Data;
};

/*-----------------------------------------------------------------------------
 * UModel
 * Holds polygon and BSP information for a level object
-----------------------------------------------------------------------------*/
class LIBUNR_API UModel : public UPrimitive
{
  DECLARE_NATIVE_CLASS( UModel, UPrimitive, CLASS_NoExport, Engine )
  UModel();

  virtual void Load();

  UPolys* Polys;
  TArray<FBspNode> Nodes;
  TArray<FVert>    Verts;
  TArray<FVector>  Vectors;
  TArray<FVector>  Points;
  TArray<FBspSurf> Surfs;
  TArray<FLightMapIndex> LightMap;
  TArray<u8> LightBits;
  TArray<FBox>  Bounds;
  TArray<int>   LeafHulls;
  TArray<FLeaf> Leaves;
  TArray<AActor*> Lights;

  bool RootOutside;
  bool Linked;
  int MoverLink;
  int NumSharedSides;
  int NumZones;
  FZoneProperties Zones[MAX_ZONES];
};

/*-----------------------------------------------------------------------------
 * USkelModel
 * Skeletal Model object. Only seems to exist in Rune
-----------------------------------------------------------------------------*/
class LIBUNR_API USkelModel : public UPrimitive
{
  DECLARE_NATIVE_CLASS( USkelModel, UPrimitive, CLASS_NoExport, Engine )
  USkelModel();

  // TODO:
};

