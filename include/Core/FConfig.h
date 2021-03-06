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
 * FConfig.h - Class for loading and saving INI files
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/FUtil.h"
#include <libxstl/XArray.h>
#include <libxstl/XFileStream.h>

using namespace xstl;
class UObject;
class UStruct;

class DLL_EXPORT FConfig
{
public:
  FConfig();
  FConfig( const char* Filename );
  ~FConfig();

  int Load( const char* Filename );
  int Save();

  // Read functions
  // These functions will try to read the requested variable
  // If the Variable does not exist, a default value will be created and added to the config
  // If the SubVariable does not exist, a default value will be returned but not added to config
  char* ReadString( const char* Category, const char* Variable, size_t Index = 0 );
  bool  ReadBool  ( const char* Category, const char* Variable, size_t Index = 0 );
  u64 ReadUInt64( const char* Category, const char* Variable, size_t Index = 0 ); 
  u32 ReadUInt32( const char* Category, const char* Variable, size_t Index = 0 );
  u16 ReadUInt16( const char* Category, const char* Variable, size_t Index = 0 );
  u8  ReadUInt8 ( const char* Category, const char* Variable, size_t Index = 0 );

  i64 ReadInt64( const char* Category, const char* Variable, size_t Index = 0 );
  i32 ReadInt32( const char* Category, const char* Variable, size_t Index = 0 );
  i16 ReadInt16( const char* Category, const char* Variable, size_t Index = 0 );
  i8  ReadInt8 ( const char* Category, const char* Variable, size_t Index = 0 );

  float  ReadFloat ( const char* Category, const char* Variable, size_t Index = 0 );
  double ReadDouble( const char* Category, const char* Variable, size_t Index = 0 );

  void ReadStruct( const char* Category, const char* Variable, UStruct* Struct, void* StructLoc, size_t Index = 0 );
  void ReadObject( const char* Category, const char* Variable, UObject* Obj, size_t Index = 0 );

  // Write functions
  // These functions will write a variable to the config, updating it if it exists, creating
  // a new one otherwise
  void WriteString( const char* Category, const char* Variable, const char* Value, size_t Index = 0 );
  void WriteBool  ( const char* Category, const char* Variable, bool Value, size_t Index = 0 );
  void WriteUInt64( const char* Category, const char* Variable, u64 Value, size_t Index = 0 );
  void WriteUInt32( const char* Category, const char* Variable, u32 Value, size_t Index = 0 );
  void WriteUInt16( const char* Category, const char* Variable, u16 Value, size_t Index = 0 );
  void WriteUInt8 ( const char* Category, const char* Variable, u8 Value, size_t Index = 0 );
   
  void WriteInt64( const char* Category, const char* Variable, i64 Value, size_t Index = 0 );
  void WriteInt32( const char* Category, const char* Variable, i32 Value, size_t Index = 0 );
  void WriteInt16( const char* Category, const char* Variable, i16 Value, size_t Index = 0 );
  void WriteInt8 ( const char* Category, const char* Variable, i8 Value, size_t Index = 0 );

  void WriteFloat ( const char* Category, const char* Variable, float Value, size_t Index = 0 );
  void WriteDouble( const char* Category, const char* Variable, double Value, size_t Index = 0 );

  void WriteObject( const char* Category, const char* Variable, UObject* Obj, size_t Index = 0 );

  Array<char*>* CreateEntry( const char* Category, const char* Variable );

  // Accessors
  const char* GetName();

private:
  struct FConfigEntry
  {
    FConfigEntry();
    ~FConfigEntry();

    char*  Name;
    FHash  Hash;
    Array<char*>* Values;
    Array<FConfigEntry*>* StructVars;
    bool bWriteIndices;
  };

  struct FConfigCategory
  {
    FConfigCategory();
    ~FConfigCategory();

    char*  Name;
    FHash  Hash;
    Array<FConfigEntry*>* Entries;
  };

  Array<FConfigCategory*> Categories;
  char* Name;
};

extern DLL_EXPORT FConfig* GLibunrConfig; // Used only before loading any game
extern DLL_EXPORT FConfig* GGameConfig;   // Used when a game is picked
extern DLL_EXPORT FConfig* GUserConfig;   // Used when a game is picked

class DLL_EXPORT FConfigManager
{
public:
  FConfigManager();
  ~FConfigManager();

  void AddConfig( FConfig* Cfg );
  FConfig* GetConfig( const char* Name );
  void DelConfig( FConfig* Cfg );
  void SaveAndCloseConfigs();
  void CloseConfigs();

private:
  Array<FConfig*> Configs;
};

extern FConfigManager* GConfigManager;

