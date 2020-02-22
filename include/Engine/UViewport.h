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
 * UViewport.h - Object that manages a clients view
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Engine/UPlayer.h"
#include "Engine/URender.h"

class UClient;
class UTexture;

#define ZNEAR (0.1f)
#define ZFAR  (65536.0f)

class LIBUNR_API UViewport : public UPlayer
{
  DECLARE_NATIVE_CLASS( UViewport, UPlayer, CLASS_NoExport, Engine )
  UViewport();

  virtual bool Init( int InWidth = 0, int InHeight = 0 );
  virtual bool Exit();
  virtual void Show();
  virtual void Hide();
  virtual bool Resize( int NewWidth, int NewHeight ) { return false; }
  virtual bool SetCursor( UTexture* Cursor ) { return false; }
  virtual void SetViewportTitle( const char* NewTitle ) {}
  virtual void AssembleClipPlanes();

  int Width;
  int Height;
  int BitsPerPixel;
  const char* Title;

  // FOV properties
  float FOV;
  float TanHalfXFov;
  float TanHalfYFov;

  // Viewport clipping properties
  FPlane Frustum[4];
  FPlane NearPlane;
  FPlane FarPlane;

  UClient* Client;
  URenderDevice* RenderDevice;
};

