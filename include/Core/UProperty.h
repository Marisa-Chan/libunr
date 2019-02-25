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
  PROP_String  = 0x7,
  PROP_Class   = 0x8,
  PROP_Array   = 0x9,
  PROP_Struct  = 0xa,
  PROP_Vector  = 0xb, // Vector and Rotator properties are not distinct property types
  PROP_Rotator = 0xc, // They use the StructProperty type (unless they don't in UE2)
  PROP_Ascii   = 0xd,
  PROP_Map     = 0xe,
  PROP_FixArr  = 0xf,
  PROP_Long    = 0x10,
};

class DLL_EXPORT UProperty : public UField
{
  DECLARE_NATIVE_ABSTRACT_CLASS( UProperty, UField, CLASS_NoExport, Core )

  UProperty();
  UProperty( int InNameIdx );

  u16 ArrayDim;
  u16 ElementSize;
  u32 PropertyFlags;
  const char* Category;
  u16 ReplicationOffset; // only exists since objects may store this variable
  
  // Instance variables
  u32 Offset; // Offset into the owner, giving the location of the value
  UClass* GlobalClass; // When reading globalconfig vars, use this
  String* Description; // Property description introduced in 227j

  // Static variables
  static u8 PropertySizes[8];
  
  virtual void   Load();
  virtual void   GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal = 0 );
  virtual size_t GetGenericValue( UObject* Obj, int Idx );
  u32 GetNativeOffset( const char* ClassName, const char* PropName );
};

class DLL_EXPORT UByteProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UByteProperty, UProperty, CLASS_NoExport, Core )

  virtual void Load();
  virtual void GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal = 0 );
  virtual size_t GetGenericValue( UObject* Obj, int Idx );

  UEnum* Enum;
};

class DLL_EXPORT UIntProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UIntProperty, UProperty, CLASS_NoExport, Core )
  virtual void Load();
  virtual void GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal = 0 );
  virtual size_t GetGenericValue( UObject* Obj, int Idx );
};

class DLL_EXPORT UBoolProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UBoolProperty, UProperty, CLASS_NoExport, Core )
  virtual void Load();
  virtual void GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal = 0 );
  virtual size_t GetGenericValue( UObject* Obj, int Idx );
};

class DLL_EXPORT UFloatProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UFloatProperty, UProperty, CLASS_NoExport, Core )
  virtual void Load();
  virtual size_t GetGenericValue( UObject* Obj, int Idx );
  virtual void GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal = 0 );
};

class DLL_EXPORT UObjectProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UObjectProperty, UProperty, CLASS_NoExport, Core )
  virtual void Load();
  virtual void GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal = 0 );
  virtual size_t GetGenericValue( UObject* Obj, int Idx );
  UClass* ObjectType;
};

class DLL_EXPORT UNameProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UNameProperty, UProperty, CLASS_NoExport, Core )
  virtual void Load();
  virtual void GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal = 0 );
  virtual size_t GetGenericValue( UObject* Obj, int Idx );
};

// ?
class DLL_EXPORT UStringProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UStringProperty, UProperty, CLASS_NoExport, Core )
  virtual void Load();
  virtual void GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal = 0 );
  virtual size_t GetGenericValue( UObject* Obj, int Idx );
};

class DLL_EXPORT UClassProperty : public UObjectProperty
{
  DECLARE_NATIVE_CLASS( UClassProperty, UObjectProperty, CLASS_NoExport, Core )
  virtual void Load();
  virtual void GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal = 0 );
  virtual size_t GetGenericValue( UObject* Obj, int Idx );
  UClass* ClassObj;
};

class DLL_EXPORT UArrayProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UArrayProperty, UProperty, CLASS_NoExport, Core )
  virtual void Load();
  virtual void GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal = 0 );
  virtual size_t GetGenericValue( UObject* Obj, int Idx );
  UProperty* Inner;
};

class DLL_EXPORT UStructProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UStructProperty, UProperty, CLASS_NoExport, Core )
  virtual void Load();
  virtual void GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal = 0 );
  virtual size_t GetGenericValue( UObject* Obj, int Idx );
  UStruct* Struct;
};

class DLL_EXPORT UStrProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UStrProperty, UProperty, CLASS_NoExport, Core )
  virtual void Load();
  virtual void GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal = 0 );
  virtual size_t GetGenericValue( UObject* Obj, int Idx );
  int Length;
};

// ?
class DLL_EXPORT UMapProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UMapProperty, UProperty, CLASS_NoExport, Core )
  virtual void Load();
  virtual void GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal = 0 );
  virtual size_t GetGenericValue( UObject* Obj, int Idx );
};

// ?
class DLL_EXPORT UFixedArrayProperty : public UProperty
{
  DECLARE_NATIVE_CLASS( UFixedArrayProperty, UProperty, CLASS_NoExport, Core )
  virtual void Load();
  virtual void GetText( char* Buf, int BufSz, UObject* Obj, int Idx, size_t DefVal = 0 );
  virtual size_t GetGenericValue( UObject* Obj, int Idx );
};

