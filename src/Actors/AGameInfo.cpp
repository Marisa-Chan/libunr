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

BEGIN_PROPERTY_LINK( AGameInfo, 110 )
  LINK_NATIVE_PROPERTY( AGameInfo, ItemGoals );
  LINK_NATIVE_PROPERTY( AGameInfo, KillGoals );
  LINK_NATIVE_PROPERTY( AGameInfo, SecretGoals );
  LINK_NATIVE_PROPERTY( AGameInfo, Difficulty );
  LINK_NATIVE_PROPERTY( AGameInfo, AutoAim );
  LINK_NATIVE_PROPERTY( AGameInfo, GameSpeed );
  LINK_NATIVE_PROPERTY( AGameInfo, StartTime );
  LINK_NATIVE_PROPERTY( AGameInfo, DefaultPlayerClass );
  LINK_NATIVE_PROPERTY( AGameInfo, DefaultWeapon );
  LINK_NATIVE_PROPERTY( AGameInfo, MaxSpectators );
  LINK_NATIVE_PROPERTY( AGameInfo, NumSpectators );
  LINK_NATIVE_PROPERTY( AGameInfo, MaxPlayers );
  LINK_NATIVE_PROPERTY( AGameInfo, NumPlayers );
  LINK_NATIVE_PROPERTY( AGameInfo, CurrentID );
  LINK_NATIVE_PROPERTY( AGameInfo, AdminPassword );
  LINK_NATIVE_PROPERTY( AGameInfo, GamePassword );
  LINK_NATIVE_PROPERTY( AGameInfo, ScoreBoardType );
  LINK_NATIVE_PROPERTY( AGameInfo, GameMenuType );
  LINK_NATIVE_PROPERTY( AGameInfo, BotMenuType );
  LINK_NATIVE_PROPERTY( AGameInfo, RulesMenuType );
  LINK_NATIVE_PROPERTY( AGameInfo, SettingsMenuType );
  LINK_NATIVE_PROPERTY( AGameInfo, GameUMenuType );
  LINK_NATIVE_PROPERTY( AGameInfo, MultiplayerUMenuType );
  LINK_NATIVE_PROPERTY( AGameInfo, GameOptionsMenuType );
  LINK_NATIVE_PROPERTY( AGameInfo, HUDType );
  LINK_NATIVE_PROPERTY( AGameInfo, MapListType );
  LINK_NATIVE_PROPERTY( AGameInfo, MapPrefix );
  LINK_NATIVE_PROPERTY( AGameInfo, BeaconName );
  LINK_NATIVE_PROPERTY( AGameInfo, SpecialDamageString );
  LINK_NATIVE_PROPERTY( AGameInfo, FemSpecialDamageString );
  LINK_NATIVE_PROPERTY( AGameInfo, SentText );
  LINK_NATIVE_PROPERTY( AGameInfo, MutatorClass );
  LINK_NATIVE_PROPERTY( AGameInfo, BaseMutator );
  LINK_NATIVE_PROPERTY( AGameInfo, WaterZoneType );
  LINK_NATIVE_PROPERTY( AGameInfo, DefaultPlayerState );
  LINK_NATIVE_PROPERTY( AGameInfo, GameReplicationInfoClass );
  LINK_NATIVE_PROPERTY( AGameInfo, GameReplicationInfo );
  LINK_NATIVE_PROPERTY( AGameInfo, ServerLogName );
  LINK_NATIVE_PROPERTY( AGameInfo, LocalLog );
  LINK_NATIVE_PROPERTY( AGameInfo, WorldLog );
  LINK_NATIVE_PROPERTY( AGameInfo, LocalLogFileName );
  LINK_NATIVE_PROPERTY( AGameInfo, WorldLogFileName );
  LINK_NATIVE_PROPERTY( AGameInfo, LocalBatcherURL );
  LINK_NATIVE_PROPERTY( AGameInfo, LocalBatcherParams );
  LINK_NATIVE_PROPERTY( AGameInfo, LocalStatsURL );
  LINK_NATIVE_PROPERTY( AGameInfo, WorldBatcherURL );
  LINK_NATIVE_PROPERTY( AGameInfo, WorldBatcherParams );
  LINK_NATIVE_PROPERTY( AGameInfo, WorldStatsURL );
  LINK_NATIVE_PROPERTY( AGameInfo, SwitchLevelMessage );
  LINK_NATIVE_PROPERTY( AGameInfo, DefaultPlayerName );
  LINK_NATIVE_PROPERTY( AGameInfo, LeftMessage );
  LINK_NATIVE_PROPERTY( AGameInfo, FemLeftMessage );
  LINK_NATIVE_PROPERTY( AGameInfo, FailedSpawnMessage );
  LINK_NATIVE_PROPERTY( AGameInfo, FailedPlaceMessage );
  LINK_NATIVE_PROPERTY( AGameInfo, FailedTeamMessage );
  LINK_NATIVE_PROPERTY( AGameInfo, NameChangedMessage );
  LINK_NATIVE_PROPERTY( AGameInfo, EnteredMessage );
  LINK_NATIVE_PROPERTY( AGameInfo, FemEnteredMessage );
  LINK_NATIVE_PROPERTY( AGameInfo, GameName );
  LINK_NATIVE_PROPERTY( AGameInfo, MaxedOutMessage );
  LINK_NATIVE_PROPERTY( AGameInfo, WrongPassword );
  LINK_NATIVE_PROPERTY( AGameInfo, NeedPassword );
  LINK_NATIVE_PROPERTY( AGameInfo, LastPreloginIP );
  LINK_NATIVE_PROPERTY( AGameInfo, LastLoginPlayerNames );
  LINK_NATIVE_PROPERTY( AGameInfo, LastPreloginIdentity );
  LINK_NATIVE_PROPERTY( AGameInfo, LastPreloginIdent );
  LINK_NATIVE_PROPERTY( AGameInfo, MaleGender );
  LINK_NATIVE_PROPERTY( AGameInfo, FemaleGender );
  LINK_NATIVE_PROPERTY( AGameInfo, GameRules );
  LINK_NATIVE_PROPERTY( AGameInfo, AccessManager );
  LINK_NATIVE_PROPERTY( AGameInfo, AccessManagerClass );
  LINK_NATIVE_PROPERTY( AGameInfo, BleedingDamageMin );
  LINK_NATIVE_PROPERTY( AGameInfo, BleedingDamageMax );
  LINK_NATIVE_PROPERTY( AGameInfo, bNoMonsters );
  LINK_NATIVE_PROPERTY( AGameInfo, bMuteSpectators );
  LINK_NATIVE_PROPERTY( AGameInfo, bHumansOnly );
  LINK_NATIVE_PROPERTY( AGameInfo, bRestartLevel );
  LINK_NATIVE_PROPERTY( AGameInfo, bPauseable );
  LINK_NATIVE_PROPERTY( AGameInfo, bCoopWeaponMode );
  LINK_NATIVE_PROPERTY( AGameInfo, bClassicDeathmessages );
  LINK_NATIVE_PROPERTY( AGameInfo, bLowGore );
  LINK_NATIVE_PROPERTY( AGameInfo, bCanChangeSkin );
  LINK_NATIVE_PROPERTY( AGameInfo, bTeamGame );
  LINK_NATIVE_PROPERTY( AGameInfo, bVeryLowGore );
  LINK_NATIVE_PROPERTY( AGameInfo, bNoCheating );
  LINK_NATIVE_PROPERTY( AGameInfo, bDeathMatch );
  LINK_NATIVE_PROPERTY( AGameInfo, bGameEnded );
  LINK_NATIVE_PROPERTY( AGameInfo, bOverTime );
  LINK_NATIVE_PROPERTY( AGameInfo, bLocalLog );
  LINK_NATIVE_PROPERTY( AGameInfo, bLocalLogQuery );
  LINK_NATIVE_PROPERTY( AGameInfo, bWorldLog );
  LINK_NATIVE_PROPERTY( AGameInfo, bLoggingGame );
  LINK_NATIVE_PROPERTY( AGameInfo, bBleedingEnabled );
  LINK_NATIVE_PROPERTY( AGameInfo, bBleedingDamageEnabled );
  LINK_NATIVE_PROPERTY( AGameInfo, bAllHealthStopsBleeding );
  LINK_NATIVE_PROPERTY( AGameInfo, bBandagesStopBleeding );
  LINK_NATIVE_PROPERTY( AGameInfo, bMessageAdminsAliases );
  LINK_NATIVE_PROPERTY( AGameInfo, bLogNewPlayerAliases );
  LINK_NATIVE_PROPERTY( AGameInfo, bLogDownloadsToClient );
  LINK_NATIVE_PROPERTY( AGameInfo, bHandleDownloadMessaging );
  LINK_NATIVE_PROPERTY( AGameInfo, bShowRecoilAnimations );
  LINK_NATIVE_PROPERTY( AGameInfo, bCastShadow );
  LINK_NATIVE_PROPERTY( AGameInfo, bDecoShadows );
  LINK_NATIVE_PROPERTY( AGameInfo, bCastProjectorShadows );
  LINK_NATIVE_PROPERTY( AGameInfo, bUseRealtimeShadow );
  LINK_NATIVE_PROPERTY( AGameInfo, bNoWalkInAir ); 
  LINK_NATIVE_PROPERTY( AGameInfo, bProjectorDecals );
  LINK_NATIVE_PROPERTY( AGameInfo, bIsSavedGame );
  LINK_NATIVE_PROPERTY( AGameInfo, bAlwaysEnhancedSightCheck );
  LINK_NATIVE_PROPERTY( AGameInfo, bRestrictMoversRetriggering );
END_PROPERTY_LINK()
