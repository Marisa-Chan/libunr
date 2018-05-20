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
 * UProperty.h - Objects that describe properties inside of objects
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "UClass.h"

enum EPropertyType
{
  PROP_Byte    = 0x1,
  PROP_Int     = 0x2,
  PROP_Bool    = 0x3,
  PROP_Float   = 0x4,
  PROP_Object  = 0x5,
  PROP_Name    = 0x6,
  PROP_String  = 0x7,
  PROP_Class   = 0x8,
  PROP_Array   = 0x9,
  PROP_Struct  = 0xa,
  PROP_Vector  = 0xb,
  PROP_Rotator = 0xc,
  PROP_Ascii   = 0xd,
  PROP_Map     = 0xe,
  PROP_FixArr  = 0xf,
};

class UProperty : public UField
{
  DECLARE_ABSTRACT_CLASS( UProperty, UField, 0 )
  
  void* GetValue() { return Value; }
  
  u16 ArrayDim;
  u16 ElementSize;
  u32 PropertyFlags;
  FName Category;
  u16 ReplicationOffset; // only exists since objects may store this variable
  
  // Instance variables
  idx Name;
  void* Value;
  
  // Static variables
  static u8 PropertySizes[8];
};

class UByteProperty : public UProperty
{
  DECLARE_CLASS( UByteProperty, UProperty, 0 )
  
  u8 GetValue() { return *((u8*)&Value); }
};

class UIntProperty : public UProperty
{
  DECLARE_CLASS( UIntProperty, UProperty, 0 )
  
  int GetValue() { return *((u16*)&Value); }
};

class UBoolProperty : public UProperty
{
  DECLARE_CLASS( UBoolProperty, UProperty, 0 )
  
  bool GetValue() { return *((bool*)&Value); }
};

class UFloatProperty : public UProperty
{
  DECLARE_CLASS( UFloatProperty, UProperty, 0 )
  
  float GetValue() { return *((float*)&Value); }
};

class UObjectProperty : public UProperty
{
  DECLARE_CLASS( UObjectProperty, UProperty, 0 )
  
  UObject* GetValue() { return (UObject*)Value; }
};

class UNameProperty : public UProperty
{
  DECLARE_CLASS( UNameProperty, UProperty, 0 )
  
  FName GetValue() { return FName( (size_t)Value ); }
};

// ?
class UStringProperty : public UProperty
{
  DECLARE_CLASS( UStringProperty, UProperty, 0 )
};

class UClassProperty : public UProperty
{
  DECLARE_CLASS( UClassProperty, UProperty, 0 )
  
  UClass* GetValue() { return (UClass*)Value; }
};

// ?
class UArrayProperty : public UProperty
{
  DECLARE_CLASS( UArrayProperty, UProperty, 0 )
};

class UStructProperty : public UProperty
{
  DECLARE_CLASS( UStructProperty, UProperty, 0 )
  
  UStruct* GetValue() { return (UStruct*)Value; }
};

// ?
class UVectorProperty : public UProperty
{
  DECLARE_CLASS( UVectorProperty, UProperty, 0 )
};

// ?
class URotatorProperty : public UProperty
{
  DECLARE_CLASS( URotatorProperty, UProperty, 0 )
};

class UAsciiStrProperty : public UProperty
{
  DECLARE_CLASS( UAsciiStrProperty, UProperty, 0 )
  
  int Length;
};

// ?
class UMapProperty : public UProperty
{
  DECLARE_CLASS( UMapProperty, UProperty, 0 )
};

// ?
class UFixedArrayProperty : public UProperty
{
  DECLARE_CLASS( UFixedArrayProperty, UProperty, 0 )
};
