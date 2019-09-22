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
 * AReplicationInfo.cpp - ReplicationInfo functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Actors/AReplicationInfo.h"

AReplicationInfo::AReplicationInfo()
  : AInfo()
{
}

AReplicationInfo::~AReplicationInfo()
{
}

APlayerReplicationInfo::APlayerReplicationInfo()
  : AReplicationInfo()
{
}

APlayerReplicationInfo::~APlayerReplicationInfo()
{
}

AGameReplicationInfo::AGameReplicationInfo()
  : AReplicationInfo()
{
}

AGameReplicationInfo::~AGameReplicationInfo()
{
}

IMPLEMENT_NATIVE_CLASS( AReplicationInfo );
IMPLEMENT_NATIVE_CLASS( AGameReplicationInfo );
IMPLEMENT_NATIVE_CLASS( APlayerReplicationInfo );

BEGIN_PROPERTY_LINK( AReplicationInfo, 0 )
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( APlayerReplicationInfo, 29 )
  LINK_NATIVE_PROPERTY( PlayerName );
  LINK_NATIVE_PROPERTY( PlayerID );
  LINK_NATIVE_PROPERTY( TeamName );
  LINK_NATIVE_PROPERTY( Team );
  LINK_NATIVE_PROPERTY( TeamID );
  LINK_NATIVE_PROPERTY( Score );
  LINK_NATIVE_PROPERTY( Spree );
  LINK_NATIVE_PROPERTY( VoiceType );
  LINK_NATIVE_PROPERTY( HasFlag );
  LINK_NATIVE_PROPERTY( Ping );
  LINK_NATIVE_PROPERTY( bIsFemale );
  LINK_NATIVE_PROPERTY( bIsABot );
  LINK_NATIVE_PROPERTY( bFeigningDeath );
  LINK_NATIVE_PROPERTY( bIsSpectator );
  LINK_NATIVE_PROPERTY( TalkTexture );
  LINK_NATIVE_PROPERTY( PlayerZone );
  LINK_NATIVE_PROPERTY( OldName );
  LINK_NATIVE_PROPERTY( Deaths );
  LINK_NATIVE_PROPERTY( PacketLoss );
  LINK_NATIVE_PROPERTY( bWaitingPlayer );
  LINK_NATIVE_PROPERTY( bAdmin );
  LINK_NATIVE_PROPERTY( PlayerLocation );
  LINK_NATIVE_PROPERTY( StartTime );
  LINK_NATIVE_PROPERTY( TimeAcc );
  LINK_NATIVE_PROPERTY( Streak );
  LINK_NATIVE_PROPERTY( bReadyToPlay );
  LINK_NATIVE_PROPERTY( bFirstBlood );
  LINK_NATIVE_PROPERTY( MaxSpree );
  LINK_NATIVE_PROPERTY( HeadKills );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AGameReplicationInfo, 25 )
  LINK_NATIVE_PROPERTY( GameName );
  LINK_NATIVE_PROPERTY( bTeamGame );
  LINK_NATIVE_PROPERTY( RemainingTime );
  LINK_NATIVE_PROPERTY( ElapsedTime );
  LINK_NATIVE_PROPERTY( ServerName );
  LINK_NATIVE_PROPERTY( ShortName );
  LINK_NATIVE_PROPERTY( AdminName );
  LINK_NATIVE_PROPERTY( AdminEmail );
  LINK_NATIVE_PROPERTY( Region );
  LINK_NATIVE_PROPERTY( ShowMOTD );
  LINK_NATIVE_PROPERTY( MOTDLine1 );
  LINK_NATIVE_PROPERTY( MOTDLine2 );
  LINK_NATIVE_PROPERTY( MOTDLine3 );
  LINK_NATIVE_PROPERTY( MOTDLine4 );
  LINK_NATIVE_PROPERTY( GameEndedComments );
  LINK_NATIVE_PROPERTY( GameClass );
  LINK_NATIVE_PROPERTY( bClassicDeathMessages );
  LINK_NATIVE_PROPERTY( bStopCountDown );
  LINK_NATIVE_PROPERTY( RemainingMinute );
  LINK_NATIVE_PROPERTY( SecondCount );
  LINK_NATIVE_PROPERTY( NumPlayers );
  LINK_NATIVE_PROPERTY( SumFrags );
  LINK_NATIVE_PROPERTY( UpdateTimer );
  LINK_NATIVE_ARRAY   ( PRIArray );
  LINK_NATIVE_PROPERTY( ServerRegion );
END_PROPERTY_LINK()

