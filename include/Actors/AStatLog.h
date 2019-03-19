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
 * AStatLog.h - Logs game events for stat collection
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Actors/AActor.h"

class AStatLog : public AInfo
{
  DECLARE_NATIVE_CLASS( AStatLog, AInfo, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AStatLog();

  void* Context; // ???
  bool  bWorld;
  float TimeStamp;

  FString* LocalStandard;
  FString* WorldStandard;
  FString* LogVersion;
  FString* LogInfoURL;
  FString* GameName;
  FString* GameCreator;
  FString* GameCreatorURL;
  FString* DecoderRingURL;

  FString* LocalLogDir;
  FString* WorldLogDir;
};

class AStatLogFile : public AStatLog
{
  DECLARE_NATIVE_CLASS( AStatLogFile, AStatLog, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AStatLogFile();

  bool bWatermark;
  Stream* LogAr;

  FString* StatLogFile;
  FString* StatLogFinal;

  void OpenLog();
  void CloseLog();
  void Watermark( FString* EventString );
  FString* GetChecksum();
  FString* GetPlayerChecksum( FString* PlayerName, FString* Secret );
  void FileFlush();
  void FileLog( FString* EventString );
};

