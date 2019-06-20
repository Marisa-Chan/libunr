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
 * UMesh.h - Mesh classes
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/UPrimitive.h"
#include "Core/UTexture.h"

class AActor;

struct DLL_EXPORT FMeshVert
{
  float X;
  float Y;
  float Z;
  bool bDeusEx;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FMeshVert& MV );
};

struct DLL_EXPORT FMeshTri
{
  u16 Vertex[3];
  u8  VertexUV[3][2];
  u32 Flags;
  u32 TextureIndex;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FMeshTri& MT );
};

struct DLL_EXPORT FMeshAnimFunc
{
  float Time;
  idx FuncIdx;
  UFunction* Function;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FMeshAnimFunc& MAF );
};

struct DLL_EXPORT FMeshAnim
{
  FName Name;
  FName Group;
  u32   StartFrame;
  u32   NumFrames;
  Array<FMeshAnimFunc> Functions;
  float Rate;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FMeshAnim& MA );
};

struct DLL_EXPORT FMeshVertConnect
{
  u32 NumVertTriangles;
  u32 TriangleListOffset;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FMeshVertConnect& MVC );
};

class DLL_EXPORT UMesh : public UPrimitive
{
  DECLARE_NATIVE_CLASS( UMesh, UPrimitive, CLASS_NoExport, Engine )

  UMesh();
  virtual void Load();

  Array<FMeshVert> Verts;
  Array<FMeshTri>  Tris;
  Array<FMeshAnim> Anims;
  Array<FMeshVertConnect> Connects;
  Array<u32> VertLinks;
  Array<UTexture*> Textures;
  Array<FBox> BoundingBoxes; // Boundaries per frame, need to store?
  Array<FSphere> BoundingSpheres; // ''
  Array<float> TextureLODs;
  u32 FrameVerts;
  u32 AnimFrames;
  FVector Scale;
  FVector Origin;
  FRotator RotOrigin;
  bool bCurvyMesh;
  bool bDeusExMesh;
};

class DLL_EXPORT UAnimationNotify : public UObject
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

