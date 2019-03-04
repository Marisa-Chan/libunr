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

UProperty::UProperty( int InNameIdx )
  : UField()
{
  NameIdx = InNameIdx;
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

void UProperty::GetText( String& Buf, UObject* Obj, int Idx, size_t DefVal, UPackage* Package )
{
  return;
}

size_t UProperty::GetGenericValue( UObject* Obj, int Idx )
{
  return MAX_SIZE;
}

u32 UProperty::GetNativeOffset( const char* ClassName, const char* PropName )
{
  FNativePropertyList* NativePropList = NULL;
  u32 Offset = MAX_UINT32;

  FHash ClassHash = FnvHashString( ClassName );
  for ( size_t i = 0; i < NativePropertyLists->Size() && i != MAX_SIZE; i++ )
  {
    // Can you take the address of an overloaded operator[] ?
    NativePropList = NativePropertyLists->Data()[i];    
    if ( ClassHash == NativePropList->Hash )
      break;
  }
 
  char* UpperPropName = strupper( PropName );
  FHash PropHash = FnvHashString( UpperPropName );
  for ( size_t i = 0; i < NativePropList->Num; i++ )
  {
    if ( NativePropList->Properties[i].Hash == PropHash )
    {
      Offset = NativePropList->Properties[i].Offset;
      break;
    }
  }
 
  xstl::Free( UpperPropName );
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
}

void UByteProperty::GetText( String& Buf, UObject* Obj, int Idx, size_t DefVal, UPackage* Package )
{
  u8 DefValCast = (u8)DefVal;
  u8 Val = Obj->GetProperty<u8>( this, Idx );
  if ( Val != DefValCast )
  {
    if ( Enum && Val < Enum->Names.Size() )
      Buf += Enum->Names[Val];
    else
      Buf += String( (int)Val );
  }
}

size_t UByteProperty::GetGenericValue( UObject* Obj, int Idx )
{
  return (size_t)Obj->GetProperty<u8>( this, Idx );
}

void UIntProperty::Load()
{
  Super::Load();
  ElementSize = 4;
}

void UIntProperty::GetText( String& Buf, UObject* Obj, int Idx, size_t DefVal, UPackage* Package )
{
  int DefValCast = (int)DefVal;
  int Val = Obj->GetProperty<int>( this, Idx );
  if ( Val != DefValCast )
    Buf += String( Val );
}

size_t UIntProperty::GetGenericValue( UObject* Obj, int Idx )
{
  return (size_t)Obj->GetProperty<int>( this, Idx );

}
void UBoolProperty::Load()
{
  Super::Load();
  ElementSize = sizeof( bool );
}

void UBoolProperty::GetText( String& Buf, UObject* Obj, int Idx, size_t DefVal, UPackage* Package )
{
  bool DefValCast = (DefVal & 0xFF) ? true : false;
  bool Val = Obj->GetProperty<bool>( this, Idx );
  if ( Val != DefValCast )
    Buf += ( Val ? "True" : "False" );
}

size_t UBoolProperty::GetGenericValue( UObject* Obj, int Idx )
{
  return (size_t)Obj->GetProperty<bool>( this, Idx );
}

void UFloatProperty::Load()
{
  Super::Load();
  ElementSize = 4;
}

void UFloatProperty::GetText( String& Buf, UObject* Obj, int Idx, size_t DefVal, UPackage* Package )
{
  float DefValCast;
  *(int*)&DefValCast = DefVal;
  float Val = Obj->GetProperty<float>( this, Idx );
  if ( !FltEqual( Val, DefValCast ) )
    Buf += String( Val );
}

size_t UFloatProperty::GetGenericValue( UObject* Obj, int Idx )
{
  float Value = Obj->GetProperty<float>( this, Idx );
  size_t GenericValue = *((size_t*)&Value);
  return GenericValue;
}

void UNameProperty::Load()
{
  Super::Load();
  ElementSize = 4;
}

void UNameProperty::GetText( String& Buf, UObject* Obj, int Idx, size_t DefVal, UPackage* Package )
{
  idx ValIdx = Obj->GetProperty<idx>( this, Idx );
  FNameEntry* DefValCast = Package->GetNameEntry( DefVal );
  FNameEntry* Val = Package->GetNameEntry( ValIdx );
  if ( Val->Hash != DefValCast->Hash )
  {
    Buf += '"';
    Buf += Val->Data;
    Buf += '"';
  }
}

size_t UNameProperty::GetGenericValue( UObject* Obj, int Idx )
{
  return (size_t)Obj->GetProperty<idx>( this, Idx );
}

void UStrProperty::Load()
{
  Super::Load();
  ElementSize = sizeof( String* );
}

void UStrProperty::GetText( String& Buf, UObject* Obj, int Idx, size_t DefVal, UPackage* Package )
{
  String* DefValCast = (String*)DefVal;
  String* Val = Obj->GetProperty<String*>( this, Idx );
  if ( Val && Val->Length() > 0 && (DefValCast == NULL || *Val == *DefValCast) )
  {
    Buf += '"';
    Buf += Val->Data();
    Buf += '"';
  }
}

size_t UStrProperty::GetGenericValue( UObject* Obj, int Idx )
{
  return (size_t)Obj->GetProperty<String*>( this, Idx );
}

void UStringProperty::Load()
{
  Super::Load();
}

void UStringProperty::GetText( String& Buf, UObject* Obj, int Idx, size_t DefVal, UPackage* Package )
{
  String* DefValCast = (String*)DefVal;
  String* Val = Obj->GetProperty<String*>( this, Idx );
  if ( Val && Val->Length() > 0 && (DefValCast == NULL || *Val == *DefValCast) )
  {
    Buf += '"';
    Buf += Val->Data();
    Buf += '"';
  }
}

size_t UStringProperty::GetGenericValue( UObject* Obj, int Idx )
{
  return (size_t)Obj->GetProperty<String*>( this, Idx );
}

void UObjectProperty::Load()
{
  Super::Load();
  ElementSize = sizeof( UObject* );

  idx ObjTypeIdx = 0;
  *PkgFile >> CINDEX( ObjTypeIdx );
  ObjectType = (UClass*)LoadObject( ObjTypeIdx, UClass::StaticClass(), Outer );
}

void UObjectProperty::GetText( String& Buf, UObject* Obj, int Idx, size_t DefVal, UPackage* Package )
{
  UObject* DefValCast = (UObject*)DefVal;
  UObject* Val = Obj->GetProperty<UObject*>( this, Idx );
  if ( Val && Val != DefValCast )
  {
    Buf += Val->Class->Name;
    Buf += '\'';
    Buf += Val->Pkg->Name;
    Buf += '.';
    Buf += Val->Name;
    Buf += '\'';
  }
}

size_t UObjectProperty::GetGenericValue( UObject* Obj, int Idx )
{
  return (size_t)Obj->GetProperty<UObject*>( this, Idx );
}

void UClassProperty::Load()
{
  Super::Load();
  ElementSize = sizeof( UClass* );

  idx ClassIdx = 0;
  *PkgFile >> CINDEX( ClassIdx );
  ClassObj = (UClass*)LoadObject( ClassIdx, UClass::StaticClass(), Outer );
}

void UClassProperty::GetText( String& Buf, UObject* Obj, int Idx, size_t DefVal, UPackage* Package )
{
  UClass* DefValCast = (UClass*)DefVal;
  UClass* Val = Obj->GetProperty<UClass*>( this, Idx );
  if ( Val && Val != DefValCast )
  {
    Buf += "Class'";
    Buf += Val->Pkg->Name;
    Buf += '.';
    Buf += Val->Name;
    Buf += '\'';
  }
}

size_t UClassProperty::GetGenericValue( UObject* Obj, int Idx )
{
  return (size_t)Obj->GetProperty<UClass*>( this, Idx );
}

void UStructProperty::Load()
{
  Super::Load();

  idx StructIdx = 0;
  *PkgFile >> CINDEX( StructIdx );
  Struct = (UStruct*)LoadObject( StructIdx, UStruct::StaticClass(), Outer );

  ElementSize = Struct->StructSize;
}

void UStructProperty::GetText( String& Buf, UObject* Obj, int Idx, size_t DefVal, UPackage* Package )
{
  UStruct* DefMem = (UStruct*)DefVal;
  UStruct* ValMem = Obj->GetProperty<UStruct*>( this, Idx );

  String InnerBuf;

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
        size_t InnerDefVal = DefMem ? Prop->GetGenericValue( DefMem, i ) : 0;
        Prop->GetText( InnerBuf, ValMem, i, InnerDefVal, Obj->Pkg );

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

size_t UStructProperty::GetGenericValue( UObject* Obj, int Idx )
{
  return (size_t)Obj->GetProperty<UStruct*>( this, Idx );
}

void UArrayProperty::Load()
{
  Super::Load();

  idx InnerIdx;
  *PkgFile >> CINDEX( InnerIdx );
  Inner = (UProperty*)LoadObject( InnerIdx, NULL, Outer );
  Inner->Offset = 0;
}

void UArrayProperty::GetText( String& Buf, UObject* Obj, int Idx, size_t DefVal, UPackage* Package )
{
/*  Array<u8>* DefGenericArray = (Array<u8>*)DefVal;
  Array<u8>* GenericArray = (Array<u8>*)GetGenericValue( Obj, Idx );
 
  // We're gonna have to cheat the system a bit by writing all values like they're properties
  // since dynamic arrays are written in that way
  for ( size_t i = 0; i < GenericArray->Size() && i != MAX_SIZE; i++ )
  {
    size_t* DefValAddr = (size_t*)PtrAdd( DefGenericArray->Data(), Inner->ElementSize*i );
    size_t* ValAddr    = (size_t*)PtrAdd( GenericArray->Data(), Inner->ElementSize*i );

    Inner->GetText( InnerBuf, ValAddr, 0, *DefValAddr, Package );
    if ( InnerBuf.Length() > 0 )
    {
      InnerBuf
    }
  }
  */
}

size_t UArrayProperty::GetGenericValue( UObject* Obj, int Idx )
{
  // Arrays are templated so we can't really deduce it's type here
  return (size_t)Obj->GetProperty<void*>( this, Idx );
}

void UFixedArrayProperty::Load()
{
  Super::Load();
  Logf( LOG_CRIT, "Go pop '%s' in UTPT and see how to load a FixedArrayProperty", Pkg->Name );
  exit( -1 ); // <- can we not do this
}

void UFixedArrayProperty::GetText( String& Buf, UObject* Obj, int Idx, size_t DefVal, UPackage* Package )
{
}

size_t UFixedArrayProperty::GetGenericValue( UObject* Obj, int Idx )
{
  return MAX_SIZE-1;
}

void UMapProperty::Load()
{
  Super::Load();
  Logf( LOG_CRIT, "Go pop '%s' in UTPT and see how to load a MapProperty", Pkg->Name );
  exit( -1 ); // <- can we not do this
}

void UMapProperty::GetText( String& Buf, UObject* Obj, int Idx, size_t DefVal, UPackage* Package )
{
}

size_t UMapProperty::GetGenericValue( UObject* Obj, int Idx )
{
  return MAX_SIZE-2;
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

