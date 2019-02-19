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

BEGIN_PROPERTY_LINK( APlayerReplicationInfo, 16 )
  LINK_NATIVE_PROPERTY( APlayerReplicationInfo, PlayerName );
  LINK_NATIVE_PROPERTY( APlayerReplicationInfo, PlayerID );
  LINK_NATIVE_PROPERTY( APlayerReplicationInfo, TeamName );
  LINK_NATIVE_PROPERTY( APlayerReplicationInfo, Team );
  LINK_NATIVE_PROPERTY( APlayerReplicationInfo, TeamID );
  LINK_NATIVE_PROPERTY( APlayerReplicationInfo, Score );
  LINK_NATIVE_PROPERTY( APlayerReplicationInfo, Spree );
  LINK_NATIVE_PROPERTY( APlayerReplicationInfo, VoiceType );
  LINK_NATIVE_PROPERTY( APlayerReplicationInfo, HasFlag );
  LINK_NATIVE_PROPERTY( APlayerReplicationInfo, Ping );
  LINK_NATIVE_PROPERTY( APlayerReplicationInfo, bIsFemale );
  LINK_NATIVE_PROPERTY( APlayerReplicationInfo, bIsABot );
  LINK_NATIVE_PROPERTY( APlayerReplicationInfo, bFeigningDeath );
  LINK_NATIVE_PROPERTY( APlayerReplicationInfo, bIsSpectator );
  LINK_NATIVE_PROPERTY( APlayerReplicationInfo, TalkTexture );
  LINK_NATIVE_PROPERTY( APlayerReplicationInfo, PlayerZone );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AGameReplicationInfo, 15 )
  LINK_NATIVE_PROPERTY( AGameReplicationInfo, GameName );
  LINK_NATIVE_PROPERTY( AGameReplicationInfo, bTeamGame );
  LINK_NATIVE_PROPERTY( AGameReplicationInfo, RemainingTime );
  LINK_NATIVE_PROPERTY( AGameReplicationInfo, ElapsedTime );
  LINK_NATIVE_PROPERTY( AGameReplicationInfo, ServerName );
  LINK_NATIVE_PROPERTY( AGameReplicationInfo, ShortName );
  LINK_NATIVE_PROPERTY( AGameReplicationInfo, AdminName );
  LINK_NATIVE_PROPERTY( AGameReplicationInfo, AdminEmail );
  LINK_NATIVE_PROPERTY( AGameReplicationInfo, Region );
  LINK_NATIVE_PROPERTY( AGameReplicationInfo, ShowMOTD );
  LINK_NATIVE_PROPERTY( AGameReplicationInfo, MOTDLine1 );
  LINK_NATIVE_PROPERTY( AGameReplicationInfo, MOTDLine2 );
  LINK_NATIVE_PROPERTY( AGameReplicationInfo, MOTDLine3 );
  LINK_NATIVE_PROPERTY( AGameReplicationInfo, MOTDLine4 );
  LINK_NATIVE_PROPERTY( AGameReplicationInfo, GameEndedComments );
END_PROPERTY_LINK()

