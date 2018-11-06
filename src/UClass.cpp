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
  : UObject()
{
  Text = NULL;
}

UTextBuffer::~UTextBuffer()
{
  if ( LIKELY( Text ) )
    delete Text;
}

void UTextBuffer::LoadFromPackage( FPackageFileIn* In )
{
  Super::LoadFromPackage( In );
  
  *In >> Pos;
  *In >> Top;

  idx TextSize;
  *In >> CINDEX( TextSize );
  if ( TextSize > 0 )
  {
    char* TextBuf = (char*)xstl::Malloc( TextSize + 1 );
    In->Read( TextBuf, TextSize + 1 );
    Text = new String( TextBuf );
    xstl::Free( TextBuf );
  }
}

// UField
UField::UField()
  : UObject()
{
  SuperField = NULL;
  Next = NULL;
}

UField::~UField()
{
  SuperField->DelRef();
  Next->DelRef();
}

void UField::LoadFromPackage( FPackageFileIn* In )
{
  Super::LoadFromPackage( In );
  
  idx SuperIdx = MAX_UINT32;
  idx NextIdx = MAX_UINT32;
  
  *In >> CINDEX( SuperIdx );
  *In >> CINDEX( NextIdx );
  
  if ( SuperIdx )
    SuperField = (UField*)UPackage::StaticLoadObject( Pkg, SuperIdx, NULL, Outer );
  
  if ( NextIdx )
    Next = (UField*)UPackage::StaticLoadObject( Pkg, NextIdx, NULL, Outer );
}

UConst::UConst()
  : UField()
{
  Value = NULL;
}

UConst::~UConst()
{
  if ( LIKELY( Value ) )
    delete Value;
}

void UConst::LoadFromPackage( FPackageFileIn* In )
{
  Super::LoadFromPackage( In );

  idx Size;
  *In >> CINDEX( Size );

  // Size includes null terminator
  char* Str = (char*)xstl::Malloc( Size ); 
  In->Read( Str, Size );

  Value = new String( Str );
  xstl::Free( Str );
}

UEnum::UEnum()
  : UField()
{
}

UEnum::~UEnum()
{
  Names.Clear();
}

void UEnum::LoadFromPackage( FPackageFileIn* In )
{
  Super::LoadFromPackage( In );

  idx ArraySize;
  *In >> CINDEX( ArraySize );
  Names.Reserve( ArraySize );

  for( int i = 0; i < ArraySize; i++ )
  {
    idx ElementIdx;
    *In >> CINDEX( ElementIdx );
    Names.PushBack( Pkg->ResolveNameFromIdx( ElementIdx ) );
  }
}

UStruct::UStruct()
  : UField()
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
  : UField()
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
  ScriptText->DelRef();
  Children->DelRef();
}

// Script loading
#define LOAD_CODE( strct, var, type ) \
  *(strct->ScriptCode)++ = var

static inline void LoadScriptByte( UStruct* Struct, FPackageFileIn* In, u32* ParsedSize )
{
  u8 Byte;
  *In >> Byte;
  *ParsedSize += 1;
  LOAD_CODE( Struct, Byte, u8 );
}

static inline void LoadScriptWord( UStruct* Struct, FPackageFileIn* In, u32* ParsedSize )
{
  u16 Word;
  *In >> Word;
  *ParsedSize += 2;
  LOAD_CODE( Struct, Word, u16 );
}

static inline void LoadScriptDword( UStruct* Struct, FPackageFileIn* In, u32* ParsedSize )
{
  u32 Dword;
  *In >> Dword;
  *ParsedSize += 4;
  LOAD_CODE( Struct, Dword, u32 );
}

static inline void LoadScriptIndex( UStruct* Struct, FPackageFileIn* In, u32* ParsedSize )
{
  idx Index;
  *In >> CINDEX( Index );
  *ParsedSize += 4;
  LOAD_CODE( Struct, Index, idx );
}

static inline void LoadScriptFloat( UStruct* Struct, FPackageFileIn* In, u32* ParsedSize )
{
  float Float;
  *In >> Float;
  *ParsedSize += 4;
  LOAD_CODE( Struct, Float, float );
}

static inline void LoadScriptString( UStruct* Struct, FPackageFileIn* In, u32* ParsedSize )
{
  u8 Char = 0;
  do
  {
    *In >> Char;
    *ParsedSize += 1;
    LOAD_CODE( Struct, Char, u8 );
    
  } while ( Char != 0 );
}

// FIXME: We don't actually deal with unicode strings because we should be using
// UTF-8 instead, should we convert the characters later?
static inline void LoadScriptUnicodeString( UStruct* Struct, FPackageFileIn* In, u32* ParsedSize )
{
  u16 Wchar = 0;
  do
  {
    *In >> Wchar;
    *ParsedSize += 2;
    LOAD_CODE( Struct, Wchar, u16 );

  } while ( Wchar != 0 ); 
}

static inline void LoadScriptLabelTable( UStruct* Struct, FPackageFileIn* In, u32* ParsedSize )
{
  FScriptLabel Label;
  Label.Index = 0;
  Label.Offset = 0;

  while ( 1 )
  {
    *In >> CINDEX( Label.Index );
    *In >> Label.Offset;
    *ParsedSize += 8;

    LOAD_CODE( Struct, Label.Index, idx );
    LOAD_CODE( Struct, Label.Offset, u32 );

    Struct->LabelTable->PushBack( Label );

    if ( strncmp( "None", Struct->Pkg->ResolveNameFromIdx( Label.Index ), 4 ) == 0 )
      break;
  }
}

// Helper function to load script code
// Does not care about type resolution whatsoever
static inline void LoadScriptCode( UStruct* Struct, FPackageFileIn* In )
{
  u8 Token = -1;
  u32 ParsedSize = 0;
  bool bQueueReadIteratorWord = false;
  bool bReadIteratorWord = false;
  while ( ParsedSize < Struct->ScriptSize )
  {
    *In >> Token;
    ParsedSize += 1;
    LOAD_CODE( Struct, Token, u8 );

    // Handle extended native case up here since switch statements
    // can't really handle this type of comparison
    if ( UNLIKELY( (Token && 0xF0) == EX_ExtendedNative ) )
    {
      LoadScriptByte( Struct, In, &ParsedSize );
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
        LoadScriptIndex( Struct, In, &ParsedSize );
        break;
      case EX_IntConstByte:
      case EX_Switch:
        LoadScriptByte( Struct, In, &ParsedSize );
        break;
      case EX_Jump:
      case EX_JumpIfNot:
      case EX_Assert:
      case EX_Case:
      case EX_Skip:
        LoadScriptWord( Struct, In, &ParsedSize );
        break;
      case EX_LabelTable:
        LoadScriptLabelTable( Struct, In, &ParsedSize );
        break;
      case EX_ClassContext:
        LoadScriptIndex( Struct, In, &ParsedSize );
        LoadScriptWord( Struct, In, &ParsedSize );
        LoadScriptByte( Struct, In, &ParsedSize );
        LoadScriptIndex( Struct, In, &ParsedSize );
        break;
      case EX_IntConst:
        LoadScriptDword( Struct, In, &ParsedSize );
        break;
      case EX_FloatConst:
        LoadScriptFloat( Struct, In, &ParsedSize );
        break;
      case EX_StringConst:
        LoadScriptString( Struct, In, &ParsedSize );
        break;
      case EX_RotationConst:
        LoadScriptDword( Struct, In, &ParsedSize );
        LoadScriptDword( Struct, In, &ParsedSize );
        LoadScriptDword( Struct, In, &ParsedSize );
        break;
      case EX_VectorConst:
        LoadScriptFloat( Struct, In, &ParsedSize );
        LoadScriptFloat( Struct, In, &ParsedSize );
        LoadScriptFloat( Struct, In, &ParsedSize );
        break;
      case EX_Iterator:
        // stupid edge cases... see below
        bReadIteratorWord = true;
        break;
      case EX_UnicodeStringConst:
        LoadScriptUnicodeString( Struct, In, &ParsedSize );
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
    if ( bReadIteratorWord )
    {
      LoadScriptWord( Struct, In, &ParsedSize );
      bReadIteratorWord = false;
    }
  }
}

void UStruct::LoadFromPackage( FPackageFileIn* In )
{
  Super::LoadFromPackage( In );
  
  idx ScriptTextIdx = MAX_UINT32;
  idx ChildIdx = MAX_UINT32;
  idx FriendlyNameIdx = MAX_UINT32;
  
  *In >> CINDEX( ScriptTextIdx );
  *In >> CINDEX( ChildIdx );
  *In >> CINDEX( FriendlyNameIdx );
  
  ScriptText = (UTextBuffer*)UPackage::StaticLoadObject( Pkg, ScriptTextIdx, 
    UTextBuffer::StaticClass(), this );
  Children = (UField*)UPackage::StaticLoadObject( Pkg, ChildIdx, NULL, this );
  FriendlyName = Pkg->ResolveNameFromIdx( FriendlyNameIdx );
  *In >> Line;
  *In >> TextPos;
  *In >> ScriptSize;
  
  if ( ScriptSize > 0 )
  {
    ScriptCode = new u8[ ScriptSize ];
    ::LoadScriptCode( this, In );
  }

  // Calculate struct size
  if ( UNLIKELY( Flags & RF_Native ) )
  {
    StructSize = NativeSize;
  }
  else
  {
    if ( SuperField != NULL && SuperField->IsA( UStruct::StaticClass() ) )
      StructSize = ((UStruct*)SuperField)->StructSize;

    for ( UField* ChildIter = Children; ChildIter != NULL; ChildIter = ChildIter->Next )
    {
      if ( ChildIter->IsA( UProperty::StaticClass() ) )
      {
        UProperty* Prop = (UProperty*)ChildIter;
        if ( !(Prop->PropertyFlags & CPF_Native) )
          Prop->Offset = StructSize;

        StructSize += Prop->ElementSize;
      }
    }
  }
}

UFunction::UFunction()
  : UStruct()
{
  ParmsSize = 0;
  iNative = 0;
  NumParms = 0;
  OperatorPrecedence = 0;
  ReturnValueOffset = 0;
  FunctionFlags = 0;
  ReplicationOffset = 0;
}

UFunction::~UFunction()
{
}

void UFunction::LoadFromPackage( FPackageFileIn* In )
{
  Super::LoadFromPackage( In );

  if ( In->Ver <= PKG_VER_UN_220 )
    *In >> CINDEX( ParmsSize );

  *In >> iNative;

  if ( In->Ver <= PKG_VER_UN_220 )
    *In >> CINDEX( NumParms );

  *In >> OperatorPrecedence;

  if ( In->Ver <= PKG_VER_UN_220 )
    *In >> CINDEX( ReturnValueOffset );

  *In >> FunctionFlags;
  if ( FunctionFlags & FUNC_Net )
    *In >> ReplicationOffset;
}

// UState
UState::UState()
  : UStruct()
{
}

UState::~UState()
{
}

void UState::LoadFromPackage( FPackageFileIn* In )
{
  Super::LoadFromPackage( In );
  
  *In >> ProbeMask;
  *In >> IgnoreMask;
  *In >> LabelTableOffset;
  *In >> StateFlags;
}

// FDependency
FDependency::FDependency()
{
  Class = NULL;
  Deep = 0;
  ScriptTextCRC = 0;
}

// UClass
UClass::UClass()
  : UState()
{
  Constructor = NULL;
}

UClass::UClass( const char* ClassName, u32 Flags, UClass* InSuperClass, 
    UObject *(*NativeCtor)(size_t) )
  : UState()
{
  Name = StringDup( ClassName );
  Hash = FnvHashString( Name );
  ClassFlags = Flags;
  SuperClass = InSuperClass;
  SuperField = InSuperClass;
  Constructor = NativeCtor;
  Default = NULL;
  NativeNeedsPkgLoad = true;
}

UClass::~UClass()
{
  SuperClass->DelRef();
  Default->DelRef();
}

bool UClass::ExportToFile( const char* Dir, const char* Type )
{
  String* Filename = new String( Dir );
#if defined LIBUNR_WIN32
  Filename->ReplaceChars( '\\', '/' );
#endif
  if ( Filename->Back() != '/' )
    Filename->Append( "/" );

  Filename->Append( Pkg->ResolveNameFromIdx( NameIdx ) );
  Filename->Append( ".uc" ); // Scripts won't get exported to any other type
 
  FileStreamOut* Out = new FileStreamOut();
  if ( Out->Open( *Filename ) != 0 )
  {
    Logf( LOG_WARN, "Failed to export script to file '%s'", Filename->Data() );
    return false;
  }
  Out->Write( ScriptText->Text->Data(), ScriptText->Text->Size() );
  Out->Close();
  delete Out;
  delete Filename;
  return true;
}

void UClass::LoadFromPackage( FPackageFileIn* In )
{
  Super::LoadFromPackage( In );

  *In >> ClassFlags;
  In->Read( ClassGuid, sizeof( ClassGuid ) );
  
  idx DepCount = 0;
  *In >> CINDEX( DepCount );
  Dependencies.Reserve( DepCount );
  
  FDependency Dep;
  for ( int i = 0; i < DepCount; i++ )
  {
    idx DepObjRef;
    *In >> CINDEX( DepObjRef );
    
    if ( stricmp( Pkg->ResolveNameFromObjRef( DepObjRef ), FriendlyName ) )
      Dep.Class = (UClass*)UPackage::StaticLoadObject( Pkg, DepObjRef, UClass::StaticClass() );
    
    *In >> Dep.Deep;
    *In >> Dep.ScriptTextCRC;
    
    Dependencies.PushBack( Dep );
  }
  
  // I don't actually know what these are for
  idx NumPkgImports = 0;
  *In >> CINDEX( NumPkgImports );
  PackageImports.Reserve( NumPkgImports );
  
  for ( int i = 0; i < NumPkgImports; i++ )
  {
    idx PkgImport = 0;
    *In >> CINDEX( PkgImport );
    PackageImports.PushBack( PkgImport );
  }
  
  if ( In->Ver >= PKG_VER_UN_220 - 1 ) // package version 62 ??
  {
    idx ClassWithinIdx = 0;
    idx ClassConfigNameIdx = 0;
    
    *In >> CINDEX( ClassWithinIdx );
    *In >> CINDEX( ClassConfigNameIdx );
    
    ClassWithin = (UClass*)UPackage::StaticLoadObject( Pkg, ClassWithinIdx, 
      UClass::StaticClass() );
    ClassConfigName = Pkg->ResolveNameFromIdx( ClassConfigNameIdx );

    FHash CfgHash = FnvHashString( ClassConfigName );
    if ( CfgHash == FnvHashString( "System" ) )
      ClassConfig = GGameConfig;
    else
      ClassConfig = GConfigManager->GetConfig( ClassConfigName );
  }
  else
    ClassConfig = GGameConfig;
  
  SuperClass = SafeCast<UClass>( SuperField );
  if ( SuperClass != NULL )
  {
    SuperClass->AddRef();
  
    // Get last child
    UField* Iter;
    if ( Children != NULL )
      for ( Iter = Children; Iter->Next != NULL; Iter = Iter->Next );

    // Link super class children to ours
    Iter->Next = SuperClass->Children;
    SuperClass->Children->AddRef();
  }

  // Construct default object
  if ( Constructor == NULL )
    Constructor = SuperClass->Constructor;

  Default = CreateObject();
  Default->Pkg   = Pkg;
  Default->Class = this;
  Default->Field = Children;

  AddRef();
  Children->AddRef();

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

