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
  LINK_NATIVE_PROPERTY( FileAr ); 
  LINK_NATIVE_PROPERTY( TimePassed );
  LINK_NATIVE_PROPERTY( TimeDilation );
  LINK_NATIVE_PROPERTY( StartTime );
  LINK_NATIVE_PROPERTY( LastSecTime );
  LINK_NATIVE_PROPERTY( LastCycleTime );
  LINK_NATIVE_PROPERTY( LastFrameTime );
  LINK_NATIVE_PROPERTY( SquareSum );
  LINK_NATIVE_PROPERTY( FrameNum );
  LINK_NATIVE_PROPERTY( FrameLastSecond );
  LINK_NATIVE_PROPERTY( FrameLastCycle );
  LINK_NATIVE_PROPERTY( CycleCount );
  LINK_NATIVE_PROPERTY( QuitAfterCycles );
  LINK_NATIVE_PROPERTY( CycleMessage );
  LINK_NATIVE_PROPERTY( CycleResult );
  LINK_NATIVE_PROPERTY( bSaveToFile );
  LINK_NATIVE_PROPERTY( bFirstFrame );
  LINK_NATIVE_PROPERTY( LastSec );
  LINK_NATIVE_PROPERTY( MinFPS );
  LINK_NATIVE_PROPERTY( MaxFPS );
  LINK_NATIVE_PROPERTY( OldPoint );
  LINK_NATIVE_PROPERTY( NewPoint );
  LINK_NATIVE_PROPERTY( Console );
END_PROPERTY_LINK()
