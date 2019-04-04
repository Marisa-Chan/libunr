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
 * AReplicationInfo.h - ReplicationInfo classes
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Actors/AActor.h"
#include "Actors/AZoneInfo.h"

class AReplicationInfo : public AInfo
{
  DECLARE_NATIVE_CLASS( AReplicationInfo, AInfo, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AReplicationInfo();
};

class APlayerReplicationInfo : public AReplicationInfo
{
  DECLARE_NATIVE_CLASS( APlayerReplicationInfo, AReplicationInfo, 0, Engine );
  EXPOSE_TO_USCRIPT()

  APlayerReplicationInfo();

  FString*     PlayerName;
  int          PlayerID;
  FString*     TeamName;
  u8           Team;
  int          TeamID;
  float        Score;
  float        Spree;
  UClass*      VoiceType;
  ADecoration* HasFlag;
  int          Ping;
  bool         bIsFemale;
  bool         bIsABot;
  bool         bFeigningDeath;
  bool         bIsSpectator;
  UTexture*    TalkTexture;
  AZoneInfo*   PlayerZone;

  // UT99 Variables
  FString*     OldName;
  float        Deaths; // ...and why is this a float???
  u8           PacketLoss;
  bool         bWaitingPlayer;
  bool         bAdmin;
  ALocationID* PlayerLocation;
  int          StartTime;
  int          TimeAcc;

  // Deus Ex Variables
  float Streak;

  // Rune Variables
  bool bReadyToPlay;
  bool bFirstBlood;
  int MaxSpree;
  int HeadKills;
};

class AGameReplicationInfo : public AReplicationInfo
{
  DECLARE_NATIVE_CLASS( AGameReplicationInfo, AReplicationInfo, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AGameReplicationInfo();

  FString* GameName;
  bool    bTeamGame;
  int     RemainingTime;
  int     ElapsedTime;

  FString* ServerName;
  FString* ShortName;
  FString* AdminName;
  FString* AdminEmail;
  int     Region;
  
  bool    ShowMOTD;
  FString* MOTDLine1;
  FString* MOTDLine2;
  FString* MOTDLine3;
  FString* MOTDLine4;

  FString* GameEndedComments;

  // UT99 Variables
  FString* GameClass;
  bool  bClassicDeathMessages;
  bool  bStopCountDown;
  int   RemainingMinute;
  float SecondCount;
  int   NumPlayers;
  int   SumFrags;
  float UpdateTimer;
  APlayerReplicationInfo* PRIArray[32];

  // Rune Variables
  int ServerRegion;
};

