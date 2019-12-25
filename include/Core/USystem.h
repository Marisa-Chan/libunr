/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU General Public License as published by     *|
|*  the Free Software Foundation, either version 3 of the License, or        *|
|*  (at your option) any later version.                                      *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU General Public License for more details.                             *|
|*                                                                           *|
|*  You should have received a copy of the GNU General Public License        *|
|*  along with this program. If not, see <https://www.gnu.org/licenses/>.    *|
\*===========================================================================*/

/*========================================================================
 * USystem.h - System class, manages runtime OS variables
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include "Core/UObject.h"

// Game specific flags
#define GAME_Unreal 0x00001
#define GAME_UT99   0x00002
#define GAME_DeusEx 0x00004
#define GAME_Rune   0x00008
#define GAME_HPSS   0x00010
#define GAME_All    0xFFFFF

class LIBUNR_API USubsystem : public UObject
{
  DECLARE_NATIVE_CLASS( USubsystem, UObject, CLASS_Abstract, Core )
  EXPOSE_TO_USCRIPT()

  USubsystem();

  virtual bool Init() { return false; }
  virtual bool Exit() { return false; }
  virtual void Tick( float DeltaTime ) {}

  bool bInitialized;
};

// Prompt callback for if the game has not been picked 
typedef int(*GamePromptCallback)( TArray<char*>* );

// Prompt callback for if audio/render devices have not been picked
typedef void(*DevicePromptCallback)(char*, char*); // RenderBuf, AudioBuf

// Function typedef for starting new threads
#if defined LIBUNR_WIN32
  typedef unsigned long ThreadReturnType;
#elif defined LIBUNR_POSIX
  typedef void* ThreadReturnType;
#else
  #error "Unknown operating system! Please add a section for ThreadReturnType in USystem.h"
#endif

typedef ThreadReturnType( *ThreadFunc )(void*);

class LIBUNR_API USystem : public USubsystem
{
  DECLARE_NATIVE_CLASS( USystem, USubsystem, CLASS_NoExport, Core )
  USystem();

  // Per instance methods
  const char* ResolvePath( const char* PkgName );
  void Exit( int ExitCode );

  bool PromptForGameInfo( char* InGameName = NULL );
  bool PromptForDeviceInfo();
  void* RunThread( ThreadFunc Func, void* Args );
  bool IsThreadActive( void* Thread );
  
  // Global methods
  static bool StaticInit( GamePromptCallback GPC, DevicePromptCallback DPC, bool InIsEditor, char* InGameName = NULL );
  static const char* GetLibunrIniPath();
  static const char* GetDefaultLibunrIniPath();
  static const char* GetNativeModulesPath();
  static bool CopyFile( const char* OrigFile, const char* NewFile );
  static bool FileExists( const char* Filename );
  static void RealPath( const char* Path, char* FullPath, size_t FullPathSize );
  static bool MakeDir( const char* Path );
  static bool IsEditor();
  static double GetSeconds();
#if defined LIBUNR_POSIX
  static const char* GetHomeDir();
#endif

  // libunr specific
  bool bEnhancedRuntime;
  bool bLogRefCntZero;
  const char* RenderDevice;
  const char* AudioDevice;
  const char* GamePath;
  const char* GameName;
  u32 GameFlags; // Flags for the games that are currently supported by this instance
  int EngineVersion;
  int MaxPackageVersion;

  // Per game
  int PurgeCacheDays;
  FString* SavePath;
  FString* CachePath;
  FString* CacheExt;
  TArray<FString*> Paths;

  static int LogLevel;
  static char* LibunrPath;

protected:
  GamePromptCallback DoGamePrompt;
  DevicePromptCallback DoDevicePrompt;
  TArray<void*> Threads;

  // Runtime
  static bool bIsEditor;
};

LIBUNR_API extern USystem* GSystem;
LIBUNR_API bool LibunrInit( GamePromptCallback GPC, DevicePromptCallback DPC, bool bIsEditor, char* GameName = NULL );

