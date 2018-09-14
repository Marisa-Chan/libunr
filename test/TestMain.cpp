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
 * TestMain.cpp - libunr test binary
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "libunr.h"

int main( int argc, char** argv )
{
  if ( UNLIKELY( !USystem::StaticInit() ) )
  {
    Logf( LOG_CRIT, "USystem::StaticInit() failed!" );
    return -1;
  }

  if ( UNLIKELY( !UPackage::StaticInit() ) )
  {
    Logf( LOG_CRIT, "UPackage::StaticInit() failed!" );
    return -1;
  }

  if ( UNLIKELY( !UObject::StaticInit() ) )
  {
    Logf( LOG_CRIT, "UObject::StaticInit() failed!" );
    return -1;
  }

  UPackage* EnginePkg = UPackage::StaticLoadPkg( "Engine" );
  UTexture* DefaultTexture = (UTexture*)UPackage::StaticLoadObject( EnginePkg, "DefaultTexture", UTexture::StaticClass() );

  Logf( LOG_INFO, "Test succeeded" );
  return 0;
}
