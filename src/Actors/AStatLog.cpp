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

BEGIN_PROPERTY_LINK( AStatLog, 21 )
  LINK_NATIVE_PROPERTY( Context );
  LINK_NATIVE_PROPERTY( bWorld );
  LINK_NATIVE_PROPERTY( TimeStamp );
  LINK_NATIVE_PROPERTY( LocalStandard );
  LINK_NATIVE_PROPERTY( WorldStandard );
  LINK_NATIVE_PROPERTY( LogVersion );
  LINK_NATIVE_PROPERTY( LogInfoURL );
  LINK_NATIVE_PROPERTY( GameName );
  LINK_NATIVE_PROPERTY( GameCreator );
  LINK_NATIVE_PROPERTY( GameCreatorURL );
  LINK_NATIVE_PROPERTY( DecoderRingURL );
  LINK_NATIVE_PROPERTY( LocalLogDir );
  LINK_NATIVE_PROPERTY( WorldLogDir );
  LINK_NATIVE_PROPERTY( LocalBatcherURL );
  LINK_NATIVE_PROPERTY( LocalBatcherParams );
  LINK_NATIVE_PROPERTY( LocalStatsURL );
  LINK_NATIVE_PROPERTY( WorldBatcherURL );
  LINK_NATIVE_PROPERTY( WorldBatcherParams );
  LINK_NATIVE_PROPERTY( WorldStatsURL );
  LINK_NATIVE_PROPERTY( bWorldBatcherError );
  LINK_NATIVE_PROPERTY( bLogTypingEvents );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AStatLogFile, 4 )
  LINK_NATIVE_PROPERTY( bWatermark );
  LINK_NATIVE_PROPERTY( LogAr );
  LINK_NATIVE_PROPERTY( StatLogFile );
  LINK_NATIVE_PROPERTY( StatLogFinal );
END_PROPERTY_LINK()

