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

struct LIBUNR_API FLodFace
{
  u16 WedgeIndex[3];
  u16 MaterialIndex;
};

struct LIBUNR_API FLodWedge
{
  u16 VertexIndex;

  // Store UV instead of ST?
  u8  S; // U
  u8  T; // 255 - V
};

struct LIBUNR_API FLodMaterial
{
  u32 Flags;
  u32 TextureIndex;
};

class LIBUNR_API ULodMesh : public UMesh
{
  DECLARE_NATIVE_CLASS( ULodMesh, UMesh, CLASS_NoExport, Engine )

  ULodMesh();
  virtual void Load();

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

protected:
  virtual bool ExportUnreal3DMesh( const char* Dir, int Frame );
  virtual bool ExportObjMesh( const char* Dir, int Frame );
};

