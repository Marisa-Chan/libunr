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
 * UNativeModule.cpp - Objects that describe code modules with native code
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Util/FString.h"
#include "Core/UNativeModule.h"
#include "Core/USystem.h"
#include "Core/UClass.h"
#include "Core/UPackage.h"

IMPLEMENT_NATIVE_CLASS( UNativeModule );
IMPLEMENT_NATIVE_CLASS( UDynamicNativeModule );

#if defined __linux__ || defined __unix__
  #define NATIVE_MODULE_EXT "so"
#elif defined __APPLE__
  #define NATIVE_MODULE_EXT "dylib" //Temporary
#elif defined _WIN32
  #define NATIVE_MODULE_EXT "dll"
  #include <Windows.h>
#elif not defined NO_DYNAMIC_NATIVE_MODULES
  #error "No dynamic native module extension provided"
#endif

UNativeModule::UNativeModule()
  : UObject()
{
}

UNativeModule::~UNativeModule()
{
}

bool UNativeModule::Load( const char* Filename )
{
  return false;
}

UClass* UNativeModule::GetNativeClass( const char* ClsName )
{
  return NULL;
}

UDynamicNativeModule::UDynamicNativeModule()
  : UNativeModule()
{
  ModulePtr = NULL;
}

UDynamicNativeModule::~UDynamicNativeModule()
{
}

bool UDynamicNativeModule::Load( const char* Filename )
{
  // Check libunr folder and game system folder
  FString SystemDir = FString( GSystem->GamePath );
  SystemDir += "/System";

  FStringFilePath* SystemPath = new FStringFilePath( SystemDir.Data(), Filename, NATIVE_MODULE_EXT );
  FStringFilePath* LibunrPath = new FStringFilePath( USystem::GetNativeModulesPath(), Filename, NATIVE_MODULE_EXT );

  FStringFilePath* Path = LibunrPath;
  if ( !USystem::FileExists( LibunrPath->Data() ) ) 
  {
    if ( !USystem::FileExists( SystemPath->Data() ) )
    {
      GLogf( LOG_ERR, "Cannot find native module '%s'", Filename );
      return false;
    }
    else
      Path = SystemPath;
  }

#if defined LIBUNR_POSIX
  ModulePtr = dlopen( Path->Data(), RTLD_LAZY );
  if ( ModulePtr == NULL )
  {
    GLogf( LOG_ERR, "Failed to open native module '%s': %s", Filename, dlerror() );
    return false;
  }
#elif defined LIBUNR_WIN32
  ModulePtr = LoadLibrary( Path->Data() );
  if ( ModulePtr == NULL )
  {
    GLogf( LOG_ERR, "Failed to open native module '%s': %d", Filename, GetLastError() );
    return false;
  }
#endif

  delete LibunrPath;
  delete SystemPath;
  return true;
}

UClass* UDynamicNativeModule::GetNativeClass( const char* ClsName )
{
  if ( ModulePtr == NULL )
  {
    GLogf( LOG_ERR, "%s->GetNativeClass() called on unloaded module", Name.Data() );
    return NULL;
  }

  FString StaticClassFuncName;
  StaticClassFuncName += 'U';
  StaticClassFuncName += ClsName;
  StaticClassFuncName += "StaticClass";

  UClass* (*StaticClassFunc)(void) = NULL;

#if defined LIBUNR_POSIX

  StaticClassFunc = (UClass*(*)(void))dlsym( ModulePtr, StaticClassFuncName.Data() );
  if ( StaticClassFunc == NULL )
  {
    GLogf( LOG_ERR, "Class '%s' does not exist in native module '%s'", ClsName, Name.Data() );
    return NULL;
  }

#elif defined LIBUNR_WIN32

  StaticClassFunc = (UClass*(*)(void))GetProcAddress( (HMODULE)ModulePtr, StaticClassFuncName.Data() );
  if ( StaticClassFunc == NULL )
  {
    GLogf( LOG_ERR, "Class '%s' does not exist in native module '%s'", ClsName, Name.Data() );
    return NULL;
  }

#endif
  return StaticClassFunc();
}
