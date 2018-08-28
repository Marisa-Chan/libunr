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
 * USystem.cpp - System class implementation 
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "FConfig.h"
#include "USystem.h"

#if defined LIBUNR_LINUX || LIBUNR_BSD
  #define LIBUNR_INI_PATH "~/.config/libunr/libunr.ini"
#elif defined LIBUNR_WIN32
  #define LIBUNR_INI_PATH "libunr.ini"
#endif

USubsystem::USubsystem()
{
}

USystem::USystem()
{
  GamePath = NULL;
  GameName = NULL;
  PurgeCacheDays = 0;
  SavePath = NULL;
  CachePath = NULL;
  CacheExt = new String(".uxx");
}

const char* USystem::ResolvePath( const char* PkgName )
{
  const char* GoodPath = NULL;
  for ( int i = 0; i < Paths.Size(); i++ )
  {
    String* PathIter = Paths[i]; 
    size_t Asterisk = PathIter->FindFirstOf( '*' );
    String RealPath = PathIter->Substr( 0, Asterisk );
    RealPath += PkgName;
    RealPath += PathIter->Substr( Asterisk + 1 );

    if ( access( RealPath.Data(), F_OK ) )
    {
      GoodPath = StringDup( RealPath.Data() );
      break;
    }
  }

  return NULL;
}

void USystem::Exit( int ExitCode )
{

}

bool USystem::StaticInit()
{
  GSystem = new USystem();
  
  // Read libunr ini and get game specific details
  GLibunrConfig = new FConfig();
  if ( !GLibunrConfig->Load( LIBUNR_INI_PATH ) )
  {
    Logf( LOG_CRIT, "Can't open or create 'libunr.ini'" );
    return false;
  }

  GSystem->GamePath = GLibunrConfig->ReadString( "Game", "Path" );
  GSystem->GameName = GLibunrConfig->ReadString( "Game", "Name" );

  // Now read the game ini
  // TODO: Use platform correct slashes for paths
  String GameCfgPath( GSystem->GamePath );
  GameCfgPath += "System";
  GameCfgPath += *GSystem->GameName;
  GameCfgPath += ".ini";

  GGameConfig = new FConfig();
  if ( !GGameConfig->Load( GameCfgPath.Data() ) )
  {
    Logf( LOG_CRIT, "Can't open or create '%s.ini'", GSystem->GameName );
    return false;
  }
  
  char* SavePath  = GGameConfig->ReadString( "Core.System", "SavePath" );
  char* CachePath = GGameConfig->ReadString( "Core.System", "CachePath" );
  char* CacheExt  = GGameConfig->ReadString( "Core.System", "CacheExt" );

  GSystem->PurgeCacheDays = GGameConfig->ReadInt32( "Core.System", "PurgeCacheDays" );
  GSystem->SavePath  = new String( ( SavePath != NULL ) ? SavePath : "" );
  GSystem->CachePath = new String( ( CachePath != NULL ) ? CachePath : "" );
  GSystem->CacheExt  = new String( ( CacheExt != NULL ) ? CacheExt : "" );

  xstl::Free( SavePath );
  xstl::Free( CachePath );
  xstl::Free( CacheExt );

  char* PkgPath = NULL;
  int i = 0;
  while( 1 )
  {
    PkgPath = GGameConfig->ReadString( "Core.System", "Paths", i );
    if ( PkgPath == NULL )
      break;

    String* PkgPathString = new String( PkgPath );
    GSystem->Paths.PushBack( PkgPathString );
    xstl::Free( PkgPath );
  }

  return true;
}

