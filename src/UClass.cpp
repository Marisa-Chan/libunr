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
 * UClass.cpp - Objects that describe classes of Unreal objects
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "UClass.h"
#include "UPackage.h"
#include "UProperty.h"
#include "UScript.h"
#include "USystem.h"

UTextBuffer::UTextBuffer()
{
  Text = NULL;
}

UTextBuffer::~UTextBuffer()
{
  if ( LIKELY( Text ) )
    delete Text;
}

void UTextBuffer::LoadFromPackage( FPackageFileIn& In )
{
  Super::LoadFromPackage( In );
  
  In >> Pos;
  In >> Top;

  idx TextSize;
  In >> CINDEX( TextSize );
  if ( TextSize > 0 )
  {
    char* TextBuf = (char*)Malloc( TextSize + 1 );
    In.Read( TextBuf, TextSize + 1 );
    Text = new String( TextBuf );
  }
}

// UField
UField::~UField()
{
  SuperField->RefCnt--;
  Next->RefCnt--;
}

void UField::LoadFromPackage( FPackageFileIn& In )
{
  Super::LoadFromPackage( In );
  
  idx SuperIdx;
  idx NextIdx;
  
  In >> CINDEX( SuperIdx );
  In >> CINDEX( NextIdx );
  
  if ( SuperIdx )
    SuperField = (UField*)UPackage::StaticLoadObject( Pkg, SuperIdx, NULL, Outer );
  
  if ( NextIdx )
    Next = (UField*)UPackage::StaticLoadObject( Pkg, NextIdx, NULL, Outer );
}

UConst::UConst()
{
  Value = NULL;
}

UConst::~UConst()
{
  if ( LIKELY( Value ) )
    delete Value;
}

void UConst::LoadFromPackage( FPackageFileIn& In )
{
  Super::LoadFromPackage( In );

  idx Size;
  In >> CINDEX( Size );

  // Size includes null terminator
  char* Str = (char*)Malloc( Size ); 
  In.Read( Str, Size );

  Value = new String( Str );
}

UEnum::UEnum()
{
}

UEnum::~UEnum()
{
  Names.Clear();
}

void UEnum::LoadFromPackage( FPackageFileIn& In )
{
  Super::LoadFromPackage( In );

  idx ArraySize;
  In >> CINDEX( ArraySize );
  Names.Reserve( ArraySize );

  for( int i = 0; i < ArraySize; i++ )
  {
    idx ElementIdx;
    In >> CINDEX( ElementIdx );
    Names.PushBack( Pkg->ResolveNameFromIdx( ElementIdx ) );
  }
}

UStruct::UStruct()
{
  ScriptText = NULL;
  Children   = NULL;
  FriendlyName = NULL;
  Line = 0;
  TextPos = 0;
  ScriptSize = 0;
  ScriptCode = NULL;
  NativeSize = 0;
  StructSize = 0;
}

UStruct::UStruct( size_t InNativeSize )
{
  ScriptText = NULL;
  Children   = NULL;
  FriendlyName = NULL;
  Line = 0;
  TextPos = 0;
  ScriptSize = 0;
  ScriptCode = NULL;
  NativeSize = InNativeSize;
  StructSize = 0;
}

// UStruct
UStruct::~UStruct()
{
}

// Script loading
#define LOAD_CODE( strct, var, type ) \
  *(strct->ScriptCode)++ = var

static inline void LoadScriptByte( UStruct* Struct, FPackageFileIn& In )
{
  u8 Byte;
  In >> Byte;
  LOAD_CODE( Struct, Byte, u8 );
}

static inline void LoadScriptWord( UStruct* Struct, FPackageFileIn& In )
{
  u16 Word;
  In >> Word;
  LOAD_CODE( Struct, Word, u16 );
}

static inline void LoadScriptDword( UStruct* Struct, FPackageFileIn& In )
{
  u32 Dword;
  In >> Dword;
  LOAD_CODE( Struct, Dword, u32 );
}

static inline void LoadScriptIndex( UStruct* Struct, FPackageFileIn& In )
{
  idx Index;
  In >> CINDEX( Index );
  LOAD_CODE( Struct, Index, idx );
}

static inline void LoadScriptFloat( UStruct* Struct, FPackageFileIn& In )
{
  float Float;
  In >> Float;
  LOAD_CODE( Struct, Float, float );
}

static inline void LoadScriptString( UStruct* Struct, FPackageFileIn& In )
{
  u8 Char = 0;
  do
  {
    In >> Char;
    LOAD_CODE( Struct, Char, u8 );
    
  } while ( Char != 0 );
}

// FIXME: We don't actually deal with unicode strings because we should be using
// UTF-8 instead, should we convert the characters later?
static inline void LoadScriptUnicodeString( UStruct* Struct, FPackageFileIn& In )
{
  u16 Wchar = 0;
  do
  {
    In >> Wchar;
    LOAD_CODE( Struct, Wchar, u16 );

  } while ( Wchar != 0 ); 
}

static inline void LoadScriptLabelTable( UStruct* Struct, FPackageFileIn& In )
{
  FScriptLabel Label;
  Label.Index = 0;
  Label.Offset = 0;

  while ( 1 )
  {
    In >> CINDEX( Label.Index );
    In >> Label.Offset;

    LOAD_CODE( Struct, Label.Index, idx );
    LOAD_CODE( Struct, Label.Offset, u32 );

    Struct->LabelTable->PushBack( Label );

    if ( strncmp( "None", Struct->Pkg->ResolveNameFromIdx( Label.Index ), 4 ) == 0 )
      break;
  }
}

// Helper function to load script code
// Does not care about type resolution whatsoever
static inline void LoadScriptCode( UStruct* Struct, FPackageFileIn& In )
{
  u8 Token = -1;
  bool bQueueReadIteratorWord = false;
  bool bReadIteratorWord = false;
  while ( Token != EX_LabelTable || Token != EX_Return )
  {
    In >> Token;
    LOAD_CODE( Struct, Token, u8 );

    // Handle extended native case up here since switch statements
    // can't really handle this type of comparison
    if ( UNLIKELY( (Token && 0xF0) == EX_ExtendedNative ) )
    {
      LoadScriptByte( Struct, In );
      continue;
    }

    switch ( Token )
    {
      case EX_LocalVariable:
      case EX_InstanceVariable:
      case EX_DefaultVariable:
      case EX_MetaCast:
      case EX_VirtualFunction:
      case EX_FinalFunction:
      case EX_ObjectConst:
      case EX_NameConst:
      case EX_NativeParm:
      case EX_DynamicCast:
      case EX_StructCmpEq:
      case EX_StructCmpNe:
      case EX_StructMember:
      case EX_GlobalFunction:
        LoadScriptIndex( Struct, In );
        break;
      case EX_Switch:
        LoadScriptByte( Struct, In );
        break;
      case EX_Jump:
      case EX_JumpIfNot:
      case EX_Assert:
      case EX_Case:
      case EX_Skip:
        LoadScriptWord( Struct, In );
        break;
      case EX_LabelTable:
        LoadScriptLabelTable( Struct, In );
        break;
      case EX_ClassContext:
        LoadScriptIndex( Struct, In );
        LoadScriptWord( Struct, In );
        LoadScriptByte( Struct, In );
        LoadScriptIndex( Struct, In );
        break;
      case EX_IntConst:
        LoadScriptDword( Struct, In );
        break;
      case EX_FloatConst:
        LoadScriptFloat( Struct, In );
        break;
      case EX_StringConst:
        LoadScriptString( Struct, In );
        break;
      case EX_RotationConst:
        LoadScriptDword( Struct, In );
        LoadScriptDword( Struct, In );
        LoadScriptDword( Struct, In );
        break;
      case EX_VectorConst:
        LoadScriptFloat( Struct, In );
        LoadScriptFloat( Struct, In );
        LoadScriptFloat( Struct, In );
        break;
      case EX_Iterator:
        // stupid edge cases... see below
        bReadIteratorWord = true;
        break;
      case EX_UnicodeStringConst:
        LoadScriptUnicodeString( Struct, In );
        break;
      case EX_Unk03:
      case EX_Unk15:
      case EX_Unk2b:
      case EX_Unk35:
      case EX_Unk37:
      case EX_Unk5a:
      case EX_Unk5b:
      case EX_Unk5c:
      case EX_Unk5d:
      case EX_Unk5e:
      case EX_Unk5f:
        Logf( LOG_WARN, "Loading unknown UnrealScript opcode 0x%x, loading may not finish properly", Token );
        break;
      // Everything else loads another token or nothing at all,
      // so we don't need explicit cases for them
      default:
        break;
    }

    // Iterator is the only opcode that has a word after a token
    // Normally we just loop around, but that doesn't work here
    if ( bQueueReadIteratorWord )
    {
      bReadIteratorWord = true;
      bQueueReadIteratorWord = false;
    }
    else if ( bReadIteratorWord )
    {
      LoadScriptWord( Struct, In );
      bReadIteratorWord = false;
    }
  }
}

void UStruct::LoadFromPackage( FPackageFileIn& In )
{
  Super::LoadFromPackage( In );
  
  idx ScriptTextIdx;
  idx ChildIdx;
  idx FriendlyNameIdx;
  
  In >> CINDEX( ScriptTextIdx );
  In >> CINDEX( ChildIdx );
  In >> CINDEX( FriendlyNameIdx );
  
  ScriptText = (UTextBuffer*)UPackage::StaticLoadObject( Pkg, ScriptTextIdx, UTextBuffer::StaticClass(), this );
  Children = (UField*)UPackage::StaticLoadObject( Pkg, ChildIdx, NULL, this );
  FriendlyName = Pkg->ResolveNameFromIdx( FriendlyNameIdx );
  In >> Line;
  In >> TextPos;
  In >> ScriptSize;
  
  ScriptCode = new u8[ ScriptSize ];
  ::LoadScriptCode( this, In );

  // Calculate struct size
  StructSize += NativeSize;
  u32 InfoSize = 0;
  for ( UField* ChildIter = Children; ChildIter != NULL; ChildIter = ChildIter->Next )
  {
    if ( ChildIter->IsA( UProperty::StaticClass() ) )
    {
      UProperty* Prop = (UProperty*)ChildIter;
      StructSize += Prop->ElementSize;
    }
    InfoSize += ChildIter->GetNativeSize();
  }
}

UFunction::UFunction()
{
}

void UFunction::LoadFromPackage( FPackageFileIn& In )
{
  Super::LoadFromPackage( In );

  if ( In.Ver <= PKG_VER_UN_220 )
    In >> CINDEX( ParmsSize );

  In >> iNative;

  if ( In.Ver <= PKG_VER_UN_220 )
    In >> CINDEX( NumParms );

  In >> OperatorPrecedence;

  if ( In.Ver <= PKG_VER_UN_220 )
    In >> CINDEX( ReturnValueOffset );

  In >> FunctionFlags;
  if ( FunctionFlags & FUNC_Net )
    In >> ReplicationOffset;
}

// UState
UState::~UState()
{
}

void UState::LoadFromPackage( FPackageFileIn& In )
{
  Super::LoadFromPackage( In );
  
  In >> ProbeMask;
  In >> IgnoreMask;
  In >> LabelTableOffset;
  In >> StateFlags;
}

// UClass
UClass::UClass()
{
  
}

UClass::UClass( const char* ClassName, u32 Flags, UClass* InSuperClass, UObject *(*NativeCtor)(size_t) )
{
  Name = ClassName;
  ClassFlags = Flags;
  SuperField = InSuperClass;
  Constructor = NativeCtor;
  NativeNeedsPkgLoad = true;
}

UClass::~UClass()
{
  Default->RefCnt--;
  delete Default;
}

void UClass::LoadFromPackage( FPackageFileIn& In )
{
  Super::LoadFromPackage( In );

  In >> ClassFlags;
  In.Read( ClassGuid, sizeof( ClassGuid ) );
  
  idx DepCount;
  In >> CINDEX( DepCount );
  Dependencies.Reserve( DepCount );
  
  FDependency Dep;
  for ( int i = 0; i < DepCount; i++ )
  {
    idx DepObjRef;
    In >> CINDEX( DepObjRef );
    
    if ( stricmp( Pkg->ResolveNameFromObjRef( DepObjRef ), FriendlyName ) )
      Dep.Class = (UClass*)UPackage::StaticLoadObject( Pkg, DepObjRef, UClass::StaticClass() );
    
    In >> Dep.Deep;
    In >> Dep.ScriptTextCRC;
    
    Dependencies.PushBack( Dep );
  }
  
  // I don't actually know what these are for
  idx NumPkgImports;
  In >> CINDEX( NumPkgImports );
  PackageImports.Reserve( NumPkgImports );
  
  for ( int i = 0; i < NumPkgImports; i++ )
  {
    idx PkgImport;
    In >> CINDEX( PkgImport );
    PackageImports.PushBack( PkgImport );
  }
  
  if ( In.Ver >= PKG_VER_UN_220 - 1 ) // package version 62 ??
  {
    idx ClassWithinIdx;
    idx ClassConfigNameIdx;
    
    In >> CINDEX( ClassWithinIdx );
    In >> CINDEX( ClassConfigNameIdx );
    
    ClassWithin = (UClass*)UPackage::StaticLoadObject( Pkg, ClassWithinIdx, UClass::StaticClass() );
    ClassConfigName = Pkg->ResolveNameFromIdx( ClassConfigNameIdx );

    FHash CfgHash = FnvHashString( ClassConfigName );
    if ( CfgHash == FnvHashString( "System" ) )
      ClassConfig = GGameConfig;
    else
      ClassConfig = GConfigManager->GetConfig( ClassConfigName );
  }
  else
    ClassConfig = GGameConfig;
  
  // Construct default object
  if ( Constructor == NULL )
    Constructor = SuperClass->Constructor;

  Default = CreateObject();
  Default->Class = this;

  // In Unreal, property lists seem to take precedent over config properties
  // While we should enforce that a config property is never in the property
  // list when saving packages, we will read the config property first before
  // overriding it with the property in the list
  if ( ( ClassFlags & CLASS_Config ) ) 
    Default->ReadConfigProperties();

  Default->ReadDefaultProperties( In );
}

bool UClass::IsNative()
{
  // NOTE: this is NOT checking class flags
  return (Flags & RF_Native) != 0;
}

UObject* UClass::CreateObject()
{
  return Constructor( StructSize );
}

char* UClass::CreateDefaultObjectName()
{
  char* DefObjName = (char*)Malloc( 1024 );
  memset( DefObjName, 0, 1024 );
  strcat( DefObjName, "Default" );
  strcat( DefObjName, Name );
  return DefObjName;
}

