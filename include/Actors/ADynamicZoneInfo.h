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
 * ADynamicZoneInfo.h - A zone info whose properties can update at runtime
 * Original 227 implementation that this is based off of written by .:..:
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Actors/AZoneInfo.h"

class DLL_EXPORT ADynamicZoneInfo : public AZoneInfo
{
  DECLARE_NATIVE_CLASS( ADynamicZoneInfo, AZoneInfo, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ADynamicZoneInfo();

  ADynamicZoneInfo* NextDynamicZone;

  enum EDynZoneInfoType
  {
    DZONE_Cube,
    DZONE_Sphere,
    DZONE_Cylinder,
    DZONE_Script // TODO: Any benefits to this?
  };
  EDynZoneInfoType ZoneAreaType;

  FVector BoxMin, BoxMax;
  float CylinderSize, SphereSize;
  AZoneInfo* MatchOnlyZone;
  bool bUseRelativeToRotation;
  bool bMovesForceTouchUpdate;
  bool bUpdateTouchers;
  FVector OldPose;
};

