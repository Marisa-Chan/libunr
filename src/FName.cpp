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
 * FName.cpp - Name table stuff
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "FName.h"
#include "FMemory.h"

FName::FName()
{
  FMemory::Set( Name, 0, NAME_LEN );
  Index = -1;
  Flags = 0;
}

FName::FName( const char* InStr )
{
  strncpy( Name, InStr, NAME_LEN );
  Name[NAME_LEN-1] = '\0';
  Flags = 0;
  Index = -1;
}

FName::~FName()
{
}

void FName::Read( FArchive& File, size_t PackageVersion )
{
  if( PackageVersion <= PKG_VER_UN_220 )
  {
    u8 b;
    char* ptr = Name;
    do
    {
      File >> b;
      *ptr++ = b;
      
    } while( b && ptr <( Name + NAME_LEN ) );
    *ptr = '\0'; // in case we ran up against the name size limit
  }
  else
  {
    int len;
    File >> CINDEX( len );
    if( len > 0 && len < NAME_LEN )
      File.Read( Name, len );
  }
  File >> Flags;
}

void FName::Write( FArchive& File, size_t PackageVersion )
{
  Name[NAME_LEN-1] = '\0'; // just in case
  
  if( PackageVersion > PKG_VER_UN_220 )
  {
    int len = strlen( Name );
    File << CINDEX( len );
  }
  
  File << Name;
}
