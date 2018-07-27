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
  PROP_Long    = 0x10,
};

class UProperty : public UField
{
  DECLARE_ABSTRACT_CLASS( UProperty, UField, 0, Core )
  UProperty();
  UProperty( int InNameIdx );

  u16 ArrayDim;
  u16 ElementSize;
  u32 PropertyFlags;
  const char* Category;
  u16 ReplicationOffset; // only exists since objects may store this variable
  
  // Instance variables
  u32 Offset; // Offset into the owner, giving the location of the value
  
  // Static variables
  static u8 PropertySizes[8];
  
  virtual void LoadFromPackage( FPackageFileIn& In );
  u32 GetNativeOffset( const char* ClassName, const char* PropName );
};

class UByteProperty : public UProperty
{
  DECLARE_CLASS( UByteProperty, UProperty, 0, Core )
  virtual void LoadFromPackage( FPackageFileIn& In );
  UEnum* Enum;
};

class UIntProperty : public UProperty
{
  DECLARE_CLASS( UIntProperty, UProperty, 0, Core )
  virtual void LoadFromPackage( FPackageFileIn& In );
};

class UBoolProperty : public UProperty
{
  DECLARE_CLASS( UBoolProperty, UProperty, 0, Core )
  virtual void LoadFromPackage( FPackageFileIn& In );
};

class UFloatProperty : public UProperty
{
  DECLARE_CLASS( UFloatProperty, UProperty, 0, Core )
  virtual void LoadFromPackage( FPackageFileIn& In );
};

class UObjectProperty : public UProperty
{
  DECLARE_CLASS( UObjectProperty, UProperty, 0, Core )
  UObject* GetValue( int Idx = 0 );
  void SetValue( UObject* NewVal, int Idx = 0 );
  virtual void LoadFromPackage( FPackageFileIn& In );
  UClass* ObjectType;
};

class UNameProperty : public UProperty
{
  DECLARE_CLASS( UNameProperty, UProperty, 0, Core )
  const char* GetValue( int Idx = 0 );
  void SetValue( idx NewVal, int Idx = 0 );
  virtual void LoadFromPackage( FPackageFileIn& In );
};

// ?
class UStringProperty : public UProperty
{
  DECLARE_CLASS( UStringProperty, UProperty, 0, Core )
  String* GetValue( int Idx = 0 );
  void SetValue( u8 NewVal, int Idx = 0 );
  virtual void LoadFromPackage( FPackageFileIn& In );
};

class UClassProperty : public UObjectProperty
{
  DECLARE_CLASS( UClassProperty, UProperty, 0, Core )
  UClass* GetValue( int Idx = 0 );
  void SetValue( UClass* NewVal, int Idx = 0 );
  virtual void LoadFromPackage( FPackageFileIn& In );
  UClass* Class;
};

// ?
class UArrayProperty : public UProperty
{
  DECLARE_CLASS( UArrayProperty, UProperty, 0, Core )
  virtual void LoadFromPackage( FPackageFileIn& In );
};

class UStructProperty : public UProperty
{
  DECLARE_CLASS( UStructProperty, UProperty, 0, Core )
  
  UStruct* GetValue( int Idx = 0 );
  void SetValue( UStruct* NewVal, int Idx = 0 );
  virtual void LoadFromPackage( FPackageFileIn& In );
};

class UStrProperty : public UProperty
{
  DECLARE_CLASS( UStrProperty, UProperty, 0, Core )
  const char* GetValue( int Idx = 0 );
  void SetValue( const char* NewVal, int Idx = 0 );
  virtual void LoadFromPackage( FPackageFileIn& In );
  int Length;
};

// ?
class UMapProperty : public UProperty
{
  DECLARE_CLASS( UMapProperty, UProperty, 0, Core )
  virtual void LoadFromPackage( FPackageFileIn& In );
};

// ?
class UFixedArrayProperty : public UProperty
{
  DECLARE_CLASS( UFixedArrayProperty, UProperty, 0, Core )
  virtual void LoadFromPackage( FPackageFileIn& In );
};


