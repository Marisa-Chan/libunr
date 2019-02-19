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
 * FUtil.cpp - Utility functions
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/FUtil.h"
#include "stdarg.h"

#define MAX_MSG_LEN  512
#define MAX_TYPE_LEN 32
#define MAX_LINE_LEN (MAX_MSG_LEN + MAX_TYPE_LEN - 1)

void Logf( const char* Type, const char* Str, ... )
{
  static char StrBuf[MAX_MSG_LEN];
  static char Msg[MAX_LINE_LEN];
  
  va_list vl;
  va_start( vl, Str );
  vsnprintf( StrBuf, MAX_MSG_LEN, Str, vl );
  va_end( vl );
  
  size_t MsgLen = snprintf( Msg, MAX_LINE_LEN, "[%s] %s\n", Type, StrBuf );
  size_t WriteLen = MsgLen;
  if ( MsgLen > MAX_LINE_LEN ) 
  {
    Logf( LOG_WARN, "Following log message exceeds maximum length" );
    WriteLen = MAX_LINE_LEN;
  }

  printf( "%s", Msg );
}
