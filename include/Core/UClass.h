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
 * UClass.h - Objects that describe classes of Unreal objects
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Util/TArray.h"
#include "Util/TQueue.h"
#include "Util/FConfig.h"
#include "Core/UObject.h"

class LIBUNR_API UTextBuffer : public UObject
{
  DECLARE_NATIVE_CLASS( UTextBuffer, UObject, CLASS_Abstract | CLASS_NoExport, Core )

  UTextBuffer();
  virtual void Load();

  u32 Pos, Top;
  FString* Text;
};

class LIBUNR_API UField : public UObject
{
  DECLARE_NATIVE_CLASS( UField, UObject, CLASS_Abstract | CLASS_NoExport, Core )

  UField();
  virtual void Load();

  UField* SuperField; // Parent object
  UField* Next;       // Next object in list
};

class LIBUNR_API UConst : public UField
{
  DECLARE_NATIVE_CLASS( UConst, UField, CLASS_NoExport, Core )

  UConst();
  virtual void Load();

  FString* Value;
};

class LIBUNR_API UEnum : public UField
{
  DECLARE_NATIVE_CLASS( UEnum, UField, CLASS_NoExport, Core )

  UEnum();
  virtual void Load();

  TArray<FName> Names;
};

struct FScriptLabel
{
  idx Index;
  u32 Offset;
};

class LIBUNR_API UStruct : public UField
{
  DECLARE_NATIVE_CLASS( UStruct, UField, CLASS_NoExport, Core )

  UStruct();
  UStruct( size_t InNativeSize );
  virtual void Load();

  void FinalizeClassLoad();

  UTextBuffer* ScriptText;
  UField* Children;
  FName FriendlyName;
  u32 Line;
  u32 TextPos;
  u32 ScriptSize;
  u8* ScriptCode;
  
  // Runtime variables
  u32 StructSize;
  TArray<FScriptLabel>* LabelTable;
  bool bFinalizedLoad; // So FinalizeClassLoad doesn't needlessly recurse
};

enum EFunctionFlags
{
  // Function flags.
  FUNC_Final       = 0x00000001,  // Function is final (prebindable, non-overridable function).
  FUNC_Defined     = 0x00000002,  // Function has been defined (not just declared).
  FUNC_Iterator    = 0x00000004,  // Function is an iterator.
  FUNC_Latent      = 0x00000008,  // Function is a latent state function.
  FUNC_PreOperator = 0x00000010,  // Unary operator is a prefix operator.
  FUNC_Singular    = 0x00000020,  // Function cannot be reentered.
  FUNC_Net         = 0x00000040,  // Function is network-replicated.
  FUNC_NetReliable = 0x00000080,  // Function should be sent reliably on the network.
  FUNC_Simulated   = 0x00000100,  // Function executed on the client side.
  FUNC_Exec        = 0x00000200,  // Executable from command line.
  FUNC_Native      = 0x00000400,  // Native function.
  FUNC_Event       = 0x00000800,  // Event function.
  FUNC_Operator    = 0x00001000,  // Operator function.
  FUNC_Static      = 0x00002000,  // Static function.
  FUNC_NoExport    = 0x00004000,  // Don't export intrinsic function to C++.
  FUNC_Const       = 0x00008000,  // Function doesn't modify this object.
  FUNC_Invariant   = 0x00010000,  // Return value is purely dependent on parameters; 
                                  //   no state dependencies or internal state changes.

  // Combinations of flags.
  FUNC_FuncInherit        = FUNC_Exec | FUNC_Event,
  FUNC_FuncOverrideMatch  = FUNC_Exec | FUNC_Final | FUNC_Latent | FUNC_PreOperator | FUNC_Iterator | FUNC_Static,
  FUNC_NetFuncFlags       = FUNC_Net  | FUNC_NetReliable,
};

class LIBUNR_API UFunction : public UStruct
{
  DECLARE_NATIVE_CLASS( UFunction, UStruct, CLASS_NoExport, Core )

  UFunction();
  virtual void Load();

  idx ParmsSize; // PackageVersion <= 63
  u16 iNative;   // Native function index
  idx NumParms;  // PackageVersion <= 63
  u8  OperatorPrecedence;
  idx ReturnValueOffset; // PackageVersion <= 63
  u32 FunctionFlags;
  u16 ReplicationOffset; // will be entirely unused
};

class LIBUNR_API UState : public UStruct
{
  DECLARE_NATIVE_CLASS( UState, UStruct, CLASS_NoExport, Core )

  UState();
  virtual void Load();
 
  u64 ProbeMask;
  u64 IgnoreMask;
  u16 LabelTableOffset;
  u32 StateFlags;
};

struct LIBUNR_API FDependency
{
  FDependency();
  
  UClass* Class;
  u32 Deep;
  u32 ScriptTextCRC;
};

class LIBUNR_API UClass : public UState
{
  DECLARE_NATIVE_CLASS( UClass, UState, CLASS_NoExport, Core )
  
  static void BootstrapStage1();
  static void BootstrapStage2();

  UClass();
  UClass( FName ClassName, u32 Flags, UClass* SuperClass, 
    u32 InStructSize, UObject *(*NativeCtor)(size_t) );
  
  virtual void Load();
  virtual void PostLoad();
  virtual bool ExportToFile( const char* Dir, const char* Type );

  bool ClassIsA( UClass* ClassType );
  bool IsNative();
  UObject* CreateObject( FName InName = 0 );
  void SetSuperClassProperties();
  void LinkSuperClassChildren();

  u32 OldClassRecordSize; // PackageVersion <= 61
  u32 ClassFlags;
  u32 ClassGuid[4];
  TArray<FDependency> Dependencies;
  TArray<int> PackageImports;
  UClass* ClassWithin;
  FName ClassConfigName;
  
  // Runtime variables
  UObject* Default;
  UObject* (*Constructor)(size_t);
  UClass*  SuperClass;
  FConfig* ClassConfig;
  bool     NativeNeedsPkgLoad;
  bool     bLinkedChildren;
  bool     bRegistered;

  size_t DefPropListOffset;

private:
  UObject* CreateDefaultObject();
  char* CreateDefaultObjectName();
  static TQueue<UClass*> DefPropQueue;
};

