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

  String* LocalStandard;
  String* WorldStandard;
  String* LogVersion;
  String* LogInfoURL;
  String* GameName;
  String* GameCreator;
  String* GameCreatorURL;
  String* DecoderRingURL;

  String* LocalLogDir;
  String* WorldLogDir;
};

class AStatLogFile : public AStatLog
{
  DECLARE_NATIVE_CLASS( AStatLogFile, AStatLog, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AStatLogFile();

  bool bWatermark;
  Stream* LogAr;

  String* StatLogFile;
  String* StatLogFinal;

  void OpenLog();
  void CloseLog();
  void Watermark( String* EventString );
  String* GetChecksum();
  String* GetPlayerChecksum( String* PlayerName, String* Secret );
  void FileFlush();
  void FileLog( String* EventString );
};

