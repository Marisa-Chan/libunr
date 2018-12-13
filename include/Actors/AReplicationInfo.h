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

  String*      PlayerName;
  int          PlayerID;
  String*      TeamName;
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
};

class AGameReplicationInfo : public AReplicationInfo
{
  DECLARE_NATIVE_CLASS( AGameReplicationInfo, AReplicationInfo, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AGameReplicationInfo();

  String* GameName;
  bool    bTeamGame;
  int     RemainingTime;
  int     ElapsedTime;

  String* ServerName;
  String* ShortName;
  String* AdminName;
  String* AdminEmail;
  int     Region;
  
  bool    ShowMOTD;
  String* MOTDLine1;
  String* MOTDLine2;
  String* MOTDLine3;
  String* MOTDLine4;

  String* GameEndedComments;
};

