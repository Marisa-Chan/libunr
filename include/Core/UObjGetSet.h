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
  if ( Out && !Out->IsA( UObject::StaticClass() ) )
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
  if ( Out && Out->StaticClass() != UClass::StaticClass() )
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
void UObject::SetArrayProperty( UArrayProperty* Prop, int Idx, u8 ByteSize, u8 NumElem )
{
  Array<T>* Arr = new Array<T>();
  Arr->Reserve( NumElem );

  while ( ByteSize && NumElem )
  {
    T Value;
    *PkgFile >> Value;
    Arr->PushBack( Value );

    NumElem--;
    ByteSize -= sizeof(T);
  }

  *GetPropAddr<Array<T>*>( this, Prop, Idx ) = Arr;
}

template<> inline
void UObject::SetArrayProperty<bool>( UArrayProperty* Prop, int Idx, u8 ByteSize, u8 NumElem )
{
  Array<bool>* Arr = new Array<bool>();
  Arr->Reserve( NumElem );

  while ( ByteSize && NumElem )
  {
    bool bValue;
    u8 Value;
    *PkgFile >> Value;

    bValue = Value;
    Arr->PushBack( bValue );

    NumElem--;
    ByteSize -= sizeof(idx);
  }

  *GetPropAddr<Array<bool>*>( this, Prop, Idx ) = Arr;
}

template<> inline
void UObject::SetArrayProperty<idx>( UArrayProperty* Prop, int Idx, u8 ByteSize, u8 NumElem )
{
  Array<idx>* Arr = new Array<idx>();
  Arr->Reserve( NumElem );

  while ( ByteSize && NumElem )
  {
    idx Value;
    *PkgFile >> CINDEX( Value );
    Arr->PushBack( Value );

    NumElem--;
    ByteSize -= sizeof(idx);
  }

  *GetPropAddr<Array<idx>*>( this, Prop, Idx ) = Arr;
}

template<> inline
void UObject::SetArrayProperty<FString*>( UArrayProperty* Prop, int Idx, u8 ByteSize, u8 NumElem )
{
  Array<FString*>* Arr = new Array<FString*>();  
  Arr->Reserve( NumElem );

  // TODO: Properly check ByteSize
  while ( NumElem )
  {
    // Read string size
    idx StringLength = 0;
    *PkgFile >> CINDEX( StringLength );

    FString* Str = new FString();
    Str->Resize( StringLength );

    PkgFile->Read( Str->Data(), StringLength );
    Arr->PushBack( Str );
    NumElem--;
  }

  *GetPropAddr<Array<FString*>*>( this, Prop, Idx ) = Arr;
}

template<> inline
void UObject::SetArrayProperty<UStruct*>( UArrayProperty* Prop, int Idx, u8 ByteSize, u8 NumElem )
{
  Array<u8>* Arr = new Array<u8>();
  UStruct* Struct = ((UStructProperty*)Prop->Inner)->Struct;
  Arr->Reserve( NumElem * Struct->StructSize );

  // Get data pointer for first element
  void* Data = Arr->Data();

  while ( NumElem )
  {
    // Iterate through struct's children
    for ( UField* Child = Struct->Children; Child != NULL; Child = Child->Next )
    {
      UProperty* ChildProp = SafeCast<UProperty>( Child );
      if ( ChildProp )
      {
        if ( ChildProp->Class == UObjectProperty::StaticClass() ||
             ChildProp->Class == UClassProperty::StaticClass() )
        {
          idx ObjRef;
          for ( int i = 0; i < ChildProp->ArrayDim; i++ )
          {
            *PkgFile >> ObjRef;
            *(UObject**)Data = LoadObject( ObjRef, ((UObjectProperty*)ChildProp)->ObjectType, NULL );
            Data = PtrAdd( Data, ChildProp->ElementSize );
          }
        }
        else
        {
          size_t ReadNum = ChildProp->ElementSize * ChildProp->ArrayDim;
          PkgFile->Read( Data, ReadNum );
          Data = PtrAdd( Data, ReadNum );
        }
      }
    }
  }
}

