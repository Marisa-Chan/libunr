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

  FOV = GLibunrConfig->ReadFloat( "libunr", "RenderFOV", 0, 90.0f );
  TanHalfXFov = tan( FOV / 2.0 );
  TanHalfYFov = ((double)Height / (double)Width) * TanHalfXFov;

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

void UViewport::AssembleClipPlanes()
{
  // Get rotation axes
  FVector Fwd, Right, Up;
  Actor->Rotation.GetAxes( Fwd, Right, Up );

  // Assemble view frustum
  Frustum[0] = (Up    ^ (Fwd    + (Right * TanHalfXFov))).Normalize();
  Frustum[1] = ((Fwd  + (-Right * TanHalfXFov)) ^ Up).Normalize();
  Frustum[2] = ((Fwd  + (Up     * TanHalfYFov)) ^ Right).Normalize();
  Frustum[3] = (Right ^ (Fwd    + (-Up * TanHalfYFov))).Normalize();

  for ( int i = 0; i < 4; i++ )
    Frustum[i].W = Frustum[i] | Actor->Location;

  // Assemble near clipping plane
  NearPlane = Fwd;
  NearPlane.W = NearPlane | (Actor->Location + (Fwd * ZNEAR));

  FarPlane = -Fwd;
  FarPlane.W = FarPlane | (Actor->Location + (Fwd * ZFAR));
}

#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( UViewport );

