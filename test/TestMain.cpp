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

void GamePromptHandler( char* PathOut, char* NameOut )
{
  // Get game name
  char* Result = NULL;
  while ( Result == NULL )
  {
    printf( "Enter the name of the game executable (i.e.; Unreal or DeusEx): " );
    Result = fgets( NameOut, GPC_NAME_BUF_LEN, stdin );
  }
  
  char* RemoveNewline = strchr( NameOut, '\n' );
  if ( RemoveNewline )
    *RemoveNewline = '\0';

  // Get path name
  Result = NULL;
  while ( Result == NULL )
  {
    printf( "Enter the absolute path of the game: " );
    Result = fgets( PathOut, GPC_PATH_BUF_LEN, stdin );
  }

  RemoveNewline = strchr( PathOut, '\n' );
  if ( RemoveNewline )
    *RemoveNewline = '\0';
}

int main( int argc, char** argv )
{
  static char wd[4096];
  getcwd( wd, sizeof( wd ) );

  if ( UNLIKELY( !USystem::StaticInit( &GamePromptHandler, NULL ) ) )
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
  DefaultTexture->ExportToFile( wd, "bmp" );

  Logf( LOG_INFO, "Test succeeded" );
  return 0;
}
