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
 * UObject.h - Base object type for Unreal
 * Design inspiration taken from UT432 pubsrc Core/UnObjBas.h
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include <libxstl/XArray.h>
#include <libxstl/XMemory.h>
#include <libxstl/XStream.h>
#include <libxstl/XStack.h>
#include "Core/FUtil.h"

using namespace xstl;

// Flags for loading objects.
enum ELoadFlags
{
  LOAD_None          = 0x0000, // No flags.
  LOAD_NoFail        = 0x0001, // Critical error if load fails.
  LOAD_NoWarn        = 0x0002, // Don't display warning if load fails.
  LOAD_Throw         = 0x0008, // Throw exceptions upon failure.
  LOAD_Verify        = 0x0010, // Only verify existance; don't actually load.
  LOAD_AllowDll      = 0x0020, // Allow plain DLLs.
  LOAD_DisallowFiles = 0x0040, // Don't load from file.
  LOAD_NoVerify      = 0x0080, // Don't verify imports yet.
  LOAD_Forgiving     = 0x1000, // Forgive missing imports (set them to NULL).
  LOAD_Quiet         = 0x2000, // No log warnings.
  LOAD_NoRemap       = 0x4000, // No remapping of packages.
  LOAD_Propagate     = 0,
};

// Package flags.
enum EPackageFlags
{
  PKG_AllowDownload  = 0x0001, // Allow downloading package.
  PKG_ClientOptional = 0x0002, // Purely optional for clients.
  PKG_ServerSideOnly = 0x0004, // Only needed on the server side.
  PKG_BrokenLinks    = 0x0008, // Loaded from linker with broken import links.
  PKG_Unsecure       = 0x0010, // Not trusted.
  PKG_Need           = 0x8000, // Client needs to download this package.
};

// Flags describing a class.
enum EClassFlags
{
  // Base flags.
  CLASS_NoSave            = 0x00000, // Disregard this object's existance entirely at save time
  CLASS_Abstract          = 0x00001, // Class is abstract and can't be instantiated directly.
  CLASS_Compiled          = 0x00002, // Script has been compiled successfully.
  CLASS_Config            = 0x00004, // Load object configuration at construction time.
  CLASS_Transient         = 0x00008, // This object type can't be saved; null it out at save time.
  CLASS_Parsed            = 0x00010, // Successfully parsed.
  CLASS_Localized         = 0x00020, // Class contains localized text.
  CLASS_SafeReplace       = 0x00040, // Objects of this class can be safely replaced with default or NULL.
  CLASS_RuntimeStatic     = 0x00080, // Objects of this class are static during gameplay.
  CLASS_NoExport          = 0x00100, // Does not have an export in script package
  CLASS_NoUserCreate      = 0x00200, // Don't allow users to create in the editor.
  CLASS_PerObjectConfig   = 0x00400, // Handle object configuration on a per-object basis, rather than per-class.
  CLASS_NativeReplication = 0x00800, // Replication handled in C++.

  // Flags to inherit from base class.
  CLASS_Inherit           = CLASS_Transient | CLASS_Config | CLASS_Localized | CLASS_SafeReplace | 
                            CLASS_RuntimeStatic | CLASS_PerObjectConfig,
  CLASS_RecompilerClear   = CLASS_Inherit | CLASS_Abstract | CLASS_NoExport | CLASS_NativeReplication,
};

// Flags associated with each property in a class, overriding the
// property's default behavior.
enum EPropertyFlags
{
  // Regular flags.
  CPF_Edit         = 0x00000001, // Property is user-settable in the editor.
  CPF_Const        = 0x00000002, // Actor's property always matches class's default actor property.
  CPF_Input        = 0x00000004, // Variable is writable by the input system.
  CPF_ExportObject = 0x00000008, // Object can be exported with actor.
  CPF_OptionalParm = 0x00000010, // Optional parameter (if CPF_Param is set).
  CPF_Net          = 0x00000020, // Property is relevant to network replication.
  CPF_ConstRef     = 0x00000040, // Reference to a constant object.
  CPF_Parm         = 0x00000080, // Function/When call parameter.
  CPF_OutParm      = 0x00000100, // Value is copied out after function call.
  CPF_SkipParm     = 0x00000200, // Property is a short-circuitable evaluation function parm.
  CPF_ReturnParm   = 0x00000400, // Return value.
  CPF_CoerceParm   = 0x00000800, // Coerce args into this function parameter.
  CPF_Native       = 0x00001000, // Property is native: C++ code is responsible for serializing it.
  CPF_Transient    = 0x00002000, // Property is transient: shouldn't be saved, zero-filled at load time.
  CPF_Config       = 0x00004000, // Property should be loaded/saved as permanent profile.
  CPF_Localized    = 0x00008000, // Property should be loaded as localizable text.
  CPF_Travel       = 0x00010000, // Property travels across levels/servers.
  CPF_EditConst    = 0x00020000, // Property is uneditable in the editor.
  CPF_GlobalConfig = 0x00040000, // Load config from base class, not subclass.
  CPF_OnDemand     = 0x00100000, // Object or dynamic array loaded on demand only.
  CPF_New          = 0x00200000, // Automatically create inner object.
  CPF_NeedCtorLink = 0x00400000, // Fields need construction/destruction.
  CPF_NeedsExport  = 0x00800000, // Object has set this property during ReadDefaultProperties

  // Combinations of flags.
  CPF_ParmFlags           = CPF_OptionalParm | CPF_Parm | CPF_OutParm | CPF_SkipParm | 
                            CPF_ReturnParm | CPF_CoerceParm,
  CPF_PropagateFromStruct = CPF_Const | CPF_Native | CPF_Transient,
};

// Flags describing an object instance.
enum EObjectFlags
{
  RF_Transactional  = 0x00000001, // Object is transactional.
  RF_Unreachable    = 0x00000002, // Object is not reachable on the object graph.
  RF_Public         = 0x00000004, // Object is visible outside its package.
  RF_TagImp         = 0x00000008, // Temporary import tag in load/save.
  RF_TagExp         = 0x00000010, // Temporary export tag in load/save.
  RF_SourceModified = 0x00000020, // Modified relative to source files.
  RF_TagGarbage     = 0x00000040, // Check during garbage collection.

  RF_NeedLoad         = 0x00000200, // During load, indicates object needs loading.
  RF_HighlightedName  = 0x00000400, // A hardcoded name which should be syntax-highlighted.
  RF_EliminateObject  = 0x00000400, // NULL out references to this during garbage collecion.
  RF_InSingularFunc   = 0x00000800, // In a singular function.
  RF_RemappedName     = 0x00000800, // Name is remapped.
  RF_Suppress         = 0x00001000, //warning: Mirrored in UnName.h. Suppressed log name.
  RF_StateChanged     = 0x00001000, // Object did a state change.
  RF_InEndState       = 0x00002000, // Within an EndState call.
  RF_Transient        = 0x00004000, // Don't save object.
  RF_Preloading       = 0x00008000, // Data is being preloaded from file.
  RF_LoadForClient    = 0x00010000, // In-file load for client.
  RF_LoadForServer    = 0x00020000, // In-file load for client.
  RF_LoadForEdit      = 0x00040000, // In-file load for client.
  RF_Standalone       = 0x00080000, // Keep object around for editing even if unreferenced.
  RF_NotForClient     = 0x00100000, // Don't load this object for the game client.
  RF_NotForServer     = 0x00200000, // Don't load this object for the game server.
  RF_NotForEdit       = 0x00400000, // Don't load this object for the editor.
  RF_Destroyed        = 0x00800000, // Object Destroy has already been called.
  RF_NeedPostLoad     = 0x01000000, // Object needs to be postloaded.
  RF_HasStack         = 0x02000000, // Has execution stack.
  RF_Native           = 0x04000000, // Native (UClass only).
  RF_Marked           = 0x08000000, // Marked (for debugging).
  RF_ErrorShutdown    = 0x10000000, // ShutdownAfterError called.
  RF_DebugPostLoad    = 0x20000000, // For debugging Serialize calls.
  RF_DebugSerialize   = 0x40000000, // For debugging Serialize calls.
  RF_DebugDestroy     = 0x80000000, // For debugging Destroy calls.
  
  RF_ContextFlags     = RF_NotForClient | RF_NotForServer | RF_NotForEdit, // All context flags.
  RF_LoadContextFlags = RF_LoadForClient | RF_LoadForServer | RF_LoadForEdit, // Flags affecting loading.
  RF_Load             = RF_ContextFlags | RF_LoadContextFlags | RF_Public | RF_Standalone | 
                        RF_Native | RF_SourceModified | RF_Transactional | RF_HasStack,

  RF_Keep             = RF_Native | RF_Marked, // Flags to persist across loads.
  RF_ScriptMask       = RF_Transactional | RF_Public | RF_Transient | RF_NotForClient | RF_NotForServer |
                        RF_NotForEdit // Script-accessible flags.
};

struct FNativePropertyLink
{
  FHash Hash;
  u32 Offset;
};

struct FNativePropertyList
{
  FNativePropertyList( FHash InHash, size_t InNum );
  ~FNativePropertyList();
  void AddProperty( const char* Name, u32 InOffset );
 
  bool bInitialized;
  FHash Hash;
  size_t Added;
  size_t Num;
  FNativePropertyLink* Properties;
};

// FIXME: Forward declarations are gross
class UObject;
class UStruct;
class UProperty;
class UByteProperty;
class UIntProperty;
class UBoolProperty;
class UFloatProperty;
class UObjectProperty;
class UNameProperty;
class UClassProperty;
class UStructProperty;
class UStrProperty;
class UArrayProperty;
class FPackageFileIn;
class FPackageFileOut;
class FPropertyHash;
class UField;
class UFunction;
class UClass;
class UPackage;
struct FPropertyRecord;
struct FExport;

#define TEXT(s) #s

#define DECLARE_NATIVE_CLASS_BASE(cls, supcls, clsflags, pkg) \
protected: \
  static UClass* ConstructNativeClass( u32 Flags ); \
  static UClass* ObjectClass; \
  static FNativePropertyList* StaticNativePropList; \
private: \
  static constexpr const char* NativePkgName = TEXT(pkg); \
  static size_t NativeSize; \
public: \
  typedef supcls Super; \
  void* operator new( size_t sz ) \
  { \
    return Malloc( sz ); \
  } \
  void* operator new( size_t sz, size_t ObjSize ) \
  { \
    void* Mem = xstl::Malloc( ObjSize ); \
    xstl::Set( Mem, 0, ObjSize ); \
    return Mem; \
  } \
  void operator delete( void* Obj ) \
  { \
    xstl::Free( Obj );  \
  } \
  static UClass* StaticClass() \
  { return ObjectClass; } \
  static UObject* NativeConstructor( size_t ObjSize ) \
  { \
    return new(ObjSize) cls(); \
  } \
  static bool StaticCreateClass() \
  { \
    if (!ObjectClass) \
    { \
      ObjectClass = UObject::StaticAllocateClass( TEXT(cls), clsflags, Super::ObjectClass,\
        NativeSize, NativeConstructor ); \
      if ( ObjectClass != NULL ) \
      { \
        ClassPool->PushBack( ObjectClass ); \
        return true; \
      } \
      return false; \
    } \
    return true; \
  } \
  static inline bool StaticInitNativePropList( size_t NumProperties ) \
  { \
    if (!StaticNativePropList) \
    { \
      const char* ClsName = TEXT(cls); \
      StaticNativePropList = new FNativePropertyList( FnvHashString( ClsName + 1 ),\
        NumProperties ); \
      if ( StaticNativePropList != NULL ) \
      { \
        NativePropertyLists->PushBack( StaticNativePropList ); \
        return true; \
      } \
      return false; \
    } \
    return true; \
  } \
  static bool StaticSetPackageProperties(); \
  static bool StaticClassInit() \
  { \
    if ( !StaticCreateClass() ) \
    { \
      Logf( LOG_CRIT, "%s::StaticCreateClass() failed!", TEXT(cls) ); \
      return false; \
    } \
    if ( !( (clsflags) & CLASS_NoExport ) ) \
    { \
      if ( !StaticLinkNativeProperties() ) \
      { \
        Logf( LOG_CRIT, "%s::StaticLinkNativeProperties() failed!", TEXT(cls) ); \
        return false; \
      } \
      if ( !StaticSetPackageProperties() ) \
      { \
        Logf( LOG_CRIT, "%s::StaticSetPackageProperties() failed!", TEXT(cls) ); \
        return false; \
      } \
    } \
    return true; \
  } \
  virtual size_t GetNativeSize() \
  { \
    return NativeSize; \
  } \
  virtual ~cls(); 

#define DECLARE_NATIVE_CLASS(cls, supcls, flags, pkg) \
  DECLARE_NATIVE_CLASS_BASE(cls, supcls, flags, pkg) \

#define DECLARE_NATIVE_ABSTRACT_CLASS(cls, supcls, flags, pkg) \
  DECLARE_NATIVE_CLASS_BASE(cls, supcls, flags | CLASS_Abstract, pkg) \
  
#define IMPLEMENT_NATIVE_CLASS(cls) \
  UClass* cls::ObjectClass = NULL; \
  size_t  cls::NativeSize  = sizeof( cls ); \
  FNativePropertyList* cls::StaticNativePropList = NULL; \
  bool cls::StaticSetPackageProperties() \
  { \
    ObjectClass->Pkg = UPackage::StaticLoadPackage( NativePkgName ); \
    if ( ObjectClass->Pkg == NULL ) \
    { \
      Logf( LOG_CRIT, "Failed to load package '%s' for class '%s'.", NativePkgName, ObjectClass->Name ); \
      return false; \
    } \
    ObjectClass->Export = ObjectClass->Pkg->GetClassExport( ObjectClass->Name ); \
    ObjectClass->Export->Obj = ObjectClass; \
    ObjectClass->ObjectFlags = ObjectClass->Export->ObjectFlags; \
    return true; \
  }
 
#define LINK_NATIVE_PROPERTY(cls, var) \
  StaticNativePropList->AddProperty( #var, OFFSET_OF( cls, var ) );
  
#define LINK_NATIVE_ARRAY(cls, var) \
  StaticNativePropList->AddProperty( #var, OFFSET_OF( cls, var[0] ) );

#define BEGIN_PROPERTY_LINK( cls, numprop ) \
bool cls::StaticLinkNativeProperties() \
{ \
  if ( StaticNativePropList != NULL ) \
    return true; \
\
  FNativePropertyList* SuperPropList = Super::StaticNativePropList; \
  if ( SuperPropList == NULL ) \
    return false; \
 \
  if ( StaticInitNativePropList( numprop ) ) \
  { \

#define END_PROPERTY_LINK() \
    return true; \
  } \
  return false; \
}

#define EXPOSE_TO_USCRIPT() \
  static bool StaticLinkNativeProperties(); 

// There's like two properties in all of UE1 that actually need this...
#define EXPOSE_PROPERTY(cls, prop, ptype, size) \
  { \
    UClass* ExpCls = cls::StaticClass(); \
    ptype* ExpProp = (ptype*)ptype::StaticClass()->CreateObject(); \
    ExpProp->ArrayDim = 1; \
    ExpProp->ElementSize = size; \
    ExpProp->PropertyFlags = CPF_Native; \
    ExpProp->Outer = ExpCls; \
    ExpProp->Offset = OFFSET_OF( cls, prop ); \
    ExpProp->Next = ExpCls->Children; \
    ExpProp->Name = TEXT( prop ); \
    ExpProp->Hash = FnvHashString( ExpProp->Name ); \
    ExpProp->ObjectFlags = RF_Native; \
    ExpProp->RefCnt = 1; \
    ExpProp->Class = ptype::StaticClass(); \
    ObjectPool->PushBack( ExpProp ); \
    ExpCls->Children = ExpProp; \
    ExpCls->Default->Field = ExpCls->Children; \
  }

#define EXPORTABLE() \
  virtual bool ExportToFile( const char* Dir, const char* Type );

/*-----------------------------------------------------------------------------
 * UObject
 * The base class of all Unreal objects
-----------------------------------------------------------------------------*/
class DLL_EXPORT UObject
{
public:
  DECLARE_NATIVE_CLASS_BASE( UObject, UObject, CLASS_Abstract, Core )
  EXPOSE_TO_USCRIPT()
  // not really exportable, but just so all subclasses can have export called generically
  EXPORTABLE() 
  UObject();

  UObject* LoadObject( idx ObjRef, UClass* ObjClass, UObject* InOuter, bool bLoadClassNow = false );
  UObject* Clone();
  virtual void PreLoad();
  virtual void Load();
  virtual void PostLoad();
  virtual void PostDefaultLoad();

  void AddRef();
  void DelRef();

  bool IsA( UClass* ClassType );
  bool IsA( char* ClassName, FHash& ClassHash );
  bool ParentsIsA( UClass* ClassType );
  void ReadDefaultProperties(); 
  void ReadConfigProperties();
  static UClass* FindClass( FHash& ClassHash );

  // Property getters
  template <class T> inline T GetProperty( UProperty* Prop, int Idx );
  UProperty* FindProperty( const char* PropName );

  // Property setters
  template<class T> inline void SetProperty( UProperty* Prop, T NewVal, int Idx = 0 );
  template<class T> inline void SetArrayProperty
    ( UArrayProperty* ArrayProp, FPackageFileIn* In, int Idx, u8 ByteSize, u8 NumElem );
  void SetStructProperty( UStructProperty* Prop, FPackageFileIn* In, 
    int Idx = 0, u32 Offset = 0);

  static bool StaticInit();
  static bool StaticExit();
  static UObject* StaticLoadObject( UPackage* ObjPkg, const char* ObjName, UClass* ObjClass, 
    UObject* InOuter, bool bLoadClassNow = false );
  static UObject* StaticLoadObject( UPackage* ObjPkg, idx ObjRef, UClass* ObjClass, 
    UObject* InOuter, bool bLoadClassNow = false );
  static UObject* StaticLoadObject( UPackage* ObjPkg, FExport* ObjExport, UClass* ObjClass,
    UObject* InOuter, bool bLoadClassNow = false );
  static UObject* StaticConstructObject( const char* InName, UClass* InClass, 
    UObject* InOuter, UPackage* InPkg, FExport* InExport );
  static UClass* StaticAllocateClass( const char* ClassName, u32 Flags, UClass* SuperClass, 
    size_t InStructSize, UObject *(*NativeCtor)(size_t) );
  static int CalcObjRefValue( idx ObjRef );

  static Array<UObject*>* ObjectPool;
  static Array<UClass*>*  ClassPool; 
  static Array<FNativePropertyList*>* NativePropertyLists;
  static Array<UFunction*>* NativeFunctions;

  FHash       Hash;     // Hash of this object
  const char* Name;     // Name of this object (This type does not match script by design)
  int         Index;    // Index of the object in object pool
  idx         NameIdx;  // Name index in the packages name table
  UObject*    NextObj;  // The next object in the list
  UPackage*   Pkg;      // Package this object was loaded from
  FExport*    Export;   // Export struct from the package of this object
  UObject*    Outer;    // Object that this object resides in
  u32         ObjectFlags;
  UClass*     Class;    // Class of this object
  UField*     Field;    // All fields relevant to this object (points to Class->Children)
  FPackageFileIn* PkgFile; // Only relevant when loading
  bool bLoading;

  // I think this was originally here to "hide" sensitive info for objects.
  // This was due to the fact that C++ property offsets *HAVE* to match up
  // with UScript offsets, or else the whole native/scripted setup falls
  // apart. We dynamically link native variables to scripted, so this will
  // be unused
  int ObjectInternal[6];

  // Advanced Animation Notify
  enum EAnimNotifyEval
  {
    ANE_Equal,
    ANE_Greater,
    ANE_Less,
    ANE_GreaterOrEqual,
    ANE_LessOrEqual
  };

protected:
  int RefCnt;
  Stack<size_t>* OldPkgFileOffsets;

  static bool bStaticBootstrapped;
};

template <class T> T* SafeCast( UObject* Obj )
{
  if ( !Obj || !Obj->IsA( T::StaticClass() ) )
    return NULL;

  return (T*)Obj;
}

class DLL_EXPORT UCommandlet : public UObject
{
  DECLARE_NATIVE_CLASS( UCommandlet, UObject, 0, Core )
  EXPOSE_TO_USCRIPT();

  UCommandlet();
  virtual int Main( String* Parms );

  String* HelpCmd;
  String* HelpOneLiner;
  String* HelpUsage;
  String* HelpWebLink;
  String* HelpParm[16];
  String* HelpDesc[16];
  bool    LogToStdout;
  bool    IsServer;
  bool    IsClient;
  bool    IsEditor;
  bool    LazyLoad;
  bool    ShowErrorCount;
  bool    ShowBanner;
  bool    ForceInt;
};

