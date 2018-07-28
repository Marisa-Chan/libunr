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
 * UObject.cpp - Generic object functions
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

//#include "FLogf.h"
#include "Memory.h"
#include "UObject.h"
#include "UClass.h"
#include "UProperty.h"
#include "UPackage.h"

FNativePropertyList::FNativePropertyList( FHash InHash, size_t InNum )
{
  Hash = InHash;
  Num = InNum;
  Added = 0;
  Properties = new FNativePropertyLink[Num];
}

void FNativePropertyList::AddProperty( const char* Name, u32 Offset )
{
  if ( LIKELY( Added < Num ) )
  {
    Properties[Added].Hash = FnvHashString( Name );
    Properties[Added].Offset = Offset;
    Added++;
  }
}

bool UObject::StaticLinkNativeProperties()
{
  if ( StaticInitNativePropList( 5 ) )
  {
    LINK_NATIVE_ARRAY( UObject, ObjectInternal );
    LINK_NATIVE_PROPERTY( UObject, Outer );
    LINK_NATIVE_PROPERTY( UObject, Flags );
    LINK_NATIVE_PROPERTY( UObject, Name );
    LINK_NATIVE_PROPERTY( UObject, Class );
    return true;
  }

  return false;
}

UObject* UObject::StaticConstructObject( const char* InName, UClass* InClass, UObject* InOuter )
{
  // Construct object with the class constructor
  UObject* Out = InClass->CreateObject();

  // Set up object properties
  Out->Hash = FnvHashString( InName );
  Out->Name = InName;
  Out->Index = ObjectPool.Size();
  Out->RefCnt = 1;
  Out->Outer = InOuter;
  Out->Flags = 0;
  Out->Class = InClass;

  // Add to object
  ObjectPool.PushBack( Out );

  // Script init (TODO)
  
  return Out;
}

UClass* AllocateClass( const char* ClassName, u32 Flags, UClass* SuperClass, 
    UObject *(*NativeCtor)(size_t) )
{
  ClassName++; // We don't want the prefix indicating object type in the name of the class
  return new UClass( ClassName, Flags, SuperClass, NativeCtor );
}

FPackageFileIn& operator>>( FPackageFileIn& Ar, UObject& Obj )
{
  Obj.LoadFromPackage( Ar );
  return Ar;
}

FPackageFileOut& operator<<( FPackageFileOut& Ar, UObject& Obj )
{
  return Ar;
}

UObject::UObject()
{
  Index = -1;
  ExpIdx = -1;
  NameIdx = -1;
  Flags = 0;
  Outer = NULL;
  Class = NULL;
  Pkg = NULL;
  RefCnt = 1;
}

//TODO: write destructor
UObject::~UObject()
{
}

bool UObject::ExportToFile()
{
  return false;
}

void UObject::LoadFromPackage( FPackageFileIn& Ar )
{
  if ( Flags & RF_HasStack )
  {
    // Load stack info
  }
  
  if ( !ObjectClass->IsA( UClass::StaticClass() ) )
  {
    // Load properties
    ReadDefaultProperties( Ar );
  }
  
  return;
}

void UObject::SetPkgProperties( UPackage* InPkg, int InExpIdx, int InNameIdx )
{
  Pkg = InPkg;
  ExpIdx = InExpIdx;
  NameIdx = InNameIdx;
  Name = Pkg->ResolveNameFromIdx( NameIdx );
}

bool UObject::IsA( UClass* ClassType )
{
  for ( UClass* Cls = Class; Cls != NULL; Cls = Cls->SuperClass )
  {
    if ( Cls == ClassType )
      return true;
  }
  return false;
}

static int ReadArrayIndex( FPackageFileIn& In )
{
  u8 ArrayIdx[4];
  In >> ArrayIdx[0];
  if ( ArrayIdx[0] >= 128 )
  {
    In >> ArrayIdx[1];
    if ( (ArrayIdx[1] & 0x80) != 0 && *((u16*)&ArrayIdx[0]) >= 16384 )
    {
      In >> ArrayIdx[2];
      In >> ArrayIdx[3];
      ArrayIdx[3] &= ~0xC0;
    }
    else
    {
      ArrayIdx[1] &= ~0x80;
    }
  }

  int Idx = *((int*)&ArrayIdx);
  return Idx;
}

void UObject::ReadDefaultProperties( FPackageFileIn& In )
{
  const char* PropName = NULL;
  idx PropNameIdx = 0;
  u8  InfoByte = 0;
  u8  PropType = 0;
  u8  SizeByte = 0;
  u8  IsArray  = 0;
  int ArrayIdx = 0;
  int RealSize = 0;
  
  while( 1 )
  {
    In >> CINDEX( PropName );
    PropName = Pkg->ResolveNameFromIdx( PropNameIdx );
    if ( UNLIKELY( strncmp( PropName, "None", 4 ) == 0 ) )
      break;

    UProperty* Prop = FindProperty( PropName );

    In >> InfoByte;
    PropType = InfoByte & 0x0F;
    SizeByte = InfoByte & 0x70;
    IsArray  = InfoByte & 0x80;

    if ( SizeByte > 4 )
    {
      u8 Size8;
      u16 Size16;
      switch( SizeByte )
      {
        case 5:
          In >> Size8;
          RealSize = Size8;
          break;
        case 6:
          In >> Size16;
          RealSize = Size16;
          break;
        case 7:
          In >> RealSize;
          break;
      }
    }
    else
    {
      RealSize = UProperty::PropertySizes[SizeByte];
    }

    if ( PropType == PROP_Byte )
    {
      UByteProperty* ByteProp = SafeCast<UByteProperty>( Prop );
      if ( !ByteProp )
      {
        Logf( LOG_CRIT, "Default property expected 'ByteProperty', but got '%s'", Prop->Class->Name );
        return;
      }

      if ( IsArray )
        ArrayIdx = ReadArrayIndex( In );
    
      u8 Value = 0;
      In >> Value;
      SetByteProperty( ByteProp, Value, ArrayIdx ); 
    }
    else if ( PropType == PROP_Int )
    {
      UIntProperty* IntProp = SafeCast<UIntProperty>( Prop );
      if ( !IntProp )
      {
        Logf( LOG_CRIT, "Default property expected 'IntProperty', but got '%s'", Prop->Class->Name );
        return;
      }

      if ( IsArray )
        ArrayIdx = ReadArrayIndex( In );

      int Value = 0;
      In >> Value;
      SetIntProperty( IntProp, Value, ArrayIdx );
    }
    else if ( PropType == PROP_Bool )
    {
      UBoolProperty* BoolProp = SafeCast<UBoolProperty>( Prop );
      if ( !BoolProp )
      {
        Logf( LOG_CRIT, "Default property expected 'BoolProperty', but got '%s'", Prop->Class->Name );
        return;
      }

      SetBoolProperty( BoolProp, IsArray == 1 );
    }
    else if ( PropType == PROP_Float )
    {
      UFloatProperty* FloatProp = SafeCast<UFloatProperty>( Prop );
      if ( !FloatProp )
      {
        Logf( LOG_CRIT, "Default property expected 'FloatProperty', but got '%s'", Prop->Class->Name );
        return;
      }

      if ( IsArray )
        ArrayIdx = ReadArrayIndex( In );

      float Value = 0;
      In >> Value;
      SetFloatProperty( FloatProp, Value, ArrayIdx );
    }
    else if ( PropType == PROP_Object )
    {
      UObjectProperty* ObjProp = SafeCast<UObjectProperty>( Prop );
      if ( !ObjProp )
      {
        Logf( LOG_CRIT, "Default property expected 'ObjectProperty', but got '%s'", Prop->Class->Name );
        return;
      }

      if ( IsArray )
        ArrayIdx = ReadArrayIndex( In );

      idx ObjRef = 0;
      In >> CINDEX( ObjRef );
      SetObjProperty( ObjProp, UPackage::StaticLoadObject( Pkg, ObjRef ), ArrayIdx );
    }
  }
}
