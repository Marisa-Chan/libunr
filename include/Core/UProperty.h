/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU General Public License as published by     *|
|*  the Free Software Foundation, either version 3 of the License, or        *|
|*  (at your option) any later version.                                      *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU General Public License for more details.                             *|
|*                                                                           *|
|*  You should have received a copy of the GNU General Public License        *|
|*  along with this program. If not, see <https://www.gnu.org/licenses/>.    *|
\*===========================================================================*/

/*========================================================================
 * UProperty.h - Objects that describe properties inside of objects
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include "Core/UClass.h"

enum EPropertyType
{
  PROP_Byte    = 0x1,
  PROP_Int     = 0x2,
  PROP_Bool    = 0x3,
  PROP_Float   = 0x4,
  PROP_Object  = 0x5,
  PROP_Name    = 0x6,
  PROP_FString  = 0x7,
  PROP_Class   = 0x8,
  PROP_Array   = 0x9,
  PROP_Struct  = 0xa,
  PROP_Vector  = 0xb, // Vector and Rotator properties are not distinct property types
  PROP_Rotator = 0xc, // They use the StructProperty type (unless they don't in UE2)
  PROP_Ascii   = 0xd,
  PROP_Map     = 0xe,
  PROP_FixArr  = 0xf,
  PROP_Ptr     = 0x10,
  PROP_Long    = 0x11,
};

const char* const PropNames[] =
{
  "None",
  "ByteProperty",
  "IntProperty",
  "BoolProperty",
  "FloatProperty",
  "ObjectProperty",
  "NameProperty",
  "FStringProperty",
  "ClassProperty",
  "ArrayProperty",
  "StructProperty",
  "VectorProperty",
  "RotatorProperty",
  "StrProperty",
  "MapProperty",
  "FixedArrayProperty",
  "PointerProperty",
  "LongProperty"
};

class LIBUNR_API UProperty : public UField
{
  DECLARE_NATIVE_CLASS( UProperty, UField, CLASS_Abstract | CLASS_NoExport, Core )

  UProperty();
  UProperty( int InNameIdx );

  u16 ArrayDim;
  u16 ElementSize;
  u32 PropertyFlags;
  FName Category;
  u16 ReplicationOffset; // only exists since objects may store this variable
  
  // Instance variables
  u16 PropertyType; // Type of property based on EPropertyType
  u32 Offset; // Offset into the owner, giving the location of the value
  UClass* GlobalClass; // When reading globalconfig vars, use this
  FString* Description; // Property description introduced in 227j

  // Static variables
  static u8 PropertySizes[8];
  
  virtual void Load();
  virtual bool LoadDefaultProperty( void* ObjMem, FPackageFileIn& In, int RealSize, int Idx = -1 );
  virtual bool LoadDefaultPropertySafe( void* ObjMem, FPackageFileIn& In, u8 Type, int RealSize, int Idx = -1 );
  virtual void GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx );

  u32 GetNativeOffset( FName ClassName, FName PropName );

  static void SkipDefaultProperty( FPackageFileIn& In, int RealSize );
};

class LIBUNR_API UByteProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UByteProperty, UProperty, CLASS_NoExport, Core )
  UByteProperty();

  virtual void Load();
  virtual void GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx );

  UEnum* Enum;
};

class LIBUNR_API UIntProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UIntProperty, UProperty, CLASS_NoExport, Core )
  virtual void Load();
  virtual void GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx );
};

class LIBUNR_API UBoolProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UBoolProperty, UProperty, CLASS_NoExport, Core )
  virtual void Load();
  virtual bool LoadDefaultProperty( void* ObjMem, FPackageFileIn& In, int RealSize, int Idx = -1 );
  virtual void GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx );
};

class LIBUNR_API UFloatProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UFloatProperty, UProperty, CLASS_NoExport, Core )
  virtual void Load();
  virtual void GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx );
};

class LIBUNR_API UObjectProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UObjectProperty, UProperty, CLASS_NoExport, Core )
  UObjectProperty();

  virtual void Load();
  virtual bool LoadDefaultProperty( void* ObjMem, FPackageFileIn& In, int RealSize, int Idx = -1 );
  virtual void GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx );

  UClass* ObjectType;
};

class LIBUNR_API UNameProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UNameProperty, UProperty, CLASS_NoExport, Core )
  virtual void Load();
  virtual bool LoadDefaultProperty( void* ObjMem, FPackageFileIn& In, int RealSize, int Idx = -1 );
  virtual void GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx );
};

// Deprecated property type, seems to be a leftover from older versions
// Array and Map property are not required to handle containing this type
class LIBUNR_API UStringProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UStringProperty, UProperty, CLASS_NoExport, Core )
  virtual void Load();
  virtual bool LoadDefaultProperty( void* ObjMem, FPackageFileIn& In, int RealSize, int Idx = -1 );
  virtual void GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx );
};

class LIBUNR_API UClassProperty : public UObjectProperty
{
  DECLARE_NATIVE_CLASS( UClassProperty, UObjectProperty, CLASS_NoExport, Core )
  UClassProperty();

  virtual void Load();
  virtual void GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx );
  UClass* ClassObj;
};

class LIBUNR_API UArrayProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UArrayProperty, UProperty, CLASS_NoExport, Core )
  UArrayProperty();

  virtual void Load();
  virtual bool LoadDefaultProperty( void* ObjMem, FPackageFileIn& In, int RealSize, int Idx = -1 );
  virtual void GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx );
  UProperty* Inner;
};

class LIBUNR_API UStructProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UStructProperty, UProperty, CLASS_NoExport, Core )
  UStructProperty();

  virtual void Load();
  virtual bool LoadDefaultProperty( void* ObjMem, FPackageFileIn& In, int RealSize, int Idx = -1 );
  virtual bool LoadDefaultPropertySafe( void* ObjMem, FPackageFileIn& In, u8 Type, int RealSize, int Idx = -1 );
  virtual void GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx );

  static void SkipDefaultProperty( FPackageFileIn& In, int RealSize );
  UStruct* Struct;
};

class LIBUNR_API UStrProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UStrProperty, UProperty, CLASS_NoExport, Core )
  virtual void Load();
  virtual bool LoadDefaultProperty( void* ObjMem, FPackageFileIn& In, int RealSize, int Idx = -1 );
  virtual bool LoadDefaultPropertySafe( void* ObjMem, FPackageFileIn& In, u8 Type, int RealSize, int Idx = -1 );
  virtual void GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx );
};

// ?
class LIBUNR_API UMapProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UMapProperty, UProperty, CLASS_NoExport, Core )
  virtual void Load();
  virtual bool LoadDefaultProperty( void* ObjMem, FPackageFileIn& In, int RealSize, int Idx = -1 );
  virtual void GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx );
};

// ?
class LIBUNR_API UFixedArrayProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UFixedArrayProperty, UProperty, CLASS_NoExport, Core )
  virtual void Load();
  virtual bool LoadDefaultProperty( void* ObjMem, FPackageFileIn& In, int RealSize, int Idx = -1 );
  virtual void GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx );
};

class LIBUNR_API UPointerProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UPointerProperty, UProperty, CLASS_NoExport, Core )
  virtual void Load();
  virtual void GetText( FString& Buf, UObject* Obj, UObject* Default, int Idx );
};