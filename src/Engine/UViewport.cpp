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
 * UViewport.cpp - Base viewport code
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Util/FConfig.h"
#include "Engine/UEngine.h"
#include "Engine/UViewport.h"
#include "Actors/APlayerPawn.h"

UViewport::UViewport()
  : UPlayer()
{
  Width = -1;
  Height = -1;
  BitsPerPixel = 0;
}

UViewport::~UViewport()
{
}

bool UViewport::Init( int InWidth, int InHeight )
{
  // Read config settings
  // TODO: Default to native res
  Width = (InWidth) ? InWidth : GLibunrConfig->ReadInt32( "libunr", "RenderWidth", 0, 1024 );
  Height = (InHeight) ? InHeight : GLibunrConfig->ReadInt32( "libunr", "RenderHeight", 0, 768 );
  BitsPerPixel = GLibunrConfig->ReadUInt8( "libunr", "RenderBpp", 0, 24 ); // Usually ignored

  Fov = GLibunrConfig->ReadFloat( "libunr", "RenderFOV", 0, 90.0f );
  TanHalfXFov = tanf( Fov / 2.0f );
  TanHalfYFov = ((float)Height / (float)Width) * TanHalfXFov;

  Client = GEngine->Client;
  return true;
}

bool UViewport::Exit()
{
  return false;
}

void UViewport::Show()
{

}

void UViewport::Hide()
{

}

void UViewport::SetupFrustum()
{
  FVector Fwd, Right, Up;
  Actor->Rotation.GetAxes( Fwd, Right, Up );

  FVector RTHXF = (Right * TanHalfXFov);
  FVector UTHYF = (Up * TanHalfYFov);
  FVector NRTHXF = -RTHXF;
  FVector NUTHYF = -UTHYF;
  FVector FwdNear = Fwd * ZNEAR;
  FVector FwdFar = Fwd * ZFAR;

  RTHXF += Fwd;
  NRTHXF += Fwd;
  UTHYF += Fwd;
  NUTHYF += Fwd;
  FwdNear += Actor->Location;
  FwdFar += Actor->Location;

  // Assemble view frustum
  *((FVector*)&Frustum[0]) = Cross( Up, RTHXF );
  *((FVector*)&Frustum[1]) = Cross( NRTHXF, Up );
  *((FVector*)&Frustum[2]) = Cross( UTHYF, Right );
  *((FVector*)&Frustum[3]) = Cross( Right, NUTHYF );

  for ( int i = 0; i < 4; i++ )
    Frustum[i].W = Dot( Frustum[i], Actor->Location );

  // Assemble near clipping plane
  NearPlane = Fwd;
  NearPlane.W = Dot( NearPlane, FwdNear );

  *((FVector*)&FarPlane) = -Fwd;
  FarPlane.W = Dot( FarPlane, FwdFar );
}

bool UViewport::IsBoxVisible( FBox& Box )
{
  // Check all frustum planes to make sure box is in all of them
  for ( int i = 0; i < 4; i++ )
  {
    if ( Frustum[i].FindBoxSide( Box ) == ORIENT_BACK )
      return false;
  }

  // All planes passed
  return true;
}


#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( UViewport );

