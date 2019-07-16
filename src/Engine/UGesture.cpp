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
 * UGesture.cpp - 2D pattern matching for mouse input
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Engine/UGesture.h"

UGesture::UGesture()
  : UObject()
{
}

UGesture::~UGesture()
{
  if ( Points )
    delete Points;

  if ( Segments )
    delete Segments;
}

#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( UGesture );

BEGIN_PROPERTY_LINK( UGesture, 2 )
  LINK_NATIVE_PROPERTY( Points );
  LINK_NATIVE_PROPERTY( Segments );
END_PROPERTY_LINK()
