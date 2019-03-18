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
 * UNet.cpp - Networking functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UPackage.h"
#include "Core/UClass.h"
#include "Core/UNet.h"

UNetConnection::UNetConnection()
  : UPlayer()
{
}

UNetConnection::~UNetConnection()
{
}

UNetDriver::UNetDriver()
  : USubsystem()
{
}

UNetDriver::~UNetDriver()
{
}

USecurityData::USecurityData()
  : UObject()
{
}

USecurityData::~USecurityData()
{
}

IMPLEMENT_NATIVE_CLASS( UNetConnection );
IMPLEMENT_NATIVE_CLASS( UNetDriver );
IMPLEMENT_NATIVE_CLASS( USecurityData );

BEGIN_PROPERTY_LINK( USecurityData, 4 )
  LINK_NATIVE_PROPERTY( BanList );
  LINK_NATIVE_PROPERTY( TempBanList );
  LINK_NATIVE_PROPERTY( LogList );
  LINK_NATIVE_PROPERTY( MaxIPLogLen );
END_PROPERTY_LINK()

