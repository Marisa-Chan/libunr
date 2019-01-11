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

#include "Core/FConfig.h"
#include "XArray.h"
#include "XString.h"
#include "Core/UObject.h"

using namespace xstl;

class DLL_EXPORT UTextBuffer : public UObject
{
  DECLARE_NATIVE_ABSTRACT_CLASS( UTextBuffer, UObject, CLASS_NoExport, Core )

  UTextBuffer();
  virtual void Load();

  u32 Pos, Top;
  String* Text;
};

class DLL_EXPORT UField : public UObject
{
  DECLARE_NATIVE_ABSTRACT_CLASS( UField, UObject, CLASS_NoExport, Core )

  UField();
  virtual void Load();

  UField* SuperField; // Parent object
  UField* Next;       // Next object in list
};

class DLL_EXPORT UConst : public UField
{
  DECLARE_NATIVE_CLASS( UConst, UField, CLASS_NoExport, Core )

  UConst();
  virtual void Load();

  String* Value;
};

class DLL_EXPORT UEnum : public UField
{
  DECLARE_NATIVE_CLASS( UEnum, UField, CLASS_NoExport, Core )

  UEnum();
  virtual void Load();

  Array<const char*> Names;
};

struct FScriptLabel
{
  idx Index;
  u32 Offset;
};

class DLL_EXPORT UStruct : public UField
{
  DECLARE_NATIVE_CLASS( UStruct, UField, CLASS_NoExport, Core )

  UStruct();
  UStruct( size_t InNativeSize );
  virtual void Load();

  void FinalizeClassLoad();

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

class DLL_EXPORT UFunction : public UStruct
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

class DLL_EXPORT UState : public UStruct
{
  DECLARE_NATIVE_CLASS( UState, UStruct, CLASS_NoExport, Core )

  UState();
  virtual void Load();
 
  u64 ProbeMask;
  u64 IgnoreMask;
  u16 LabelTableOffset;
  u32 StateFlags;
};

struct DLL_EXPORT FDependency
{
  FDependency();
  
  UClass* Class;
  u32 Deep;
  u32 ScriptTextCRC;
};

class DLL_EXPORT UClass : public UState
{
  DECLARE_NATIVE_CLASS( UClass, UState, CLASS_NoExport, Core )
  EXPORTABLE();
  static void BootstrapStage1();
  static void BootstrapStage2();

  UClass();
  UClass( const char* ClassName, u32 Flags, UClass* SuperClass, 
      size_t InStructSize, UObject *(*NativeCtor)(size_t) );
  
  virtual void Load();
  virtual void PostLoad();

  bool IsNative();
  UObject* CreateObject();
  void SetSuperClassProperties();
  void LinkSuperClassChildren();

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
  FConfig* ClassConfig;
  bool     NativeNeedsPkgLoad;
  bool     bLinkedChildren;

  size_t DefPropListOffset;

private:
  char* CreateDefaultObjectName();
};

