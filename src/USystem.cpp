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
#include "UPackage.h"

USystem* GSystem = NULL;

USubsystem::USubsystem()
{
}

USubsystem::~USubsystem()
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

USystem::~USystem()
{
  if ( GamePath != NULL )
    delete GamePath;

  if ( GameName != NULL )
    delete GameName;

  if ( SavePath != NULL )
    delete SavePath;

  if ( CachePath != NULL )
    delete CachePath;

  delete CacheExt;
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

    if ( FileExists( RealPath.Data() ) )
    {
      GoodPath = StringDup( RealPath.Data() );
      break;
    }
  }

  if ( !GoodPath )
    perror("ResolvePath");
  return GoodPath;
}

void USystem::Exit( int ExitCode )
{
  // We do not want to save any bad info to any configs, just close them
  GConfigManager->CloseConfigs();

  // TODO: Kill viewports and free resources they use
  // TODO: We should reeeeally free object memory too

  // Close down all subsystems
  UPackage::StaticExit( true );

  // TODO: Print some kind of message box showing an error

  // Exit
  exit( ExitCode );
}

bool USystem::PromptForGameInfo()
{
  if ( DoGamePrompt == NULL )
    return false;

  // Create buffers for callback to use
  char* PathBuf = new char[GPC_PATH_BUF_LEN];
  char* NameBuf = new char[GPC_NAME_BUF_LEN];

  // Run callback
  DoGamePrompt( PathBuf, NameBuf );

  // Update game variables
  GamePath = PathBuf;
  GameName = NameBuf;

  // Write to config and save
  GLibunrConfig->WriteString( "Game", "Path", GamePath );
  GLibunrConfig->WriteString( "Game", "Name", GameName );
  GLibunrConfig->Save();

  return true;
}

bool USystem::StaticInit( GamePromptCallback GPC, DevicePromptCallback DPC )
{
  // Create global system
  GSystem = new USystem();
  
  // (?) Should we check the function addresses to make sure its in text section?
  // Set callbacks
  GSystem->DoGamePrompt = GPC;
  GSystem->DoDevicePrompt = DPC;

  // Read libunr ini and get game specific details
  GLibunrConfig = new FConfig();
  const char* LibunrIniPath = GetLibunrIniPath();
  int IniStatus = GLibunrConfig->Load( LibunrIniPath );
  if ( IniStatus == ERR_FILE_NOT_EXIST )
  {
    // Create an ini if its missing
    Logf( LOG_WARN, "Main ini file '%s' does not exist; creating one", LibunrIniPath );

    const char* DefaultLibunrIniPath = GetDefaultLibunrIniPath();
    if ( !CopyFile( DefaultLibunrIniPath, LibunrIniPath ) )
    {
      Logf( LOG_CRIT, "Default ini file '%s' does not exist; aborting", 
          DefaultLibunrIniPath );
      return false;
    }

    // If we can't make a new one, fail
    if ( GLibunrConfig->Load( LibunrIniPath ) != 0 )
    {
      Logf( LOG_CRIT, "Error creating new main ini file '%s'; aborting",
          LibunrIniPath );
      return false;
    }
  }
  else if ( IniStatus != 0 )
  {
    // Something else went wrong with the ini file that already exists
    Logf( LOG_CRIT, "Error parsing main ini file '%s'; aborting",
        LibunrIniPath );
    return false;
  }

  // Get libunr specific info
  GSystem->bLogRefCntZero = GLibunrConfig->ReadBool( "libunr", "bLogRefCntZero" );
  GSystem->RenderDevice = GLibunrConfig->ReadString( "libunr", "RenderDevice" );
  GSystem->AudioDevice  = GLibunrConfig->ReadString( "libunr", "AudioDevice" );
  // if ( strnicmp( GSystem->RenderDevice, None ) == 0 ||
  //      strnicmp( GSystem->AudioDevice, None ) == 0 )
  // {
  //   if ( !GSystem->PromptForDeviceInfo() )
  //   {
  //     Logf( LOG_CRT, "DoDevicePrompt() callback is not set; aborting" );
  //     return false;
  //   }
  // }

  // Get game info
  GSystem->GamePath = GLibunrConfig->ReadString( "Game", "Path" );
  GSystem->GameName = GLibunrConfig->ReadString( "Game", "Name" );
  if ( strnicmp( GSystem->GamePath, "None", 4 ) == 0 ||
       strnicmp( GSystem->GameName, "None", 4 ) == 0 )
  {
    if ( !GSystem->PromptForGameInfo() )
    {
      Logf( LOG_CRIT, "DoGamePrompt() callback is not set; aborting" );
      return false;
    }
  }

  // Change directory to the game's system folder
  String GameSysPath( GSystem->GamePath );
  GameSysPath += "/System/";
  if ( chdir( GameSysPath.Data() ) < 0 )
  {
    Logf( LOG_WARN, "Game directory system folder '%s' does not exist; aborting (errno = %s)", GameSysPath.Data(), strerror( errno ) );
    return false;
  }

  // Read the game ini
  String GameCfgPath( GSystem->GameName );
  GameCfgPath += ".ini";

  GGameConfig = new FConfig();
  IniStatus = GGameConfig->Load( GameCfgPath.Data() );
  if ( IniStatus == ERR_FILE_NOT_EXIST )
  {
    Logf( LOG_WARN, "Game ini file '%s' does not exist; creating one", GameCfgPath.Data() );

    // Create a new one from the default
    if ( !CopyFile( "Default.ini", GameCfgPath.Data() ) )
    {
      Logf( LOG_CRIT, "Default game ini '%s' is missing; aborting", GameCfgPath.Data() );
      return false;
    }

    // If we can't make a new one, fail
    if ( GGameConfig->Load( GameCfgPath.Data() ) != 0 )
    {
      Logf( LOG_CRIT, "Error creating new game ini '%s'; aborting", GameCfgPath.Data() );
      return false;
    }
  }
  else if ( IniStatus != 0 )
  {
    // Something else went wrong with the ini file that already exists
    Logf( LOG_CRIT, "Error parsing game ini '%s'; aborting", GameCfgPath.Data() );
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
    PkgPath = GGameConfig->ReadString( "Core.System", "Paths", i++ );
    if ( PkgPath == NULL )
      break;

    String* PkgPathString = new String( PkgPath );
#ifndef LIBUNR_WIN32
    PkgPathString->ReplaceChars( '\\', '/' );
#endif
    GSystem->Paths.PushBack( PkgPathString );
    xstl::Free( PkgPath );
  }

  // Open the user config
  GUserConfig = new FConfig();
  IniStatus = GUserConfig->Load( "User.ini" );
  if ( IniStatus == ERR_FILE_NOT_EXIST )
  {
    // If one doesn't exist, make a new one
    if ( !FileExists( "DefUser.ini" ) )
    {
      // There isn't one at all for this game, use the game config instead
      GUserConfig = GGameConfig;
    }

    else if ( !CopyFile( "DefUser.ini", "User.ini" ) )
    {
      Logf( LOG_CRIT, "Could not create new User.ini; aborting" );
      return false;
    }

    if ( GUserConfig->Load( "User.ini" ) != 0 )
    {
      Logf( LOG_CRIT, "Error creating new User.ini; aborting" );
      return false;
    }
  }

  return true;
}

const char* USystem::GetLibunrIniPath()
{
#if defined LIBUNR_LINUX || LIBUNR_BSD
  struct ::passwd* pw = getpwuid( getuid() );
  static char LibUnrPath[1024] = { 0 };
  strcpy( LibUnrPath, pw->pw_dir );
  strcat( LibUnrPath, "/.config/libunr/libunr.ini" ); 
  return LibUnrPath;
#elif defined LIBUNR_WIN32
  return "libunr.ini";
#else
  #error "Unknown operating system! Please add a section to USystem::GetLibunrIniPath()"
  return -1;
#endif
}

const char* USystem::GetDefaultLibunrIniPath()
{
#if defined LIBUNR_LINUX || LIBUNR_BSD
  static char DefLibUnrPath[1024] = { 0 };
  strcpy( DefLibUnrPath, INSTALL_PREFIX );
  strcat( DefLibUnrPath, "/share/libunr/DefaultLibunr.ini" );
  return DefLibUnrPath;
#elif defined LIBUNR_WIN32
  return "DefaultLibunr.ini";
#else
  #error "Unknown operating system! Please add a section to USystem::GetLibunrIniPath()"
  return -1;
#endif
}

bool USystem::CopyFile( const char* OrigFile, const char* NewFile )
{
  FileStreamIn Orig;
  FileStreamOut New;

  int Status = Orig.Open( OrigFile );
  if ( Status != 0 )
  {
    Logf( LOG_WARN, "Copy operation failed: can't open original file '%s' (errno = %s)", OrigFile, strerror( Status ) );
    return false;
  }

  Status = New.Open( NewFile );
  if ( Status != 0 )
  {
    Logf( LOG_WARN, "Copy operation failed: can't create new file '%s' (errno = %s)", NewFile, strerror( Status ) );
    return false;
  }

  size_t OrigSize = Orig.Seek( 0, End );
  Orig.Seek( 0, Begin );
  u8* Buffer = new u8[OrigSize];

  Orig.Read( Buffer, OrigSize );
  New.Write( Buffer, OrigSize );

  Orig.Close();
  New.Close();

  return true;
}

bool USystem::FileExists( const char* Filename )
{
  FileStreamIn File;
  
  int Status = File.Open( Filename );
  File.Close();

  return (Status != ENOENT) ? true : false;
}

