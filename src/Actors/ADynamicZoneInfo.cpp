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
 * ADynamicZoneInfo.h - DynamicZoneInfo functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Actors/ADynamicZoneInfo.h"

ADynamicZoneInfo::ADynamicZoneInfo()
  : AZoneInfo()
{
}

ADynamicZoneInfo::~ADynamicZoneInfo()
{
}

IMPLEMENT_NATIVE_CLASS( ADynamicZoneInfo );

BEGIN_PROPERTY_LINK( ADynamicZoneInfo, 13 )
  LINK_NATIVE_PROPERTY( NextDynamicZone );
  LINK_NATIVE_PROPERTY( ZoneAreaType );
  LINK_NATIVE_PROPERTY( BoxMin );
  LINK_NATIVE_PROPERTY( BoxMax );
  LINK_NATIVE_PROPERTY( CylinderSize );
  LINK_NATIVE_PROPERTY( SphereSize );
  LINK_NATIVE_PROPERTY( MatchOnlyZone );
  LINK_NATIVE_PROPERTY( bUseRelativeToRotation );
  LINK_NATIVE_PROPERTY( bMovesForceTouchUpdate );
  LINK_NATIVE_PROPERTY( bQuickFilter );
  LINK_NATIVE_PROPERTY( bScriptEvent );
  LINK_NATIVE_PROPERTY( bUpdateTouchers );
  LINK_NATIVE_PROPERTY( OldPose );
END_PROPERTY_LINK()

