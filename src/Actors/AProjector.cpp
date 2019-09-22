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
 * AProjector.cpp - Projector class implementation
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Actors/AProjector.h"

AProjector::AProjector()
  : AActor()
{
}

AProjector::~AProjector()
{
}

IMPLEMENT_NATIVE_CLASS( AProjector );

BEGIN_PROPERTY_LINK( AProjector, 19 )
  LINK_NATIVE_ARRAY   ( FrustrumPlanes );
  LINK_NATIVE_PROPERTY( DecalActors );
  LINK_NATIVE_PROPERTY( DecalNodes );
  LINK_NATIVE_PROPERTY( Box );
  LINK_NATIVE_ARRAY   ( VisBox );
  LINK_NATIVE_PROPERTY( TexData );
  LINK_NATIVE_PROPERTY( ProjectTexture );
  LINK_NATIVE_PROPERTY( MaxDistance );
  LINK_NATIVE_PROPERTY( ProjectorScale );
  LINK_NATIVE_PROPERTY( ProjectStyle );
  LINK_NATIVE_PROPERTY( FOV );
  LINK_NATIVE_PROPERTY( bProjectActors );
  LINK_NATIVE_PROPERTY( bProjectBSPBackfaces );
  LINK_NATIVE_PROPERTY( bProjectMeshBackfaces );
  LINK_NATIVE_PROPERTY( bProjectBSP );
  LINK_NATIVE_PROPERTY( bGradualFade );
  LINK_NATIVE_PROPERTY( bUseBetterActorAttach );
  LINK_NATIVE_PROPERTY( bHasAttached );
  LINK_NATIVE_PROPERTY( bProjecting );
END_PROPERTY_LINK()

