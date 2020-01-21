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
 * UPlayer.cpp - Player object functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Core/UPackage.h"
#include "Engine/UPlayer.h"

UPlayer::UPlayer()
  : UObject()
{
}

UPlayer::~UPlayer()
{
}

void UPlayer::Possess( APlayerPawn* PlayerPawn )
{
  Actor = PlayerPawn;
  // PlayerPawn->EventPossess();
}

IMPLEMENT_NATIVE_CLASS( UPlayer );

BEGIN_PROPERTY_LINK( UPlayer, 15 )
  LINK_USELESS_PROPERTY( vfOut );
  LINK_USELESS_PROPERTY( vfExec );
  LINK_NATIVE_PROPERTY( Actor );
  LINK_NATIVE_PROPERTY( Console );
  LINK_NATIVE_PROPERTY( bWindowsMouseAvailable );
  LINK_NATIVE_PROPERTY( bShowWindowsMouse );
  LINK_NATIVE_PROPERTY( WindowsMouseX );
  LINK_NATIVE_PROPERTY( WindowsMouseY );
  LINK_NATIVE_PROPERTY( SelectedCursor );
  LINK_NATIVE_PROPERTY( bSuspendPrecaching );
  LINK_NATIVE_PROPERTY( CurrentNetSpeed );
  LINK_NATIVE_PROPERTY( ConfiguredInternetSpeed );
  LINK_NATIVE_PROPERTY( ConfiguredLanSpeed );
  LINK_NATIVE_PROPERTY( StaticUpdateInterval );
  LINK_NATIVE_PROPERTY( DynamicUpdateInterval );
END_PROPERTY_LINK()

