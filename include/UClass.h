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
 * UClass.h - Objects that describe classes of Unreal objects
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Array.h"
#include "String.h"
#include "UObject.h"

using namespace xstl;

class UTextBuffer : public UObject
{
  DECLARE_ABSTRACT_CLASS( UTextBuffer, UObject, 0, Core )
	virtual void LoadFromPackage( FPackageFileIn& In );

  u32 Pos, Top;
  String Text;
};

class UField : public UObject
{
  DECLARE_ABSTRACT_CLASS( UField, UObject, 0, Core )
  
  virtual void LoadFromPackage( FPackageFileIn& In );
  
  UField* SuperField; // Parent object
  UField* Next;       // Next object in list
};

class UConst : public UField
{
  DECLARE_CLASS( UConst, UField, 0, Core )

	UConst();
	virtual void LoadFromPackage( FPackageFileIn& In );
  
  String Value;
};

class UEnum : public UField
{
  DECLARE_CLASS( UEnum, UField, 0, Core )
	
	UEnum();
	virtual void LoadFromPackage( FPackageFileIn& In );
  
  Array<const char*> Names;
};

struct FScriptLabel
{
	idx Index;
	u32 Offset;
};

class UStruct : public UField
{
  DECLARE_CLASS( UStruct, UField, 0, Core )
 
	UStruct();
	UStruct( size_t InNativeSize );
  virtual void LoadFromPackage( FPackageFileIn& In );
  
  UTextBuffer* ScriptText;
  UField* Children;
  const char* FriendlyName;
  u32 Line;
  u32 TextPos;
  u32 ScriptSize;
  u8* ScriptCode;
  
  // Runtime variables
  u32 StructSize;
	Array<FScriptLabel>* LabelTable;
};

class UFunction : public UStruct
{
  DECLARE_CLASS( UFunction, UStruct, 0, Core )
  
  idx ParmsSize; // PackageVersion <= 63
  u16 iNative;   // Native function index
  idx NumParms;  // PackageVersion <= 63
  u8  OperatorPrecedence;
  idx ReturnValueOffset; // PackageVersion <= 63
  u32 FunctionFlags;
  u16 ReplicationOffset; // will be entirely unused
};

class UState : public UStruct
{
  DECLARE_CLASS( UState, UStruct, 0, Core )
  
  virtual void LoadFromPackage( FPackageFileIn& In );
  
  u64 ProbeMask;
  u64 IgnoreMask;
  u16 LabelTableOffset;
  u32 StateFlags;
};

struct FDependency
{
  FDependency();
  
  UClass* Class;
  u32 Deep;
  u32 ScriptTextCRC;
};

class UClass : public UState
{
  DECLARE_CLASS( UClass, UState, 0, Core )
  
  UClass();
  UClass( const char* ClassName, u32 Flags, UClass* SuperClass, UObject *(*NativeCtor)(size_t) );
  
  virtual void LoadFromPackage( FPackageFileIn& In );
  
  bool IsNative();
	UObject* CreateObject();

  u32 OldClassRecordSize; // PackageVersion <= 61
  u32 ClassFlags;
  u32 ClassGuid[4];
  Array<FDependency> Dependencies;
  Array<int> PackageImports;
  UClass* ClassWithin;
  const char* ClassConfigName;
  
  // Runtime variables
	UObject* Default;
  UObject* (*Constructor)(size_t);
	UClass*  SuperClass;
	bool     NativeNeedsPkgLoad;

private:
	char* CreateDefaultObjectName();
};

