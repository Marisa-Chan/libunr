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
  Out->Field = InClass->Default == NULL ? NULL : InClass->Default->Field;
 
  // Add to object
  ObjectPool->PushBack( Out );

  // Script init (TODO)
  
  return Out;
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
}

void UObject::Load()
{
  if ( Flags & RF_HasStack )
  {
    // Load stack info
  }
  
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
      // Add some exit thing here
    }

    if ( Cls == ClassType )
      return true;
  }
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

  while( 1 )
  {
    *PkgFile >> CINDEX( PropNameIdx );
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
    
    if ( IsArray && PropType != PROP_Bool )
      ArrayIdx = ReadArrayIndex( PkgFile );

    if ( PropType == PROP_Byte )
    {
      u8 Value = 0;
      *PkgFile >> Value;

      if ( Prop )
      {
        UByteProperty* ByteProp = SafeCast<UByteProperty>( Prop );
        if ( !ByteProp )
          Logf( LOG_CRIT, "Default property expected 'ByteProperty', but got '%s'", 
            Prop->Class->Name );
        else
          SetByteProperty( ByteProp, Value, ArrayIdx );
      }
    }
    else if ( PropType == PROP_Int )
    {
      int Value = 0;
      *PkgFile >> Value;
     
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
      *PkgFile >> Value;

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
      *PkgFile >> CINDEX( ObjRef );
 
      if ( Prop )
      {
        UObjectProperty* ObjProp = SafeCast<UObjectProperty>( Prop );
        if ( !ObjProp )
          Logf( LOG_CRIT, "Default property expected 'ObjectProperty', but got '%s'", 
            Prop->Class->Name );
        else
          SetObjProperty( ObjProp, LoadObject( ObjRef, ObjProp->ObjectType, NULL ), 
            ArrayIdx );
      }
    }
    else if ( PropType == PROP_Name )
    {
      idx Name = 0;
      *PkgFile >> CINDEX( Name );

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
      *PkgFile >> CINDEX( ObjRef );

      if ( Prop )
      {
        UClassProperty* ClassProp = SafeCast<UClassProperty>( Prop );
        if ( !ClassProp )
          Logf( LOG_CRIT, "Default property expected 'ClassProperty', but got '%s'", 
            Prop->Class->Name );
        else
          SetObjProperty( ClassProp, LoadObject( ObjRef, UClass::StaticClass(), NULL ), 
            ArrayIdx );
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
      *PkgFile >> CINDEX( StructName );
      
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
          SetStructProperty( StructProp, PkgFile, ArrayIdx );
        }
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

UObject* UObject::LoadObject( const char* ObjName, UClass* ObjClass, UObject* InOuter, bool bLoadClassNow )
{
  FExport* ObjExport = Pkg->GetExport( ObjName );
  if ( UNLIKELY( ObjExport == NULL ) )
  {
    Logf( LOG_CRIT, "Can't load object '%s.%s', object does not exist", Pkg->Name, ObjName );
    return NULL;
  }

  // Is it already loaded?
  if ( ObjExport->Obj != NULL && ~ObjExport->bNeedsFullLoad )
    return ObjExport->Obj;

  return StaticLoadObject( Pkg, ObjExport, ObjClass, InOuter, bLoadClassNow );
}

UObject* UObject::LoadObject( idx ObjRef, UClass* ObjClass, UObject* InOuter, bool bLoadClassNow )
{
  return StaticLoadObject( Pkg, ObjRef, ObjClass, InOuter, bLoadClassNow );
}

UObject* UObject::StaticLoadObject( UPackage* Pkg, const char* ObjName, UClass* ObjClass, UObject* InOuter, 
  bool bLoadClassNow )
{
  FExport* ObjExport = Pkg->GetExport( ObjName );
  if ( UNLIKELY( ObjExport == NULL ) )
  {
    Logf( LOG_CRIT, "Can't load object '%s.%s', object does not exist", Pkg->Name, ObjName );
    return NULL;
  }

  // Is it already loaded?
  if ( ObjExport->Obj != NULL && !ObjExport->bNeedsFullLoad )
    return ObjExport->Obj;

  return StaticLoadObject( Pkg, ObjExport, ObjClass, InOuter, bLoadClassNow );
}

UObject* UObject::StaticLoadObject( UPackage* Pkg, idx ObjRef, UClass* ObjClass, 
  UObject* InOuter, bool bLoadClassNow )
{
  FExport* ObjExport = NULL;
  UPackage* ObjPkg = NULL;
  const char* ObjName = Pkg->ResolveNameFromObjRef( ObjRef );
  const char* ObjPkgName = NULL;
  FNameEntry ClassNameEntry( "Class" );

  // Is our object in this package?
  if ( ObjRef < 0 )
  {
    // We can figure out which package it should be in from our current package
    FImport* Import = &(*Pkg->GetImportTable())[ CalcObjRefValue( ObjRef ) ];
    const char* ClsName = Pkg->ResolveNameFromIdx( Import->ClassName );

    FHash ObjNameHash = FnvHashString( ObjName );
    FHash ClsNameHash = FnvHashString( ClsName );

    // No, but check to see if we're trying to load some intrinsic class
    if ( ObjClass == UClass::StaticClass() && ObjClass->ClassFlags & CLASS_NoExport )
    {
      // Go find it in the class pool
      for ( size_t i = 0; i < ClassPool->Size() && i != MAX_SIZE; i++ )
      {
        UClass* ClsIter = (*ClassPool)[i];
        if ( ClsIter->Hash == ObjNameHash )
          return ClsIter;
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
        ObjClass = (UClass*)StaticLoadObject( ClsPkg, ClsPkg->GetExport( ClsName ), 
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
    } while ( strnicmp( Pkg->ResolveNameFromIdx( PkgImport->ClassName ), "Package", 7 ) != 0 );

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
          ExpClsName = ObjPkg->GetNameEntry( (*Exports)[ ExpIter->Class ].ObjectName );
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

