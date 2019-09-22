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
 * UNativeModule.h - Objects that describe code modules with native code
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/UObject.h"

#if defined __linux__ || defined __unix__
  #include <dlfcn.h>
#endif

#define IMPLEMENT_MODULE_CLASS(cls) \
  extern "C" UClass* cls##StaticClass() { return cls::StaticClass(); }

class DLL_EXPORT UNativeModule : public UObject
{
  DECLARE_NATIVE_CLASS( UNativeModule, UObject, CLASS_Abstract | CLASS_NoExport, Core )

  UNativeModule();
  virtual bool Load( const char* Filename );
  virtual UClass* GetNativeClass( const char* ClsName );
};

class DLL_EXPORT UDynamicNativeModule : public UNativeModule
{
  DECLARE_NATIVE_CLASS( UDynamicNativeModule, UNativeModule, CLASS_NoExport, Core )

  UDynamicNativeModule();
  virtual bool Load( const char* Filename );
  virtual UClass* GetNativeClass( const char* ClsName );

  void* ModulePtr;
};
