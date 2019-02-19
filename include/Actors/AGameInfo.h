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
 * AGameInfo.h - Base class of all game types
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Actors/AActor.h"
#include "Actors/AReplicationInfo.h"
#include "Actors/AStatLog.h"

class AGameInfo : public AInfo
{
  DECLARE_NATIVE_CLASS( AGameInfo, AInfo, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AGameInfo();

  int ItemGoals, KillGoals, SecretGoals;
  u8  Difficulty;

  float   AutoAim;
  float   GameSpeed;
  float   StartTime;
  UClass* DefaultPlayerClass;
  UClass* DefaultWeapon;
  int     MaxSpectators;
  int     NumSpectators;
  int     MaxPlayers;
  int     NumPlayers;
  int     CurrentID;

  String* AdminPassword;
  String* GamePassword;

  UClass* ScoreBoardType;
  UClass* GameMenuType;
  String* BotMenuType;
  String* RulesMenuType;
  String* SettingsMenuType;
  String* GameUMenuType;
  String* MultiplayerUMenuType;
  String* GameOptionsMenuType;
  UClass* HUDType;
  UClass* MapListType;
  String* MapPrefix;
  String* BeaconName;
  String* SpecialDamageString, FemSpecialDamageString;
  int     SentText;

  UClass*   MutatorClass;
  AMutator* BaseMutator;

  UClass* WaterZoneType;

  idx DefaultPlayerState;

  UClass* GameReplicationInfoClass;
  AGameReplicationInfo* GameReplicationInfo;

  String* ServerLogName;

  AStatLog* LocalLog;
  AStatLog* WorldLog;
  String*   LocalLogFileName;
  String*   WorldLogFileName;
  String*   LocalBatcherURL;
  String*   LocalBatcherParams;
  String*   LocalStatsURL;
  String*   WorldBatcherURL;
  String*   WorldBatcherParams;
  String*   WorldStatsURL;

  String* SwitchLevelMessage;
  String* DefaultPlayerName;
  String* LeftMessage, FemLeftMessage;
  String* FailedSpawnMessage;
  String* FailedPlaceMessage;
  String* FailedTeamMessage;
  String* NameChangedMessage;
  String* EnteredMessage, FemEnteredMessage;
  String* GameName;
  String* MaxedOutMessage;
  String* WrongPassword;
  String* NeedPassword;

  String* LastPreloginIP;
  String* LastLoginPlayerNames;
  String* LastPreloginIdentity;
  String* LastPreloginIdent;
  String* MaleGender, FemaleGender;
  class AGameRules* GameRules;
  class AAdminAccessManager* AccessManager;

  String* AccessManagerClass;
  int     BleedingDamageMin;
  int     BleedingDamageMax;

  bool bNoMonsters;
  bool bMuteSpectators;
  bool bHumansOnly;
  bool bRestartLevel;
  bool bPauseable;
  bool bCoopWeaponMode;
  bool bClassicDeathmessages;
  bool bLowGore;
  bool bCanChangeSkin;
  bool bTeamGame;
  bool bVeryLowGore;
  bool bNoCheating;
  bool bDeathMatch;
  bool bGameEnded;
  bool bOverTime;
  
  bool bLocalLog;
  bool bLocalLogQuery;
  bool bWorldLog;
  bool bLoggingGame;

  bool bBleedingEnabled;
  bool bBleedingDamageEnabled;
  bool bAllHealthStopsBleeding;
  bool bBandagesStopBleeding;
  bool bMessageAdminsAliases;
  bool bLogNewPlayerAliases;
  bool bLogDownloadsToClient;
  bool bHandleDownloadMessaging;

  bool bShowRecoilAnimations;
  bool bCastShadow;
  bool bDecoShadows;
  bool bCastProjectorShadows;
  bool bUseRealtimeShadow;
  bool bNoWalkInAir;
  bool bProjectorDecals;
  bool bIsSavedGame;
  bool bAlwaysEnhancedSightCheck;

  bool bRestrictMoversRetriggering;
};

