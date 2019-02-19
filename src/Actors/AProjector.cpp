/*========================================================================*\
|*  This file is part of libunr.                                          *|
|*                                                                        *|
|*  libunr is free software: you can redistribute it and/or modify        *|
|*  it under the terms of the GNU General Public License as published by  *|
|*  the Free Software Foundation, either version 3 of the License, or     *|
|*  (at your option) any later version.                                   *|
|*                                                                        *|
|*  libunr is distributed in the hope that it will be useful,             *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of        *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *|
|*  GNU General Public License for more details.                          *|
|*                                                                        *|
|*  You should have received a copy of the GNU General Public License     *|
|*  along with libunr.  If not, see <http://www.gnu.org/licenses/>.       *|
|*                                                                        *|
\*========================================================================*/

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

BEGIN_PROPERTY_LINK( AProjector, 17 )
  LINK_NATIVE_ARRAY   ( AProjector, FrustrumPlanes );
  LINK_NATIVE_PROPERTY( AProjector, DecalActors );
  LINK_NATIVE_PROPERTY( AProjector, DecalNodes );
  LINK_NATIVE_PROPERTY( AProjector, Box );
  LINK_NATIVE_ARRAY   ( AProjector, VisBox );
  LINK_NATIVE_PROPERTY( AProjector, TexData );
  LINK_NATIVE_PROPERTY( AProjector, ProjectTexture );
  LINK_NATIVE_PROPERTY( AProjector, ProjectStyle );
  LINK_NATIVE_PROPERTY( AProjector, FOV );
  LINK_NATIVE_PROPERTY( AProjector, bProjectActors );
  LINK_NATIVE_PROPERTY( AProjector, bProjectBSPBackfaces );
  LINK_NATIVE_PROPERTY( AProjector, bProjectMeshBackfaces );
  LINK_NATIVE_PROPERTY( AProjector, bProjectBSP );
  LINK_NATIVE_PROPERTY( AProjector, bGradualFade );
  LINK_NATIVE_PROPERTY( AProjector, bUseBetterActorAttach );
  LINK_NATIVE_PROPERTY( AProjector, bHasAttached );
  LINK_NATIVE_PROPERTY( AProjector, bProjecting );
END_PROPERTY_LINK()

