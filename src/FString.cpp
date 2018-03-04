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
 * FString.cpp - Implements useful string functions
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "FString.h"

char* strupper( const char* str )
{
  char* strUp = strdup( str );
  size_t len = strlen( strUp );
  for (size_t i = 0; i < len && i != MAX_SIZE; i++)
    strUp[i] = toupper( strUp[i] );
  
  return strUp;
}

int stricmp( const char* str1, const char* str2 )
{
  char* strUp1 = strupper( str1 );
  char* strUp2 = strupper( str2 );
  
  int result = strcmp( strUp1, strUp2 );
  
  free( strUp1 );
  free( strUp2 );
  
  return result;
}

int strnicmp( const char* str1, const char* str2, size_t count )
{
  char* strUp1 = strupper( str1 );
  char* strUp2 = strupper( str2 );
  
  int result = strncmp( strUp1, strUp2, count );
  
  free( strUp1 );
  free( strUp2 );
  
  return result;
}

#if defined LIBUNR_USE_STL
  bool operator==( const FString& lhs, const FString& rhs ) { return lhs.RealStr == rhs.RealStr; }
  bool operator==( const char* lhs,    const FString& rhs ) { return lhs == rhs.RealStr; }
  bool operator==( const FString& lhs, const char*    rhs ) { return lhs.RealStr == rhs; }

  bool operator!=( const FString& lhs, const FString& rhs ) { return lhs.RealStr != rhs.RealStr; }
  bool operator!=( const char* lhs,    const FString& rhs ) { return lhs != rhs.RealStr; }
  bool operator!=( const FString& lhs, const char*    rhs ) { return lhs.RealStr != rhs; }
#endif

