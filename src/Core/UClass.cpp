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
 * UClass.cpp - Objects that describe classes of Unreal objects
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Core/UPackage.h"
#include "Core/UProperty.h"
#include "Core/UScript.h"
#include "Core/USystem.h"
#include "Core/UObjGetSet.h"

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

void UTextBuffer::Load()
{
  Super::Load();
  
  *PkgFile >> Pos;
  *PkgFile >> Top;

  idx TextSize;
  *PkgFile >> CINDEX( TextSize );
  if ( TextSize > 0 )
  {
    char* TextBuf = (char*)xstl::Malloc( TextSize + 1 );
    PkgFile->Read( TextBuf, TextSize + 1 );
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

void UField::Load()
{
  Super::Load();
  
  idx SuperIdx = MAX_UINT32;
  idx NextIdx = MAX_UINT32;
  
  *PkgFile >> CINDEX( SuperIdx );
  *PkgFile >> CINDEX( NextIdx );

  // Never lazily load superfields
  if ( SuperIdx )
    SuperField = (UField*)LoadObject( SuperIdx, NULL, Outer, true );
  
  if ( NextIdx )
    Next = (UField*)LoadObject( NextIdx, NULL, Outer );
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

void UConst::Load()
{
  Super::Load();

  idx Size;
  *PkgFile >> CINDEX( Size );

  // Size includes null terminator
  char* Str = (char*)xstl::Malloc( Size ); 
  PkgFile->Read( Str, Size );

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

void UEnum::Load()
{
  Super::Load();

  idx ArraySize;
  *PkgFile >> CINDEX( ArraySize );
  Names.Reserve( ArraySize );

  for( int i = 0; i < ArraySize; i++ )
  {
    idx ElementIdx;
    *PkgFile >> CINDEX( ElementIdx );
    Names.PushBack( Pkg->ResolveNameFromIdx( ElementIdx ) );
  }
}

static u8 LoadScriptToken( UStruct* Struct, FPackageFileIn* PkgFile, u32* ParsedSize );

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
  StructSize = 0;
  LabelTable = new Array<FScriptLabel>();
  bFinalizedLoad = false;
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
  StructSize = 0;
  LabelTable = new Array<FScriptLabel>();
  bFinalizedLoad = false;
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

static inline void LoadScriptByte( UStruct* Struct, FPackageFileIn* PkgFile, u32* ParsedSize )
{
  u8 Byte;
  *PkgFile >> Byte;
  *ParsedSize += 1;
  LOAD_CODE( Struct, Byte, u8 );
}

static inline void LoadScriptWord( UStruct* Struct, FPackageFileIn* PkgFile, u32* ParsedSize )
{
  u16 Word;
  *PkgFile >> Word;
  *ParsedSize += 2;
  LOAD_CODE( Struct, Word, u16 );
}

static inline void LoadScriptDword( UStruct* Struct, FPackageFileIn* PkgFile, u32* ParsedSize )
{
  u32 Dword;
  *PkgFile >> Dword;
  *ParsedSize += 4;
  LOAD_CODE( Struct, Dword, u32 );
}

static inline void LoadScriptIndex( UStruct* Struct, FPackageFileIn* PkgFile, u32* ParsedSize )
{
  idx Index;
  *PkgFile >> CINDEX( Index );
  *ParsedSize += 4;
  LOAD_CODE( Struct, Index, idx );
}

static inline void LoadScriptFloat( UStruct* Struct, FPackageFileIn* PkgFile, u32* ParsedSize )
{
  float Float;
  *PkgFile >> Float;
  *ParsedSize += 4;
  LOAD_CODE( Struct, Float, float );
}

static inline void LoadScriptString( UStruct* Struct, FPackageFileIn* PkgFile, u32* ParsedSize )
{
  u8 Char = 0;
  do
  {
    *PkgFile >> Char;
    *ParsedSize += 1;
    LOAD_CODE( Struct, Char, u8 );
    
  } while ( Char != 0 );
}

// FIXME: We don't actually deal with unicode strings because we should be using
// UTF-8 instead, should we convert the characters later?
static inline void LoadScriptUnicodeString( UStruct* Struct, FPackageFileIn* PkgFile, u32* ParsedSize )
{
  u16 Wchar = 0;
  do
  {
    *PkgFile >> Wchar;
    *ParsedSize += 2;
    LOAD_CODE( Struct, Wchar, u16 );

  } while ( Wchar != 0 ); 
}

static inline void LoadScriptLabelTable( UStruct* Struct, FPackageFileIn* PkgFile, u32* ParsedSize )
{
  FScriptLabel Label;
  Label.Index = 0;
  Label.Offset = 0;

  while ( 1 )
  {
    *PkgFile >> CINDEX( Label.Index );
    *PkgFile >> Label.Offset;
    *ParsedSize += 8;

    LOAD_CODE( Struct, Label.Index, idx );
    LOAD_CODE( Struct, Label.Offset, u32 );

    Struct->LabelTable->PushBack( Label );

    if ( strncmp( "None", Struct->Pkg->ResolveNameFromIdx( Label.Index ), 4 ) == 0 )
      break;
  }
}

static void LoadScriptFunctionParms( UStruct* Struct, FPackageFileIn* PkgFile, u32* ParsedSize )
{
  u8 Token;

  do
  {
    Token = LoadScriptToken( Struct, PkgFile, ParsedSize );
  } while( Token != EX_EndFunctionParms );
}

static u8 LoadScriptToken( UStruct* Struct, FPackageFileIn* PkgFile, u32* ParsedSize )
{
  u8 Token = -1;

  *PkgFile >> Token;
  *ParsedSize += 1;
  LOAD_CODE( Struct, Token, u8 );

  if ( (Token & 0xF0) == EX_ExtendedNative )
  {
    LoadScriptByte( Struct, PkgFile, ParsedSize );
    *ParsedSize += 1;

    LoadScriptFunctionParms( Struct, PkgFile, ParsedSize );
  }

  else if ( Token >= EX_FirstNative )
    LoadScriptFunctionParms( Struct, PkgFile, ParsedSize );

  else
  {
    switch ( Token )
    {
      case EX_LocalVariable:
      case EX_InstanceVariable:
      case EX_DefaultVariable:
        LoadScriptIndex( Struct, PkgFile, ParsedSize );
        break;
      case EX_Return:
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        break;
      case EX_Switch:
        LoadScriptByte( Struct, PkgFile, ParsedSize );
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        break;
      case EX_Jump:
        LoadScriptWord( Struct, PkgFile, ParsedSize );
        break;
      case EX_JumpIfNot:
        LoadScriptWord( Struct, PkgFile, ParsedSize );
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        break;
      case EX_Stop:
        break;
      case EX_Assert:
        LoadScriptWord( Struct, PkgFile, ParsedSize );
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        break;
      case EX_Case:
        // There is supposed to be a conditional load token after, but
        // it's much easier to let it fall through to the main LoadScriptCode loop
        LoadScriptWord( Struct, PkgFile, ParsedSize );
        break;
      case EX_Nothing:
        break;
      case EX_LabelTable:
        LoadScriptLabelTable( Struct, PkgFile, ParsedSize );
        break;
      case EX_GotoLabel:
      case EX_EatString:
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        break;
      case EX_Let:
      case EX_DynArrayElement:
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        break;
      case EX_New:
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        break;
      case EX_ClassContext:
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        LoadScriptWord( Struct, PkgFile, ParsedSize );
        LoadScriptByte( Struct, PkgFile, ParsedSize );
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        break;
      case EX_MetaCast:
        LoadScriptIndex( Struct, PkgFile, ParsedSize );
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        break;
      case EX_LetBool:
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        break;
      case EX_EndFunctionParms:
      case EX_Self:
        break;
      case EX_Skip:
        LoadScriptWord( Struct, PkgFile, ParsedSize );
        break;
      case EX_Context:
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        LoadScriptWord( Struct, PkgFile, ParsedSize );
        LoadScriptByte( Struct, PkgFile, ParsedSize );
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        break;
      case EX_ArrayElement:
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        break;
      case EX_VirtualFunction:
      case EX_FinalFunction:
        LoadScriptIndex( Struct, PkgFile, ParsedSize );
        LoadScriptFunctionParms( Struct, PkgFile, ParsedSize );
        break;
      case EX_IntConst:
        LoadScriptDword( Struct, PkgFile, ParsedSize );
        break;
      case EX_FloatConst:
        LoadScriptFloat( Struct, PkgFile, ParsedSize );
        break;
      case EX_StringConst:
        LoadScriptString( Struct, PkgFile, ParsedSize );
        break;
      case EX_ObjectConst:
      case EX_NameConst:
        LoadScriptIndex( Struct, PkgFile, ParsedSize );
        break;
      case EX_RotationConst:
        LoadScriptDword( Struct, PkgFile, ParsedSize );
        LoadScriptDword( Struct, PkgFile, ParsedSize );
        LoadScriptDword( Struct, PkgFile, ParsedSize );
        break;
      case EX_VectorConst:
        LoadScriptFloat( Struct, PkgFile, ParsedSize );
        LoadScriptFloat( Struct, PkgFile, ParsedSize );
        LoadScriptFloat( Struct, PkgFile, ParsedSize );
        break;
      case EX_ByteConst:
        LoadScriptByte( Struct, PkgFile, ParsedSize );
        break;
      case EX_IntZero:
      case EX_IntOne:
      case EX_True:
      case EX_False:
        break;
      case EX_NativeParm:
        LoadScriptIndex( Struct, PkgFile, ParsedSize );
        break;
      case EX_NoObject:
        break;
      case EX_IntConstByte:
        LoadScriptByte( Struct, PkgFile, ParsedSize );
        break;
      case EX_BoolVariable:
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        break;
      case EX_DynamicCast:
        LoadScriptIndex( Struct, PkgFile, ParsedSize );
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        break;
      case EX_Iterator:
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        LoadScriptWord( Struct, PkgFile, ParsedSize );
        break;
      case EX_IteratorPop:
      case EX_IteratorNext:
        break;
      case EX_StructCmpEq:
      case EX_StructCmpNe:
        LoadScriptIndex( Struct, PkgFile, ParsedSize );
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        break;
      case EX_UnicodeStringConst:
        LoadScriptUnicodeString( Struct, PkgFile, ParsedSize );
        break;
      case EX_StructMember:
        LoadScriptIndex( Struct, PkgFile, ParsedSize );
        LoadScriptToken( Struct, PkgFile, ParsedSize );
        break;
      case EX_GlobalFunction:
        LoadScriptIndex( Struct, PkgFile, ParsedSize );
        LoadScriptFunctionParms( Struct, PkgFile, ParsedSize );
        break;
      case EX_RotatorToVector:
      case EX_ByteToInt:
      case EX_ByteToBool:
      case EX_ByteToFloat:
      case EX_IntToByte:
      case EX_IntToBool:
      case EX_IntToFloat:
      case EX_BoolToByte:
      case EX_BoolToInt:
      case EX_BoolToFloat:
      case EX_FloatToByte:
      case EX_FloatToInt:
      case EX_FloatToBool:
      case EX_StringToName:
      case EX_ObjectToBool:
      case EX_NameToBool:
      case EX_StringToByte:
      case EX_StringToInt:
      case EX_StringToBool:
      case EX_StringToFloat:
      case EX_StringToVector:
      case EX_StringToRotator:
      case EX_VectorToBool:
      case EX_VectorToRotator:
      case EX_RotatorToBool:
      case EX_ByteToString:
      case EX_IntToString:
      case EX_BoolToString:
      case EX_FloatToString:
      case EX_ObjectToString:
      case EX_NameToString:
      case EX_VectorToString:
      case EX_RotatorToString:
        LoadScriptToken( Struct, PkgFile, ParsedSize );
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
        Logf( LOG_WARN, "Unknown opcode located at offset 0x%x", PkgFile->Tell() );
        break;
      // Everything else loads another token or nothing at all,
      // so we don't need explicit cases for them
      default:
        break;
    }
  }
  // This will go mostly unused, but we need this for LoadScriptFunctionParms()
  return Token;
}

// Helper function to load script code
// Does not care about type resolution whatsoever
static inline void LoadScriptCode( UStruct* Struct, FPackageFileIn* PkgFile )
{
  u8 Token = -1;
  u32 ParsedSize = 0;
  bool bQueueReadIteratorWord = false;
  bool bReadIteratorWord = false;
  while ( ParsedSize < Struct->ScriptSize )
  {
    LoadScriptToken( Struct, PkgFile, &ParsedSize );
  }
}

void UStruct::Load()
{
  Super::Load();
 
  // With circular dependencies, it's possible that we accidentally loaded
  // this struct already
  if ( UNLIKELY( !bLoading ) )
    return;

  if ( !(ObjectFlags & RF_Native) || StructSize == 0 )
  {
    if ( SuperField != NULL && SuperField->IsA( UStruct::StaticClass() ) )
      StructSize = ((UStruct*)SuperField)->StructSize;
  }

  idx ScriptTextIdx = MAX_UINT32;
  idx ChildIdx = MAX_UINT32;
  idx FriendlyNameIdx = MAX_UINT32;
  
  *PkgFile >> CINDEX( ScriptTextIdx );
  *PkgFile >> CINDEX( ChildIdx );
  *PkgFile >> CINDEX( FriendlyNameIdx );
 
  if ( ScriptTextIdx )
    ScriptText = (UTextBuffer*)LoadObject( ScriptTextIdx, UTextBuffer::StaticClass(), this );

  if ( ChildIdx )
    Children = (UField*)LoadObject( ChildIdx, NULL, this );

  FriendlyName = Pkg->ResolveNameFromIdx( FriendlyNameIdx );
  *PkgFile >> Line;
  *PkgFile >> TextPos;
  *PkgFile >> ScriptSize;
  
  if ( ScriptSize > 0 )
  {
    ScriptCode = new u8[ ScriptSize ];
    ::LoadScriptCode( this, PkgFile );
  }

  // Calculate struct size
  if ( !(ObjectFlags & RF_Native) || StructSize == 0 )
  {
    for ( UField* ChildIter = Children; ChildIter != NULL; ChildIter = ChildIter->Next )
    {
      if ( ChildIter->IsA( UProperty::StaticClass() ) )
      {
        UProperty* Prop = (UProperty*)ChildIter;
        if ( !(Prop->PropertyFlags & CPF_Native) )
        {
          // UPak.SpaceMarine has this cool thing where the struct MarineSound
          // contains a variable of type SpaceMarine, the class actually being loaded.
          // Because of that, the struct doesn't finish loading until it gets to this
          // point, but the struct property VoiceList will have an element size of 0.
          // As a result, SaluteTarget and VoiceList end up having the same Offset value.
          // Needless to say, this causes problems when accessing both variables.
          if ( Prop->Class == UStructProperty::StaticClass() )
            Prop->ElementSize = ((UStructProperty*)Prop)->Struct->StructSize;

          Prop->Offset = StructSize;
        }
        StructSize += (Prop->ElementSize * Prop->ArrayDim);
      }
    }
  }
}

void UStruct::FinalizeClassLoad()
{
  if ( bFinalizedLoad )
    return;

  UField* Iter;
  for ( Iter = Children; Iter != NULL; Iter = Iter->Next )
  {
    if ( Class == UClass::StaticClass() && Iter->Outer != this )
    {
      // Don't finalize loads on parent class variables
      break;
    }
    else if ( Iter->Export->bNeedsFullLoad && !Iter->bLoading )
    {
      if ( Iter->IsA( UClass::StaticClass() ) )
      {
        // Only do full loads on classes
        Iter->PreLoad();
        Iter->Load();
        Iter->PostLoad();
      }
    }
    else if ( Iter->Class == UObjectProperty::StaticClass() )
    {
      UObjectProperty* ObjPropIter = (UObjectProperty*)Iter;
      if ( ObjPropIter->ObjectType->Export != NULL &&
           ObjPropIter->ObjectType->Export->bNeedsFullLoad &&
           !ObjPropIter->ObjectType->bLoading )
      {
        ObjPropIter->ObjectType->PreLoad();
        ObjPropIter->ObjectType->Load();
        ObjPropIter->ObjectType->PostLoad();
      }
    }
    else if ( Iter->Class == UClassProperty::StaticClass() )
    {
      UClassProperty* ClsPropIter = (UClassProperty*)Iter;
      if ( ClsPropIter->ClassObj->Export != NULL &&
           ClsPropIter->ClassObj->Export->bNeedsFullLoad && 
           !ClsPropIter->ClassObj->bLoading )
      {
        ClsPropIter->ClassObj->PreLoad();
        ClsPropIter->ClassObj->Load();
        ClsPropIter->ClassObj->PostLoad();
      }
    }
    else if ( Iter->IsA( UStruct::StaticClass() ) )
    {
      // Make sure any children that have their own child classes get loaded
      UStruct* StructIter = (UStruct*)Iter;
      StructIter->FinalizeClassLoad();
    }
  }

  bFinalizedLoad = true;
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

void UFunction::Load()
{
  Super::Load();

  if ( PkgFile->Ver <= PKG_VER_UN_220 )
    *PkgFile >> CINDEX( ParmsSize );

  *PkgFile >> iNative;

  if ( PkgFile->Ver <= PKG_VER_UN_220 )
    *PkgFile >> CINDEX( NumParms );

  *PkgFile >> OperatorPrecedence;

  if ( PkgFile->Ver <= PKG_VER_UN_220 )
    *PkgFile >> CINDEX( ReturnValueOffset );

  *PkgFile >> FunctionFlags;

  if ( FunctionFlags & FUNC_Native )
    UObject::NativeFunctions->PushBack( this );

  if ( FunctionFlags & FUNC_Net )
    *PkgFile >> ReplicationOffset;
}

// UState
UState::UState()
  : UStruct()
{
}

UState::~UState()
{
}

void UState::Load()
{
  Super::Load();

  if ( !bLoading )
    return;

  *PkgFile >> ProbeMask;
  *PkgFile >> IgnoreMask;
  *PkgFile >> LabelTableOffset;
  *PkgFile >> StateFlags;
}

// FDependency
FDependency::FDependency()
{
  Class = NULL;
  Deep = 0;
  ScriptTextCRC = 0;
}

// UClass
void UClass::BootstrapStage1()
{
  ObjectClass = new UClass( "Class", CLASS_NoExport, NULL, 
      sizeof(UClass), UClass::NativeConstructor );
  ObjectClass->Class = ObjectClass;
  UObject::ClassPool->PushBack( ObjectClass );
}

void UClass::BootstrapStage2()
{
  ObjectClass->SuperClass = UState::StaticClass();
}

UClass::UClass()
  : UState()
{
  Constructor = NULL;
  StructSize = 0;
  bLinkedChildren = false;
}

UClass::UClass( const char* ClassName, u32 Flags, UClass* InSuperClass, 
    size_t InStructSize, UObject *(*NativeCtor)(size_t) )
  : UState()
{
  Name = StringDup( ClassName );
  Hash = FnvHashString( Name );
  ClassFlags = Flags;
  SuperClass = InSuperClass;
  SuperField = InSuperClass;
  Constructor = NativeCtor;
  NativeNeedsPkgLoad = true;
  StructSize = InStructSize;
  bLinkedChildren = false;

  Default = CreateDefaultObject();
  if ( UNLIKELY( bStaticBootstrapped && (Flags & CLASS_NoExport) ) )
  {
    LinkSuperClassChildren();
    Default->Field = Children;
  }
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

  Filename->Append( Pkg->ResolveNameFromIdx( Export->ObjectName ) );
  Filename->Append( ".uc" ); // Scripts won't get exported to any other type
 
  // Write script text
  FileStreamOut* Out = new FileStreamOut();
  if ( Out->Open( *Filename ) != 0 )
  {
    Logf( LOG_WARN, "Failed to export script to file '%s'", Filename->Data() );
    return false;
  }
  Out->Write( ScriptText->Text->Data(), ScriptText->Text->Size() );

  // Write default properties
  String ValueBuf;
  String DefProp;
  Out->Write( (char*)"defaultproperties\r\n{\r\n", 22 );
  for ( UField* Iter = Default->Field; Iter != NULL; Iter = Iter->Next )
  {
    if ( Iter->IsA( UProperty::StaticClass() ) )
    {
      UProperty* PropIter = (UProperty*)Iter;
      if ( UNLIKELY( PropIter->Offset & 0x80000000 ) )
      {
        Logf( LOG_WARN, "Bad offset for property '%s' (Offset = %x)",
              PropIter->Name, PropIter->Offset );
        continue;
      }

      for ( int i = 0; i < PropIter->ArrayDim; i++ )
      {
        if ( LIKELY( !(PropIter->PropertyFlags & CPF_Native) ) )
        {
          // Get default property from super class
          size_t DefValue = 0;
          if ( SuperClass != NULL && PropIter->Outer != this )
            DefValue = PropIter->GetGenericValue( SuperClass->Default, i );

          PropIter->GetText( ValueBuf, Default, i, DefValue, Pkg );

          if ( ValueBuf.Length() > 0 )
          {
            if ( PropIter->Class == UArrayProperty::StaticClass() )
            {
              Out->Write( ValueBuf.Data(), ValueBuf.Length() );
              ValueBuf.Erase();
              continue;
            }

            DefProp += '\t';
            DefProp += PropIter->Name;
            if ( PropIter->ArrayDim > 1 )
            {
              DefProp += '[';
              DefProp += String( i );
              DefProp += ']';
            }
            DefProp += '=';
            DefProp += ValueBuf;
            DefProp += "\r\n";

            Out->Write( DefProp.Data(), DefProp.Length() );
            DefProp.Erase();
            ValueBuf.Erase();
          }
        }
      }
    }
  }
  Out->Write( (char*)"}\r\n", 3 );

  Out->Close();
  delete Out;
  delete Filename;
  return true;
}

void UClass::Load()
{
  Export->bNeedsFullLoad = true;
  Super::Load();

  if ( !bLoading )
    return;

  DefPropQueue.Push( this );

  *PkgFile >> ClassFlags;
  PkgFile->Read( ClassGuid, sizeof( ClassGuid ) );
  
  idx DepCount = 0;
  *PkgFile >> CINDEX( DepCount );
  Dependencies.Reserve( DepCount );
  
  FDependency Dep;
  for ( int i = 0; i < DepCount; i++ )
  {
    idx DepObjRef;
    *PkgFile >> CINDEX( DepObjRef );
    /*
    if ( stricmp( Pkg->ResolveNameFromObjRef( DepObjRef ), FriendlyName ) )
      Dep.Class = (UClass*)UPackage::LoadObject( Pkg, DepObjRef, UClass::StaticClass() );
    */
    *PkgFile >> Dep.Deep;
    *PkgFile >> Dep.ScriptTextCRC;
    
    //Dependencies.PushBack( Dep );
  }
  
  // I don't actually know what these are for
  idx NumPkgImports = 0;
  *PkgFile >> CINDEX( NumPkgImports );
  PackageImports.Reserve( NumPkgImports );
  
  for ( int i = 0; i < NumPkgImports; i++ )
  {
    idx PkgImport = 0;
    *PkgFile >> CINDEX( PkgImport );
    PackageImports.PushBack( PkgImport );
  }
  
  if ( PkgFile->Ver >= PKG_VER_UN_220 - 1 ) // package version 62 ??
  {
    idx ClassWithinIdx = 0;
    idx ClassConfigNameIdx = 0;
    
    *PkgFile >> CINDEX( ClassWithinIdx );
    *PkgFile >> CINDEX( ClassConfigNameIdx );
   
    ClassWithin = (UClass*)LoadObject( ClassWithinIdx, UClass::StaticClass(), NULL );
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
  NativeNeedsPkgLoad = false;
}

void UClass::PostLoad()
{
  if ( bLoading )
  {
    // Set this before finalizing class load
    Export->bNeedsFullLoad = false;

    if ( SuperClass != NULL ) 
    {
      if ( Constructor == NULL )
        Constructor = SuperClass->Constructor;

      if ( SuperClass->bLinkedChildren )
        LinkSuperClassChildren();
    }
    // Fully load all class dependencies
    FinalizeClassLoad();

    if ( !bLinkedChildren )
      LinkSuperClassChildren();

    // Setup default object
    if ( Default == NULL )
      Default = CreateDefaultObject(); 

    Default->Pkg   = Pkg;
    Default->PkgFile = PkgFile;
    Default->Field = Children;

    if ( LIKELY( Children ) )
      Children->AddRef();

    // In Unreal, property lists seem to take precedent over config properties
    // While we should enforce that a config property is never in the property
    // list when saving packages, we will read the config property first before
    // overriding it with the property in the list
//    if ( ( ClassFlags & CLASS_Config ) ) 
//      Default->ReadConfigProperties();

    DefPropListOffset = PkgFile->Tell();

    // Default properties must be read in the proper class loading order
    if ( DefPropQueue.Front() == this )
    {
      UClass* QueuedClass;
      while ( DefPropQueue.Size() > 0 )
      {
        QueuedClass = DefPropQueue.Front();
        int OldOffset = QueuedClass->Default->PkgFile->Tell();
        QueuedClass->Default->PkgFile->Seek( QueuedClass->DefPropListOffset, Begin );

        // Copy property values from SuperClass default object
        if ( LIKELY( SuperClass != NULL ) )
          xstl::Copy( PtrAdd( Default, UObject::StaticClass()->StructSize ),
                      SuperClass->StructSize - UObject::StaticClass()->StructSize,
                      PtrAdd( SuperClass->Default, UObject::StaticClass()->StructSize ),
                      SuperClass->StructSize - UObject::StaticClass()->StructSize );

        QueuedClass->Default->ReadDefaultProperties();
        QueuedClass->Default->PkgFile = NULL;
        DefPropQueue.Pop();
      }
    }
  }
  Super::PostLoad();
}

bool UClass::IsNative()
{
  // NOTE: this is NOT checking class flags
  return (ObjectFlags & RF_Native) != 0;
}

UObject* UClass::CreateObject()
{
  if ( ClassFlags & CLASS_NoExport )
    return Constructor( StructSize );
  else
    return Default->Clone();
}

UObject* UClass::CreateDefaultObject()
{
  Default = Constructor( StructSize );
  Default->Name = StringDup( Name );
  Default->Class = this;
}

char* UClass::CreateDefaultObjectName()
{
  char* DefObjName = new char[1024];
  xstl::Set( DefObjName, 0, 1024 );
  strcat( DefObjName, "Default" );
  strcat( DefObjName, Name );
  return DefObjName;
}

void UClass::SetSuperClassProperties()
{
  SuperClass = SafeCast<UClass>( SuperField );
  if ( SuperClass == NULL )
  {
    Logf( LOG_CRIT, "SuperField of class '%s' is not a class!!!", Name );
  }
  else
  {
    if ( SuperClass->SuperClass == NULL )
      SuperClass->SetSuperClassProperties();

    if ( Constructor == NULL )
      Constructor = SuperClass->Constructor;
  }
}

void UClass::LinkSuperClassChildren()
{
  if ( SuperClass != NULL && !bLinkedChildren )
  {
    SuperClass->AddRef();
  
    // If SuperClass does not have its SuperClass set, then set its
    // properties that depend on it
    if ( LIKELY( SuperClass != UObject::StaticClass() ) )
      if ( SuperClass->SuperClass == NULL )
        SuperClass->SetSuperClassProperties();

    // Apparently a child class can call this before it's parent class?
    if ( UNLIKELY( !SuperClass->bLinkedChildren ) )
      SuperClass->LinkSuperClassChildren();

    // Get last child
    UField* Iter = Children;
    if ( Iter != NULL )
      for ( ; Iter->Next != NULL; Iter = Iter->Next );

    // Link super class children to ours
    if ( Iter )
      Iter->Next = SuperClass->Children;
    else
      Children = SuperClass->Children;

    SuperClass->Children->AddRef();

    bLinkedChildren = true;
  }
}

Queue<UClass*> UClass::DefPropQueue;

