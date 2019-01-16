#/*========================================================================*\
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
 * UObjGetSet.h - Inline definitions of property getters and setters
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include <libxstl/XMemory.h>
#include "Core/UClass.h"
#include "Core/UMath.h"
#include "Core/UObject.h"
#include "Core/UProperty.h"
#include "Core/UPackage.h"
#include "Core/USystem.h"

//-------------------------------------------------
// Property getters
//-------------------------------------------------

template <class T> inline T* GetPropAddr( UObject* Obj, UProperty* Prop, int Idx )
{
  return (T*)((u8*)Obj + Prop->Offset + (Idx * sizeof(T)));
}

template <class T> inline T UObject::GetProperty( UProperty* Prop, int Idx )
{
  return *GetPropAddr<T>( this, Prop, Idx );
}

// TODO: Need some better checking here
template <> inline UObject* UObject::GetProperty<UObject*>( UProperty* Prop, int Idx )
{
  UObject* Out = *GetPropAddr<UObject*>( this, Prop, Idx );
  if ( !Out->IsA( UObject::StaticClass() ) )
  {
    // Maybe make a config way for this check to be optional? Could get very slow
    Logf( LOG_CRIT, "OBJECT PROPERTY DOES NOT POINT TO AN OBJECT!!!" );
    GSystem->Exit( -1 );
  }
  return Out;
}

template<> inline UClass* UObject::GetProperty<UClass*>( UProperty* Prop, int Idx )
{
  UClass* Out = *GetPropAddr<UClass*>( this, Prop, Idx );
  if ( Out->StaticClass() != UClass::StaticClass() )
  {
    // see GetObjProperty for same comment
    Logf( LOG_CRIT, "CLASS PROPERTY DOES NOT POINT TO A CLASS!!!" );
    GSystem->Exit( -1 );
  }
  return Out;
}

template<> inline UStruct* UObject::GetProperty<UStruct*>( UProperty* Prop, int Idx )
{
  // this one just returns the pointer to where a struct would begin, you still need the struct
  // definition to access anything meaningful here
  UStructProperty* StructProp = SafeCast<UStructProperty>( Prop );
  if ( !StructProp )
  {
    Logf( LOG_CRIT, "GetProperty<UStruct*> did not get a StructProperty" );
    GSystem->Exit( -1 );
  }
  return (UStruct*)( (u8*)this + Prop->Offset + ( Idx * StructProp->Struct->StructSize ) );
}

template <class T> inline Array<T>** GetArrayPropAddr( UObject* Obj, UArrayProperty* Prop, int Idx )
{
  return (Array<T>**)((u8*)Obj + Prop->Offset + (Idx * sizeof(Array<T>*)));
}

//-------------------------------------------------
// Property setters
//-------------------------------------------------

template <class T> inline void UObject::SetProperty( UProperty* Prop, T NewVal, int Idx )
{
  *GetPropAddr<T>( this, Prop, Idx ) = NewVal;
}

// TODO: Need some better checking here
template <> 
inline void UObject::SetProperty<UObject*>( UProperty* Prop, UObject* NewVal, int Idx )
{
  UObject** Out = GetPropAddr<UObject*>( this, Prop, Idx );
  //if ( !(*Out)->IsA( UObject::StaticClass() ) )
  //{
  //  // Maybe make a config way for this check to be optional? Could get very slow
  //  Logf( LOG_CRIT, "OBJECT PROPERTY DOES NOT POINT TO AN OBJECT!!!" );
  //  GSystem->Exit( -1 );
  //}
  *Out = NewVal;
}

template <>
inline void UObject::SetProperty<UClass*>( UProperty* Prop, UClass* NewVal, int Idx )
{
  UClass** Out = GetPropAddr<UClass*>( this, Prop, Idx );
  if ( !(*Out)->IsA( UClass::StaticClass() ) )
  {
    // Maybe make a config way for this check to be optional? Could get very slow
    Logf( LOG_CRIT, "CLASS PROPERTY DOES NOT POINT TO A CLASS!!!" );
    GSystem->Exit( -1 );
  }
  *Out = NewVal;
}

template<class T> inline
void UObject::SetArrayProperty( UArrayProperty* Prop, FPackageFileIn* In, int Idx,
  u8 ByteSize, u8 NumElem )
{
  Array<T>* ArrayAddr = GetProperty<Array<T>*>( Prop, Idx );
  ArrayAddr->Reserve( NumElem );

  while ( ByteSize && NumElem )
  {
    T Value;
    *In >> Value;
    ArrayAddr->PushBack( Value );

    NumElem--;
    ByteSize -= sizeof(T);
  }
}

template<> inline
void UObject::SetArrayProperty<bool>( UArrayProperty* Prop, FPackageFileIn* In, int Idx,
  u8 ByteSize, u8 NumElem )
{
  Array<bool>* ArrayAddr = GetProperty<Array<bool>*>( Prop, Idx );
  ArrayAddr->Reserve( NumElem );

  while ( ByteSize && NumElem )
  {
    bool bValue;
    u8 Value;
    *In >> Value;

    bValue = Value;
    ArrayAddr->PushBack( bValue );

    NumElem--;
    ByteSize -= sizeof(idx);
  }
}

template<> inline
void UObject::SetArrayProperty<idx>( UArrayProperty* Prop, FPackageFileIn* In, int Idx,
  u8 ByteSize, u8 NumElem )
{
  Array<idx>* ArrayAddr = GetProperty<Array<idx>*>( Prop, Idx );
  ArrayAddr->Reserve( NumElem );

  while ( ByteSize && NumElem )
  {
    idx Value;
    *In >> CINDEX( Value );
    ArrayAddr->PushBack( Value );

    NumElem--;
    ByteSize -= sizeof(idx);
  }
}

template<> inline
void UObject::SetArrayProperty<String*>( UArrayProperty* Prop, FPackageFileIn* In, int Idx,
  u8 ByteSize, u8 NumElem )
{
  Array<String*>* ArrayAddr = GetProperty<Array<String*>*>( Prop, Idx );
  ArrayAddr->Reserve( NumElem );

  // TODO: Properly check ByteSize
  while ( NumElem )
  {
    // Read string size
    idx StringLength = 0;
    *In >> CINDEX( StringLength );

    String* Str = new String();
    Str->Resize( StringLength );

    In->Read( Str->Data(), StringLength );
    ArrayAddr->PushBack( Str );
    NumElem--;
  }
}

