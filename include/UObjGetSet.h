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

#include "UClass.h"
#include "UObject.h"
#include "UProperty.h"
#include "USystem.h"

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

inline UObject* UObject::GetObjProperty( UObjectProperty* Prop, int Idx )
{
  UObject* Out = *GetObjPropAddr( this, Prop, Idx );
  if ( !Out->IsA( UObject::StaticClass() ) )
  {
    // Maybe make a config way for this check to be optional? Could get very slow
    Logf( LOG_CRIT, "OBJECT PROPERTY DOES NOT POINT TO AN OBJECT!!!" );
    GSystem->Exit( -1 );
  }
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
}

void* UObject::GetStructProperty( UStruct* Struct, UStructProperty* Prop, int Idx )
{
  // this one just returns the pointer to where a struct would begin, you still need the struct
  // definition to access anything meaningful here
  return (void*)( (u8*)this + Prop->Offset + ( Idx * Struct->StructSize ) );
}

const char* UObject::GetStrProperty( UStrProperty* Prop, int Idx )
{
  return *GetStrPropAddr( this, Prop, Idx );
}

