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
 * UEngine.cpp - Base Engine Functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UEngine.h"

UClient::UClient()
  : UObject()
{
}

UClient::~UClient()
{
}

UEngine::UEngine()
  : USubsystem()
{
}

UEngine::~UEngine()
{
}

#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( UClient );
IMPLEMENT_NATIVE_CLASS( UEngine );

BEGIN_PROPERTY_LINK( UEngine, 15 )
  LINK_NATIVE_PROPERTY( GameRenderDevice );
  LINK_NATIVE_PROPERTY( AudioDevice );
  LINK_NATIVE_PROPERTY( Console );
  LINK_NATIVE_PROPERTY( NetworkDevice );
  LINK_NATIVE_PROPERTY( Language );
  LINK_NATIVE_PROPERTY( Cylinder );
  LINK_NATIVE_PROPERTY( Client );
  LINK_NATIVE_PROPERTY( Render );
  LINK_NATIVE_PROPERTY( Audio );
  LINK_NATIVE_PROPERTY( TickCycles );
  LINK_NATIVE_PROPERTY( GameCycles );
  LINK_NATIVE_PROPERTY( ClientCycles );
  LINK_NATIVE_PROPERTY( CacheSizeMegs );
  LINK_NATIVE_PROPERTY( UseSound );
  LINK_NATIVE_PROPERTY( CurrentTickRate );
END_PROPERTY_LINK()


