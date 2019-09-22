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
 * ULodMesh.h - "Level of Detail" Mesh Class
 * See the following in in UT-Package-File-Format.pdf
 *   * Class 'LodMesh'
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Engine/UMesh.h"

// "James Mesh Types"
// See http://paulbourke.net/dataformats/unreal/ for more info
#define JMT_Normal            0x0
#define JMT_TwoSided          0x1
#define JMT_Translucent       0x2
#define JMT_MaskedTwoSided    0x3
#define JMT_ModulatedTwoSided 0x4
#define JMT_WeaponTriangle    0x8

struct DLL_EXPORT FLodFace
{
  u16 WedgeIndex[3];
  u16 MaterialIndex;
};

struct DLL_EXPORT FLodWedge
{
  u16 VertexIndex;

  // Store UV instead of ST?
  u8  S; // U
  u8  T; // 255 - V
};

struct DLL_EXPORT FLodMaterial
{
  u32 Flags;
  u32 TextureIndex;
};

// Vertex mesh file format can be found here
// http://paulbourke.net/dataformats/unreal/
struct FVertexDataHeader
{
  u16 NumPolygons;
  u16 NumVertices;
  u16 BogusRot;
  u16 BogusFrame;
  u32 BogusNormX;
  u32 BogusNormY;
  u32 BogusNormZ;
  u32 FixScale;
  u32 Unused[3];
  u8  Unknown[12];
};

struct FVertexDataTri
{
  u16 Vertex[3];
  i8  Type;
  i8  Color;
  u8  VertexUV[3][2];
  i8  TexNum;
  i8  Flags; // unused
};

struct FVertexAnivHeader
{
  u16 NumFrames;
  u16 FrameSize;
};

class DLL_EXPORT ULodMesh : public UMesh
{
  DECLARE_NATIVE_CLASS( ULodMesh, UMesh, CLASS_NoExport, Engine )

  ULodMesh();
  virtual void Load();
  virtual bool ExportToFile( const char* Dir, const char* Type );
  virtual bool ExportToFile( const char* Dir, const char* Type, int Frame = -1 );

  TArray<u16> CollapsePoints;
  TArray<u16> FaceLevels;
  TArray<FLodFace> Faces;
  TArray<u16> CollapseWedges;
  TArray<FLodWedge> Wedges;
  TArray<FLodMaterial> Materials;
  TArray<FLodFace> SpecialFaces;
  TArray<u16> ReMapAnimVerts;
  u32 ModelVerts;
  u32 SpecialVerts;
  float MeshScaleMax;
  float LODHysteresis;
  float LODStrength;
  u32 LODMinVerts;
  float LODMorph;
  float LODZDisplace;
  u32 OldFrameVerts;

private:
  bool ExportUnreal3DMesh( const char* Dir, int Frame );
  bool ExportObjMesh( const char* Dir, int Frame );
};

