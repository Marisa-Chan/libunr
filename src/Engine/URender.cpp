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

void URenderDevice::GetModelMatrix( FMatrix4x4& Mat, FVector& Location, FRotator& Rotation, FVector& Scale )
{
  FMatrix4x4 RotMat;
  FMatrix4x4 ScaleMat;

  // Set up translation matrix first
  Location.GetTranslationMatrix( Mat );

  // Get rotation matrix and multiply
  Rotation.GetMatrix( RotMat );
  Mat *= RotMat;

  // Get scale matrix and multiply
  Scale.GetScaleMatrix( ScaleMat );
  Mat *= ScaleMat;
}

void URenderDevice::GetViewMatrix( FMatrix4x4& Mat, FVector& ViewLoc, FRotator& ViewRot )
{
  // Convert rotation to radians
  FVector Rads;
  ViewRot.GetRadians( Rads );

  // Get a vector for where the camera is looking at (directly in front)
  float cosYaw = cosf( Rads.Y );
  float sinYaw = sinf( Rads.Y );
  float cosPitch = cosf( Rads.X );
  float sinPitch = sinf( Rads.X );

  FVector Direction( cosYaw * sinPitch, sinYaw, cosYaw * cosPitch );

  // Now get the orthogonal vector to the right of the camera
  // TODO: The camera roll is factored in here
  float PiOverTwo = 3.14f * 0.5f;
  float cosPitchRight = cosf( Rads.X - PiOverTwo );
  float sinPitchRight = sinf( Rads.X - PiOverTwo );

  FVector Right( sinPitchRight, 0, cosPitchRight );

  // Get the up vector by taking a cross product of direction and right
  FVector Up( Right );
  Up.Cross( Direction );

  // Calculate view matrix
  FVector F( Direction );
  F.Normalize();

  FVector S( Up );
  S.Cross( F );
  S.Normalize();

  FVector U( F );
  U.Cross( S );

  float SDotView = S.Dot( ViewLoc );
  float UDotView = U.Dot( ViewLoc );
  float FDotView = F.Dot( ViewLoc );

  Mat.Data[0][0] = S.X; 
  Mat.Data[0][1] = S.Y; 
  Mat.Data[0][2] = S.Z;
  Mat.Data[1][0] = U.X; 
  Mat.Data[1][1] = U.Y;    
  Mat.Data[1][2] = U.Z;
  Mat.Data[2][0] = F.X;
  Mat.Data[2][1] = F.Y;
  Mat.Data[2][2] = F.Z;
  Mat.Data[3][0] = -SDotView;
  Mat.Data[3][1] = -UDotView;
  Mat.Data[3][2] = -FDotView;
  Mat.Data[3][3] = 1.0f;
}

// TODO: Platform specific optimizations
void URenderDevice::GetOrthoMatrix( FMatrix4x4& Mat, float Left, float Right, float Top, float Bottom, float zNear, float zFar )
{
  float zFarMinusNear = (zFar - zNear);
  float RightMinusLeft = (Right - Left);
  float TopMinusBottom = (Top - Bottom);

  // Validate parameters
  if ( fabsf( RightMinusLeft ) <= FLT_EPSILON || fabsf( TopMinusBottom ) <= FLT_EPSILON || fabsf( zFarMinusNear ) <= FLT_EPSILON )
  {
    GLogf( LOG_WARN, "Invalid ortho matrix parameters" );
    return;
  }

  memset( &Mat, 0, sizeof( Mat ) );
  Mat.Data[0][0] = 2.0f / RightMinusLeft;
  Mat.Data[1][1] = 2.0f / TopMinusBottom;
  Mat.Data[2][2] = -2.0f / zFarMinusNear;
  Mat.Data[3][0] = -(Right + Left) / RightMinusLeft;
  Mat.Data[3][1] = -(Top + Bottom) / TopMinusBottom;
  Mat.Data[3][2] = -(zFar + zNear) / zFarMinusNear;
  Mat.Data[3][3] = 1.0f;
}

// TODO: Platform specific optimizations
void URenderDevice::GetPerspectiveMatrix( FMatrix4x4& Mat, float FOV, float Width, float Height, float zNear, float zFar )
{
  float zFarMinusNear = (zFar - zNear);
  float Aspect = (Width) / (Height);
  float tanFov = tanf( FOV / 2 );

  // Validate parameters
  if ( Width <= FLT_EPSILON || Height <= FLT_EPSILON || fabsf( zFarMinusNear ) <= FLT_EPSILON )
  {
    GLogf( LOG_WARN, "Invalid ortho matrix parameters" );
    return;
  }

  memset( &Mat, 0, sizeof( Mat ) );
  Mat.Data[0][0] = 1 / (Aspect * tanFov);
  Mat.Data[1][1] = 1 / tanFov;
  Mat.Data[2][2] = -(zFar + zNear) / zFarMinusNear;
  Mat.Data[2][3] = -1.0f;
  Mat.Data[3][2] = -(2.0f * zFar * zNear) / zFarMinusNear;
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

