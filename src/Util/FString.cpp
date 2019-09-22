/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU General Public License as published by     *|
|*  the Free Software Foundation, either version 3 of the License, or        *|
|*  (at your option) any later version.                                      *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU General Public License for more details.                             *|
|*                                                                           *|
|*  You should have received a copy of the GNU General Public License        *|
|*  along with this program. If not, see <https://www.gnu.org/licenses/>.    *|
\*===========================================================================*/

/*========================================================================
 * FString.cpp - string helper implementation
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Util/FMacro.h"
#include "Util/FString.h"
#include "Core/UPackage.h"

/*-----------------------------------------------------------------------------
 * FString
-----------------------------------------------------------------------------*/
FString& FString::operator+=( const FString& Str )
{
  *(string*)this += Str;
  return *this;
}

FString& FString::operator+=( const string& Str )
{
  *(string*)this += Str;
  return *this;
}

FString& FString::operator+=( const char* s )
{
  *(string*)this += s;
  return *this;
}

FString& FString::operator+=( char c )
{
  *(string*)this += c;
  return *this;
}

FString& FString::operator+=( FName Name )
{
  *(string*)this += Name.Data();
  return *this;
}

bool operator!=( const FString& lhs, const FString& rhs )
{
  return (string&)lhs != (string&)rhs;
}

DLL_EXPORT FPackageFileIn& operator>>( FPackageFileIn& In, FString& Str )
{
  idx Size = 0;
  In >> CINDEX( Size );

  if ( Size > 0 )
  {
    Str.reserve( Size-1 );
    
    // Slow but secure
    for ( int i = 0; i < Size; i++ )
    {
      char C = '\0';
      In.Read( &C, 1 );

      if ( C == '\0' )
        break;

      Str += C;
    }
  }

  return In;
}

DLL_EXPORT FPackageFileOut& operator<<( FPackageFileOut& Out, FString& Str )
{
  size_t Length = Str.length();
  Out << CINDEX( Length );
  Out.Write( (void*)Str.data(), Length+1 );
  return Out;
}

//#ifndef LIBUNR_WIN32
static const char StricmpTable[] =
{
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
  0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
  0x60, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
};

int stricmp ( const char* str1, const char* str2 )
{  
  if (!str1 || !str2)
    return 0;
  
  while (1)
  { 
    if (StricmpTable[*str1] != StricmpTable[*str2])
      return StricmpTable[*str1] < StricmpTable[*str2] ? -1 : 1;
    
    if (StricmpTable[*str1] == '\0')
      break;

    str1++;
    str2++;
  }
  
  return 0;
}

int strnicmp( const char* str1, const char* str2, size_t count )
{
  if (!str1 || !str2)
    return 0;
  
  while (count)
  { 
    if (StricmpTable[*str1] != StricmpTable[*str2])
      return StricmpTable[*str1] < StricmpTable[*str2] ? -1 : 1;
    
    if (StricmpTable[*str1] == '\0')
      break;

    str1++;
    str2++;
    count--;
  }
  
  return 0;

}
//#endif

char* strupper( const char* str )
{
  size_t len = strlen( str );
  char* strUp = new char[len+1];
      
  for (size_t i = 0; i < len && i != MAX_SIZE; i++)
    strUp[i] = toupper( str[i] );
  
  strUp[len] = '\0';
  return strUp;
}

char* GetDateString( const char* Fmt )
{
  static char DateString[32];
  
  time_t Now;
  struct tm* TimeInfo = {0};
  
  time( &Now );
  TimeInfo = localtime( &Now );
  
  if (strftime( DateString, 32, Fmt, TimeInfo ) == 0)
    return NULL;
  else
    return DateString;
}

