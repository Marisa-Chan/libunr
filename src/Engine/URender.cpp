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
 * URender.cpp - Rendering Functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Core/UPackage.h"
#include "Engine/URender.h"
//#include "APlayerPawn.h"

URenderIterator::URenderIterator()
  : UObject()
{
  MaxItems = -1;
  Index = -1;
  Observer = NULL;
  Frame = NULL;
}

URenderIterator::~URenderIterator()
{
  //if ( Observer )
  //  Observer->DelRef();
}

URenderDevice::URenderDevice()
  : USubsystem()
{
}

URenderDevice::~URenderDevice()
{
}

// TODO: Platform specific optimizations
void URenderDevice::GetOrthoMatrix( FMatrix4x4& Mat, float Left, float Right, float Top, float Bottom, float zNear, float zFar )
{
#ifndef ARCH_OPTIMIZATIONS
  // Validate parameters
  if ( fabsf( Left - Right ) <= FLT_EPSILON || fabsf( Top - Bottom ) <= FLT_EPSILON || fabsf( zNear - zFar ) <= FLT_EPSILON )
  {
    GLogf( LOG_WARN, "Invalid ortho matrix parameters" );
    return;
  }

  memset( &Mat, 0, sizeof( Mat ) );
  Mat.Data[0][0] = 2.0f / (Right - Left);
  Mat.Data[1][1] = 2.0f / (Top - Bottom);
  Mat.Data[2][2] = -2.0f / (zFar - zNear);
  Mat.Data[3][0] = -(Right + Left) / (Right - Left);
  Mat.Data[3][1] = -(Top + Bottom) / (Top - Bottom);
  Mat.Data[3][2] = -(zFar + zNear) / (zFar - zNear);
  Mat.Data[3][3] = 1.0f;
#endif
}

// TODO: Platform specific optimizations
void URenderDevice::GetPerspectiveMatrix( FMatrix4x4& Mat, float FOV, float Width, float Height, float zNear, float zFar )
{
  // Validate parameters
  if ( Width <= FLT_EPSILON || Height <= FLT_EPSILON || fabsf( zNear - zFar ) <= FLT_EPSILON )
  {
    GLogf( LOG_WARN, "Invalid ortho matrix parameters" );
    return;
  }

  float Aspect = (Width) / (Height);
  float tanFov = tanf( FOV / 2 );

  memset( &Mat, 0, sizeof( Mat ) );
  Mat.Data[0][0] = 1 / (Aspect * tanFov );
  Mat.Data[1][1] = 1 / tanFov;
  Mat.Data[2][2] = -(zFar + zNear) / (zFar - zNear);
  Mat.Data[2][3] = -1.0f;
  Mat.Data[3][2] = -(2 * zFar * zNear) / (zFar - zNear);
}

URenderBase::URenderBase()
  : USubsystem()
{
  RenderDevice = NULL;
}

URenderBase::~URenderBase()
{
  if ( RenderDevice )
    delete RenderDevice;
}

UStaticLightData::UStaticLightData()
  : UObject()
{
}

UStaticLightData::~UStaticLightData()
{
}

IMPLEMENT_NATIVE_CLASS( URenderBase );
IMPLEMENT_NATIVE_CLASS( URenderDevice );
IMPLEMENT_NATIVE_CLASS( URenderIterator );
IMPLEMENT_NATIVE_CLASS( UStaticLightData );

BEGIN_PROPERTY_LINK( URenderIterator, 4 )
  LINK_NATIVE_PROPERTY( MaxItems );
  LINK_NATIVE_PROPERTY( Index );
  LINK_NATIVE_PROPERTY( Observer );
  LINK_NATIVE_PROPERTY( Frame );
END_PROPERTY_LINK()

