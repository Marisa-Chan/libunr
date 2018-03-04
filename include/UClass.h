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

#include "UObject.h"
#include "TArray.h"

class UTextBuffer : public UObject
{
  DECLARE_ABSTRACT_CLASS( UTextBuffer, UObject, 0 )
  NO_DEFAULT_CONSTRUCTOR( UTextBuffer )
  
  u32 Pos, Top;
  FString Text;
};

class UField : public UObject
{
  DECLARE_ABSTRACT_CLASS( UField, UObject, 0 )
  NO_DEFAULT_CONSTRUCTOR( UField )
  
  UField* SuperField; // Parent object
  UField* Next;       // Next object in list
};

class UConst : public UField
{
  DECLARE_CLASS( UConst, UField, 0 )
  NO_DEFAULT_CONSTRUCTOR( UConst )
  
  FString Value;
};

class UEnum : public UField
{
  DECLARE_CLASS( UEnum, UField, 0 )
  NO_DEFAULT_CONSTRUCTOR( UEnum )
  
  TArray<FName> Names;
};

class UProperty : public UField
{
  DECLARE_ABSTRACT_CLASS( UProperty, UField, 0 )
  
  u16 ArrayDim;
  u16 ElementSize;
  u32 PropertyFlags;
  FName Category;
  u16 ReplicationOffset; // only exists since objects may store this variable
};

class UStruct : public UField
{
  DECLARE_CLASS( UStruct, UField, 0 )
  NO_DEFAULT_CONSTRUCTOR( UStruct )
  
  UTextBuffer* ScriptText;
  UField* Children;
  FName FriendlyName;
  u32 Line;
  u32 TextPos;
  u32 ScriptSize;
  u8* ScriptCode;
};

class UFunction : public UStruct
{
  DECLARE_CLASS( UFunction, UStruct, 0 )
  NO_DEFAULT_CONSTRUCTOR( UFunction )
  
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
  DECLARE_CLASS( UState, UStruct, 0 )
  NO_DEFAULT_CONSTRUCTOR( UState )
  
  u64 ProbeMask;
  u64 IgnoreMask;
  u16 LabelTableOffset;
  u32 StateFlags;
};

class FDependency
{
  FDependency();
  FDependency( UClass* InClass, bool InDeep );
  
  UClass* Class;
  u32 Deep;
  u32 ScriptTextCRC;
};

class UClass : public UState
{
  DECLARE_CLASS( UClass, UState, 0 )
  
  UClass();
  UClass( u32 Flags );
  
  u32 OldClassRecordSize; // PackageVersion <= 61
  u32 ClassFlags;
  u32 ClassGuid[4];
  idx DepCount;
  TArray<FDependency> Dependencies;
  TArray<FName> PackageImports;
  UClass* ClassWithin;
  FName ClassConfigName;
};
