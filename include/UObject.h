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
 * UObject.h - Base object type for Unreal
 * Design inspiration taken from UT432 pubsrc Core/UnObjBas.h
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#ifndef __UOBJECT__
#define __UOBJECT__

#include "Array.h"
#include "Stream.h"
#include "FName.h"

using namespace xstl;

class FPackageFileIn;
class FPackageFileOut;
class UClass;
class UPackage;
class UProperty;
struct FPropertyRecord;

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
  CLASS_NoExport          = 0x00100, // no longer needed
  CLASS_NoUserCreate      = 0x00200, // Don't allow users to create in the editor.
  CLASS_PerObjectConfig   = 0x00400, // Handle object configuration on a per-object basis, rather than per-class.
  CLASS_NativeReplication = 0x00800, // Replication handled in C++.

  // Flags to inherit from base class.
  CLASS_Inherit           = CLASS_Transient | CLASS_Config | CLASS_Localized | CLASS_SafeReplace | CLASS_RuntimeStatic | CLASS_PerObjectConfig,
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

  // Combinations of flags.
  CPF_ParmFlags           = CPF_OptionalParm | CPF_Parm | CPF_OutParm | CPF_SkipParm | CPF_ReturnParm | CPF_CoerceParm,
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
  RF_Load             = RF_ContextFlags | RF_LoadContextFlags | RF_Public | RF_Standalone | RF_Native | RF_SourceModified | RF_Transactional | RF_HasStack, // Flags to load from Unrealfiles.
  RF_Keep             = RF_Native | RF_Marked, // Flags to persist across loads.
  RF_ScriptMask       = RF_Transactional | RF_Public | RF_Transient | RF_NotForClient | RF_NotForServer | RF_NotForEdit // Script-accessible flags.
};

#define NO_DEFAULT_CONSTRUCTOR(cls) \
  protected: cls() {} public:

#define DECLARE_BASE_CLASS(cls, supcls, clsflags) \
protected: \
  static UClass* ConstructNativeClass( u32 Flags ); \
private: \
  static UClass* ObjectClass; \
public: \
  static bool StaticInitializeClass() \
  { \
    if (!ObjectClass) \
      return ((ObjectClass = UObject::StaticAllocateClass( clsflags )) != NULL); \
    return true; \
  } \
  typedef supcls Super; \
  virtual ~cls(); \
  friend FPackageFileIn& operator>>( FPackageFileIn& Ar, cls& Obj ); \
  friend FPackageFileOut& operator<<( FPackageFileOut& Ar, cls& Obj ); \

#define DECLARE_CLASS(cls, supcls, flags) \
  DECLARE_BASE_CLASS(cls, supcls, flags) \
  
//  virtual void InitFromPkg( int InPkgIdx, u32 InFlags, UObject* InOuter, FName InName, UClass* InClass = NULL );\

#define DECLARE_ABSTRACT_CLASS(cls, supcls, flags) \
  DECLARE_BASE_CLASS(cls, supcls, flags | CLASS_Abstract) 
  
#define IMPLEMENT_CLASS(cls) \
  UClass* cls::ObjectClass = NULL; \
  
class UObject
{
  DECLARE_BASE_CLASS( UObject, UObject, CLASS_Abstract )
  UObject();
  
  virtual bool ExportToFile();
  virtual void LoadFromPackage( FPackageFileIn& Ar );
  
  virtual void SetPkgProperties( UPackage* InPkg, int InExpIdx, int InNameIdx );
  
protected:
  void ReadProperties( FPackageFileIn& Ar );
  
  static UClass* StaticAllocateClass( u32 Flags );

  int       Index;   // Index of the object in object pool
  int       ExpIdx;  // Index of this object in the export table
  int       NameIdx; // Index of this object's name in the package's name table
  u32       Flags;   // Object flags
  UObject*  Outer;   // Object that this object resides in
  UClass*   Class;   // Class of this object
  UPackage* Pkg;     // Package this object was loaded from
  int       RefCnt;  // Number of references this object has (-1 = object must be explicitly purged)
  
  Array<UProperty*> Properties;
};

bool InitStaticUClasses();

#endif
