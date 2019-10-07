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
  extern "C" UClass* cls##StaticClass() { return cls::StaticClass(); }  \
  DLL_EXPORT UClass* cls::ObjectClass = NULL; \
  DLL_EXPORT size_t  cls::NativeSize  = sizeof( cls ); \
  DLL_EXPORT FNativePropertyList* cls::StaticNativePropList = NULL; \
  bool cls::StaticLoadNativePackage( const char* NativePkgName ) \
  { \
    ObjectClass->Pkg = UPackage::StaticLoadPackage( NativePkgName ); \
    if ( ObjectClass->Pkg == NULL ) \
    { \
      GLogf( LOG_CRIT, "Failed to load package '%s' for class '%s'.", NativePkgName, ObjectClass->Name ); \
      return false; \
    } \
    return true; \
  } \
  bool cls::StaticSetPackageProperties() \
  { \
    ObjectClass->Pkg = UPackage::StaticLoadPackage( NativePkgName ); \
    if ( ObjectClass->Pkg == NULL ) \
    { \
      GLogf( LOG_CRIT, "Failed to load package '%s' for class '%s'.", NativePkgName, ObjectClass->Name ); \
      return false; \
    } \
    ObjectClass->Export = ObjectClass->Pkg->GetClassExport( ObjectClass->Name.Data() ); \
    if ( ObjectClass->Export == NULL ) \
    { \
      ObjectClass->ObjectFlags = RF_EliminateObject; \
      return true; \
    } \
    ObjectClass->Export->Obj = ObjectClass; \
    ObjectClass->ObjectFlags = ObjectClass->Export->ObjectFlags; \
    ObjectClass->Pkg->bIntrinsicPackage = true; \
    return true; \
  } \
  bool cls::StaticCreateClass() \
  { \
    if (!ObjectClass) \
    { \
      UPackage* ClsPkg = UPackage::StaticLoadPackage( NativePkgName ); \
      if (!ClsPkg) \
      { \
        GLogf( LOG_CRIT, "Package '%s' for class '%s' could not be opened", \
          NativePkgName, TEXT(cls) ); \
        return false; \
      } \
      const char* ClsNameStr = TEXT(cls); \
      ClsNameStr++; \
      size_t PkgNameIdx = ClsPkg->FindName( ClsNameStr ); \
      FName ClsName = 0; \
      if ( PkgNameIdx != MAX_SIZE ) \
        ClsName = ClsPkg->GetGlobalName( PkgNameIdx ); \
      else \
        ClsName = FName::CreateName( ClsNameStr, RF_TagExp | RF_LoadContextFlags | RF_Native ); \
      ObjectClass = UObject::StaticAllocateClass( ClsName, StaticFlags, Super::ObjectClass,\
        NativeSize, NativeConstructor ); \
      if ( ObjectClass != NULL ) \
      { \
        ClassPool.push_back( ObjectClass ); \
        ObjectClass->bRegistered = true; \
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
