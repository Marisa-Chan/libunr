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
  SurfList = new Array<int>();
}

ADecal::~ADecal()
{
}

IMPLEMENT_NATIVE_CLASS( ADecal );

BEGIN_PROPERTY_LINK( ADecal, 4 )
  LINK_NATIVE_PROPERTY( ADecal, MultiDecalLevel );
  LINK_NATIVE_PROPERTY( ADecal, bAttachPanningSurfs );
  LINK_NATIVE_PROPERTY( ADecal, bAttachUnlitSurfs );
  LINK_NATIVE_PROPERTY( ADecal, SurfList );
END_PROPERTY_LINK()

