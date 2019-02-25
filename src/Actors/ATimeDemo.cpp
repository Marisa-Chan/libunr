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
 * ATimeDemo.cpp - Time demo functionality (Unreal only)
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Actors/ATimeDemo.h"

ATimeDemo::ATimeDemo()
  : AInfo()
{
}

ATimeDemo::~ATimeDemo()
{
}

IMPLEMENT_NATIVE_CLASS( ATimeDemo );

BEGIN_PROPERTY_LINK( ATimeDemo, 23 )
  LINK_NATIVE_PROPERTY( ATimeDemo, FileAr ); 
  LINK_NATIVE_PROPERTY( ATimeDemo, TimePassed );
  LINK_NATIVE_PROPERTY( ATimeDemo, TimeDilation );
  LINK_NATIVE_PROPERTY( ATimeDemo, StartTime );
  LINK_NATIVE_PROPERTY( ATimeDemo, LastSecTime );
  LINK_NATIVE_PROPERTY( ATimeDemo, LastCycleTime );
  LINK_NATIVE_PROPERTY( ATimeDemo, LastFrameTime );
  LINK_NATIVE_PROPERTY( ATimeDemo, SquareSum );
  LINK_NATIVE_PROPERTY( ATimeDemo, FrameNum );
  LINK_NATIVE_PROPERTY( ATimeDemo, FrameLastSecond );
  LINK_NATIVE_PROPERTY( ATimeDemo, FrameLastCycle );
  LINK_NATIVE_PROPERTY( ATimeDemo, CycleCount );
  LINK_NATIVE_PROPERTY( ATimeDemo, QuitAfterCycles );
  LINK_NATIVE_PROPERTY( ATimeDemo, CycleMessage );
  LINK_NATIVE_PROPERTY( ATimeDemo, CycleResult );
  LINK_NATIVE_PROPERTY( ATimeDemo, bSaveToFile );
  LINK_NATIVE_PROPERTY( ATimeDemo, bFirstFrame );
  LINK_NATIVE_PROPERTY( ATimeDemo, LastSec );
  LINK_NATIVE_PROPERTY( ATimeDemo, MinFPS );
  LINK_NATIVE_PROPERTY( ATimeDemo, MaxFPS );
  LINK_NATIVE_PROPERTY( ATimeDemo, OldPoint );
  LINK_NATIVE_PROPERTY( ATimeDemo, NewPoint );
  LINK_NATIVE_PROPERTY( ATimeDemo, Console );
END_PROPERTY_LINK()

