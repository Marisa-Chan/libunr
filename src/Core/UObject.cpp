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
#include "Core/UObject.h"
#include "Core/UObjGetSet.h"
#include "Core/UClass.h"
#include "Core/UProperty.h"
#include "Core/UPackage.h"
#include "Core/USystem.h"

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
    char* UpperName = strupper( Name );
    Properties[Added].Hash = FnvHashString( UpperName );
    Properties[Added].Offset = Offset;
    Added++;
    xstl::Free( UpperName );
  }
}

Array<UObject*>* UObject::ObjectPool = NULL;
Array<UClass*>*  UObject::ClassPool = NULL;
Array<FNativePropertyList*>* UObject::NativePropertyLists = NULL;
Array<UFunction*>* UObject::NativeFunctions = NULL;

IMPLEMENT_NATIVE_CLASS( UObject );
IMPLEMENT_NATIVE_CLASS( UCommandlet );

UObject* UObject::StaticConstructObject( const char* InName, UClass* InClass, UObject* InOuter, 
  UPackage* InPkg, FExport* InExport )
{
  // Construct object with the class constructor
  UObject* Out = InClass->CreateObject();

  // Set up object properties
  Out->Pkg = InPkg;
  Out->Export = InExport;
  Out->NameIdx = InExport->ObjectName;
  Out->Hash = FnvHashString( InName );
  Out->Name = InName;
  Out->Index = ObjectPool->Size();
  Out->RefCnt = 1;
  Out->Outer = InOuter;
  Out->Flags = InExport->ObjectFlags;
  Out->Class = InClass;
  Out->Field = InClass->Children;
 
  // Copy property data from Default object
  if ( !(InClass->ClassFlags & CLASS_NoExport) )
  {
    if ( UNLIKELY( InClass->Default == NULL ) )
    {
      Logf( LOG_CRIT, "Can't construct object '%s.%s'; Default object missing for class '%s'",
          InPkg->Name, InName, InClass->Name );
      GSystem->Exit( -1 );
    } 
  }

  // Add to object
  ObjectPool->PushBack( Out );

  // Script init (TODO)
  
  return Out;
}

UClass* UObject::StaticAllocateClass( const char* ClassName, u32 Flags, UClass* SuperClass, 
    size_t InStructSize, UObject *(*NativeCtor)(size_t) )
{
  ClassName++; // We don't want the prefix indicating object type in the name of the class
  UClass* Out = new UClass( ClassName, Flags, SuperClass, InStructSize, NativeCtor );
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
  Export = NULL;
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

void UObject::PreLoad()
{
  PkgFile = Pkg->GetStream();
  OldPkgFileOffset = PkgFile->Tell();
  PkgFile->Seek( Export->SerialOffset, Begin );
  Export->bNeedsFullLoad = false;
  bLoading = true;
}

void UObject::Load()
{
  if ( Flags & RF_HasStack )
  {
    // Load stack info
    idx Node;
    idx StateNode;
    u64 ProbeMask;
    u32 LatentAction;
    idx Off;

    *PkgFile >> CINDEX( Node );
    *PkgFile >> CINDEX( StateNode );
    *PkgFile >> ProbeMask;
    *PkgFile >> LatentAction;

    if ( Node != 0 )
      *PkgFile >> CINDEX( Off );
  }
 
  // TODO: For UE2, Classes DO have a property list
  if ( Class != UClass::StaticClass() )
  {
    // Load properties
    ReadDefaultProperties();
  }
}

void UObject::PostLoad()
{
  PkgFile->Seek( OldPkgFileOffset, Begin );
  OldPkgFileOffset = 0;
  PkgFile = NULL;
  bLoading = false;
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

bool UObject::IsA( UClass* ClassType )
{
  for ( UClass* Cls = Class; Cls != NULL; Cls = Cls->SuperClass )
  {
    if ( UNLIKELY( Cls->Class != UClass::StaticClass() ) )
    {
      Logf( LOG_CRIT, "CLASS SUPERFIELD IS NOT A UCLASS INSTANCE!!!" );
      GSystem->Exit( -1 );
    }

    if ( Cls == ClassType )
      return true;
  }
  return false;
}

bool UObject::IsA( char* ClassName, FHash& ClassHash )
{
  if ( strnicmp( ClassName, "None", 4 ) == 0 )
    ClassHash = FnvHashString( "Class" );

  UClass* Cls = FindClass( ClassHash );
  if ( Cls != NULL )
    return IsA( Cls );

  return false;
}

bool UObject::ParentsIsA( UClass* ClassType )
{
  for ( UObject* Parent = Outer; Parent != NULL; Parent = Parent->Outer )
  {
    if ( Parent->IsA( ClassType ) )
      return true;
  }

  return false;
}

UClass* UObject::FindClass( FHash& ClassHash )
{
  if ( ClassHash == FnvHashString("None") )
    return UClass::StaticClass();

  for ( size_t i = 0; i < ClassPool->Size() && i < MAX_SIZE; i++ )
  {
    UClass* Cls = (*ClassPool)[i];
    if ( Cls->Hash == ClassHash )
      return Cls;
  }

  return NULL;
}

static int ReadArrayIndex( FPackageFileIn* PkgFile )
{
  u8 ArrayIdx[4] = { 0, 0 ,0 ,0 };
  *PkgFile >> ArrayIdx[0];
  if ( ArrayIdx[0] >= 128 )
  {
    *PkgFile >> ArrayIdx[1];
    if ( (ArrayIdx[1] & 0x80) != 0 && *((u16*)&ArrayIdx[0]) >= 16384 )
    {
      *PkgFile >> ArrayIdx[2];
      *PkgFile >> ArrayIdx[3];
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
void UObject::ReadDefaultProperties()
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

  // For script exporting
  Class->DefPropListOffset = PkgFile->Tell();

  while( 1 )
  {
    *PkgFile >> CINDEX( PropNameIdx );
    PropName = Pkg->ResolveNameFromIdx( PropNameIdx );
    if ( UNLIKELY( strncmp( PropName, "None", 4 ) == 0 ) )
      break;

    UProperty* Prop = NULL;
    ArrayIdx = 0;
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

    *PkgFile >> InfoByte;
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
          *PkgFile >> Size8;
          RealSize = Size8;
          break;
        case 6:
          *PkgFile >> Size16;
          RealSize = Size16;
          break;
        case 7:
          *PkgFile >> RealSize;
          break;
      }
    }
    else
    {
      RealSize = UProperty::PropertySizes[SizeByte];
    }
    
    if ( IsArray && PropType != PROP_Bool && PropType != PROP_Struct )
      ArrayIdx = ReadArrayIndex( PkgFile );

    if ( PropType == PROP_Byte )
    {
      u8 Value = 0;
      *PkgFile >> Value;

      if ( Prop )
      {
        if ( Prop->Class != UByteProperty::StaticClass() )
          Logf( LOG_CRIT, "Default property expected 'ByteProperty', but got '%s'", 
            Prop->Class->Name );
        else
          SetProperty<u8>( Prop, Value, ArrayIdx );
      }
    }
    else if ( PropType == PROP_Int )
    {
      int Value = 0;
      *PkgFile >> Value;
     
      if ( Prop )
      {
        if ( Prop->Class != UIntProperty::StaticClass() )
          Logf( LOG_CRIT, "Default property expected 'IntProperty', but got '%s'", Prop->Class->Name );
        else
          SetProperty<int>( Prop, Value, ArrayIdx );
      }
    }
    else if ( PropType == PROP_Bool )
    {
      if ( Prop )
      {
        if ( Prop->Class != UBoolProperty::StaticClass() )
          Logf( LOG_CRIT, "Default property expected 'BoolProperty', but got '%s'", Prop->Class->Name );
        else
          SetProperty<bool>( Prop, IsArray == 1 );
      }
    }
    else if ( PropType == PROP_Float )
    {
      float Value = 0;
      *PkgFile >> Value;

      if ( Prop )
      {
        if ( Prop->Class != UFloatProperty::StaticClass() )
          Logf( LOG_CRIT, "Default property expected 'FloatProperty', but got '%s'", Prop->Class->Name );
        else
          SetProperty<float>( Prop, Value, ArrayIdx );
      }
    }
    else if ( PropType == PROP_Object )
    {
      idx ObjRef = 0;
      *PkgFile >> CINDEX( ObjRef );
 
      if ( Prop )
      {
        UObjectProperty* ObjProp = SafeCast<UObjectProperty>( Prop );
        if ( !ObjProp )
          Logf( LOG_CRIT, "Default property expected 'ObjectProperty', but got '%s'", 
            Prop->Class->Name );
        else
          SetProperty<UObject*>( ObjProp, LoadObject( ObjRef, ObjProp->ObjectType, NULL ), 
            ArrayIdx );
      }
    }
    else if ( PropType == PROP_Name )
    {
      idx Name = 0;
      *PkgFile >> CINDEX( Name );

      if ( Prop )
      {
        if ( Prop->Class != UNameProperty::StaticClass() )
          Logf( LOG_CRIT, "Default property expected 'NameProperty', but got '%s'", Prop->Class->Name );
        else    
          SetProperty<idx>( Prop, Name, ArrayIdx );
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
      *PkgFile >> CINDEX( ObjRef );

      if ( Prop )
      {
        if ( Prop->Class != UClassProperty::StaticClass() )
          Logf( LOG_CRIT, "Default property expected 'ClassProperty', but got '%s'", 
            Prop->Class->Name );
        else
          SetProperty<UObject*>( Prop, LoadObject( ObjRef, UClass::StaticClass(), NULL ), 
            ArrayIdx );
      }
    }
    else if ( PropType == PROP_Array )
    {
      if ( !Prop )
      {
        Logf( LOG_CRIT, "Property does not exist, but property type is array" );
        goto err;
      }

      UArrayProperty* ArrayProp = SafeCast<UArrayProperty>( Prop );
      if ( !ArrayProp )
      {
        Logf( LOG_CRIT, "Default property expected 'ArrayProperty', but got '%s'", Prop->Class->Name );
        goto err;
      }

      u8 NumElem  = 0;
      *PkgFile >> NumElem;

      // I reeeeeeally hate not using switch/case here
      if ( ArrayProp->Inner->Class == UByteProperty::StaticClass() )
      {
        *GetArrayPropAddr<u8>( this, ArrayProp, ArrayIdx ) = new Array<u8>();
        SetArrayProperty<u8>( ArrayProp, PkgFile, ArrayIdx, RealSize, NumElem );
      }
      else if ( ArrayProp->Inner->Class == UIntProperty::StaticClass() )
      {
        *GetArrayPropAddr<int>( this, ArrayProp, ArrayIdx ) = new Array<int>();
        SetArrayProperty<int>( ArrayProp, PkgFile, ArrayIdx, RealSize, NumElem );
      }
      else if ( ArrayProp->Inner->Class == UIntProperty::StaticClass() )
      {
        *GetArrayPropAddr<bool>( this, ArrayProp, ArrayIdx ) = new Array<bool>();
        SetArrayProperty<bool>( ArrayProp, PkgFile, ArrayIdx, RealSize, NumElem );
      }
      else if ( ArrayProp->Inner->Class == UFloatProperty::StaticClass() )
      {
        *GetArrayPropAddr<float>( this, ArrayProp, ArrayIdx ) = new Array<float>();
        SetArrayProperty<float>( ArrayProp, PkgFile, ArrayIdx, RealSize, NumElem );
      }
      else if ( ArrayProp->Inner->Class == UObjectProperty::StaticClass() )
      {
        Logf( LOG_WARN, "Default ArrayProperty contains Objects" );
      }
      else if ( ArrayProp->Inner->Class == UNameProperty::StaticClass() )
      {
        *GetArrayPropAddr<idx>( this, ArrayProp, ArrayIdx ) = new Array<idx>();
        SetArrayProperty<idx>( ArrayProp, PkgFile, ArrayIdx, RealSize, NumElem );
      }
      else if ( ArrayProp->Inner->Class == UStringProperty::StaticClass() )
      {
        Logf( LOG_WARN, "Default ArrayProperty contains Strings (but not ascii???)" );
      }
      else if ( ArrayProp->Inner->Class == UClassProperty::StaticClass() )
      {
        Logf( LOG_WARN, "Default ArrayProperty contains Classes" );
      }
      else if ( ArrayProp->Inner->Class == UArrayProperty::StaticClass() )
      {
        Logf( LOG_WARN, "Default ArrayProperty contains Arrays" );
      }
      else if ( ArrayProp->Inner->Class == UStructProperty::StaticClass() )
      {
        Logf( LOG_WARN, "Default ArrayProperty contains Structs" );
      }
      else if ( ArrayProp->Inner->Class == UStrProperty::StaticClass() )
      {
        *GetArrayPropAddr<String*>( this, ArrayProp, ArrayIdx ) = new Array<String*>();
        SetArrayProperty<String*>( ArrayProp, PkgFile, ArrayIdx, RealSize, NumElem );
      }
      else if ( ArrayProp->Inner->Class == UMapProperty::StaticClass() )
      {      
        Logf( LOG_WARN, "Default ArrayProperty contains Maps" );
      }
      else if ( ArrayProp->Inner->Class == UFixedArrayProperty::StaticClass() )
      {
        Logf( LOG_WARN, "Default ArrayProperty contains FixedArrays" );
      }
/*    else if ( ArrayProp->Inner->Class == ULongProperty::StaticClass() )
      {
        *GetArrayPropAddr<i64>( this, ArrayProp, ArrayIdx ) = new Array<i64>();
        SetArrayProperty<i64>( ArrayProp, PkgFile, ArrayIdx, ByteSize, NumElem );
      }*/
      else
      {
        Logf( LOG_CRIT, "Default ArrayProperty contains unknown type '%x'", PropType );
      }
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
        goto err;
      }

      UStructProperty* StructProp = SafeCast<UStructProperty>( Prop );
      if ( !StructProp )
      {
        Logf( LOG_CRIT, "Default property expected 'StructProperty', but got '%s'", Prop->Class->Name );
        goto err;
      }

      // Verify struct name
      idx StructName = 0;
      *PkgFile >> CINDEX( StructName );
      
      if ( StructName < 0 )
        Logf( LOG_CRIT, "Bad struct name index for StructProperty '%s'", Prop->Name );

      FHash StructHash = Pkg->GetNameEntry( StructName )->Hash;
      if ( StructProp && StructProp->Struct->Hash != StructHash )
      {
        Logf( LOG_CRIT, "Default property expected struct type '%s', but got '%s'",
              StructProp->Struct->Name, Pkg->ResolveNameFromIdx( StructName ) );
        goto err;
      }

      // Verify struct size
      int StructSize = 0;
      if ( SizeByte > 4 )
      {
        switch( SizeByte )
        {
          case 5:
            *PkgFile >> *(u8*)&StructSize;
            break;
          case 6:
            *PkgFile >> *(u16*)&StructSize;
            break;
          case 7:
            *PkgFile >> StructSize;
            break;
        }
      }
      else
      {
        StructSize = UProperty::PropertySizes[SizeByte];
      }

      if ( IsArray )
        ArrayIdx = ReadArrayIndex( PkgFile );


      if ( StructProp )
      {
        if ( StructSize > StructProp->Struct->StructSize )
        {
          Logf( LOG_CRIT, "StructProperty '%s' struct size too large (Expected %i, got %i)", 
              PropName, StructProp->Struct->StructSize, StructSize );
          goto err;
        }
        // Read in struct properties based on our definition that we have loaded
        SetStructProperty( StructProp, PkgFile, ArrayIdx );
      }
    }
    else if ( PropType == PROP_Ascii )
    {
      idx StrLength = 0;
      *PkgFile >> CINDEX( StrLength );

      char* NewStr = new char[StrLength]; // Serialized length includes null terminator
      PkgFile->Read( NewStr, StrLength );

      if ( Prop )
      {
        if ( Prop->Class != UStrProperty::StaticClass() )
          Logf( LOG_CRIT, "Default property expected 'StrProperty', but got '%s'", Prop->Class->Name );
        else
          SetProperty<char*>( Prop, NewStr, ArrayIdx );
      }

      if ( bIsDescription )
      {
        UProperty* PropThis = (UProperty*)this;
        PropThis->Description = StringDup( NewStr );
      }
    }
  }

  return;

err:
  Logf( LOG_CRIT, "Cannot continue parsing defaultproperty list" );
}

void UObject::ReadConfigProperties()
{
  for( UField* FieldIter = Field; FieldIter != NULL; FieldIter = FieldIter->Next )
  {
    UProperty* Prop = SafeCast<UProperty>( FieldIter );
    if ( Prop )
    {
      if ( Prop->Offset == MAX_UINT32 )
      {
        if ( Outer )
          Logf( LOG_WARN, "Property '%s' in '%s.%s.%s' has no native component",
              Prop->Name, Pkg->Name, Outer->Name, Name );
        else
          Logf( LOG_WARN, "Property '%s' in '%s.%s' has no native component",
              Prop->Name, Pkg->Name, Name );

        Prop = NULL;
        continue;
      }

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
      else
      {
        continue;
      }
      const char* Variable = Prop->Name;

      for( int i = 0; i < Prop->ArrayDim; i++ )
      {
        if ( Prop->Class == UByteProperty::StaticClass() )
          SetProperty<u8>( (UByteProperty*)Prop, Cfg->ReadUInt8( Category->Data(), Variable, i ), i );

        else if ( Prop->Class == UIntProperty::StaticClass() )
          SetProperty<int>( (UIntProperty*)Prop, Cfg->ReadInt32( Category->Data(), Variable, i ), i );

        else if ( Prop->Class == UBoolProperty::StaticClass() )
          SetProperty<bool>( (UBoolProperty*)Prop, Cfg->ReadBool( Category->Data(), Variable ) );

        else if ( Prop->Class == UFloatProperty::StaticClass() )
          SetProperty<float>( (UFloatProperty*)Prop, Cfg->ReadFloat( Category->Data(), Variable, i ), i );

        else if ( Prop->Class == UObjectProperty::StaticClass() )
        {
          /*UObject* ObjProp = GetObjProperty( (UObjectProperty*)Prop, i );
          if ( ObjProp == NULL )
            ObjProp = StaticConstructObject( 
          SetObjectProperty( (UObjectProperty*)Prop, Cfg->ReadUInt8( Category->Data(), Variable, i ), i );*/
          Logf( LOG_WARN, "Found an object property, look at '%s'.ini", Cfg->GetName() );
          GSystem->Exit( -1 );
        }

        else if ( Prop->Class == UClassProperty::StaticClass() )
        {
          Logf( LOG_WARN, "Found a class property, look at '%s'.ini", Cfg->GetName() );
          GSystem->Exit( -1 );
        }

        else if ( Prop->Class == UArrayProperty::StaticClass() )
        {
          Logf( LOG_WARN, "Found an array property, look at '%s'.ini", Cfg->GetName() );
          GSystem->Exit( -1 );
        }

        else if ( Prop->Class == UStructProperty::StaticClass() )
        {
          UStructProperty* StructProp = (UStructProperty*)Prop;
          Cfg->ReadStruct( Category->Data(), Variable, StructProp->Struct, 
              GetProperty<UStruct*>( StructProp, i ), i );
        }

        else if ( Prop->Class == UStrProperty::StaticClass() )
          SetProperty<char*>( (UStrProperty*)Prop, Cfg->ReadString( Category->Data(), Variable, i ), i );
        
        else if ( Prop->Class == UMapProperty::StaticClass() )
        {
          Logf( LOG_WARN, "Found a map property, look at '%s'.ini", Cfg->GetName() );
          GSystem->Exit( -1 );
        }

        else if ( Prop->Class == UFixedArrayProperty::StaticClass() )
        {
          Logf( LOG_WARN, "Found a fixed array property, look at '%s'.ini", Cfg->GetName() );
          GSystem->Exit( -1 );
        }

        else if ( Prop->Class == UStringProperty::StaticClass() )
        {
          Logf( LOG_WARN, "How is this different from UStrProperty in a config?" );
          GSystem->Exit( -1 );
        }
      }
      delete Category;
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

UObject* UObject::LoadObject( idx ObjRef, UClass* ObjClass, UObject* InOuter, bool bLoadClassNow )
{
  return StaticLoadObject( Pkg, ObjRef, ObjClass, InOuter, bLoadClassNow );
}

UObject* UObject::StaticLoadObject( UPackage* Pkg, const char* ObjName, UClass* ObjClass,
  UObject* InOuter, bool bLoadClassNow )
{
  FExport* Export = Pkg->GetExportByName( Pkg->FindName( ObjName ) );
  if ( UNLIKELY( Export == NULL ) )
  {
    Logf( LOG_WARN, "Can't load object '%s.%s', object does not exist", Pkg->Name, ObjName );
    return NULL;
  }

  return StaticLoadObject( Pkg, Export, ObjClass, InOuter, bLoadClassNow );
}

UObject* UObject::StaticLoadObject( UPackage* Pkg, idx ObjRef, UClass* ObjClass, 
  UObject* InOuter, bool bLoadClassNow )
{
  FExport* ObjExport = NULL;
  UPackage* ObjPkg = NULL;
  const char* ObjName = Pkg->ResolveNameFromObjRef( ObjRef );
  const char* ObjPkgName = NULL;
  FNameEntry ClassNameEntry( "Class" );

  // Is the object reference None?
  if ( strnicmp( ObjName, "None", 4 ) == 0 )
    return NULL;

  // Is our object in this package?
  if ( ObjRef < 0 )
  {
    // We can figure out which package it should be in from our current package
    FImport* Import = &(*Pkg->GetImportTable())[ CalcObjRefValue( ObjRef ) ];
    const char* ClsName = Pkg->ResolveNameFromIdx( Import->ClassName );

    FHash ObjNameHash = FnvHashString( ObjName );
    FHash ClsNameHash = FnvHashString( ClsName );

    // Check to see if we're trying to load some intrinsic class
    if ( ObjClass == UClass::StaticClass() && ObjClass->ClassFlags & CLASS_NoExport )
    {
      // Go find it in the class pool
      for ( size_t i = 0; i < ClassPool->Size() && i != MAX_SIZE; i++ )
      {
        UClass* ClsIter = (*ClassPool)[i];
        if ( ClsIter->Hash == ObjNameHash )
        {
          // Does it need to be loaded?
          if ( !(ClsIter->ClassFlags & CLASS_NoExport) && ClsIter->NativeNeedsPkgLoad )
          {
            // Yup, load it in place
            ClsIter->PreLoad();
            ClsIter->Load();
            ClsIter->PostLoad();
          }
          return ClsIter;
        }
      }
    }

    // We'll likely need the class too
    if ( LIKELY( ObjClass == NULL ) )
    {
      for ( size_t i = 0; i < ClassPool->Size() && i != MAX_SIZE; i++ )
      {
        UClass* ClsIter = (*ClassPool)[i];
        if ( ClsIter->Hash == ClsNameHash )
        {
          ObjClass = ClsIter;
          break;
        }
      }

      if ( ObjClass == NULL )
      {
        // Alright, it's not loaded at all. Grab the package it belongs to
        const char* ClsPkgName = Pkg->ResolveNameFromIdx( Import->ClassPackage );
        UPackage* ClsPkg = UPackage::StaticLoadPackage( ClsPkgName );
        if ( ClsPkg == NULL )
        {
          // We can't find the package we need, bail out
          Logf( LOG_CRIT, "Can't load class '%s.%s', package is missing", ClsPkgName, ClsName );
          return NULL;
        }

        // Got it, now load the class
        ObjClass = (UClass*)StaticLoadObject( ClsPkg, ClsPkg->GetClassExport( ClsName ), 
            UClass::StaticClass(), NULL );

        if ( UNLIKELY( ObjClass == NULL ) )
        {
          // Class doesn't exist in that package, bail out
          Logf( LOG_CRIT, "Can't load class '%s.%s', class does not exist", ClsPkgName, ClsName );
          return NULL;
        }
      }
    }

    FImport* PkgImport = Import;
    do
    {
      // The 'Package' field for FImport does not actually tell what package it is in,
      // but what 'Group' it belongs to (which may be the package it belongs to)
      // i.e., "Core.Object" -> Package points to the import for the 'Core' package
      // "Core.Object.Color" -> Package points to the import for 'Object'
      // to get around this, we keep going back and getting the package until Import->Class 
      // points to "Package"
      PkgImport = &(*Pkg->GetImportTable())[ CalcObjRefValue( PkgImport->Package ) ]; 
    } while ( strnicmp( Pkg->ResolveNameFromIdx( PkgImport->ClassName ), "Package", 7 ) != 0 ||
              PkgImport->Package != 0 );

    ObjPkgName = Pkg->ResolveNameFromIdx( PkgImport->ObjectName );
    ObjPkg = UPackage::StaticLoadPackage( ObjPkgName );
    if ( UNLIKELY( ObjPkg == NULL ) )
    {
      Logf( LOG_CRIT, "Can't load object '%s.%s', package is missing", ObjPkgName, ObjName );
      return NULL;
    } 

    // Get the corresponding export index for this package
    Array<FExport>* Exports = ObjPkg->GetExportTable();
    for ( size_t i = 0; i < Exports->Size() && i != MAX_SIZE; i++ )
    {
      FExport* ExpIter = &(*Exports)[i];

      // Get object name entry
      FNameEntry* ExpObjName = ObjPkg->GetNameEntry( ExpIter->ObjectName );

      // Get class name entry
      FNameEntry* ExpClsName = NULL;
      if ( ExpIter->Class < 0 )
      {
        Import = &(*ObjPkg->GetImportTable())[ CalcObjRefValue( ExpIter->Class ) ];
        ExpClsName = ObjPkg->GetNameEntry( Import->ObjectName );
      }
      else
      {
        // Check if the export's class type is a class
        if ( ExpIter->Class == 0 )
          ExpClsName = &ClassNameEntry;
        else
          ExpClsName = ObjPkg->GetNameEntry( (*Exports)[ ExpIter->Class - 1 ].ObjectName );
      }

      if ( ExpObjName->Hash == ObjNameHash && ExpClsName->Hash == ClsNameHash )
      {
        ObjExport = ExpIter;
        break;
      }
    }
  }
  else
  {
    ObjPkg = Pkg;
    ObjExport = ObjPkg->GetExport( ObjRef - 1 );
  }

  if ( UNLIKELY( ObjExport == NULL ) )
  {
    // Stupid package remap stuff...
    if ( UNLIKELY( ObjPkg->Hash == FnvHashString( "UnrealI" ) ) )
    {
      ObjPkg = UPackage::StaticLoadPackage( "UnrealShare" );
      if ( UNLIKELY( ObjPkg == NULL ) )
        goto Error;

      return StaticLoadObject( ObjPkg, ObjName, ObjClass, InOuter, bLoadClassNow );
    }
    else if ( UNLIKELY( ObjPkg->Hash == FnvHashString( "UnrealShare" ) ) )
    {
      ObjPkg = UPackage::StaticLoadPackage( "UnrealI" );
      if ( UNLIKELY( ObjPkg == NULL ) )
        goto Error;

      return StaticLoadObject( ObjPkg, ObjName, ObjClass, InOuter, bLoadClassNow );
    }

  Error:
    Logf( LOG_CRIT, "Can't load object '%s.%s', object does not exist", ObjPkgName, ObjName );
    return NULL;
  }

  return StaticLoadObject( ObjPkg, ObjExport, ObjClass, InOuter, bLoadClassNow );
}

UObject* UObject::StaticLoadObject( UPackage* ObjPkg, FExport* ObjExport, UClass* ObjClass, 
  UObject* InOuter, bool bLoadClassNow )
{
  const char* ObjName = ObjPkg->ResolveNameFromIdx( ObjExport->ObjectName );
  bool bNeedsFullLoad = true;

  // 'None' object means NULL, don't load anything
  if ( strnicmp( ObjName, "None", 4 ) == 0 )
    return NULL;

  if ( ObjClass == NULL )
  {
    const char* ClsName = ObjPkg->ResolveNameFromObjRef( ObjExport->Class );
    if ( strnicmp( ClsName, "None", 4 ) == 0 )
    {
      ObjClass = UClass::StaticClass();
    }
    else
    {
      FHash ClsNameHash  = FnvHashString( ClsName );
      for ( size_t i = 0; i < ClassPool->Size() && i != MAX_SIZE; i++ )
      {
        UClass* ClsIter = (*ClassPool)[i];
        if ( ClsIter->Hash == ClsNameHash )
        {
          ObjClass = ClsIter;
          break; // Already loaded the class apparently, great
        }
      }
    }

    // If not, then we need to load it
    if ( UNLIKELY( ObjClass == NULL ) )
    {
      ObjClass = (UClass*)StaticLoadObject( ObjPkg, ObjExport->Class, UClass::StaticClass(), NULL );
      if ( UNLIKELY( ObjClass == NULL ) )
      {
        Logf( LOG_CRIT, "Can't load object '%s.%s', cannot load class", 
          ObjPkg->Name, ObjName );
        return NULL;
      }
    }
  }

  // Type checking
  FNameEntry* ClassName = ObjPkg->GetNameEntryByObjRef( ObjExport->Class );
  UClass* ClassType = FindClass( ClassName->Hash );
  
  if ( UNLIKELY( !ClassType->Default->IsA( ObjClass ) ) )
  {
    Logf( LOG_WARN, "Object '%s.%s' was expected to be of type '%s' but was '%s'",
        ObjPkg->Name, ObjName, ObjClass->Name, ClassName->Data );
    return NULL;
  }

  if ( !(ObjClass->ClassFlags & CLASS_NoExport) && ObjClass->NativeNeedsPkgLoad )
  {
    ObjClass->PreLoad();
    ObjClass->Load();
    ObjClass->PostLoad();
  }

  UObject* Obj = NULL;

  if ( ( !bLoadClassNow && ObjClass == UClass::StaticClass() ) || !ObjExport->bNeedsFullLoad ) 
    bNeedsFullLoad = false;

  // Determine if the object has already been constructed or needs constructing
  if ( ObjExport->Obj != NULL )
  {
    Obj = ObjExport->Obj;
  }
  else
  {
    // Construct the object
    Obj = StaticConstructObject( ObjName, ObjClass, InOuter, ObjPkg, ObjExport );
    if ( Obj == NULL )
      return NULL;

    // In case of circular dependencies
    ObjExport->Obj = Obj;
  }

  // If we are fully loading...
  if ( bNeedsFullLoad )
  {
    // Do a full object load
    Obj->PreLoad(); 
    Obj->Load();
    Obj->PostLoad();
  }

  return Obj;
}

int UObject::CalcObjRefValue( idx ObjRef )
{
  if ( ObjRef == 0 )
    return ObjRef;
  
  else if ( ObjRef < 0 )
    ObjRef = -ObjRef;
  
  return ObjRef - 1;
}

// Not something that should be called in the scripting environment
// Only for defaultproperty lists
void UObject::SetStructProperty( UStructProperty* Prop, FPackageFileIn* In, int Idx, u32 Offset )
{
  void* StructAddr = PtrAdd( GetProperty<UStruct*>( Prop, Idx ), Offset );

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
        *GetPropAddr<u8>( (UObject*)StructAddr, ChildProp, i++ ) = Byte;
      } while ( i < ChildProp->ArrayDim );
    }

    else if ( ChildProp->Class == UIntProperty::StaticClass() )
    {
      do
      {
        int Int = 0;
        *In >> Int;
        *GetPropAddr<int>( (UObject*)StructAddr, ChildProp, i++ ) = Int;
      } while ( i < ChildProp->ArrayDim );
    }

    else if ( ChildProp->Class == UFloatProperty::StaticClass() )
    {
      do
      {
        float Float = 0;
        *In >> Float;
        *GetPropAddr<float>( (UObject*)StructAddr, ChildProp, i++ ) = Float;
      } while ( i < ChildProp->ArrayDim );
    }

    else if ( ChildProp->Class == UBoolProperty::StaticClass() )
    {
      u8 Bool = 0;
      *In >> Bool;
      *GetPropAddr<bool>( (UObject*)StructAddr, ChildProp, 0 ) = (Bool == 1);
    }

    else if ( ChildProp->Class == UNameProperty::StaticClass() )
    {
      do
      {
        idx Name = 0;
        *In >> CINDEX( Name );
        *GetPropAddr<idx>( (UObject*)StructAddr, ChildProp, i++ ) = Name;
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

        if ( ObjRef != 0 && Obj == NULL )
        {
          Logf( LOG_CRIT, 
            "Can't load object '%s' for struct property '%s' in property list for object '%s'", 
            Prop->Pkg->ResolveNameFromObjRef( ObjRef ), Prop->Name, Prop->Outer->Name );
          return;
        }

        *GetPropAddr<UObject*>( (UObject*)StructAddr, ChildProp, i++ ) = Obj;
      } while ( i < ChildProp->ArrayDim );
    }

    else if ( ChildProp->Class == UClassProperty::StaticClass() )
    {
      do
      {
        idx ObjRef = 0;
        *In >> CINDEX( ObjRef );
        UClass* Cls = (UClass*)LoadObject( ObjRef, UClass::StaticClass(), NULL );

        if ( ObjRef != 0 && Cls == NULL )
        {
          Logf( LOG_CRIT, 
            "Can't load class '%s' for struct property '%s' in property list for object '%s'", 
            Prop->Pkg->ResolveNameFromObjRef( ObjRef ), Prop->Name, Prop->Outer->Name );
          return;
        }

        *GetPropAddr<UClass*>( (UObject*)StructAddr, ChildProp, i++ ) = Cls;
      } while ( i < ChildProp->ArrayDim );
    }

    else if ( ChildProp->Class == UStructProperty::StaticClass() )
    {
      for ( int i = 0; i < ChildProp->ArrayDim; i++ )
        SetStructProperty( (UStructProperty*)ChildProp, In, i, Prop->Offset );
    }

    else if ( ChildProp->Class == UStrProperty::StaticClass() )
    {
      do
      {
        idx StringLength = 0;
        *In >> CINDEX( StringLength );

        char* String = new char[StringLength];
        In->Read( String, StringLength );

        *GetPropAddr<char*>( (UObject*)StructAddr, ChildProp, i++ ) = String;
      } while ( i < ChildProp->ArrayDim );
    }

    else if ( ChildProp->Class != UEnum::StaticClass() )
    {
      Logf( LOG_WARN, "Unhandled case for StructProperty loading (Class = '%s')", ChildProp->Class->Name );
    }
  }
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

BEGIN_PROPERTY_LINK( UCommandlet, 14 )
  LINK_NATIVE_PROPERTY( UCommandlet, HelpCmd );
  LINK_NATIVE_PROPERTY( UCommandlet, HelpOneLiner );
  LINK_NATIVE_PROPERTY( UCommandlet, HelpUsage );
  LINK_NATIVE_PROPERTY( UCommandlet, HelpWebLink );
  LINK_NATIVE_ARRAY   ( UCommandlet, HelpParm );
  LINK_NATIVE_ARRAY   ( UCommandlet, HelpDesc );
  LINK_NATIVE_PROPERTY( UCommandlet, LogToStdout );
  LINK_NATIVE_PROPERTY( UCommandlet, IsServer );
  LINK_NATIVE_PROPERTY( UCommandlet, IsClient );
  LINK_NATIVE_PROPERTY( UCommandlet, IsEditor );
  LINK_NATIVE_PROPERTY( UCommandlet, LazyLoad );
  LINK_NATIVE_PROPERTY( UCommandlet, ShowErrorCount );
  LINK_NATIVE_PROPERTY( UCommandlet, ShowBanner );
  LINK_NATIVE_PROPERTY( UCommandlet, ForceInt );
END_PROPERTY_LINK()

