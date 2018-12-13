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

#include "XMemory.h"
#include "Core/UClass.h"
#include "Core/UObject.h"
#include "Core/UProperty.h"
#include "Core/UPackage.h"
#include "Core/USystem.h"

#define DEFINE_GET_PROPERTY_ADDRESS(name, type, proptype) \
  inline type* name( UObject* Obj, proptype* Prop, int Idx ) { \
    return (type*)((u8*)Obj + Prop->Offset + (Idx * sizeof(type))); \
  } \

DEFINE_GET_PROPERTY_ADDRESS( GetBytePropAddr,  u8,        UByteProperty   )
DEFINE_GET_PROPERTY_ADDRESS( GetIntPropAddr,   int,       UIntProperty    )
DEFINE_GET_PROPERTY_ADDRESS( GetBoolPropAddr,  bool,      UBoolProperty   )
DEFINE_GET_PROPERTY_ADDRESS( GetFloatPropAddr, float,     UFloatProperty  )
DEFINE_GET_PROPERTY_ADDRESS( GetObjPropAddr,   UObject*,  UObjectProperty )
DEFINE_GET_PROPERTY_ADDRESS( GetNamePropAddr,  idx,       UNameProperty   )
DEFINE_GET_PROPERTY_ADDRESS( GetClassPropAddr, UClass*,   UClassProperty  )
DEFINE_GET_PROPERTY_ADDRESS( GetStrPropAddr, const char*, UStrProperty    )

// Property getters
inline u8 UObject::GetByteProperty( UByteProperty* Prop, int Idx )
{
  return *GetBytePropAddr( this, Prop, Idx );
}

inline int UObject::GetIntProperty( UIntProperty* Prop, int Idx )
{
  return *GetIntPropAddr( this, Prop, Idx );
}

inline bool UObject::GetBoolProperty( UBoolProperty* Prop )
{
  // Unreal currently doesn't support bool arrays, maybe in the future?
  return *GetBoolPropAddr( this, Prop, 0 );
}

inline float UObject::GetFloatProperty( UFloatProperty* Prop, int Idx )
{
  return *GetFloatPropAddr( this, Prop, Idx );
}

// TODO: Need some better checking here
inline UObject* UObject::GetObjProperty( UObjectProperty* Prop, int Idx )
{
  UObject* Out = *GetObjPropAddr( this, Prop, Idx );
  if ( !Out->IsA( UObject::StaticClass() ) )
  {
    // Maybe make a config way for this check to be optional? Could get very slow
    Logf( LOG_CRIT, "OBJECT PROPERTY DOES NOT POINT TO AN OBJECT!!!" );
    GSystem->Exit( -1 );
  }
  return Out;
}

inline idx UObject::GetNameProperty( UNameProperty* Prop, int Idx )
{
  return *GetNamePropAddr( this, Prop, Idx );
}

inline UClass* UObject::GetClassProperty( UClassProperty* Prop, int Idx )
{
  UClass* Out = *GetClassPropAddr( this, Prop, Idx );
  if ( Out->StaticClass() != UClass::StaticClass() )
  {
    // see GetObjProperty for same comment
    Logf( LOG_CRIT, "CLASS PROPERTY DOES NOT POINT TO A CLASS!!!" );
    GSystem->Exit( -1 );
  }
  return Out;
}

inline void* UObject::GetStructProperty( UStructProperty* Prop, int Idx )
{
  // this one just returns the pointer to where a struct would begin, you still need the struct
  // definition to access anything meaningful here
  return (void*)( (u8*)this + Prop->Offset + ( Idx * Prop->Struct->StructSize ) );
}

inline const char* UObject::GetStrProperty( UStrProperty* Prop, int Idx )
{
  return *GetStrPropAddr( this, Prop, Idx );
}

inline void UObject::SetByteProperty( UByteProperty* Prop, u8 NewVal, int Idx )
{
  *GetBytePropAddr( this, Prop, Idx ) = NewVal;
}

inline void UObject::SetIntProperty( UIntProperty* Prop, int NewVal, int Idx )
{
  *GetIntPropAddr( this, Prop, Idx ) = NewVal;
}

inline void UObject::SetBoolProperty( UBoolProperty* Prop, bool NewVal )
{
  *GetBoolPropAddr( this, Prop, 0 ) = NewVal;
}

inline void UObject::SetFloatProperty( UFloatProperty* Prop, float NewVal, int Idx )
{
  *GetFloatPropAddr( this, Prop, Idx ) = NewVal;
}

// TODO: Need some better checking here
inline void UObject::SetObjProperty( UObjectProperty* Prop, UObject* NewVal, int Idx )
{
  UObject** Out = GetObjPropAddr( this, Prop, Idx );
  //if ( !(*Out)->IsA( UObject::StaticClass() ) )
  //{
  //  // Maybe make a config way for this check to be optional? Could get very slow
  //  Logf( LOG_CRIT, "OBJECT PROPERTY DOES NOT POINT TO AN OBJECT!!!" );
  //  GSystem->Exit( -1 );
  //}
  *Out = NewVal;
}

inline void UObject::SetNameProperty( UNameProperty* Prop, idx NewVal, int Idx )
{
  *GetNamePropAddr( this, Prop, Idx ) = NewVal;
}

inline void UObject::SetClassProperty( UClassProperty* Prop, UClass* NewVal, int Idx )
{
  UClass** Out = GetClassPropAddr( this, Prop, Idx );
  if ( !(*Out)->IsA( UClass::StaticClass() ) )
  {
    // Maybe make a config way for this check to be optional? Could get very slow
    Logf( LOG_CRIT, "OBJECT PROPERTY DOES NOT POINT TO AN OBJECT!!!" );
    GSystem->Exit( -1 );
  }
  *Out = NewVal;
}

inline void UObject::SetStrProperty( UStrProperty* Prop, const char* NewVal, int Idx )
{
  *GetStrPropAddr( this, Prop, Idx ) = NewVal;
}

// Not something that should be called in the scripting environment
// Only for defaultproperty lists
void UObject::SetStructProperty( UStructProperty* Prop, FPackageFileIn* In, int Idx )
{
  void* StructAddr = GetStructProperty( Prop, Idx );

  for ( UField* Child = Prop->Struct->Children; Child != NULL; Child = Child->Next )
  {
    // Pure structs should not have anything besides property types or enums
    if ( !Child->IsA( UProperty::StaticClass() ) && !Child->IsA( UEnum::StaticClass() ) )
    {
      Logf( LOG_CRIT, "Pure struct type has a bad child type!" );
      return;
    }
    
    // Determine property type and set property
    int i = 0;
    UProperty* ChildProp = (UProperty*)Child;
    if ( ChildProp->Class == UByteProperty::StaticClass() )
    {
      do
      {
        u8 Byte = 0;
        *In >> Byte;
        *GetBytePropAddr( (UObject*)StructAddr, (UByteProperty*)ChildProp, i++ ) = Byte;
      } while ( i < ChildProp->ArrayDim );
    }

    else if ( ChildProp->Class == UIntProperty::StaticClass() )
    {
      do
      {
        int Int = 0;
        *In >> Int;
        *GetIntPropAddr( (UObject*)StructAddr, (UIntProperty*)ChildProp, i++ ) = Int;
      } while ( i < ChildProp->ArrayDim );
    }

    else if ( ChildProp->Class == UFloatProperty::StaticClass() )
    {
      do
      {
        float Float = 0;
        *In >> Float;
        *GetFloatPropAddr( (UObject*)StructAddr, (UFloatProperty*)ChildProp, i++ ) = Float;
      } while ( i < ChildProp->ArrayDim );
    }

    else if ( ChildProp->Class == UBoolProperty::StaticClass() )
    {
      u8 Bool = 0;
      *In >> Bool;
      *GetBoolPropAddr( (UObject*)StructAddr, (UBoolProperty*)ChildProp, 0 ) = (Bool == 1);
    }

    else if ( ChildProp->Class == UNameProperty::StaticClass() )
    {
      do
      {
        idx Name = 0;
        *In >> CINDEX( Name );
        *GetNamePropAddr( (UObject*)StructAddr, (UNameProperty*)ChildProp, i++ ) = Name;
      } while ( i < ChildProp->ArrayDim );
    }

    else if ( ChildProp->Class == UObjectProperty::StaticClass() )
    {
      do
      {
        idx ObjRef = 0;
        *In >> CINDEX( ObjRef );
        UObject* Obj = (UObject*)LoadObject( ObjRef, 
          ((UObjectProperty*)ChildProp)->ObjectType, NULL );

        if ( Obj == NULL )
        {
          Logf( LOG_CRIT, "Can't load object '%s' for struct property '%s' in property list for object '%s'", 
                Prop->Pkg->ResolveNameFromObjRef( ObjRef ), Prop->Name, Prop->Outer );
          return;
        }

        *GetObjPropAddr( (UObject*)StructAddr, (UObjectProperty*)ChildProp, i++ ) = Obj;
      } while ( i < ChildProp->ArrayDim );
    }

    else if ( ChildProp->Class == UClassProperty::StaticClass() )
    {
      do
      {
        idx ObjRef = 0;
        *In >> CINDEX( ObjRef );
        UClass* Cls = (UClass*)LoadObject( ObjRef, UClassProperty::StaticClass(), NULL );

        if ( Cls == NULL )
        {
          Logf( LOG_CRIT, "Can't load class '%s' for struct property '%s' in property list for object '%s'", 
                Prop->Pkg->ResolveNameFromObjRef( ObjRef ), Prop->Name, Prop->Outer );
          return;
        }

        *GetClassPropAddr( (UObject*)StructAddr, (UClassProperty*)ChildProp, i++ ) = Cls;
      } while ( i < ChildProp->ArrayDim );
    }

    else if ( ChildProp->Class == UStructProperty::StaticClass() )
    {
      for ( int i = 0; i < ChildProp->ArrayDim; i++ )
        SetStructProperty( (UStructProperty*)ChildProp, In, i );
    }

    else if ( ChildProp->Class == UStrProperty::StaticClass() )
    {
      do
      {
        idx StringLength = 0;
        *In >> CINDEX( StringLength );

        char* String = new char[StringLength];
        In->Read( String, StringLength );

        *GetStrPropAddr( (UObject*)StructAddr, (UStrProperty*)ChildProp, i++ ) = String;
      } while ( i < ChildProp->ArrayDim );
    }

    else if ( ChildProp->Class != UEnum::StaticClass() )
    {
      Logf( LOG_WARN, "Unhandled case for StructProperty loading (Class = '%s')", ChildProp->Class->Name );
    }
  }
}
