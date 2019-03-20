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
 * AHUD.h - Heads-Up Display Class
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Actors/AHUD.h"

AHUD::AHUD()
  : AActor()
{
}

AHUD::~AHUD()
{
}

AHUDOverlay::AHUDOverlay()
  : AActor()
{
}

AHUDOverlay::~AHUDOverlay()
{
}

IMPLEMENT_NATIVE_CLASS( AHUD );
IMPLEMENT_NATIVE_CLASS( AHUDOverlay );

BEGIN_PROPERTY_LINK( AHUD, 9 )
  LINK_NATIVE_PROPERTY( HudMode );
  LINK_NATIVE_PROPERTY( Crosshair );
  LINK_NATIVE_PROPERTY( MainMenuType );
  LINK_NATIVE_PROPERTY( HUDConfigWindowType );
  LINK_NATIVE_PROPERTY( MainMenu );
  LINK_NATIVE_PROPERTY( Overlays );
  LINK_NATIVE_PROPERTY( WhiteColor );
  LINK_NATIVE_PROPERTY( HUDMutator );
  LINK_NATIVE_PROPERTY( PlayerOwner );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AHUDOverlay, 3 )
  LINK_NATIVE_PROPERTY( myHUD );
  LINK_NATIVE_PROPERTY( bPostRender );
  LINK_NATIVE_PROPERTY( bPreRender );
END_PROPERTY_LINK()


