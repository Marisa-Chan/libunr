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
 * FLogFile.h - Class for serializing text to a log file
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include "Util/FFileArchive.h"

// Log levels
#define LOG_DEV  0
#define LOG_INFO 1
#define LOG_WARN 2
#define LOG_ERR  3
#define LOG_CRIT 4

const char* const LogLevelStrings[] =
{
  "D",
  "I",
  "W",
  "E",
  "!"
};

class DLL_EXPORT FLogFile : public FFileArchiveOut
{
public:
  virtual int Open( const char* Filename );
  virtual void Close();
  virtual void Flush();
  virtual void Logf( int Type, const char* Str, ... );
  virtual void SetLogLevel( int Level );

protected:
  int LogLevel;
};

extern DLL_EXPORT FLogFile* GLogFile;
#define GLogf GLogFile->Logf

