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
 * ADecal.cpp - Decal functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Actors/ADecal.h"

ADecal::ADecal()
  : AActor()
{
  SurfList = new TArray<int>();
}

ADecal::~ADecal()
{
}

IMPLEMENT_NATIVE_CLASS( ADecal );

BEGIN_PROPERTY_LINK( ADecal, 6 )
  LINK_NATIVE_PROPERTY( MultiDecalLevel );
  LINK_NATIVE_PROPERTY( bAttachPanningSurfs );
  LINK_NATIVE_PROPERTY( bAttachUnlitSurfs );
  LINK_NATIVE_PROPERTY( bBloodyDecal );
  LINK_NATIVE_PROPERTY( SurfList );
  LINK_NATIVE_PROPERTY( LastRenderedTime );
END_PROPERTY_LINK()

