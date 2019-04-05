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

class DLL_EXPORT AGameInfo : public AInfo
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

  FString* AdminPassword;
  FString* GamePassword;

  UClass* ScoreBoardType;
  UClass* GameMenuType;
  FString* BotMenuType;
  FString* RulesMenuType;
  FString* SettingsMenuType;
  FString* GameUMenuType;
  FString* MultiplayerUMenuType;
  FString* GameOptionsMenuType;
  UClass* HUDType;
  UClass* MapListType;
  FString* MapPrefix;
  FString* BeaconName;
  FString* SpecialDamageString, FemSpecialDamageString;
  int     SentText;

  UClass*   MutatorClass;
  AMutator* BaseMutator;

  UClass* WaterZoneType;

  idx DefaultPlayerState;

  UClass* GameReplicationInfoClass;
  AGameReplicationInfo* GameReplicationInfo;

  FString* ServerLogName;

  AStatLog* LocalLog;
  AStatLog* WorldLog;
  FString*   LocalLogFileName;
  FString*   WorldLogFileName;
  FString*   LocalBatcherURL;
  FString*   LocalBatcherParams;
  FString*   LocalStatsURL;
  FString*   WorldBatcherURL;
  FString*   WorldBatcherParams;
  FString*   WorldStatsURL;

  FString* SwitchLevelMessage;
  FString* DefaultPlayerName;
  FString* LeftMessage, FemLeftMessage;
  FString* FailedSpawnMessage;
  FString* FailedPlaceMessage;
  FString* FailedTeamMessage;
  FString* NameChangedMessage;
  FString* EnteredMessage, FemEnteredMessage;
  FString* GameName;
  FString* MaxedOutMessage;
  FString* WrongPassword;
  FString* NeedPassword;

  FString* LastPreloginIP;
  FString* LastLoginPlayerNames;
  FString* LastPreloginIdentity;
  FString* LastPreloginIdent;
  FString* MaleGender, FemaleGender;
  class AGameRules* GameRules;
  class AAdminAccessManager* AccessManager;

  FString* AccessManagerClass;
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

  // UT99 Variables
  bool bAllowFOV;
  bool bAlternateMode;
  bool bCanViewOthers;
  bool bExternalBatcher;
  FString* IPBanned;
  FString* IPPolicies[50]; // Dynamic arrays are cool, y'know...
  UClass* DeathMessageClass;
  UClass* DMMessageClass;
  AMutator* DamageMutator;
  AMutator* MessageMutator;
  bool bBatchLocal;
  UClass* StatLogClass;
  int DemoBuild;
  int DemoHasTuts;
  FString* EnabledMutators;

  // Rune Variables
  bool bSubtitles;
  bool bAllowWeaponDrop;
  bool bAllowShieldDrop;
  bool bAutoPickup;
  bool bEllipticalMovement;
  UClass* DefaultShield;
  FString* MutatorMenuType;
  FString* MaplistMenuType;
  float DebrisPercentage;
  float ParticlePercentage;
  bool bAllowLimbSever;
};

