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
 * FString.h - Master header file for string class
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#ifndef __FSTRING_H__
#define __FSTRING_H__

#include "FUtil.h"

#if defined LIBUNR_USE_STL
  #include "FStringStl.h"
#else
  #include "FStringNull.h"
#endif

bool operator==(const FString& lhs, const FString& rhs);
bool operator==(const char* lhs, const FString& rhs);
bool operator==(const FString& lhs, const char* rhs);
bool operator!=(const FString& lhs, const FString& rhs);
bool operator!=(const char* lhs, const FString& rhs);
bool operator!=(const FString& lhs, const char* rhs);
void Swap(FString& x, FString& y);

#endif
