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
  
  if ( Outer->Flags & RF_Native )
    Offset = GetNativeOffset( Outer->Name, Name );

  if ( Outer->Class == UClass::StaticClass() && PropertyFlags & CPF_GlobalConfig )
    GlobalClass = (UClass*)Outer;
}

void UProperty::GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal )
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

void UByteProperty::GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal )
{
  u8 DefValCast = (u8)DefVal;
  u8 Val = Obj->GetProperty<u8>( this, Idx );
  if ( Val != DefValCast )
  {
    if ( Enum && Val < Enum->Names.Size() )
      snprintf( Buf, BufSz, "%s", Enum->Names[Val] );
    else
      snprintf( Buf, BufSz, "%i", Val );
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

void UIntProperty::GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal )
{
  int DefValCast = (int)DefVal;
  int Val = Obj->GetProperty<int>( this, Idx );
  if ( Val != DefValCast )
    snprintf( Buf, BufSz, "%i", Val );
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

void UBoolProperty::GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal )
{
  bool DefValCast = (DefVal & 0xFF) ? true : false;
  bool Val = Obj->GetProperty<bool>( this, Idx );
  if ( Val != DefValCast )
    snprintf( Buf, BufSz, "%s", Val ? "True" : "False" );
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

void UFloatProperty::GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal )
{
  float DefValCast;
  *(int*)&DefValCast = DefVal;
  float Val = Obj->GetProperty<float>( this, Idx );
  if ( !FltEqual( Val, DefValCast ) )
    snprintf( Buf, BufSz, "%f", Val );
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

void UNameProperty::GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal )
{
  FNameEntry* DefValCast = Pkg->GetNameEntry( (idx)DefVal );
  FNameEntry* Val = Pkg->GetNameEntry( Obj->GetProperty<idx>( this, Idx ) );
  if ( Val->Hash != DefValCast->Hash )
    snprintf( Buf, BufSz, "\"%s\"", Val->Data );
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

void UStrProperty::GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal )
{
  String* DefValCast = (String*)DefVal;
  String* Val = Obj->GetProperty<String*>( this, Idx );
  if ( Val && (DefValCast == NULL || *Val == *DefValCast) )
    snprintf( Buf, BufSz, "\"%s\"", Val->Data() );
}

size_t UStrProperty::GetGenericValue( UObject* Obj, int Idx )
{
  return (size_t)Obj->GetProperty<String*>( this, Idx );
}

void UStringProperty::Load()
{
  Super::Load();
}

void UStringProperty::GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal )
{
  String* DefValCast = (String*)DefVal;
  String* Val = Obj->GetProperty<String*>( this, Idx );
  if ( DefValCast == NULL || (Val && *Val == *DefValCast) )
    snprintf( Buf, BufSz, "\"%s\"", Val->Data() );
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

void UObjectProperty::GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal )
{
  UObject* DefValCast = (UObject*)DefVal;
  UObject* Val = Obj->GetProperty<UObject*>( this, Idx );
  if ( Val && Val != DefValCast )
    snprintf( Buf, BufSz, "%s'%s.%s'", Val->Class->Name, Val->Pkg->Name, Val->Name );
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

void UClassProperty::GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal )
{
  UClass* DefValCast = (UClass*)DefVal;
  UClass* Val = Obj->GetProperty<UClass*>( this, Idx );
  if ( Val && Val != DefValCast )
    snprintf( Buf, BufSz, "Class'%s.%s'", Val->Pkg->Name, Val->Name );
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

void UStructProperty::GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal )
{
  UStruct* DefMem = (UStruct*)DefMem;
  UStruct* ValMem = Obj->GetProperty<UStruct*>( this, Idx );

  char InnerBuf[128] = { 0 };

  if ( !xstl::Compare( ValMem, DefMem, Struct->StructSize ) )
  {
    strcat( Buf++, "(" );
    BufSz--;
    for ( UField* Iter = Struct->Children; Iter != NULL; Iter = Iter->Next )
    {
      UProperty* Prop = (UProperty*)Iter;
      for ( int i = 0; i < Prop->ArrayDim; i++ )
      {
        size_t InnerDefVal = Prop->GetGenericValue( DefMem, i );
        Prop->GetText( InnerBuf, sizeof( InnerBuf ), ValMem, i, InnerDefVal );

        int NameLen  = strlen( Prop->Name ) + 1;
        int InnerLen = strlen( InnerBuf ) + 1;
  
        if ( InnerLen > 1 )
        {
          if ( UNLIKELY( (NameLen + InnerLen) > BufSz ) )
          {
            Logf( LOG_WARN, "UStructProperty::GetText() truncated: Struct = '%s', Property = '%s'",
                Name, Prop->Name );
            return;
          }

          strncat( Buf, Prop->Name, BufSz );
          strncat( Buf, "=", BufSz );
          strncat( Buf, InnerBuf, BufSz );
          strncat( Buf, ",", BufSz );
          Buf += InnerLen;
        }
      }
    }
    // Snip off the last comma
    *--Buf = '\0';
    strncat( Buf, ")", BufSz );
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
}

void UArrayProperty::GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal )
{
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

void UFixedArrayProperty::GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal )
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

void UMapProperty::GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal )
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

