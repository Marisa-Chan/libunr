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
 * AGameInfo.cpp - GameInfo functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Actors/AGameInfo.h"
#include "Core/UClass.h"

AGameInfo::AGameInfo()
  : AInfo()
{
}

AGameInfo::~AGameInfo()
{
}

IMPLEMENT_NATIVE_CLASS( AGameInfo );

BEGIN_PROPERTY_LINK( AGameInfo, 125 )
  LINK_NATIVE_PROPERTY( ItemGoals );
  LINK_NATIVE_PROPERTY( KillGoals );
  LINK_NATIVE_PROPERTY( SecretGoals );
  LINK_NATIVE_PROPERTY( Difficulty );
  LINK_NATIVE_PROPERTY( AutoAim );
  LINK_NATIVE_PROPERTY( GameSpeed );
  LINK_NATIVE_PROPERTY( StartTime );
  LINK_NATIVE_PROPERTY( DefaultPlayerClass );
  LINK_NATIVE_PROPERTY( DefaultWeapon );
  LINK_NATIVE_PROPERTY( MaxSpectators );
  LINK_NATIVE_PROPERTY( NumSpectators );
  LINK_NATIVE_PROPERTY( MaxPlayers );
  LINK_NATIVE_PROPERTY( NumPlayers );
  LINK_NATIVE_PROPERTY( CurrentID );
  LINK_NATIVE_PROPERTY( AdminPassword );
  LINK_NATIVE_PROPERTY( GamePassword );
  LINK_NATIVE_PROPERTY( ScoreBoardType );
  LINK_NATIVE_PROPERTY( GameMenuType );
  LINK_NATIVE_PROPERTY( BotMenuType );
  LINK_NATIVE_PROPERTY( RulesMenuType );
  LINK_NATIVE_PROPERTY( SettingsMenuType );
  LINK_NATIVE_PROPERTY( GameUMenuType );
  LINK_NATIVE_PROPERTY( MultiplayerUMenuType );
  LINK_NATIVE_PROPERTY( GameOptionsMenuType );
  LINK_NATIVE_PROPERTY( HUDType );
  LINK_NATIVE_PROPERTY( MapListType );
  LINK_NATIVE_PROPERTY( MapPrefix );
  LINK_NATIVE_PROPERTY( BeaconName );
  LINK_NATIVE_PROPERTY( SpecialDamageString );
  LINK_NATIVE_PROPERTY( FemSpecialDamageString );
  LINK_NATIVE_PROPERTY( SentText );
  LINK_NATIVE_PROPERTY( MutatorClass );
  LINK_NATIVE_PROPERTY( BaseMutator );
  LINK_NATIVE_PROPERTY( WaterZoneType );
  LINK_NATIVE_PROPERTY( DefaultPlayerState );
  LINK_NATIVE_PROPERTY( GameReplicationInfoClass );
  LINK_NATIVE_PROPERTY( GameReplicationInfo );
  LINK_NATIVE_PROPERTY( ServerLogName );
  LINK_NATIVE_PROPERTY( LocalLog );
  LINK_NATIVE_PROPERTY( WorldLog );
  LINK_NATIVE_PROPERTY( LocalLogFileName );
  LINK_NATIVE_PROPERTY( WorldLogFileName );
  LINK_NATIVE_PROPERTY( LocalBatcherURL );
  LINK_NATIVE_PROPERTY( LocalBatcherParams );
  LINK_NATIVE_PROPERTY( LocalStatsURL );
  LINK_NATIVE_PROPERTY( WorldBatcherURL );
  LINK_NATIVE_PROPERTY( WorldBatcherParams );
  LINK_NATIVE_PROPERTY( WorldStatsURL );
  LINK_NATIVE_PROPERTY( SwitchLevelMessage );
  LINK_NATIVE_PROPERTY( DefaultPlayerName );
  LINK_NATIVE_PROPERTY( LeftMessage );
  LINK_NATIVE_PROPERTY( FemLeftMessage );
  LINK_NATIVE_PROPERTY( FailedSpawnMessage );
  LINK_NATIVE_PROPERTY( FailedPlaceMessage );
  LINK_NATIVE_PROPERTY( FailedTeamMessage );
  LINK_NATIVE_PROPERTY( NameChangedMessage );
  LINK_NATIVE_PROPERTY( EnteredMessage );
  LINK_NATIVE_PROPERTY( FemEnteredMessage );
  LINK_NATIVE_PROPERTY( GameName );
  LINK_NATIVE_PROPERTY( MaxedOutMessage );
  LINK_NATIVE_PROPERTY( WrongPassword );
  LINK_NATIVE_PROPERTY( NeedPassword );
  LINK_NATIVE_PROPERTY( LastPreloginIP );
  LINK_NATIVE_PROPERTY( LastLoginPlayerNames );
  LINK_NATIVE_PROPERTY( LastPreloginIdentity );
  LINK_NATIVE_PROPERTY( LastPreloginIdent );
  LINK_NATIVE_PROPERTY( MaleGender );
  LINK_NATIVE_PROPERTY( FemaleGender );
  LINK_NATIVE_PROPERTY( GameRules );
  LINK_NATIVE_PROPERTY( AccessManager );
  LINK_NATIVE_PROPERTY( AccessManagerClass );
  LINK_NATIVE_PROPERTY( BleedingDamageMin );
  LINK_NATIVE_PROPERTY( BleedingDamageMax );
  LINK_NATIVE_PROPERTY( bNoMonsters );
  LINK_NATIVE_PROPERTY( bMuteSpectators );
  LINK_NATIVE_PROPERTY( bHumansOnly );
  LINK_NATIVE_PROPERTY( bRestartLevel );
  LINK_NATIVE_PROPERTY( bPauseable );
  LINK_NATIVE_PROPERTY( bCoopWeaponMode );
  LINK_NATIVE_PROPERTY( bClassicDeathmessages );
  LINK_NATIVE_PROPERTY( bLowGore );
  LINK_NATIVE_PROPERTY( bCanChangeSkin );
  LINK_NATIVE_PROPERTY( bTeamGame );
  LINK_NATIVE_PROPERTY( bVeryLowGore );
  LINK_NATIVE_PROPERTY( bNoCheating );
  LINK_NATIVE_PROPERTY( bDeathMatch );
  LINK_NATIVE_PROPERTY( bGameEnded );
  LINK_NATIVE_PROPERTY( bOverTime );
  LINK_NATIVE_PROPERTY( bLocalLog );
  LINK_NATIVE_PROPERTY( bLocalLogQuery );
  LINK_NATIVE_PROPERTY( bWorldLog );
  LINK_NATIVE_PROPERTY( bLoggingGame );
  LINK_NATIVE_PROPERTY( bBleedingEnabled );
  LINK_NATIVE_PROPERTY( bBleedingDamageEnabled );
  LINK_NATIVE_PROPERTY( bAllHealthStopsBleeding );
  LINK_NATIVE_PROPERTY( bBandagesStopBleeding );
  LINK_NATIVE_PROPERTY( bMessageAdminsAliases );
  LINK_NATIVE_PROPERTY( bLogNewPlayerAliases );
  LINK_NATIVE_PROPERTY( bLogDownloadsToClient );
  LINK_NATIVE_PROPERTY( bHandleDownloadMessaging );
  LINK_NATIVE_PROPERTY( bShowRecoilAnimations );
  LINK_NATIVE_PROPERTY( bCastShadow );
  LINK_NATIVE_PROPERTY( bDecoShadows );
  LINK_NATIVE_PROPERTY( bCastProjectorShadows );
  LINK_NATIVE_PROPERTY( bUseRealtimeShadow );
  LINK_NATIVE_PROPERTY( bNoWalkInAir ); 
  LINK_NATIVE_PROPERTY( bProjectorDecals );
  LINK_NATIVE_PROPERTY( bIsSavedGame );
  LINK_NATIVE_PROPERTY( bAlwaysEnhancedSightCheck );
  LINK_NATIVE_PROPERTY( bRestrictMoversRetriggering );
  LINK_NATIVE_PROPERTY( bAllowFOV );
  LINK_NATIVE_PROPERTY( bAlternateMode );
  LINK_NATIVE_PROPERTY( bCanViewOthers );
  LINK_NATIVE_PROPERTY( bExternalBatcher );
  LINK_NATIVE_PROPERTY( IPBanned );
  LINK_NATIVE_ARRAY   ( IPPolicies );
  LINK_NATIVE_PROPERTY( DeathMessageClass );
  LINK_NATIVE_PROPERTY( DMMessageClass );
  LINK_NATIVE_PROPERTY( DamageMutator );
  LINK_NATIVE_PROPERTY( MessageMutator );
  LINK_NATIVE_PROPERTY( bBatchLocal );
  LINK_NATIVE_PROPERTY( StatLogClass );
  LINK_NATIVE_PROPERTY( DemoBuild );
  LINK_NATIVE_PROPERTY( DemoHasTuts );
  LINK_NATIVE_PROPERTY( EnabledMutators );
END_PROPERTY_LINK()
