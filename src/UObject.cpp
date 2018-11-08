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
#include "XMemory.h"
#include "UObject.h"
#include "UObjGetSet.h"
#include "UClass.h"
#include "UProperty.h"
#include "UPackage.h"
#include "USystem.h"

FNativePropertyList::FNativePropertyList( FHash InHash, size_t InNum )
{
  Hash = InHash;
  Num = InNum;
  Added = 0;
  Properties = new FNativePropertyLink[Num];
}

FNativePropertyList::~FNativePropertyList()
{
  if ( Properties )
    delete Properties;
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

Array<UObject*>* UObject::ObjectPool = NULL;
Array<UClass*>*  UObject::ClassPool = NULL;
Array<FNativePropertyList*>* UObject::NativePropertyLists = NULL;
Array<UFunction*>* UObject::NativeFunctions = NULL;

UObject* UObject::StaticConstructObject( const char* InName, UClass* InClass, UObject* InOuter )
{
  // Construct object with the class constructor
  UObject* Out = InClass->CreateObject();

  // Set up object properties
  Out->Hash = FnvHashString( InName );
  Out->Name = InName;
  Out->Index = ObjectPool->Size();
  Out->RefCnt = 1;
  Out->Outer = InOuter;
  Out->Flags = 0;
  Out->Class = InClass;
  Out->Field = InClass->Default == NULL ? NULL : InClass->Default->Field;

  // Add to object
  ObjectPool->PushBack( Out );

  // Script init (TODO)
  
  return Out;
}

UClass* UObject::GetUClassClass()
{
  return UClass::StaticClass();
}

UClass* UObject::StaticAllocateClass( const char* ClassName, u32 Flags, UClass* SuperClass, 
    UObject *(*NativeCtor)(size_t) )
{
  ClassName++; // We don't want the prefix indicating object type in the name of the class
  UClass* Out = new UClass( ClassName, Flags, SuperClass, NativeCtor );
  Out->Class = UClass::StaticClass();
  return Out;
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
  NextObj = NULL;
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

bool UObject::ExportToFile( const char* Dir, const char* Type )
{
  return false;
}

void UObject::LoadFromPackage( FPackageFileIn* In )
{
  if ( Flags & RF_HasStack )
  {
    // Load stack info
  }
  
  if ( Class != UClass::StaticClass() )
  {
    // Load properties
    ReadDefaultProperties( In );
  }
  
  return;
}

void UObject::AddRef()
{
  RefCnt++;
}

void UObject::DelRef()
{
  if ( UNLIKELY( RefCnt == 0 ) )
  {
    Logf( LOG_WARN, "Reference count decrement on irrelevant object '%s'", Name );
    return;
  }

  RefCnt--;
  if ( RefCnt == 0 && GSystem->bLogRefCntZero )
    Logf( LOG_INFO, "Reference count is zero for '%s'", Name );
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
    if ( UNLIKELY( Cls->StaticClass() != UClass::StaticClass() ) )
    {
      Logf( LOG_CRIT, "CLASS SUPERFIELD IS NOT A UCLASS INSTANCE!!!" );
      // Add some exit thing here
    }

    if ( Cls == ClassType )
      return true;
  }
  return false;
}

static int ReadArrayIndex( FPackageFileIn* In )
{
  u8 ArrayIdx[4] = { 0, 0 ,0 ,0 };
  *In >> ArrayIdx[0];
  if ( ArrayIdx[0] >= 128 )
  {
    *In >> ArrayIdx[1];
    if ( (ArrayIdx[1] & 0x80) != 0 && *((u16*)&ArrayIdx[0]) >= 16384 )
    {
      *In >> ArrayIdx[2];
      *In >> ArrayIdx[3];
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

// TODO: Print out full names of objects for when properties don't exist
void UObject::ReadDefaultProperties( FPackageFileIn* In )
{
  const char* PropName = NULL;
  idx PropNameIdx = 0;
  u8  InfoByte = 0;
  u8  PropType = 0;
  u8  SizeByte = 0;
  u8  IsArray  = 0;
  int ArrayIdx = 0;
  int RealSize = 0;
  bool bIsDescription = false;

  while( 1 )
  {
    *In >> CINDEX( PropNameIdx );
    PropName = Pkg->ResolveNameFromIdx( PropNameIdx );
    if ( UNLIKELY( strncmp( PropName, "None", 4 ) == 0 ) )
      break;

    UProperty* Prop = NULL;
    bIsDescription = false;

    // Support for 227j property descriptions
    if ( strnicmp( PropName, "Description", 11 ) == 0 && IsA( UProperty::StaticClass() ) )
    {
      bIsDescription = true;
    }
    else
    {
      Prop = FindProperty( PropName );

      // Read the actual value of the properties even if they don't exist
      // We don't want to totally fail if some property just doesn't exist
      // Instead, the value will just go unused (because nothing is there to use it)
      if ( !Prop )
      {
        if ( Outer )
          Logf( LOG_CRIT, "Property '%s' in '%s.%s.%s' does not exist",
            PropName, Pkg->Name, Outer->Name, Name );
        else
          Logf( LOG_CRIT, "Property '%s' in '%s.%s' does not exist",
            PropName, Pkg->Name, Name );
      }

      else if ( Prop->Offset == MAX_UINT32 )
      {
        if ( Outer )
          Logf( LOG_WARN, "Property '%s' in '%s.%s.%s' has no native component",
              PropName, Pkg->Name, Outer->Name, Name );
        else
          Logf( LOG_WARN, "Property '%s' in '%s.%s' has no native component",
              PropName, Pkg->Name, Name );

        Prop = NULL;
      }
    }

    *In >> InfoByte;
    PropType = InfoByte & 0x0F;
    SizeByte = (InfoByte & 0x70) >> 4;
    IsArray  = InfoByte & 0x80;

    if ( SizeByte > 4 && PropType != PROP_Struct )
    {
      u8 Size8;
      u16 Size16;
      switch( SizeByte )
      {
        case 5:
          *In >> Size8;
          RealSize = Size8;
          break;
        case 6:
          *In >> Size16;
          RealSize = Size16;
          break;
        case 7:
          *In >> RealSize;
          break;
      }
    }
    else
    {
      RealSize = UProperty::PropertySizes[SizeByte];
    }
    
    if ( IsArray && PropType != PROP_Bool )
      ArrayIdx = ReadArrayIndex( In );

    if ( PropType == PROP_Byte )
    {
      u8 Value = 0;
      *In >> Value;

      if ( Prop )
      {
        UByteProperty* ByteProp = SafeCast<UByteProperty>( Prop );
        if ( !ByteProp )
          Logf( LOG_CRIT, "Default property expected 'ByteProperty', but got '%s'", Prop->Class->Name );
        else
          SetByteProperty( ByteProp, Value, ArrayIdx );
      }
    }
    else if ( PropType == PROP_Int )
    {
      int Value = 0;
      *In >> Value;
     
      if ( Prop )
      {
        UIntProperty* IntProp = SafeCast<UIntProperty>( Prop );
        if ( !IntProp )
          Logf( LOG_CRIT, "Default property expected 'IntProperty', but got '%s'", Prop->Class->Name );
        else
          SetIntProperty( IntProp, Value, ArrayIdx );
      }
    }
    else if ( PropType == PROP_Bool )
    {
      if ( Prop )
      {
        UBoolProperty* BoolProp = SafeCast<UBoolProperty>( Prop );
        if ( !BoolProp )
          Logf( LOG_CRIT, "Default property expected 'BoolProperty', but got '%s'", Prop->Class->Name );
        else
          SetBoolProperty( BoolProp, IsArray == 1 );
      }
    }
    else if ( PropType == PROP_Float )
    {
      float Value = 0;
      *In >> Value;

      if ( Prop )
      {
        UFloatProperty* FloatProp = SafeCast<UFloatProperty>( Prop );
        if ( !FloatProp )
          Logf( LOG_CRIT, "Default property expected 'FloatProperty', but got '%s'", Prop->Class->Name );
        else
          SetFloatProperty( FloatProp, Value, ArrayIdx );
      }
    }
    else if ( PropType == PROP_Object )
    {
      idx ObjRef = 0;
      *In >> CINDEX( ObjRef );
 
      if ( Prop )
      {
        UObjectProperty* ObjProp = SafeCast<UObjectProperty>( Prop );
        if ( !ObjProp )
          Logf( LOG_CRIT, "Default property expected 'ObjectProperty', but got '%s'", Prop->Class->Name );
        else
          SetObjProperty( ObjProp, UPackage::StaticLoadObject( Pkg, ObjRef ), ArrayIdx );
      }
    }
    else if ( PropType == PROP_Name )
    {
      idx Name = 0;
      *In >> CINDEX( Name );

      if ( Prop )
      {
        UNameProperty* NameProp = SafeCast<UNameProperty>( Prop );
        if ( !NameProp )
          Logf( LOG_CRIT, "Default property expected 'NameProperty', but got '%s'", Prop->Class->Name );
        else    
          SetNameProperty( NameProp, Name, ArrayIdx );
      }
    }
    else if ( PropType == PROP_String )
    {
      // TODO: Crash here
      Logf( LOG_WARN, "StringProperty loading in UObject::ReadDefaultProperties() is stubbed" );
      Logf( LOG_WARN, "  Info: (Package = '%s', Class = '%s', Object = '%s', Property = '%s')",
            Pkg->Name, Class->Name, Name, Prop->Name );
    }
    else if ( PropType == PROP_Class )
    {
      idx ObjRef = 0;
      *In >> CINDEX( ObjRef );

      if ( Prop )
      {
        UClassProperty* ClassProp = SafeCast<UClassProperty>( Prop );
        if ( !ClassProp )
          Logf( LOG_CRIT, "Default property expected 'ClassProperty', but got '%s'", Prop->Class->Name );
        else
          SetObjProperty( ClassProp, UPackage::StaticLoadObject( Pkg, ObjRef, UClass::StaticClass() ), ArrayIdx );
      }
    }
    else if ( PropType == PROP_Array )
    {
      // TODO: Crash here
      Logf( LOG_WARN, "ArrayProperty loading in UObject::ReadDefaultProperties() is stubbed" );
      Logf( LOG_WARN, "  Info: (Package = '%s', Class = '%s', Object = '%s', Property = '%s')",
            Pkg->Name, Class->Name, Name, Prop->Name );
    }
    else if ( PropType == PROP_Struct )
    {
      // TODO: Any way to get around this?
      //
      // If we don't have a StructProperty, then we're really screwed since
      // structs can hold any amount of data and we won't know how much to seek
      // ahead. We can't use the size reported by the list entry because
      // that only accounts for final size, not the number of bytes to read
      if ( !Prop )
      {
        Logf( LOG_CRIT, "Property does not exist, but property type is a struct." );
        Logf( LOG_CRIT, "Cannot continue parsing defaultproperty list.");
        return;
      }

      UStructProperty* StructProp = SafeCast<UStructProperty>( Prop );
      if ( !StructProp )
      {
        Logf( LOG_CRIT, "Default property expected 'StructProperty', but got '%s'", Prop->Class->Name );
        Logf( LOG_CRIT, "Cannot continue parsing defaultproperty list.");
        return;
      }

      // Verify struct name
      idx StructName = 0;
      *In >> CINDEX( StructName );
      
      if ( StructName < 0 )
        Logf( LOG_CRIT, "Bad struct name index for StructProperty '%s'", Prop->Name );

      FHash StructHash = Pkg->GetNameEntry( StructName )->Hash;
      if ( StructProp && StructProp->Struct->Hash != StructHash )
      {
        Logf( LOG_CRIT, "Default property expected struct type '%s', but got '%s'",
              Prop->Class->Name, Pkg->ResolveNameFromIdx( StructName ) );
        Logf( LOG_CRIT, "Cannot continue parsing defaultproperty list.");
        return;
      }

      // Verify struct size
      int StructSize = 0;
      if ( SizeByte > 4 )
      {
        switch( SizeByte )
        {
          case 5:
            *In >> *(u8*)&StructSize;
            break;
          case 6:
            *In >> *(u16*)&StructSize;
            break;
          case 7:
            *In >> StructSize;
            break;
        }
      }
      else
      {
        StructSize = UProperty::PropertySizes[SizeByte];
      }

      if ( Prop && StructProp )
      {
        if ( StructSize != StructProp->Struct->StructSize )
        {
          Logf( LOG_CRIT, "Struct size of default property '%i' does not match actual size '%i'", 
              StructSize, StructProp->Struct->StructSize );
          Logf( LOG_CRIT, "Cannot continue parsing defaultproperty list.");
          return;
        }
        else
        {
          // Read in struct properties based on our definition that we have loaded
          SetStructProperty( StructProp, In, ArrayIdx );
        }
      }
    }
    else if ( PropType == PROP_Ascii )
    {
      idx StrLength = 0;
      *In >> CINDEX( StrLength );

      char* NewStr = new char[StrLength]; // Serialized length includes null terminator
      In->Read( NewStr, StrLength );

      if ( Prop )
      {
        UStrProperty* StrProp = SafeCast<UStrProperty>( Prop );
        if ( !StrProp )
          Logf( LOG_CRIT, "Default property expected 'StrProperty', but got '%s'", Prop->Class->Name );
        else
          SetStrProperty( StrProp, NewStr, ArrayIdx );
      }

      if ( bIsDescription )
      {
        UProperty* PropThis = (UProperty*)this;
        PropThis->Description = StringDup( NewStr );
      }
    }
  }
}

void UObject::ReadConfigProperties()
{
  for( UField* FieldIter = Field; FieldIter != NULL; FieldIter = FieldIter->Next )
  {
    UProperty* Prop = SafeCast<UProperty>( FieldIter );
    if ( Prop )
    {
      FConfig* Cfg;
      String* Category = new String();
      if ( Prop->PropertyFlags & CPF_GlobalConfig )
      {
        Cfg = Prop->GlobalClass->ClassConfig;
        *Category += Prop->GlobalClass->Pkg->Name;
        *Category += ".";
        *Category += Prop->GlobalClass->Name;
      }
      else if ( Prop->PropertyFlags & CPF_Config )
      {
        Cfg = Class->ClassConfig;
        *Category += Pkg->Name;
        *Category += ".";
        *Category += Name;
      }
      const char* Variable = Prop->Name;

      
      for( int i = 0; i < Prop->ArrayDim; i++ )
      {
        if ( Prop->Class == UByteProperty::StaticClass() )
          SetByteProperty( (UByteProperty*)Prop, Cfg->ReadUInt8( Category->Data(), Variable, i ), i );

        else if ( Prop->Class == UIntProperty::StaticClass() )
          SetIntProperty( (UIntProperty*)Prop, Cfg->ReadInt32( Category->Data(), Variable, i ), i );

        else if ( Prop->Class == UBoolProperty::StaticClass() )
          SetBoolProperty( (UBoolProperty*)Prop, Cfg->ReadBool( Category->Data(), Variable ) );

        else if ( Prop->Class == UFloatProperty::StaticClass() )
          SetFloatProperty( (UFloatProperty*)Prop, Cfg->ReadFloat( Category->Data(), Variable, i ), i );

        else if ( Prop->Class == UObjectProperty::StaticClass() )
        {
          /*UObject* ObjProp = GetObjProperty( (UObjectProperty*)Prop, i );
          if ( ObjProp == NULL )
            ObjProp = StaticConstructObject( 
          SetObjectProperty( (UObjectProperty*)Prop, Cfg->ReadUInt8( Category->Data(), Variable, i ), i );*/
          Logf( LOG_WARN, "Found an object property, look at the class in ued/utpt and figure out how to handle this" );
          GSystem->Exit( -1 );
        }

        else if ( Prop->Class == UClassProperty::StaticClass() )
        {
          Logf( LOG_WARN, "Found a class property, look at the class in ued/utpt and figure out how to handle this" );
          GSystem->Exit( -1 );
        }

        else if ( Prop->Class == UArrayProperty::StaticClass() )
        {
          Logf( LOG_WARN, "Found an array property, look at the class in ued/utpt and figure out how to handle this" );
          GSystem->Exit( -1 );
        }

        else if ( Prop->Class == UStructProperty::StaticClass() )
        {
          UStructProperty* StructProp = (UStructProperty*)Prop;
          Cfg->ReadStruct( Category->Data(), Variable, StructProp->Struct, 
              GetStructProperty( StructProp, i ), i );
        }

        else if ( Prop->Class == UStrProperty::StaticClass() )
          SetStrProperty( (UStrProperty*)Prop, Cfg->ReadString( Category->Data(), Variable, i ), i );
        
        else if ( Prop->Class == UMapProperty::StaticClass() )
        {
          Logf( LOG_WARN, "Found a map property, look at the class in ued/utpt and figure out how to handle this" );
          GSystem->Exit( -1 );
        }

        else if ( Prop->Class == UFixedArrayProperty::StaticClass() )
        {
          Logf( LOG_WARN, "Found a fixed array property, look at the class in ued/utpt and figure out how to handle this" );
          GSystem->Exit( -1 );
        }

        else if ( Prop->Class == UStringProperty::StaticClass() )
        {
          Logf( LOG_WARN, "How is this different from UStrProperty?" );
          GSystem->Exit( -1 );
        }
      }
    }
  }
}

UProperty* UObject::FindProperty( const char* PropName )
{
  FHash PropHash = FnvHashString( PropName );
  for ( UField* Iter = Field; Iter != NULL; Iter = Iter->Next )
  {
    if ( Iter->IsA( UProperty::StaticClass() ) && Iter->Hash == PropHash )
      return (UProperty*)Iter;
  }

  return NULL;
}

UCommandlet::UCommandlet()
  : UObject()
{
}

UCommandlet::~UCommandlet()
{
  // TODO:
}

int UCommandlet::Main( String* Parms )
{
  return 0;
}

