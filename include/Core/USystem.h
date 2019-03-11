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
 * USystem.h - System class, manages runtime OS variables
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include "Core/UObject.h"

class DLL_EXPORT USubsystem : public UObject
{
  DECLARE_NATIVE_ABSTRACT_CLASS( USubsystem, UObject, 0, Core )
  EXPOSE_TO_USCRIPT()

  USubsystem();

  // ??
  int ExecVtbl;
};

// Prompt callback for if the game has not been picked 
typedef int(*GamePromptCallback)(Array<char*>*); // Names

// Prompt callback for if audio/render devices have not been picked
typedef void(*DevicePromptCallback)(char*, char*); // RenderBuf, AudioBuf

class DLL_EXPORT USystem : public USubsystem
{
  DECLARE_NATIVE_CLASS( USystem, USubsystem, CLASS_NoExport, Core )
  USystem();

  // Per instance method
  const char* ResolvePath( const char* PkgName );
  void Exit( int ExitCode );

  bool PromptForGameInfo();
  bool PromptForDeviceInfo();
  
  // Global methods
  static bool StaticInit( GamePromptCallback GPC, DevicePromptCallback DPC, bool InIsEditor );
  static const char* GetLibunrIniPath();
  static const char* GetDefaultLibunrIniPath();
  static bool CopyFile( const char* OrigFile, const char* NewFile );
  static bool FileExists( const char* Filename );
  static void RealPath( const char* Path, char* FullPath, size_t FullPathSize );
  static bool MakeDir( const char* Path );
  static bool IsEditor();
#if defined __linux__ || __unix__
  static const char* GetHomeDir();
#endif

  // libunr specific
  bool bEnhancedRuntime;
  bool bLogRefCntZero;
  const char* RenderDevice;
  const char* AudioDevice;
  const char* GamePath;
  const char* GameName;

  // Per game
  int PurgeCacheDays;
  String* SavePath;
  String* CachePath;
  String* CacheExt;
  Array<String*> Paths;

protected:
  GamePromptCallback DoGamePrompt;
  DevicePromptCallback DoDevicePrompt;

  // Runtime
  static bool bIsEditor;
};

extern USystem* GSystem;
DLL_EXPORT bool LibunrInit( GamePromptCallback GPC, DevicePromptCallback DPC, bool bIsEditor );

