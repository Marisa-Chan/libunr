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

#if defined __linux__ || defined __unix__ || defined __APPLE__
  #include <dlfcn.h>
#endif
//TODO: LIne 65, fix that shit
#define IMPLEMENT_MODULE_CLASS(cls) \
  extern "C" DLL_EXPORT UClass* cls##StaticClass() \
  { \
    if ( cls::StaticClass() == NULL ) \
      cls::StaticClassInit(); \
    return cls::StaticClass(); \
  } \
  DLL_EXPORT UClass* cls::ObjectClass = NULL; \
  DLL_EXPORT size_t  cls::NativeSize  = sizeof( cls ); \
  DLL_EXPORT FNativePropertyList* cls::StaticNativePropList = NULL; \
  bool cls::StaticSetPackageProperties() \
  { \
    ObjectClass->Export = ObjectClass->Pkg->GetClassExport( ObjectClass->Name.Data() ); \
    if ( ObjectClass->Export == NULL ) \
    { \
      ObjectClass->ObjectFlags = RF_EliminateObject; \
      return true; \
    } \
    ObjectClass->Export->Obj = ObjectClass; \
    ObjectClass->ObjectFlags = ObjectClass->Export->ObjectFlags; \
    return true; \
  } \
  bool cls::StaticCreateClass() \
  { \
    const char* ClsNameStr = TXT(cls); \
    ClsNameStr++; \
    if (!ObjectClass) { \
      FName ClsName; \
      UPackage* ClsPkg = UPackage::StaticLoadPackage( NativePkgName ); \
      if ( ClsPkg == NULL ) { \
        if ( StaticFlags & CLASS_NoExport ) { \
          ClsPkg = UPackage::StaticCreatePackage( NativePkgName, NULL ); \
          ClsName = FName( ClsNameStr, RF_LoadContextFlags | RF_Native ); \
        } else { \
          GLogf( LOG_CRIT, "Failed to load package '%s' for class '%s'.", NativePkgName, ClsNameStr ); \
          return false; \
        } \
      } else { \
        ClsName = ClsPkg->FindLocalName( ClsNameStr ); \
        if ( ClsName == MAX_SIZE ) { \
          GLogf( LOG_CRIT, "Failed to find class '%s' in package '%s'", ClsNameStr, NativePkgName ); \
          return false; \
        } \
      } \
      ObjectClass = UObject::StaticAllocateClass( ClsName, StaticFlags, Super::ObjectClass,\
        NativeSize, NativeConstructor ); \
      if ( ObjectClass != NULL ) \
      { \
        GetGlobalClassPool()->PushBack( ObjectClass ); \
        ObjectClass->bRegistered = true; \
        ObjectClass->Pkg = ClsPkg; \
        return true; \
      } \
      return false; \
    } \
    return true; \
  } \

class LIBUNR_API UNativeModule : public UObject
{
  DECLARE_NATIVE_CLASS( UNativeModule, UObject, CLASS_Abstract | CLASS_NoExport, Core )

  UNativeModule();
  virtual bool Load( const char* Filename );
  virtual UClass* GetNativeClass( const char* ClsName );
};

class LIBUNR_API UDynamicNativeModule : public UNativeModule
{
  DECLARE_NATIVE_CLASS( UDynamicNativeModule, UNativeModule, CLASS_NoExport, Core )

  UDynamicNativeModule();
  virtual bool Load( const char* Filename );
  virtual UClass* GetNativeClass( const char* ClsName );

  void* ModulePtr;
};
