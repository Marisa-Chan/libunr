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
    snprintf( Buf, BufSz, "%i", Val );
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

void UFloatProperty::Load()
{
  Super::Load();
  ElementSize = 4;
}

void UFloatProperty::GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal )
{
  float DefValCast = (float)DefVal;
  float Val = Obj->GetProperty<float>( this, Idx );
  if ( fabs( Val - DefValCast ) >= FLT_EPSILON )
    snprintf( Buf, BufSz, "%f", Val );
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

void UStrProperty::Load()
{
  Super::Load();
  ElementSize = sizeof( char* );
}

void UStrProperty::GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal )
{
  String* DefValCast = (String*)DefVal;
  String* Val = Obj->GetProperty<String*>( this, Idx );
  if ( Val && (DefValCast == NULL || *Val == *DefValCast) )
    snprintf( Buf, BufSz, "\"%s\"", Val->Data() );
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
/*  UObject* DefValCast = (UObject*)DefVal;
  UObject* Val = Obj->GetProperty<UObject*>( this, Idx );
  
  UStruct* DefMem = DefValCast->GetProperty<UStruct*>( this, Idx );
  UStruct* ValMem = Val->GetProperty<UStruct*>( this, Idx );

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
        // This won't work on big endian...
        size_t InnerDefVal = DefMem->GetProperty<size_t>( this, i );
        Prop->GetText( InnerBuf, sizeof( InnerBuf ), ValMem, i, InnerDefVal );

        int InnerLen = strlen( InnerBuf ) + 1;
        if ( UNLIKELY( InnerLen > BufSz ) )
        {
          Logf( LOG_WARN, "UStructProperty::GetText() truncated: Struct = '%s', Property = '%s'",
              Name, Prop->Name );
          return;
        }

        strncat( Buf, InnerBuf, BufSz );
        strncat( Buf, ",", BufSz );
        Buf += InnerLen;
      }
    }
    // Snip off the last comma
    *Buf = '\0';
    strncat( Buf, ")", BufSz );
  } */
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

void UFixedArrayProperty::Load()
{
  Super::Load();
  Logf( LOG_CRIT, "Go pop '%s' in UTPT and see how to load a FixedArrayProperty", Pkg->Name );
  exit( -1 ); // <- can we not do this
}

void UFixedArrayProperty::GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal )
{
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

