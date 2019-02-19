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
 * AStatLog.h - Logs game events for stat collection
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Actors/AStatLog.h"
#include "Core/UClass.h"

AStatLog::AStatLog()
  : AInfo()
{
}

AStatLog::~AStatLog()
{
}

AStatLogFile::AStatLogFile()
  : AStatLog()
{
}

AStatLogFile::~AStatLogFile()
{
}

IMPLEMENT_NATIVE_CLASS( AStatLog );
IMPLEMENT_NATIVE_CLASS( AStatLogFile );

BEGIN_PROPERTY_LINK( AStatLog, 13 )
  LINK_NATIVE_PROPERTY( AStatLog, Context );
  LINK_NATIVE_PROPERTY( AStatLog, bWorld );
  LINK_NATIVE_PROPERTY( AStatLog, TimeStamp );
  LINK_NATIVE_PROPERTY( AStatLog, LocalStandard );
  LINK_NATIVE_PROPERTY( AStatLog, WorldStandard );
  LINK_NATIVE_PROPERTY( AStatLog, LogVersion );
  LINK_NATIVE_PROPERTY( AStatLog, LogInfoURL );
  LINK_NATIVE_PROPERTY( AStatLog, GameName );
  LINK_NATIVE_PROPERTY( AStatLog, GameCreator );
  LINK_NATIVE_PROPERTY( AStatLog, GameCreatorURL );
  LINK_NATIVE_PROPERTY( AStatLog, DecoderRingURL );
  LINK_NATIVE_PROPERTY( AStatLog, LocalLogDir );
  LINK_NATIVE_PROPERTY( AStatLog, WorldLogDir );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AStatLogFile, 4 )
  LINK_NATIVE_PROPERTY( AStatLogFile, bWatermark );
  LINK_NATIVE_PROPERTY( AStatLogFile, LogAr );
  LINK_NATIVE_PROPERTY( AStatLogFile, StatLogFile );
  LINK_NATIVE_PROPERTY( AStatLogFile, StatLogFinal );
END_PROPERTY_LINK()

