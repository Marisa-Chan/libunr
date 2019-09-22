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
 * AInternetLink.cpp - Internet connection info implementation
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Actors/AInternetLink.h"

AInternetInfo::AInternetInfo()
  : AInfo()
{
}

AInternetInfo::~AInternetInfo()
{
}

AInternetLink::AInternetLink()
  : AInternetInfo()
{
}

AInternetLink::~AInternetLink()
{
}

ATcpLink::ATcpLink()
  : AInternetLink()
{
}

ATcpLink::~ATcpLink()
{
}

AUdpLink::AUdpLink()
  : AInternetLink()
{
}

AUdpLink::~AUdpLink()
{
}

IMPLEMENT_NATIVE_CLASS( AInternetInfo );
IMPLEMENT_NATIVE_CLASS( AInternetLink );
IMPLEMENT_NATIVE_CLASS( ATcpLink );
IMPLEMENT_NATIVE_CLASS( AUdpLink );

BEGIN_PROPERTY_LINK( AInternetInfo, 0 )
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AInternetLink, 7 )
  LINK_NATIVE_PROPERTY( LinkMode );
  LINK_NATIVE_PROPERTY( Socket );
  LINK_NATIVE_PROPERTY( Port );
  LINK_NATIVE_PROPERTY( RemoteSocket );
  LINK_NATIVE_PROPERTY( PrivateResolveInfo );
  LINK_NATIVE_PROPERTY( DataPending );
  LINK_NATIVE_PROPERTY( ReceiveMode );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ATcpLink, 4 )
  LINK_NATIVE_PROPERTY( LinkState );
  LINK_NATIVE_PROPERTY( RemoteAddr );
  LINK_NATIVE_PROPERTY( AcceptClass );
  LINK_NATIVE_PROPERTY( SendFIFO );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AUdpLink, 1 )
  LINK_NATIVE_PROPERTY( BroadcastAddr );
END_PROPERTY_LINK()

