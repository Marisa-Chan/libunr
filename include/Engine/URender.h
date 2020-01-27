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
 * URender.h - Rendering classes
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/UMath.h"
#include "Core/USystem.h"
#include "Engine/UTexture.h"
#include "Engine/UFire.h"

class AActor;
class APlayerPawn;
class ULevel;
class UMesh;
class UViewport;

/*-----------------------------------------------------------------------------
 * URenderIterator
 * Defines an object which can render many things at one time
-----------------------------------------------------------------------------*/
class LIBUNR_API URenderIterator : public UObject
{
  DECLARE_NATIVE_CLASS( URenderIterator, UObject, 0, Engine )
  EXPOSE_TO_USCRIPT()

  URenderIterator();

  // Leaving out some structs here until they're actually needed
  // struct ActorBuffer
  // {
  //   u8 Padding[564];
  // };
  //
  // struct ActorNode
  // {
  //   ActorBuffer ActorProxy;
  //   ActorNode*  NextNode;
  // };

  int MaxItems;
  int Index;
  APlayerPawn* Observer;
  void* Frame;
};

/*-----------------------------------------------------------------------------
 * FPostRenderNode
 * Defines a set of geometry to draw after the scene has been rendered
-----------------------------------------------------------------------------*/
struct FPostRenderNode
{
  FBox Dim;
  FRotator Rot;
};

/*-----------------------------------------------------------------------------
 * URenderDevice
 * Defines the base interface for render devices to display the world
-----------------------------------------------------------------------------*/
class LIBUNR_API URenderDevice : public USubsystem
{
  DECLARE_NATIVE_CLASS( URenderDevice, USubsystem, CLASS_NoExport, Engine )

  URenderDevice();

  virtual bool Init() { return false; }
  virtual bool Exit() { return false; }
  virtual void Tick( float DeltaTime ) {}

  bool bAccelerateFractalTextures;

  /*-----------------------------------------------------------------------------
   * Matrix functions
   * Used for generating matrices for the render device to use
-  ----------------------------------------------------------------------------*/

  // Generates an orthographic matrix for 2D drawing
  virtual void GetOrthoMatrix( FMatrix4x4& Mat, float Left, float Right, float Top, float Bottom, float zNear, float zFar );

  // Generates a perspective matrix for 3D drawing
  virtual void GetPerspectiveMatrix( FMatrix4x4& Mat, float FOV, float Width, float Height, float zNear, float zFar );

  /*-----------------------------------------------------------------------------
   * Complex drawing functions
   * Used for drawing the world, including occlusion and lighting
-  ----------------------------------------------------------------------------*/

  // Draws the current world from the perspective of a viewport
  virtual void DrawWorld( ULevel* Level, UViewport* Viewport ) {}

  // Draws an actor in the world
  virtual void DrawActor( AActor* Actor ) {}

  /*-----------------------------------------------------------------------------
   * Simple drawing functions
   * Can be used for things like Canvas, texture and mesh browser, etc
  -----------------------------------------------------------------------------*/

  // Draws text flat to the screen
  virtual void DrawText( UFont* Font, FBoxInt2D& Dim, FString& Text, int PolyFlags = 0 ) {}

  // Draws a flat tile with a single texture
  virtual void DrawTile( UTexture* Tex, FBoxInt2D& Dim, FRotator& Rot, float U, float V, float UL, float VL, int PolyFlags = 0 ) {}

  // Draws a mesh on the screen
  virtual void DrawMesh( UMesh* Mesh, FName AnimSeq, float AnimRate, FVector& Loc, FVector& Scale, FRotator& Rot, int PolyFlags = 0 ) {}

  // Draws a frame of a fire texture with renderer accelerated method
  virtual void DrawFireTexFrame( UFireTexture* Tex, float DeltaTime ) {}

  /*-----------------------------------------------------------------------------
   * Utility functions
   * These generally have some specific, one-time purpose
  -----------------------------------------------------------------------------*/

  // Draws a grid in 3D space
  virtual void DrawGrid( FBox& Dim, FColor& Color ) {}

  // Test drawing a cube
  virtual void DrawCube( FVector& Loc, FRotator& Rot, FVector& Scale, UTexture* Tex ) {}

  // Initializes a viewport for use with the renderer
  virtual bool InitViewport( UViewport* Viewport ) { return false; }

  // Sets the current viewport to render to
  virtual bool SetActiveViewport( UViewport* Viewport ) { return false; }
};

/*-----------------------------------------------------------------------------
 * URenderBase
 * Deprecated, holds a reference to render device, which does all rendering
-----------------------------------------------------------------------------*/
class LIBUNR_API URenderBase : public USubsystem
{
  DECLARE_NATIVE_CLASS( URenderBase, USubsystem, CLASS_NoExport, Engine )

  URenderBase();
  URenderDevice* RenderDevice;
};

/*-----------------------------------------------------------------------------
 * UStaticLightData
 * TODO
-----------------------------------------------------------------------------*/
class LIBUNR_API UStaticLightData : public UObject
{
  DECLARE_NATIVE_CLASS( UStaticLightData, UObject, CLASS_NoExport, Engine )
  UStaticLightData();
};
