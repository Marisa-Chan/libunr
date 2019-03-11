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

void UProperty::GetText( String& Buf, UObject* Obj, UObject* Default, int Idx )
{
  return;
}

void UProperty::GetTextContainer( String& Buf, UObject* ObjMem, UObject* DefMem, 
  UPackage* ObjPkg, UPackage* DefPkg, int Idx )
{
  GetText( Buf, ObjMem, DefMem, Idx );
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

void UProperty::ReadDynamicArrayProperty( UObject* Obj, FPackageFileIn* In, int Idx, int RealSize, u8 Num )
{
  Logf( LOG_WARN, "Default ArrayProperty contains '%s'", Class->Name );
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

void UByteProperty::GetText( String& Buf, UObject* Obj, UObject* Default, int Idx )
{
  u8 DefVal = (Default) ? Default->GetProperty<u8>( this, Idx ) : 0;
  u8 Val = Obj->GetProperty<u8>( this, Idx );
  if ( Val != DefVal )
  {
    if ( Enum && Val < Enum->Names.Size() )
      Buf += Enum->Names[Val];
    else
      Buf += String( (int)Val );
  }
}

void UByteProperty::ReadDynamicArrayProperty( UObject* Obj, FPackageFileIn* In, int Idx, int RealSize, u8 Num )
{
  Obj->SetArrayProperty<u8>( (UArrayProperty*)Outer, In, Idx, RealSize, Num );
}

void UIntProperty::Load()
{
  Super::Load();
  ElementSize = 4;
}

void UIntProperty::GetText( String& Buf, UObject* Obj, UObject* Default, int Idx )
{
  int DefVal = (Default) ? Default->GetProperty<int>( this, Idx ) : 0;
  int Val = Obj->GetProperty<int>( this, Idx );
  if ( Val != DefVal )
    Buf += String( Val );
}

void UIntProperty::ReadDynamicArrayProperty( UObject* Obj, FPackageFileIn* In, int Idx, int RealSize, u8 Num )
{
  Obj->SetArrayProperty<int>( (UArrayProperty*)Outer, In, Idx, RealSize, Num );
}

void UBoolProperty::Load()
{
  Super::Load();
  ElementSize = sizeof( bool );
}

void UBoolProperty::GetText( String& Buf, UObject* Obj, UObject* Default, int Idx )
{
  bool DefVal = (Default) ? Default->GetProperty<bool>( this, Idx ) : 0;
  bool Val = Obj->GetProperty<bool>( this, Idx );
  if ( Val != DefVal )
    Buf += ( Val ? "True" : "False" );
}

void UBoolProperty::ReadDynamicArrayProperty( UObject* Obj, FPackageFileIn* In, int Idx, int RealSize, u8 Num )
{
  Obj->SetArrayProperty<bool>( (UArrayProperty*)Outer, In, Idx, RealSize, Num );
}

void UFloatProperty::Load()
{
  Super::Load();
  ElementSize = 4;
}

void UFloatProperty::GetText( String& Buf, UObject* Obj, UObject* Default, int Idx )
{
  float DefVal = (Default) ? Default->GetProperty<float>( this, Idx ) : 0;
  float Val = Obj->GetProperty<float>( this, Idx );
  if ( !FltEqual( Val, DefVal ) )
    Buf += String( Val );
}

void UFloatProperty::ReadDynamicArrayProperty( UObject* Obj, FPackageFileIn* In, int Idx, int RealSize, u8 Num )
{
  Obj->SetArrayProperty<float>( (UArrayProperty*)Outer, In, Idx, RealSize, Num );
}

void UNameProperty::Load()
{
  Super::Load();
  ElementSize = 4;
}

void UNameProperty::GetText( String& Buf, UObject* Obj, UObject* Default, int Idx )
{
  idx DefIdx = (Default) ? Default->GetProperty<idx>( this, Idx ) : 0;
  idx ValIdx = Obj->GetProperty<idx>( this, Idx );
  FNameEntry* DefVal = (Default) ? Default->Pkg->GetNameEntry( DefIdx ) : Obj->Pkg->GetNameEntry( 0 );
  FNameEntry* Val;

  // FIXME: Is there an easier way to check this?
  // In this case, it is very likely that the value in the object has not changed from the default
  if ( Default && (Default->Pkg != Obj->Pkg) && (Default->Pkg == Obj->Class->Pkg) && DefIdx == ValIdx )
    return;
  else
    Val = Obj->Pkg->GetNameEntry( ValIdx );

  if ( Val->Hash != DefVal->Hash )
  {
    Buf += '"';
    Buf += Val->Data;
    Buf += '"';
  }
}

void UNameProperty::GetTextContainer( String& Buf, UObject* ObjMem, UObject* DefMem, UPackage* ObjPkg, UPackage* DefPkg, int Idx )
{
  idx DefIdx = (DefMem) ? DefMem->GetProperty<idx>( this, Idx ) : 0;
  idx ValIdx = ObjMem->GetProperty<idx>( this, Idx );
  FNameEntry* DefVal = (DefPkg) ? DefPkg->GetNameEntry( DefIdx ) : ObjPkg->GetNameEntry( 0 );
  FNameEntry* Val = ObjPkg->GetNameEntry( ValIdx );
  if ( Val->Hash != DefVal->Hash )
  {
    Buf += '"';
    Buf += Val->Data;
    Buf += '"';
  }
}

void UNameProperty::ReadDynamicArrayProperty( UObject* Obj, FPackageFileIn* In, int Idx, int RealSize, u8 Num )
{
  Obj->SetArrayProperty<idx>( (UArrayProperty*)Outer, In, Idx, RealSize, Num );
}

void UStrProperty::Load()
{
  Super::Load();
  ElementSize = sizeof( String* );
}

void UStrProperty::GetText( String& Buf, UObject* Obj, UObject* Default, int Idx )
{
  String* DefVal = (Default) ? Default->GetProperty<String*>( this, Idx ) : NULL;
  String* Val = Obj->GetProperty<String*>( this, Idx );
  if ( Val && Val->Length() > 0 && (DefVal == NULL || *Val != *DefVal) )
  {
    Buf += '"';
    Buf += Val->Data();
    Buf += '"';
  }
}

void UStrProperty::ReadDynamicArrayProperty( UObject* Obj, FPackageFileIn* In, int Idx, int RealSize, u8 Num )
{
  Obj->SetArrayProperty<String*>( (UArrayProperty*)Outer, In, Idx, RealSize, Num );
}

void UStringProperty::Load()
{
  Super::Load();
  ElementSize = sizeof( String* );
}

void UStringProperty::GetText( String& Buf, UObject* Obj, UObject* Default, int Idx )
{
  String* DefVal = (Default) ? Default->GetProperty<String*>( this, Idx ) : NULL;
  String* Val = Obj->GetProperty<String*>( this, Idx );
  if ( Val && Val->Length() > 0 && (DefVal == NULL || *Val != *DefVal) )
  {
    Buf += '"';
    Buf += Val->Data();
    Buf += '"';
  }
}

void UStringProperty::ReadDynamicArrayProperty( UObject* Obj, FPackageFileIn* In, int Idx, int RealSize, u8 Num )
{
  Obj->SetArrayProperty<String*>( (UArrayProperty*)Outer, In, Idx, RealSize, Num );
}

void UObjectProperty::Load()
{
  Super::Load();
  ElementSize = sizeof( UObject* );

  idx ObjTypeIdx = 0;
  *PkgFile >> CINDEX( ObjTypeIdx );
  ObjectType = (UClass*)LoadObject( ObjTypeIdx, UClass::StaticClass(), Outer );
}

void UObjectProperty::GetText( String& Buf, UObject* Obj, UObject* Default, int Idx )
{
  UObject* DefVal = (Default) ? Default->GetProperty<UObject*>( this, Idx ) : NULL;
  UObject* Val = Obj->GetProperty<UObject*>( this, Idx );
  if ( Val && Val != DefVal )
  {
    Buf += Val->Class->Name;
    Buf += '\'';
    
    String* FullName = Val->Pkg->GetFullObjName( Val->Export );
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
  ClassObj = (UClass*)LoadObject( ClassIdx, UClass::StaticClass(), Outer );
}

void UClassProperty::GetText( String& Buf, UObject* Obj, UObject* Default, int Idx )
{
  UClass* DefVal = (Default) ? Default->GetProperty<UClass*>( this, Idx ) : NULL;
  UClass* Val = Obj->GetProperty<UClass*>( this, Idx );
  if ( Val && Val != DefVal )
  {
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
}

void UStructProperty::GetText( String& Buf, UObject* Obj, UObject* Default, int Idx )
{
  UStruct* DefMem = (Default) ? Default->GetProperty<UStruct*>( this, Idx ) : NULL;
  UStruct* ValMem = Obj->GetProperty<UStruct*>( this, Idx );

  String InnerBuf;

  // Check if it's worth writing anything at all
  if ( DefMem == NULL || !xstl::Compare( ValMem, DefMem, Struct->StructSize ) )
  {
    UPackage* ObjPkg = Obj->Pkg;
        UPackage* DefPkg = (Default) ? Default->Pkg : NULL;

    for ( UField* Iter = Struct->Children; Iter != NULL; Iter = Iter->Next )
    {
      UProperty* Prop = SafeCast<UProperty>( Iter );
      if ( UNLIKELY( Prop == NULL ) )
        continue; 

      for ( int i = 0; i < Prop->ArrayDim; i++ )
      {
        Prop->GetTextContainer( InnerBuf, ValMem, DefMem, ObjPkg, DefPkg, i );

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
}

void UArrayProperty::GetText( String& Buf, UObject* Obj, UObject* Default, int Idx )
{
  ArrayNoType* GenericArray = Obj->GetProperty<ArrayNoType*>( this, Idx );
  if ( GenericArray == NULL )
    return;

  // Since dynamic arrays have their values printed one per line, but technically count
  // as one property themselves, we have to cheat by writing the name and value for
  // each value that is different
  String InnerBuf;
  ArrayNoType* DefGenericArray = (Default) ? Default->GetProperty<ArrayNoType*>( this, Idx ) : NULL;

  UPackage* ObjPkg = Obj->Pkg;
  UPackage* DefPkg = (Default) ? Default->Pkg : NULL;
  size_t Num = GenericArray->Size();
  for ( size_t i = 0; i < Num && i != MAX_SIZE; i++ )
  {
    void* DefValAddr = 0;
    if ( DefGenericArray && i < DefGenericArray->Size() )
      DefValAddr = (*DefGenericArray)[i];

    void* ValAddr = (*GenericArray)[i];
    Inner->GetTextContainer( InnerBuf, (UObject*)ValAddr, (UObject*)DefValAddr, ObjPkg, DefPkg, 0 );

    if ( InnerBuf.Length() > 0 )
    {
      Buf += '\t';
      Buf += Name;
      if ( ArrayDim > 1 )
      {
        Buf += '[';
        Buf += String( Idx );
        Buf += ']';
      }
      Buf += '(';
      Buf += String( (u64)i );
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

void UFixedArrayProperty::GetText( String& Buf, UObject* Obj, UObject* Default, int Idx )
{
}

void UMapProperty::Load()
{
  Logf( LOG_CRIT, "Go pop '%s' in UTPT and see how to load a MapProperty", Pkg->Name );
  exit( -1 ); // <- can we not do this
}

void UMapProperty::GetText( String& Buf, UObject* Obj, UObject* Default, int Idx )
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

