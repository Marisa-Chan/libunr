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
 * TArray.h - Master header for array class
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#ifndef __TARRAY_H__
#define __TARRAY_H__

#include "FUtil.h"

#define TARRAY_LOOP( type, array, itname ) \
  for (TArray<type>::Iterator itname = array->Begin(); itname != array->End(); ++itname)

#if defined LIBUNR_USE_STL
  #include "TArrayStl.h"
#else
  #include "TArrayNull.h"
#endif
    
#endif
