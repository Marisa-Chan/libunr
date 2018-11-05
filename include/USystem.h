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
 * USystem.h - System class, manages runtime OS variables
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include "UObject.h"

class DLL_EXPORT USubsystem : public UObject
{
  DECLARE_NATIVE_ABSTRACT_CLASS( USubsystem, UObject, 0, Core )
  USubsystem();
};

// Prompt callback for if the game has not been picked 
typedef void(*GamePromptCallback)(char*, char*); // PathBuf, NameBuf
#define GPC_NAME_BUF_LEN 32
#define GPC_PATH_BUF_LEN 4096

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
  static bool StaticInit( GamePromptCallback GPC, DevicePromptCallback DPC );
  static const char* GetLibunrIniPath();
  static const char* GetDefaultLibunrIniPath();
  static bool CopyFile( const char* OrigFile, const char* NewFile );
  static bool FileExists( const char* Filename );
#if defined __linux__ || __unix__
  static const char* GetHomeDir();
#endif

  // libunr specific
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
};

extern USystem* GSystem;
DLL_EXPORT bool LibunrInit( GamePromptCallback GPC, DevicePromptCallback DPC );


