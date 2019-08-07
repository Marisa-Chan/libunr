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
 * UObject.cpp - Generic object functions
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UObject.h"
#include "Core/UObjGetSet.h"
#include "Core/UClass.h"
#include "Core/UProperty.h"
#include "Core/UPackage.h"
#include "Core/USystem.h"

/*-----------------------------------------------------------------------------
 * FNativePropertyList
-----------------------------------------------------------------------------*/
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
    free( UpperName );
  }
}

void FNativePropertyList::AppendList( FNativePropertyList* List )
{
  if ( UNLIKELY( !List ) )
    return;

  Num += List->Num;

  Properties = (FNativePropertyLink*)realloc( Properties, sizeof(FNativePropertyLink) * Num );
  memcpy( &Properties[Added], List->Properties, sizeof(FNativePropertyLink)*List->Added );

  Added += List->Added;
}

/*-----------------------------------------------------------------------------
 * UObject
-----------------------------------------------------------------------------*/
TArray<UObject*> UObject::ObjectPool;
TArray<UClass*>  UObject::ClassPool;
TArray<FNativePropertyList*> UObject::NativePropertyLists;
TArray<UFunction*> UObject::NativeFunctions;
TArray<FNameEntry*> UObject::NameTable;
bool UObject::bStaticBootstrapped = false;

UObject* UObject::StaticConstructObject( FName InName, UClass* InClass, UObject* InOuter, 
  UPackage* InPkg, FExport* InExport )
{
  // Construct object with the class constructor
  UObject* Out = InClass->CreateObject();

  // Set up object properties
  Out->Pkg = InPkg;
  Out->Export = InExport;
  Out->Name = InName;
  Out->Index = ObjectPool.Size();
  Out->RefCnt = 1;
  Out->Outer = InOuter;
  Out->ObjectFlags = InExport->ObjectFlags;
  Out->Class = InClass;
  Out->Field = InClass->Children;
 
  if ( !(InClass->ClassFlags & CLASS_NoExport) )
  {
    if ( UNLIKELY( InClass->Default == NULL ) )
    {
      GLogf( LOG_CRIT, "Can't construct object '%s.%s'; Default object missing for class '%s'",
          InPkg->Name, InName, InClass->Name );
      GSystem->Exit( -1 );
    }
  }

  // Add to object
  ObjectPool.PushBack( Out );

  // Script init (TODO)
  
  return Out;
}

UClass* UObject::StaticAllocateClass( FName ClassName, u32 Flags, UClass* SuperClass, 
    size_t InStructSize, UObject *(*NativeCtor)(size_t) )
{
  UClass* Out = new UClass( ClassName, Flags, SuperClass, InStructSize, NativeCtor );
  Out->Class = UClass::StaticClass();
  return Out;
}

UObject::UObject()
{
  Index = -1;
  Export = NULL;
  ObjectFlags = 0;
  Outer = NULL;
  Class = NULL;
  Pkg = NULL;
  RefCnt = 1;
  OldPkgFileOffsets = new TStack<size_t>();
  Name = 0;
}

//TODO: write destructor
UObject::~UObject()
{
  delete OldPkgFileOffsets;
}

bool UObject::ExportToFile( const char* Dir, const char* Type )
{
  return false;
}

void UObject::PreLoad()
{
  PkgFile = Pkg->GetStream();
  OldPkgFileOffsets->Push( PkgFile->Tell() );
  PkgFile->Seek( Export->SerialOffset, Begin );
  Export->bNeedsFullLoad = false;
  bLoading = true;
}

void UObject::Load()
{
  if ( ObjectFlags & RF_HasStack )
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
  PkgFile->Seek( OldPkgFileOffsets->Top(), Begin );
  OldPkgFileOffsets->Pop();
  bLoading = false;
}

void UObject::PostDefaultLoad()
{
  // For setting any values in the default object after it's been initialized
}

void UObject::AddRef()
{
  RefCnt++;
}

void UObject::DelRef()
{
  if ( UNLIKELY( RefCnt == 0 ) )
  {
    GLogf( LOG_WARN, "Reference count decrement on irrelevant object '%s'", Name );
    return;
  }

  RefCnt--;
  if ( RefCnt == 0 && GSystem->bLogRefCntZero )
    GLogf( LOG_INFO, "Reference count is zero for '%s'", Name );
}

bool UObject::IsA( UClass* ClassType )
{
  for ( UClass* Cls = Class; Cls != NULL; Cls = Cls->SuperClass )
  {
    if ( UNLIKELY( Cls->Class != UClass::StaticClass() ) )
    {
      GLogf( LOG_CRIT, "CLASS SUPERFIELD IS NOT A UCLASS INSTANCE!!!" );
      GSystem->Exit( -1 );
    }

    if ( Cls == ClassType )
      return true;
  }
  return false;
}

bool UObject::IsA( FName ClassName )
{
  if ( *(int*)ClassName.Data() == NONE_STR )
    ClassName = Pkg->GetGlobalName( Pkg->FindName( "Class" ) );

  UClass* Cls = FindClass( ClassName );
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

UClass* UObject::FindClass( FName ClassName )
{
  if ( ClassName == 0 )
    return UClass::StaticClass();

  for ( size_t i = 0; i < ClassPool.Size(); i++ )
  {
    UClass* Cls = ClassPool[i];
    if ( Cls->Name == ClassName )
      return Cls;
  }

  return NULL;
}

UObject* UObject::StaticFindObject( UPackage* Pkg, FName ObjName )
{
  if ( FName::IsNameNone( ObjName ) )
    return NULL;

  for ( size_t i = 0; i < ObjectPool.Size(); i++ )
  {
    UObject* Obj = ObjectPool[i];
    if ( Obj->Name == ObjName )
      return Obj;
  }

  return NULL;
}

void UObject::ReadDefaultProperties()
{
  FName PropName = 0;
  idx PropNameIdx = 0;
  u8  InfoByte = 0;
  u8  PropType = 0;
  u8  SizeByte = 0;
  u8  IsArray  = 0;
  int ArrayIdx;
  int RealSize = 0;

  while( 1 )
  {
    *PkgFile >> CINDEX( PropNameIdx );
    PropName = FName( Pkg->GetGlobalName( PropNameIdx ) );
    if ( UNLIKELY( strncmp( PropName.Data(), "None", 4 ) == 0 ) )
      break;

    UProperty* Prop = FindProperty( PropName );

    // Read the actual value of the properties even if they don't exist
    // We don't want to totally fail if some property just doesn't exist
    // Instead, the value will just go unused (because nothing is there to use it)
    if ( !Prop )
    {
      if ( Outer )
        GLogf( LOG_CRIT, "Property '%s' in '%s.%s.%s' does not exist",
          PropName.Data(), Pkg->Name.Data(), Outer->Name.Data(), Name.Data() );
      else
        GLogf( LOG_CRIT, "Property '%s' in '%s.%s' does not exist",
          PropName.Data(), Pkg->Name.Data(), Name.Data() );
    }
    else if ( Prop->Offset == MAX_UINT32 )
    {
      if ( Outer )
        GLogf( LOG_WARN, "Property '%s' in '%s.%s.%s' has no native component",
          PropName.Data(), Pkg->Name.Data(), Outer->Name.Data(), Name.Data() );
      else
        GLogf( LOG_WARN, "Property '%s' in '%s.%s' has no native component",
          PropName.Data(), Pkg->Name.Data(), Name.Data() );

      Prop = NULL;
    }
    else
    {
      // Since classes share property objects, this is really only a hint to an
      // exporter that it's even worth checking to see if this property should be
      // exported to text, not a guarantee that it has been modified. This works
      // out for things like "Tag" in Actor where the property value is different
      // from it's SuperClass value, but doesn't need to be recorded since that value
      // will always be the name of the script being exported while simultaneously 
      // covering that the Tag value be written when exporting a level.
      Prop->ObjectFlags |= RF_TagExp;
    }

    *PkgFile >> InfoByte;
    PropType = InfoByte & 0x0F;
    SizeByte = (InfoByte & 0x70) >> 4;
    IsArray  = (InfoByte & 0x80) >> 7;

    if ( PropType != PROP_Struct )
    {
      switch( SizeByte )
      {
        case 5:
          *PkgFile >> *(u8*)&RealSize;
          break;
        case 6:
          *PkgFile >> *(u16*)&RealSize;
          break;
        case 7:
          *PkgFile >> RealSize;
          break;
        default:
          RealSize = UProperty::PropertySizes[SizeByte];
          break;
      }
    }
   
    switch( PropType )
    {
      case PROP_Bool:
        ArrayIdx = IsArray;
        break;
      case PROP_Struct:
        RealSize = InfoByte;
        break;
      default:
        ArrayIdx = (IsArray) ? ReadArrayIndex( *PkgFile ) : 0;
        break;
    }

    if ( !Prop )
    {
      switch( PropType )
      {
        case PROP_Struct:
          UStructProperty::SkipDefaultProperty( *PkgFile, RealSize );
          break;
        default:
          UProperty::SkipDefaultProperty( *PkgFile, RealSize );
          break;
      }
    }
    else if ( !Prop->LoadDefaultPropertySafe( this, *PkgFile, PropType, RealSize, ArrayIdx ) )
    {
      GLogf( LOG_CRIT, "Cannot continue parsing defaultproperty list for object '%s'", Name.Data() );
      return;
    }
  }
}

// TODO: Rewrite this awful crap...
void UObject::ReadConfigProperties()
{
}

UProperty* UObject::FindProperty( FName PropName )
{
  for ( UField* Iter = Field; Iter != NULL; Iter = Iter->Next )
  {
    if ( Iter->IsA( UProperty::StaticClass() ) && Iter->Name == PropName )
      return (UProperty*)Iter;
  }

  return NULL;
}

UProperty* UObject::FindProperty( const char* PropName )
{
  for ( UField* Iter = Field; Iter != NULL; Iter = Iter->Next )
  {
    if ( Iter->IsA( UProperty::StaticClass() ) && stricmp( Iter->Name.Data(), PropName ) )
      return (UProperty*)Iter;
  }

  return NULL;
}

// This will probably be a major source of garbage-collection 
// (or memory corruption) woes later in the future...
UObject* UObject::Clone()
{
  // Enforce explicitly that both classes are the same.
  UClass* ThisClass = Class;
  UClass* ClonedClass;

  UObject* ClonedObj = ThisClass->Constructor( Class->StructSize );
  ClonedClass = ClonedObj->Class;

#ifdef __clang__
  // Yes this looks spooky, but it should be enforced that the cloned
  // and parent classes are the same. In this case, it is fine
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdynamic-class-memaccess"
#endif
  if ( ClonedObj != NULL && ThisClass == ClonedClass )
    memcpy( ClonedObj, this, ThisClass->StructSize );
#ifdef __clang__
#pragma clang diagnostic pop
#endif

  ThisClass->AddRef();
  return ClonedObj;
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
    GLogf( LOG_WARN, "Can't load object '%s.%s', object does not exist", Pkg->Name, ObjName );
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
  if ( *(int*)ObjName == NONE_STR )
    return NULL;

  // Is our object in this package?
  if ( ObjRef < 0 )
  {
    // We can figure out which package it should be in from our current package
    FImport* Import = &Pkg->GetImportTable()[ CalcObjRefValue( ObjRef ) ];
    const char* ClsName = Pkg->ResolveNameFromIdx( Import->ClassName );
    const char* GroupName = Pkg->ResolveNameFromObjRef( Import->Package );

    FHash ObjNameHash = FnvHashString( ObjName );
    FHash ClsNameHash = FnvHashString( ClsName );
    FHash GroupHash   = FnvHashString( GroupName );

    // Check to see if we're trying to load some intrinsic class
    if ( ObjClass == UClass::StaticClass() && ObjClass->ClassFlags & CLASS_NoExport )
    {
      // Go find it in the class pool
      for ( int i = 0; i < ClassPool.Size(); i++ )
      {
        UClass* ClsIter = ClassPool[i];
        if ( ClsIter->Name.Hash() == ObjNameHash )
        {
          // Does it need to be loaded?
          if ( !(ClsIter->ClassFlags & CLASS_NoExport) &&
               ClsIter->Export != NULL &&
               ClsIter->NativeNeedsPkgLoad )
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

    // At this point, we'll likely need the class too
    if ( LIKELY( ObjClass == NULL ) )
    {
      for ( int i = 0; i < ClassPool.Size(); i++ )
      {
        UClass* ClsIter = ClassPool[i];
        if ( ClsIter->Name.Hash() == ClsNameHash )
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
          GLogf( LOG_CRIT, "Can't load class '%s.%s', package is missing", ClsPkgName, ClsName );
          return NULL;
        }

        // Got it, now load the class
        ObjClass = (UClass*)StaticLoadObject( ClsPkg, ClsPkg->GetClassExport( ClsName ), 
            UClass::StaticClass(), NULL );

        if ( UNLIKELY( ObjClass == NULL ) )
        {
          // Class doesn't exist in that package, bail out
          GLogf( LOG_CRIT, "Can't load class '%s.%s', class does not exist", ClsPkgName, ClsName );
          return NULL;
        }
      }
    }

    FImport* PkgImport = Import;
    TArray<FImport>& PkgImports = Pkg->GetImportTable();
    do
    {
      // The 'Package' field for FImport does not actually tell what package it is in,
      // but what 'Group' it belongs to (which may be the package it belongs to)
      // i.e., "Core.Object" -> Package points to the import for the 'Core' package
      // "Core.Object.Color" -> Package points to the import for 'Object'
      // to get around this, we keep going back and getting the package until Import->Class 
      // points to "Package"
      PkgImport = &PkgImports[ CalcObjRefValue( PkgImport->Package ) ]; 
    } while ( strnicmp( Pkg->ResolveNameFromIdx( PkgImport->ClassName ), "Package", 7 ) != 0 ||
              PkgImport->Package != 0 );

    ObjPkgName = Pkg->ResolveNameFromIdx( PkgImport->ObjectName );
    ObjPkg = UPackage::StaticLoadPackage( ObjPkgName );
    if ( UNLIKELY( ObjPkg == NULL ) )
    {
      GLogf( LOG_CRIT, "Can't load object '%s.%s', package is missing", ObjPkgName, ObjName );
      return NULL;
    } 

    // Get the corresponding export index for this package
    // Optimization: Get the package tables here rather than inside of the loop
    TArray<FExport>& Exports = ObjPkg->GetExportTable();
    TArray<FImport>& Imports = ObjPkg->GetImportTable();
    TArray<FNameEntry>& Names = ObjPkg->GetNameTable();
    for ( int i = 0; i < Exports.Size(); i++ )
    {
      FExport* ExpIter = &Exports[i];

      // Optimization: don't go through function calls to get this info. It's
      // much faster to do everything here even if it is slightly on the not-so-great
      // side from a design perspective.
      // Get object name entry
      FNameEntry* ExpObjName = &Names[ExpIter->ObjectName];
      FNameEntry* ExpGrpName;
      if ( ExpIter->Group == 0 )
        ExpGrpName = ObjPkg->NoneNameEntry;
      else
        ExpGrpName = &Names[Exports[ExpIter->Group-1].ObjectName];

      // Optimization: stricmp eats a large chunk of execution time here, so we
      // elect to treat this comparison as a single int
      if ( *(int*)&ExpGrpName->Data[0] == NONE_STR )
        ExpGrpName = NameTable[ObjPkg->Name.Index];

      // Get class name entry
      FNameEntry* ExpClsName = NULL;
      if ( ExpIter->Class < 0 )
      {
        Import = &Imports[(-ExpIter->Class)-1];
        ExpClsName = &Names[Import->ObjectName];
      }
      else
      {
        // Check if the export's class type is a class
        if ( ExpIter->Class == 0 )
          ExpClsName = &ClassNameEntry;
        else
          ExpClsName = &Names[Exports[ExpIter->Class - 1].ObjectName];
      }

      if ( ExpObjName->Hash == ObjNameHash && ExpClsName->Hash == ClsNameHash && ExpGrpName->Hash == GroupHash )
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
    if ( UNLIKELY( stricmp( ObjPkg->Name.Data(), "UnrealI" ) == 0 ) )
    {
      ObjPkg = UPackage::StaticLoadPackage( "UnrealShare" );
      if ( UNLIKELY( ObjPkg == NULL ) )
        goto Error;

      return StaticLoadObject( ObjPkg, ObjName, ObjClass, InOuter, bLoadClassNow );
    }
    else if ( UNLIKELY( stricmp( ObjPkg->Name.Data(), "UnrealShare" ) == 0 ) )
    {
      ObjPkg = UPackage::StaticLoadPackage( "UnrealI" );
      if ( UNLIKELY( ObjPkg == NULL ) )
        goto Error;

      return StaticLoadObject( ObjPkg, ObjName, ObjClass, InOuter, bLoadClassNow );
    }

  Error:
    GLogf( LOG_CRIT, "Can't load object '%s.%s', object does not exist", ObjPkgName, ObjName );
    return NULL;
  }

  return StaticLoadObject( ObjPkg, ObjExport, ObjClass, InOuter, bLoadClassNow );
}

UObject* UObject::StaticLoadObject( UPackage* ObjPkg, FExport* ObjExport, UClass* ObjClass, 
  UObject* InOuter, bool bLoadClassNow )
{
  FName ObjName = ObjPkg->GetGlobalName( ObjExport->ObjectName );
  bool bNeedsFullLoad = true;

  // 'None' object means NULL, don't load anything
  if ( *(int*)ObjName.Data() == NONE_STR )
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
      for ( int i = 0; i < ClassPool.Size(); i++ )
      {
        UClass* ClsIter = ClassPool[i];
        if ( stricmp( ClsIter->Name.Data(), ClsName ) == 0 )
        {
          ObjClass = ClsIter;
          break; // Already loaded the class apparently, great
        }
      }
    }

    // If not, then we need to load it
    if ( UNLIKELY( ObjClass == NULL ) )
    {
      ObjClass = (UClass*)StaticLoadObject( ObjPkg, ObjExport->Class, UClass::StaticClass(), NULL, true );
      if ( UNLIKELY( ObjClass == NULL ) )
      {
        GLogf( LOG_CRIT, "Can't load object '%s.%s', cannot load class", 
          ObjPkg->Name, ObjName );
        return NULL;
      }
    }
  }

  // Type checking
  FName ClassName = ObjPkg->ResolveGlobalNameObjRef( ObjExport->Class );
  UClass* ClassType = FindClass( ClassName );
  if ( UNLIKELY( ClassType == NULL ) )
  {
    // Load the actual type that it is
    ClassType = (UClass*)StaticLoadObject( ObjPkg, ObjExport->Class, UClass::StaticClass(), NULL, true );
    if ( UNLIKELY( ClassType == NULL ) )
    {
      GLogf( LOG_WARN, "Can't load object '%s.%s', cannot load class",
          ObjPkg->Name.Data(), ObjName.Data() );
      return NULL;
    }
  }

  if ( UNLIKELY( !ClassType->Default->IsA( ObjClass ) ) )
  {
    GLogf( LOG_WARN, "Object '%s.%s' was expected to be of type '%s' but was '%s'",
        ObjPkg->Name.Data(), ObjName.Data(), ObjClass->Name.Data(), ClassName.Data() );
    return NULL;
  }

  // Type checks out
  ObjClass = ClassType;

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

UCommandlet::UCommandlet()
  : UObject()
{
}

UCommandlet::~UCommandlet()
{
  // TODO:
}

int UCommandlet::Main( FString* Parms )
{
  return 0;
}

bool UObject::StaticLinkNativeProperties()
{
  typedef UObject LocalClassType;
  if ( StaticInitNativePropList( 6 ) )
  {
    LINK_NATIVE_ARRAY( ObjectInternal );
    LINK_NATIVE_PROPERTY( Outer );
    LINK_NATIVE_PROPERTY( ObjectFlags );
    LINK_NATIVE_PROPERTY( Name );
    LINK_NATIVE_PROPERTY( Class );
    LINK_NATIVE_PROPERTY_ALIASED( Parent, Outer ); 
    return true;
  }
  return false;
}

IMPLEMENT_NATIVE_CLASS( UObject );
IMPLEMENT_NATIVE_CLASS( UCommandlet );

BEGIN_PROPERTY_LINK( UCommandlet, 14 )
  LINK_NATIVE_PROPERTY( HelpCmd );
  LINK_NATIVE_PROPERTY( HelpOneLiner );
  LINK_NATIVE_PROPERTY( HelpUsage );
  LINK_NATIVE_PROPERTY( HelpWebLink );
  LINK_NATIVE_ARRAY   ( HelpParm );
  LINK_NATIVE_ARRAY   ( HelpDesc );
  LINK_NATIVE_PROPERTY( LogToStdout );
  LINK_NATIVE_PROPERTY( IsServer );
  LINK_NATIVE_PROPERTY( IsClient );
  LINK_NATIVE_PROPERTY( IsEditor );
  LINK_NATIVE_PROPERTY( LazyLoad );
  LINK_NATIVE_PROPERTY( ShowErrorCount );
  LINK_NATIVE_PROPERTY( ShowBanner );
  LINK_NATIVE_PROPERTY( ForceInt );
END_PROPERTY_LINK()

