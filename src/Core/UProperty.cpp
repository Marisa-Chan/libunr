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
 * UProperty.cpp - Objects that describe properties inside of objects
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UProperty.h"
#include "Core/UPackage.h"
#include "Core/UObjGetSet.h"

u8 UProperty::PropertySizes[8] = { 1, 2, 4, 12, 16, 1, 2, 4 };

// TODO: Create variable in Object to check for uninitialized properties?
UProperty::UProperty()
  : UField()
{
  ArrayDim = 0;
  ElementSize = 0;
  PropertyFlags = 0;
  Category = 0;
  Offset = 0;
}

UProperty::~UProperty()
{
}

void UProperty::Load()
{
  Super::Load();
  
  *PkgFile >> ArrayDim;
  *PkgFile >> ElementSize;
  *PkgFile >> PropertyFlags;
  *PkgFile >> CINDEX( Category );
  
  if ( PropertyFlags & CPF_Net )
    *PkgFile >> ReplicationOffset;
 
  if ( Outer->ObjectFlags & RF_Native )
    Offset = GetNativeOffset( Outer->Name, Name );

  if ( Outer->Class == UClass::StaticClass() && PropertyFlags & CPF_GlobalConfig )
    GlobalClass = (UClass*)Outer;
}

bool UProperty::LoadDefaultProperty( void* ObjMem, FPackageFileIn& In, int RealSize, int Idx )
{
  void* Data = PtrAdd( ObjMem, (Offset + ( MAX(Idx,0) * ElementSize) ) );
  size_t Num = ElementSize;
  if ( Idx < 0 )
    Num *= ArrayDim;

  while ( Num )
  {
    In.Read( Data, ElementSize );
    Data = PtrAdd( Data, ElementSize );
    Num -= ElementSize;
  }

  return true;
}

bool UProperty::LoadDefaultPropertySafe( void* ObjMem, FPackageFileIn& In, u8 Type, int RealSize, int Idx )
{
  bool Ret = false;
  if ( Type != PropertyType )
  {
    Logf( LOG_CRIT, "Default property '%s' expected '%s' but got '%s'", 
        Name.Data(), PropNames[Type], Class->Name.Data() );
    return false;
  }
  else
    Ret = LoadDefaultProperty( ObjMem, In, RealSize, Idx );

  return Ret;
}

void UProperty::GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx )
{
  return;
}

void UProperty::SkipDefaultProperty( FPackageFileIn& In, int RealSize )
{
  In.Seek( RealSize, Cur );
}

u32 UProperty::GetNativeOffset( FName ClassName, FName PropName )
{
  FNativePropertyList* NativePropList = NULL;
  u32 Offset = MAX_UINT32;

  for ( int i = 0; i < NativePropertyLists.Size(); i++ )
  {
    // Can you take the address of an overloaded operator[] ?
    NativePropList = NativePropertyLists.Data()[i];    
    if ( ClassName.Hash() == NativePropList->Hash )
      break;
  }
 
  for ( int i = 0; i < NativePropList->Num; i++ )
  {
    if ( NativePropList->Properties[i].Hash == PropName.Hash() )
    {
      Offset = NativePropList->Properties[i].Offset;
      break;
    }
  }
 
  return Offset;
}

void UByteProperty::Load()
{
  Super::Load();
 
  idx EnumType = 0;
  *PkgFile >> CINDEX( EnumType );
  if ( EnumType )
    Enum = (UEnum*)LoadObject( EnumType, UEnum::StaticClass(), Outer );

  ElementSize = 1;
  PropertyType = PROP_Byte;
}

void UByteProperty::GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx )
{
  u8 DefVal = (Default) ? Default->GetProperty<u8>( this, Idx ) : 0;
  u8 Val = Obj->GetProperty<u8>( this, Idx );
  if ( Val != DefVal )
  {
    if ( Enum && Val < Enum->Names.Size() )
      Buf += Enum->Names[Val];
    else
      Buf += FString( (int)Val );
  }
}

void UIntProperty::Load()
{
  Super::Load();
  ElementSize = 4;
  PropertyType = PROP_Int;
}

void UIntProperty::GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx )
{
  int DefVal = (Default) ? Default->GetProperty<int>( this, Idx ) : 0;
  int Val = Obj->GetProperty<int>( this, Idx );
  if ( Val != DefVal )
    Buf += FString( Val );
}

void UBoolProperty::Load()
{
  Super::Load();
  ElementSize = sizeof( bool );
  PropertyType = PROP_Bool;
}

bool UBoolProperty::LoadDefaultProperty( void* ObjMem, FPackageFileIn& In, int RealSize, int Idx )
{
  // BoolProperties store their value in the 'IsArray' part of the info byte.
  // This value is passed through Idx since BoolProperties cannot stored as an array directly.
  // However, they can be part of a struct which is in a dynamic array, in which case, read a byte
  if ( RealSize < 0 )
    In.Read( PtrAdd( ObjMem, Offset ), 1 );
  else
    *(bool*)(PtrAdd(ObjMem, Offset)) = Idx == 1;

  return true;
}

void UBoolProperty::GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx )
{
  bool DefVal = (Default) ? Default->GetProperty<bool>( this, Idx ) : 0;
  bool Val = Obj->GetProperty<bool>( this, Idx );
  if ( Val != DefVal )
    Buf += ( Val ? "True" : "False" );
}

void UFloatProperty::Load()
{
  Super::Load();
  ElementSize = 4;
  PropertyType = PROP_Float;
}

void UFloatProperty::GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx )
{
  float DefVal = (Default) ? Default->GetProperty<float>( this, Idx ) : 0;
  float Val = Obj->GetProperty<float>( this, Idx );
  if ( !FltEqual( Val, DefVal ) )
    Buf += FString( Val );
}

void UNameProperty::Load()
{
  Super::Load();
  ElementSize = 4;
  PropertyType = PROP_Name;
}

bool UNameProperty::LoadDefaultProperty( void* ObjMem, FPackageFileIn& In, int RealSize, int Idx )
{
  void* Data = PtrAdd( ObjMem, (Offset + ( MAX(Idx,0) * ElementSize) ) );
  size_t Num = ElementSize;
  if ( Idx < 0 )
    Num *= ArrayDim;

  while ( Num )
  {
    idx PkgNameIdx;
    In >> CINDEX( PkgNameIdx );
    *(FName*)Data = In.Pkg->GetGlobalName( PkgNameIdx );

    Data = PtrAdd( Data, ElementSize );
    Num -= ElementSize;
  }

  return true;
}

void UNameProperty::GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx )
{
  FName DefVal = (Default) ? Default->GetProperty<FName>( this, Idx ) : FName( 0 );
  FName Val    = Obj->GetProperty<FName>( this, Idx );

  if ( DefVal != Val )
  {
    Buf += '"';
    Buf += Val.Data();
    Buf += '"';
  }
}

void UStrProperty::Load()
{
  Super::Load();
  ElementSize = sizeof( FString* );
  PropertyType = PROP_Ascii;
}

bool UStrProperty::LoadDefaultProperty( void* ObjMem, FPackageFileIn& In, int RealSize, int Idx )
{
  void* Data = PtrAdd( ObjMem, (Offset + ( MAX(Idx,0) * sizeof( FString* )) ) );
  size_t Num = sizeof( FString* );
  if ( Idx < 0 )
    Num *= ArrayDim;
  
  while ( Num )
  {
    if ( RealSize < 0 )
    {
      FString* NewStr = new FString();
      In >> *NewStr;
      *(FString**)Data = NewStr;
    }
    else if ( RealSize > 0 )
    {
      char* NewStr = new char[RealSize];
      In.Read( NewStr, RealSize-- );

      FString* RealNewStr = new FString( NewStr, RealSize );
      *(FString**)Data = RealNewStr;
    }

    Num -= sizeof( FString* );
    Data = PtrAdd( Data, sizeof( FString* ) );
  }

  return true;
}

bool UStrProperty::LoadDefaultPropertySafe( void* ObjMem, FPackageFileIn& In, u8 Type, int RealSize, int Idx )
{
  switch( Type )
  {
    case PROP_Ascii:
      RealSize = -1;
      break;
    case PROP_FString:
      break;
    default:
      Logf( LOG_CRIT, "Default property expected '%s' but got 'StrProperty'", PropNames[Type] );
      return true;
  }

  return LoadDefaultProperty( ObjMem, In, RealSize, Idx );
}

void UStrProperty::GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx )
{
  FString* DefVal = (Default) ? Default->GetProperty<FString*>( this, Idx ) : NULL;
  FString* Val = Obj->GetProperty<FString*>( this, Idx );
  if ( Val && Val->Length() > 0 && (DefVal == NULL || *Val != *DefVal) )
  {
    Buf += '"';
    Buf += Val->Data();
    Buf += '"';
  }
}

void UStringProperty::Load()
{
  Super::Load();
  ElementSize = sizeof( FString* );
  PropertyType = PROP_FString;
}

bool UStringProperty::LoadDefaultProperty( void* ObjMem, FPackageFileIn& In, int RealSize, int Idx )
{
  void* Data = PtrAdd( ObjMem, (Offset + ( MAX(Idx,0) * sizeof( FString* )) ) );
  size_t Num = sizeof( FString* );
  if ( Idx < 0 )
    Num *= ArrayDim;
  
  while ( Num )
  {
    char* NewStr = new char[RealSize];
    In.Read( NewStr, RealSize-- );

    FString* RealNewStr = new FString( NewStr, RealSize );
    *(FString**)Data = RealNewStr;

    Num -= sizeof( FString* );
    Data = PtrAdd( Data, sizeof( FString* ) );
  }

  return true;
}

void UStringProperty::GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx )
{
  FString* DefVal = (Default) ? Default->GetProperty<FString*>( this, Idx ) : NULL;
  FString* Val = Obj->GetProperty<FString*>( this, Idx );
  if ( Val && Val->Length() > 0 && (DefVal == NULL || *Val != *DefVal) )
  {
    Buf += '"';
    Buf += Val->Data();
    Buf += '"';
  }
}

void UObjectProperty::Load()
{
  Super::Load();
  ElementSize = sizeof( UObject* );

  idx ObjTypeIdx = 0;
  *PkgFile >> CINDEX( ObjTypeIdx );
  ObjectType = (UClass*)LoadObject( ObjTypeIdx, UClass::StaticClass(), NULL );
  PropertyType = PROP_Object;
}

bool UObjectProperty::LoadDefaultProperty( void* ObjMem, FPackageFileIn& In, int RealSize, int Idx )
{
  void* Data = PtrAdd( ObjMem, (Offset + ( MAX(Idx,0) * sizeof( UObject* ) ) ) );
  size_t Num = sizeof( UObject* );
  if ( Idx < 0 )
    Num *= ArrayDim;
  
  idx ObjRef = 0;
  while ( Num )
  {
    In >> CINDEX( ObjRef );
    *(UObject**)Data = StaticLoadObject( In.Pkg, ObjRef, ObjectType, NULL );

    Data = PtrAdd( Data, sizeof( UObject* ) );
    Num -= sizeof( UObject* );
  }

  return true;
}

void UObjectProperty::GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx )
{
  UObject* DefVal = (Default) ? Default->GetProperty<UObject*>( this, Idx ) : NULL;
  UObject* Val = Obj->GetProperty<UObject*>( this, Idx );
  if ( Val != DefVal )
  {
    if ( Val == NULL )
    {
      Buf += "None";
      return;
    }
    
    Buf += Val->Class->Name;
    Buf += '\'';
    
    FString* FullName = Val->Pkg->GetFullObjName( Val->Export );
    Buf += (FullName) ? *FullName : "<NULL>";

    Buf += '\'';
  }
}

void UClassProperty::Load()
{
  Super::Load();
  ElementSize = sizeof( UClass* );

  idx ClassIdx = 0;
  *PkgFile >> CINDEX( ClassIdx );
  ClassObj = (UClass*)LoadObject( ClassIdx, UClass::StaticClass(), NULL );
  PropertyType = PROP_Object;
}

void UClassProperty::GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx )
{
  UClass* DefVal = (Default) ? Default->GetProperty<UClass*>( this, Idx ) : NULL;
  UClass* Val = Obj->GetProperty<UClass*>( this, Idx );
  if ( Val != DefVal )
  {
    if ( Val == NULL )
    {
      Buf += "None";
      return;
    }

    Buf += "Class'";
    Buf += Val->Pkg->Name;
    Buf += '.';
    Buf += Val->Name;
    Buf += '\'';
  }
}

void UStructProperty::Load()
{
  Super::Load();

  idx StructIdx = 0;
  *PkgFile >> CINDEX( StructIdx );
  Struct = (UStruct*)LoadObject( StructIdx, UStruct::StaticClass(), Outer );

  ElementSize = Struct->StructSize;
  PropertyType = PROP_Struct;
}

bool UStructProperty::LoadDefaultProperty( void* ObjMem, FPackageFileIn& In, int RealSize, int Idx )
{
  void* Data = PtrAdd( ObjMem, Offset + (MAX(Idx,0) * Struct->StructSize) );
  size_t Num = Struct->StructSize;
  if ( Idx < 0 )
    Num *= ArrayDim;

  UField* Child = Struct->Children;
  while ( Num )
  {
    if ( !Child )
    {
      Data = PtrAdd( Data, Struct->StructSize );
      Child = Struct->Children;
    }

    if ( Child->IsA( UProperty::StaticClass() ) )
    {
      UProperty* ChildProp = (UProperty*)Child;
      ChildProp->LoadDefaultProperty( Data, In, -1 );

      Num -= ChildProp->ElementSize * ChildProp->ArrayDim;
    }
    else if ( !Child->IsA( UEnum::StaticClass() ) )
    {
      Logf( LOG_CRIT, "Pure struct of type '%s' has a bad child type of '%s'", Struct->Name, Child->Class->Name );
      return false;
    }

    Child = Child->Next;
  }

  return true;
}

bool UStructProperty::LoadDefaultPropertySafe( void* ObjMem, FPackageFileIn& In, u8 Type, int RealSize, int Idx )
{
  // Check if type in default property list matches
  if ( Type != PropertyType )
  {
    Logf( LOG_CRIT, "Default property expected '%s' but got 'StructProperty'", PropNames[Type] );
    return false;
  }

  // Verify struct name
  idx StructName = 0;
  In >> CINDEX( StructName );
  if ( StructName < 0 )
  {
    Logf( LOG_CRIT, "Bad struct name index for StructProperty '%s'", Name );
    return false;
  }

  FHash StructHash = In.Pkg->GetNameEntry( StructName )->Hash;
  if ( Struct->Name.Hash() != StructHash )
  {
    Logf( LOG_CRIT, "Default property expected struct type '%s' but got '%s'", 
      In.Pkg->ResolveNameFromIdx( StructName ), Struct->Name );
    return false;
  }

  // RealSize masquerades as the InfoByte here
  // Yes, variable reuse/repurposing isn't a great practice
  // No, I'm not changing it because I don't want to add another argument for one use case
  u8 SizeByte = (RealSize & 0x70) >> 4;
  u8 IsArray  = (RealSize & 0x80) >> 7;
  int ArrayIdx = 0;

  // Verify struct size
  int StructSize = 0;
  switch( SizeByte )
  {
    case 5:
      In >> *(u8*)&StructSize;
      break;
    case 6:
      In >> *(u16*)&StructSize;
      break;
    case 7:
      In >> StructSize;
      break;
    default:
      StructSize = (SizeByte>0) ? UProperty::PropertySizes[SizeByte] : 0;
      break;
  }

  if ( IsArray )
    ArrayIdx = ReadArrayIndex( In );

  return LoadDefaultProperty( ObjMem, In, -1, ArrayIdx );
}

void UStructProperty::SkipDefaultProperty( FPackageFileIn& In, int RealSize )
{
  idx StructName;
  In >> CINDEX( StructName );

  u8 SizeByte = (RealSize & 0x70) >> 4;
  u8 IsArray  = (RealSize & 0x80) >> 7;
  int ArrayIdx = 0;

  // Verify struct size
  int StructSize = 0;
  switch( SizeByte )
  {
    case 5:
      In >> *(u8*)&StructSize;
      break;
    case 6:
      In >> *(u16*)&StructSize;
      break;
    case 7:
      In >> StructSize;
      break;
    default:
      StructSize = (SizeByte>0) ? UProperty::PropertySizes[SizeByte] : 0;
      break;
  }

  if ( IsArray )
    ArrayIdx = ReadArrayIndex( In );

  In.Seek( StructSize, Cur ); 
}

void UStructProperty::GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx )
{
  UStruct* DefMem = (Default) ? Default->GetProperty<UStruct*>( this, Idx ) : NULL;
  UStruct* ValMem = Obj->GetProperty<UStruct*>( this, Idx );

  FString InnerBuf;

  // Check if it's worth writing anything at all
  if ( DefMem == NULL || !xstl::Compare( ValMem, DefMem, Struct->StructSize ) )
  {
    for ( UField* Iter = Struct->Children; Iter != NULL; Iter = Iter->Next )
    {
      UProperty* Prop = SafeCast<UProperty>( Iter );
      if ( UNLIKELY( Prop == NULL ) )
        continue; 

      for ( int i = 0; i < Prop->ArrayDim; i++ )
      {
        Prop->GetText( InnerBuf, ValMem, DefMem, i );

        if ( InnerBuf.Length() > 0 )
        {
          // Write the first parenthesis
          if ( Buf.Front() == '\0' )
            Buf += '(';

          // Write text to buffer
          Buf += Prop->Name;
          Buf += '=';
          Buf += InnerBuf;
          Buf += ',';

          InnerBuf.Erase();
        }
      }
    }

    // Snip off the last comma only if we actually wrote anything
    if ( Buf.Length() > 0 )
    {
      Buf.PopBack();
      Buf += ')';
    }
  }
}

void UArrayProperty::Load()
{
  Super::Load();

  idx InnerIdx;
  *PkgFile >> CINDEX( InnerIdx );
  Inner = (UProperty*)LoadObject( InnerIdx, NULL, this );
  Inner->Offset = 0;

  ElementSize = sizeof( ArrayNoType* );
  PropertyType = PROP_Array;
}

bool UArrayProperty::LoadDefaultProperty( void* ObjMem, FPackageFileIn& In, int RealSize, int Idx )
{
  u8 NumElem = 0;
  In >> NumElem;

  // Maybe ArrayNoType can be modified when the type is not used?
  Array<u8>** Arr = (Array<u8>**)PtrAdd( ObjMem, Offset + ( MAX(Idx,0) * ElementSize ) );
  *Arr = new Array<u8>( NumElem * Inner->ElementSize );
  (*Arr)->ElementSize = Inner->ElementSize;

  void* Data = (*Arr)->Data();
  for ( int i = 0; i < NumElem; i++ )
    Inner->LoadDefaultProperty( Data, In, -1, i );

  return true;
}

void UArrayProperty::GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx )
{
  ArrayNoType* GenericArray = Obj->GetProperty<ArrayNoType*>( this, Idx );
  if ( GenericArray == NULL )
    return;

  // Since dynamic arrays have their values printed one per line, but technically count
  // as one property themselves, we have to cheat by writing the name and value for
  // each value that is different
  FString InnerBuf;
  ArrayNoType* DefGenericArray = (Default) ? Default->GetProperty<ArrayNoType*>( this, Idx ) : NULL;

  size_t Num = GenericArray->Size();
  for ( size_t i = 0; i < Num; i++ )
  {
    void* DefValAddr = 0;
    if ( DefGenericArray && i < DefGenericArray->Size() )
      DefValAddr = (*DefGenericArray)[i];

    void* ValAddr = (*GenericArray)[i];
    Inner->GetText( InnerBuf, (UObject*)ValAddr, (UObject*)DefValAddr, 0 );

    if ( InnerBuf.Length() > 0 )
    {
      Buf += '\t';
      Buf += Name;
      if ( ArrayDim > 1 )
      {
        Buf += '[';
        Buf += FString( Idx );
        Buf += ']';
      }
      Buf += '(';
      Buf += FString( (u64)i );
      Buf += ")=";
      Buf += InnerBuf;
      Buf += "\r\n";
      
      InnerBuf.Erase();
    }
  }
}

void UFixedArrayProperty::Load()
{
  Logf( LOG_CRIT, "Go pop '%s' in UTPT and see how to load a FixedArrayProperty", Pkg->Name );
  exit( -1 ); // <- can we not do this
}

bool UFixedArrayProperty::LoadDefaultProperty( void* ObjMem, FPackageFileIn& In, int RealSize, int Idx )
{
  return false;
}

void UFixedArrayProperty::GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx )
{
}

void UMapProperty::Load()
{
  Logf( LOG_CRIT, "Go pop '%s' in UTPT and see how to load a MapProperty", Pkg->Name );
  exit( -1 ); // <- can we not do this
}

bool UMapProperty::LoadDefaultProperty( void* ObjMem, FPackageFileIn& In, int RealSize, int Idx )
{
  return false;
}

void UMapProperty::GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx )
{
}

UByteProperty::~UByteProperty()
{}

UIntProperty::~UIntProperty()
{}

UBoolProperty::~UBoolProperty()
{}

UFloatProperty::~UFloatProperty()
{}

UObjectProperty::~UObjectProperty()
{
  if ( ObjectType != NULL )
    ObjectType->DelRef();
}

UNameProperty::~UNameProperty()
{}

UStringProperty::~UStringProperty()
{}

UClassProperty::~UClassProperty()
{}

UArrayProperty::~UArrayProperty()
{
  if ( Inner != NULL )
    Inner->DelRef();
}

UStructProperty::~UStructProperty()
{
  if ( Struct != NULL )
    Struct->DelRef();
}

UStrProperty::~UStrProperty()
{}

UMapProperty::~UMapProperty()
{}

UFixedArrayProperty::~UFixedArrayProperty()
{}

IMPLEMENT_NATIVE_CLASS( UProperty );
  IMPLEMENT_NATIVE_CLASS( UByteProperty );
  IMPLEMENT_NATIVE_CLASS( UIntProperty );
  IMPLEMENT_NATIVE_CLASS( UBoolProperty );
  IMPLEMENT_NATIVE_CLASS( UFloatProperty );
  IMPLEMENT_NATIVE_CLASS( UObjectProperty );
    IMPLEMENT_NATIVE_CLASS( UClassProperty );
  IMPLEMENT_NATIVE_CLASS( UNameProperty );
  IMPLEMENT_NATIVE_CLASS( UStringProperty );
  IMPLEMENT_NATIVE_CLASS( UArrayProperty );
  IMPLEMENT_NATIVE_CLASS( UStructProperty );
  IMPLEMENT_NATIVE_CLASS( UStrProperty );
  IMPLEMENT_NATIVE_CLASS( UMapProperty );
  IMPLEMENT_NATIVE_CLASS( UFixedArrayProperty );

