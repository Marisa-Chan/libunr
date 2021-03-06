/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU Affero General Public License as           *|
|*  published by the Free Software Foundation, either version 3 of the       *|
|*  License, or (at your option) any later version.                          *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU Affero General Public License for more details.                      *|
|*                                                                           *|
|*  You should have received a copy of the GNU Affero General Public License *|
|*  along with this program.  If not, see <https://www.gnu.org/licenses/>.   *|
\*===========================================================================*/

/*========================================================================
 * USystem.cpp - System class implementation 
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/FConfig.h"
#include "Core/USystem.h"
#include "Core/UPackage.h"

#ifdef LIBUNR_WIN32
  #include "Windows.h"
  #undef CopyFile
  #undef TEXT
  #define TEXT(s) #s
#endif

USystem* GSystem = NULL;
int USystem::LogLevel = LOG_INFO;
bool USystem::bIsEditor = false;

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
  GameFlags = NULL;
  PurgeCacheDays = 0;
  SavePath = NULL;
  CachePath = NULL;
  CacheExt = new FString(".uxx");
  bEnhancedRuntime = false;
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

  if ( UNLIKELY( !GoodPath ) )
  {
#ifndef LIBUNR_WIN32
    char FullPath[4096];
    DIR* dir;
    struct dirent* ent;

    // Didn't find the package but that could be due to case sensitivity
    for ( int i = 0; i < Paths.Size(); i++ )
    {
      String* PathIter = Paths[i]; 
      size_t Asterisk = PathIter->FindFirstOf( '*' );
      String RealPath = PathIter->Substr( 0, Asterisk );

      String FullPkgName( PkgName );
      FullPkgName += PathIter->Substr( Asterisk + 1 );

      dir = opendir( RealPath.Data() );
      if ( dir != NULL )
      {
        while ((ent = readdir(dir)) != NULL)
        {
          if ( UNLIKELY( ent->d_name[0] == '.' ) )
            continue;

          if ( stricmp( ent->d_name, FullPkgName.Data() ) == 0 )
          {
            RealPath += ent->d_name;
            return StringDup( RealPath.Data() );
          }
        }
        closedir(dir);
      }
    }
#endif
    perror("ResolvePath");
    Logf( LOG_WARN, "Error resolving path '%s'", PkgName );
  }

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
  CloseLogFile();
  exit( ExitCode );
}

bool USystem::PromptForGameInfo( char* InGameName )
{
  if ( DoGamePrompt == NULL )
    return false;

  Array<char*>* Names = GLibunrConfig->CreateEntry( "Game", "Name" );
  char* NameBuf = NULL;
  char* ExecBuf = NULL;
  char* PathBuf = NULL;

  // Run callback
  int i;
  if ( InGameName != NULL )
  {
    for ( i = 0; i < Names->Size(); i++ )
    {
      if ( stricmp( (*Names)[i], InGameName ) == 0 )
        break;
    }

    if ( i == Names->Size() )
    {
      // Didn't find the game they asked for, let them know
      Logf( LOG_WARN, "Specified game '%s' does not exist", InGameName );
      i = DoGamePrompt( Names );
    }
  }
  else
  {
    i = DoGamePrompt( Names );
  }

  // Get name and set
  NameBuf = GLibunrConfig->ReadString( "Game", "Name", i );
  ExecBuf = GLibunrConfig->ReadString( "Game", "Exec", i );
  PathBuf = GLibunrConfig->ReadString( "Game", "Path", i );
  GameName = ExecBuf;

  // Get real path 
  GamePath = new char[4096];
  xstl::Set( (char*)GamePath, 0, 4096 );
  RealPath( PathBuf, (char*)GamePath, 4096 );

  // Detect specific game support
  if ( stricmp( ExecBuf, "Unreal" ) == 0 )
    GameFlags |= GAME_Unreal;
  else if ( stricmp( ExecBuf, "UnrealTournament" ) == 0 )
    GameFlags |= GAME_UT99;
  else if ( stricmp( ExecBuf, "DeusEx" ) == 0 )
    GameFlags |= GAME_DeusEx;
  else if ( stricmp( ExecBuf, "Rune" ) == 0 )
    GameFlags |= GAME_Rune;
  else if ( stricmp( ExecBuf, "HP" ) == 0 )
    GameFlags |= GAME_HPSS;
  else if ( stricmp( ExecBuf, "OpenUE" ) == 0 )
  {
    bEnhancedRuntime = true;
    GameFlags |= GAME_All;
  }

  return true;
}

bool USystem::IsEditor()
{
  return bIsEditor;
}

bool USystem::StaticInit( GamePromptCallback GPC, DevicePromptCallback DPC, bool InIsEditor, char* InGameName )
{
  // Create global system
  GSystem = new USystem();

  // (?) Should we check the function addresses to make sure its in text section?
  // Set callbacks
  GSystem->DoGamePrompt = GPC;
  GSystem->DoDevicePrompt = DPC;
  bIsEditor = InIsEditor;

  // Read libunr ini and get game specific details
  GLibunrConfig = new FConfig();
  const char* LibunrIniPath = GetLibunrIniPath();
  int IniStatus = GLibunrConfig->Load( LibunrIniPath );
  if ( IniStatus == ERR_FILE_NOT_EXIST )
  {
    // Create an ini if its missing
    Logf( LOG_WARN, "Main ini file '%s' does not exist; creating one", LibunrIniPath );
#if defined LIBUNR_POSIX
    String* ConfigLibunr = new String( GetHomeDir() );
    ConfigLibunr->Append( "/.config/libunr/" );
    DIR* ConfigLibunrDir = opendir( ConfigLibunr->Data() );
    if ( ConfigLibunrDir == NULL )
      mkdir( ConfigLibunr->Data(), S_IRWXU | S_IRGRP | S_IROTH );
#endif

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
  //     Logf( LOG_WARN, "DoDevicePrompt() callback is not set" );
  // }

  // Get game info
  if ( !GSystem->PromptForGameInfo( InGameName ) )
  {
    Logf( LOG_CRIT, "DoGamePrompt() callback is not set; aborting" );
    return false;
  }
  
  // Change directory to the game's system folder
  String GameSysPath( GSystem->GamePath );
  GameSysPath += DIRECTORY_SEPARATOR;
  GameSysPath += "System";
  GameSysPath += DIRECTORY_SEPARATOR;
  
#ifdef LIBUNR_WIN32
  if ( !SetCurrentDirectory( GameSysPath.Data() ) )
#else
  if ( chdir( GameSysPath.Data() ) < 0 )
#endif
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
  GSystem->SavePath  = new FString( ( SavePath != NULL ) ? SavePath : "" );
  GSystem->CachePath = new FString( ( CachePath != NULL ) ? CachePath : "" );
  GSystem->CacheExt  = new FString( ( CacheExt != NULL ) ? CacheExt : "" );

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

    FString* PkgPathString = new FString( PkgPath );
#ifndef LIBUNR_WIN32
    PkgPathString->ReplaceChars( '\\', '/' );
#endif
    GSystem->Paths.PushBack( PkgPathString );
    xstl::Free( PkgPath );
  }

  // Set up config manager
  GConfigManager = new FConfigManager();

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
      return true;
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

  GConfigManager->AddConfig( GUserConfig );

  return true;
}

const char* USystem::GetLibunrIniPath()
{
#if defined LIBUNR_POSIX
  static char LibUnrPath[1024] = { 0 };
  strcpy( LibUnrPath, GetHomeDir() );
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
#if defined LIBUNR_POSIX
  static char DefLibUnrPath[1024] = { 0 };
  strcpy( DefLibUnrPath, INSTALL_PREFIX );
  strcat( DefLibUnrPath, "/share/libunr/DefaultLibunr.ini" );
  return DefLibUnrPath;
#elif defined LIBUNR_WIN32
  return "DefaultLibunr.ini";
#else
  #error "Unknown operating system! Please add a section to USystem::GetDefaultLibunrIniPath()"
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

// FIXME: Verify that a directory traversal backwards is actually possible
// This code looks *VERY* exploitable at the moment...
void USystem::RealPath( const char* Path, char* FullPath, size_t FullPathSize )
{
  char CurrentFolder[128] = { 0 };
  const char* p = Path;
  char* f = FullPath;
  char* c = &CurrentFolder[0];
  u16 Len = 0;

#ifdef LIBUNR_WIN32
  char* FwdSlsh = strchr( Path, '/' );
  if ( FwdSlsh != NULL )
  {
    *FwdSlsh++ = '\\';
    while ( *FwdSlsh )
    {
      if ( *FwdSlsh == '/' )
        *FwdSlsh = '\\';
      FwdSlsh++;
    }
  }
#endif

  if ( *p != DIRECTORY_SEPARATOR )
  {
#ifndef LIBUNR_WIN32
    if ( *p == '~' )
    {
      strcat( FullPath, GetHomeDir() );
      p++;
      f += strlen( FullPath );
    }
    else
#else
	if ( !(p[1] == ':' && p[2] == '\\') )
#endif
    {
      // If the path doesn't start from root, get the current directory
      char* ignored = getcwd( FullPath, FullPathSize );
      f += strlen( FullPath );
      *f++ = DIRECTORY_SEPARATOR;
      *c++ = DIRECTORY_SEPARATOR;
    }
  }

  while ( 1 )
  {
    if ( *p == DIRECTORY_SEPARATOR || *p == '\0' )
    {
      *c = '\0';
      c = CurrentFolder;
     
      // If just a dot, do nothing
      if ( c[1] == '.' && c[2] == '\0' )
      {
        continue;
      }

      // Check if we're going backwards
      else if ( c[1] == '.' && c[2] == '.' && c[3] == '\0' )
      {  
        // Go back to first actual character
        f--;

        // Go backwards in the full path until a slash is seen, then null terminate
        do        
        {
          f--;
        } while ( *f != DIRECTORY_SEPARATOR );

        *f = '\0';
      }
      else
      {
        // Copy the current folder contents to the full path
        u16 CfLen = strlen( c );
        if ( CfLen > 0 )
        {
          memcpy( f, c, CfLen );
          f += CfLen;
          *f = '\0';
        }
      }

      if ( !*p )
        break;
    }
    *c++ = *p++;
  }
}

bool USystem::MakeDir( const char* Path )
{
  size_t PathLen = strlen( Path );
  if ( PathLen >= 4096 )
  {
    Logf( LOG_WARN, "Can't create directory '%s'; path is too long", Path );
    return false;
  }
  
#ifdef LIBUNR_WIN32
  char* FwdSlsh = strchr( Path, '/' );
  if ( FwdSlsh != NULL )
  {
    *FwdSlsh++ = '\\';
    while ( *FwdSlsh )
    {
      if ( *FwdSlsh == '/' )
        *FwdSlsh = '\\';
      FwdSlsh++;
    }
  }
#endif

  char CurrentPath[4096] = { 0 };
  strcpy( CurrentPath, Path );
  if ( CurrentPath[PathLen-1] != DIRECTORY_SEPARATOR )
    CurrentPath[PathLen] = DIRECTORY_SEPARATOR;

  char FullPath[4096] = { 0 };
  RealPath( CurrentPath, FullPath, sizeof(FullPath) );
  xstl::Set( CurrentPath, 0, sizeof(CurrentPath) );

  struct stat sb;
  char* s = FullPath;
  char* c = &CurrentPath[0];

  while ( *s )
  {
    if ( *s == DIRECTORY_SEPARATOR && CurrentPath[0] != '\0' )
    {
    #ifdef LIBUNR_WIN32
      if ( *(s-1) == ':' )
      {
		*c++ = *s++;
        continue;
	  }
	#endif
      // Folder should now have the name of our current folder
      // stat() once to check for existence at all
      if (stat(CurrentPath, &sb) == 0)
      {
        if ( !S_ISDIR(sb.st_mode) )
        {
          Logf( LOG_WARN, "Failed to create directory '%s'; '%s' is a file",
                Path, CurrentPath );
          return false;
        }
      }
      else
      {
        // Directory does not exist, try to make it
	  #ifndef LIBUNR_WIN32
        if ( mkdir( CurrentPath, S_IRWXU | S_IRGRP | S_IROTH ) < 0 ) 
	  #else
		if ( mkdir( CurrentPath ) < 0 ) 
	  #endif
        {
          int err = errno;
          Logf( LOG_WARN, "Failed to create directory '%s'; couldn't create directory '%s'",
                Path, CurrentPath );
          return false;
        }
      }
    }

    *c++ = *s++;
  }

  return true;
}

#if defined LIBUNR_POSIX
const char* USystem::GetHomeDir()
{
  struct ::passwd* pw = getpwuid( getuid() );
  return pw->pw_dir;
}
#endif

bool LibunrInit( GamePromptCallback GPC, DevicePromptCallback DPC, bool bIsEditor, char* GameName )
{
  if ( UNLIKELY( !USystem::StaticInit( GPC, DPC, bIsEditor, GameName ) ) )
  {
    Logf( LOG_CRIT, "USystem::StaticInit() failed!" );
    return false;
  }

  if ( UNLIKELY( !UPackage::StaticInit() ) )
  {
    Logf( LOG_CRIT, "UPackage::StaticInit() failed!" );
    return false;
  }

  if ( UNLIKELY( !UObject::StaticInit() ) )
  {
    Logf( LOG_CRIT, "UObject::StaticInit() failed!" );
    return false;
  } 

  return true;
}

#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( USubsystem );
IMPLEMENT_NATIVE_CLASS( USystem );

BEGIN_PROPERTY_LINK( USubsystem, 1 )
  LINK_USELESS_PROPERTY( ExecVtbl );
END_PROPERTY_LINK()

