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
 * FName.h - Name table stuff
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#ifndef __FNAME_H__
#define __FNAME_H__

#include "FArchive.h"
#define NAME_LEN 64

struct DLL_EXPORT FName
{
   FName();
   FName(const char* InStr);
  ~FName();
  
  void Read ( FArchive& File, size_t PackageVersion );
  void Write( FArchive& File, size_t PackageVersion );
  
  char Name[NAME_LEN];
  u32 Index; // is this really needed?
  int Flags;
};

#endif
