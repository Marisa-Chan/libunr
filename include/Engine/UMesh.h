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
 * UMesh.h - Mesh classes
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/UPrimitive.h"
#include "Engine/UTexture.h"

class AActor;

// "James Mesh Types"
// See http://paulbourke.net/dataformats/unreal/ for more info
#define JMT_Normal            0x0
#define JMT_TwoSided          0x1
#define JMT_Translucent       0x2
#define JMT_MaskedTwoSided    0x3
#define JMT_ModulatedTwoSided 0x4
#define JMT_WeaponTriangle    0x8

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

struct LIBUNR_API FMeshVert
{
  float X;
  float Y;
  float Z;
  bool bDeusEx;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FMeshVert& MV );
};

struct LIBUNR_API FMeshTri
{
  u16 Vertex[3];
  u8  VertexUV[3][2];
  u32 Flags;
  u32 TextureIndex;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FMeshTri& MT );
};

struct LIBUNR_API FMeshAnimFunc
{
  float Time;
  idx FuncIdx;
  UFunction* Function;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FMeshAnimFunc& MAF );
};

struct LIBUNR_API FMeshAnimSeq
{
  FName Name;
  FName Group;
  u32   StartFrame;
  u32   NumFrames;
  TArray<FMeshAnimFunc> Functions;
  float Rate;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FMeshAnimSeq& MA );
};

struct LIBUNR_API FMeshVertConnect
{
  u32 NumVertTriangles;
  u32 TriangleListOffset;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FMeshVertConnect& MVC );
};

class LIBUNR_API UMesh : public UPrimitive
{
  DECLARE_NATIVE_CLASS( UMesh, UPrimitive, CLASS_NoExport, Engine )

  UMesh();
  virtual void Load();

  TArray<FMeshVert> Verts;
  TArray<FMeshTri>  Tris;
  TArray<FMeshAnimSeq> Anims;
  TArray<FMeshVertConnect> Connects;
  TArray<u32> VertLinks;
  TArray<UTexture*> Textures;
  TArray<FBox> BoundingBoxes; // Boundaries per frame, need to store?
  TArray<FSphere> BoundingSpheres; // ''
  TArray<float> TextureLODs;
  u32 FrameVerts;
  u32 AnimFrames;
  FVector Scale;
  FVector Origin;
  FRotator RotOrigin;
  bool bCurvyMesh;
  bool bDeusExMesh;
};

class LIBUNR_API UAnimationNotify : public UObject
{
  DECLARE_NATIVE_CLASS( UAnimationNotify, UObject, 0, Engine )
  EXPOSE_TO_USCRIPT()

  UAnimationNotify();

  struct AnimNotify
  {
    idx AnimName;
    idx FunctionName;
    int KeyFrame;
    EAnimNotifyEval NotifyEval;
    bool bCallOncePerLoop;
    bool bCalculatedFrame;
    bool bAlreadyCalled;
    int NumFrames;
    float CallKey;
  };

  AnimNotify AnimationNotify[255];
  int NumNotifies;
  AActor* Owner;
  bool bInitialized, bErrorOccured;
};

