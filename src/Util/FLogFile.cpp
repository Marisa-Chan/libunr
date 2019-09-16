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
 * FLogFile.cpp - Class for serializing text to a log file
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include <stdarg.h>
#include "Util/FLogFile.h"
#include "Core/USystem.h"

#define MAX_MSG_LEN  512
#define MAX_TYPE_LEN 32
#define MAX_LINE_LEN (MAX_MSG_LEN + MAX_TYPE_LEN - 1)
DLL_EXPORT FLogFile* GLogFile = NULL;

int FLogFile::Open( const char* Filename )
{
  char FullPath[4096];
  USystem::RealPath( Filename, FullPath, sizeof(FullPath) );
  if ( FFileArchiveOut::Open( FullPath ) != 0 )
    return errno;

  char TimeStr[32];
  time_t TimeVal;
  struct tm* TimeInfo;
  time( &TimeVal );
  TimeInfo = localtime( &TimeVal );
  strftime( TimeStr, sizeof(TimeStr), "%Y-%m-%d %H:%M:%S", TimeInfo );

  Logf( LOG_INFO, "Log file opened on %s", TimeStr );
  return true;
}

void FLogFile::Close()
{
  char TimeStr[32];
  time_t TimeVal;
  struct tm* TimeInfo;
  time( &TimeVal );
  TimeInfo = localtime( &TimeVal );
  strftime( TimeStr, sizeof(TimeStr), "%Y-%m-%d %H:%M:%S", TimeInfo );
  
  Logf( LOG_INFO, "Log file closed on %s", TimeStr );
}

void FLogFile::Flush()
{
  FFileArchiveOut::Flush();
  fflush(stdout);
}

void FLogFile::Logf( int Type, const char* Str, ... )
{
  if ( Type < LogLevel )
    return;

  static char StrBuf[MAX_MSG_LEN];
  static char Msg[MAX_LINE_LEN];
  
  va_list vl;
  va_start( vl, Str );
  vsnprintf( StrBuf, MAX_MSG_LEN, Str, vl );
  va_end( vl );
  
  size_t MsgLen = snprintf( Msg, MAX_LINE_LEN, "[%s] %s\n", LogLevelStrings[Type], StrBuf );
  size_t WriteLen = MsgLen;
  if ( MsgLen > MAX_LINE_LEN ) 
  {
    Logf( LOG_CRIT, "Following log message exceeds maximum length" );
    WriteLen = MAX_LINE_LEN;
  }
  
  printf( "%s", Msg );
  FFileArchive::Printf( "%s", Msg ); // making a distinction here
}

void FLogFile::SetLogLevel( int Level )
{
  if ( Level < LOG_DEV )
    LogLevel = LOG_DEV;
  else if ( Level > LOG_CRIT )
    LogLevel = LOG_CRIT;
  else
    LogLevel = Level;
}

