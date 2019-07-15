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
 * UNet.h - Networking classes
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/USystem.h"
#include "Engine/UPlayer.h"

class DLL_EXPORT UNetConnection : public UPlayer
{
  DECLARE_NATIVE_CLASS( UNetConnection, UPlayer, 
      CLASS_NoExport | CLASS_Transient | CLASS_Config, Engine )

 
  UNetConnection();
};

class DLL_EXPORT UNetDriver : public USubsystem
{
  DECLARE_NATIVE_CLASS( UNetDriver, USubsystem,
      CLASS_NoExport | CLASS_Transient, Engine )

  UNetDriver();

  // TODO:
};

class DLL_EXPORT USecurityData : public UObject
{
  DECLARE_NATIVE_CLASS( USecurityData, UObject, CLASS_Config, Engine )
  EXPOSE_TO_USCRIPT()
  USecurityData();

  struct BanEntry
  {
    FString* ClientIP;
    FString* ClientName;
    FString* ClientID;
    FString* ClientIdentNr;
    int IPStart;
    int IPEnd;
  };

  struct TempBanEntry
  {
    FString* ClientName;
    FString* ClientID;
    FString* ClientIdentNr;
    int IPAddress;
  };

  struct ClientLogEntry
  {
    FString* PlayerNames;
    FString* PlayerIP;
    FString* ClientID;
    FString* ClientIdentNr;
    int IPAddress;
  };

  TArray<BanEntry>* BanList;
  TArray<TempBanEntry>* TempBanList;
  TArray<ClientLogEntry>* LogList;
  int MaxIPLogLen;
};

