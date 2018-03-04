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
 * UStaticClassInit.cpp - Initializes the object class for all native objects
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "UObject.h"
#include "UClass.h"
#include "UMusic.h"
#include "USound.h"

#define INIT_CLASS(cls) cls::StaticInitializeClass()

bool InitStaticUClasses()
{
  INIT_CLASS( UClass );
  INIT_CLASS( UField );
  INIT_CLASS( UConst );
  INIT_CLASS( UEnum );
  INIT_CLASS( UProperty );
  INIT_CLASS( UStruct );
  INIT_CLASS( UFunction );
  INIT_CLASS( UState );
  INIT_CLASS( UObject );
  
  INIT_CLASS( UMusic );
  INIT_CLASS( USound );
}

IMPLEMENT_CLASS( UClass );
IMPLEMENT_CLASS( UField );
IMPLEMENT_CLASS( UConst );
IMPLEMENT_CLASS( UEnum );
IMPLEMENT_CLASS( UProperty );
IMPLEMENT_CLASS( UStruct );
IMPLEMENT_CLASS( UFunction );
IMPLEMENT_CLASS( UState );
IMPLEMENT_CLASS( UObject );
IMPLEMENT_CLASS( UMusic );
IMPLEMENT_CLASS( USound );
