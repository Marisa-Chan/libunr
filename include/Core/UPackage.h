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
 * UPackage.h - Object for keeping track of a single UE1 package
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include "Core/UObject.h"
#include "Core/UNativeModule.h"

#define UE1_PKG_SIG 0x9e2a83c1

/*-----------------------------------------------------------------------------
 * FPackageFileIn
 * Keeps track of package specifics when reading a package from a file
-----------------------------------------------------------------------------*/
class LIBUNR_API FPackageFileIn : public FFileArchiveIn
{
public:
  int Ver;
  UPackage* Pkg;
};

int ReadArrayIndex( FPackageFileIn& PkgFile );

/*-----------------------------------------------------------------------------
 * FPackageFileOut
 * Keeps track of package specifics when writing a package to a file
-----------------------------------------------------------------------------*/
class LIBUNR_API FPackageFileOut : public FFileArchiveOut
{
public:
  int Ver;
  UPackage* Pkg;
};

/*-----------------------------------------------------------------------------
 * FExport
 * A data struct that contains information about a single object
 * that a package will expose to outside sources
-----------------------------------------------------------------------------*/
struct LIBUNR_API FExport
{
  idx Class;        // Object reference to Class
  idx Super;        // Object reference to Parent
  u32 Group;        // Object reference to Package/Group
  idx ObjectName;   // Name index to Object Name
  u32 ObjectFlags;  // Object flags
  idx SerialSize;   // Compact index to size inside file
  idx SerialOffset; // Compact index to offset (if SerialSize>0)
  
  UObject* Obj;        // Pointer to object (if loading/already loaded)
  int Index;           // Index in Export Table
  bool bNeedsFullLoad; // Does this object need a full load?

  void Read( FPackageFileIn& Pkg );
};

/*-----------------------------------------------------------------------------
 * FImport
 * A data struct that contains information about a single object
 * which is required for the complete loading of a package
-----------------------------------------------------------------------------*/
struct LIBUNR_API FImport
{
  idx ClassPackage; // Name index to Package name of Class
  idx ClassName;    // Name index to Class Name
  int Package;      // Object reference to Package of object
  idx ObjectName;   // Name index to Object Name
  
  void Read( FPackageFileIn& Pkg );
};

/*-----------------------------------------------------------------------------
 * UPackageHeader
 * A data struct which contains information about a single package
 * This information is always located at the top of a package file
-----------------------------------------------------------------------------*/
struct LIBUNR_API UPackageHeader
{
  u32 Signature;
  u16 PackageVersion;
  u16 LicenseMode;
  u32 PackageFlags;
  u32 NameCount;
  u32 NameOffset;
  u32 ExportCount;
  u32 ExportOffset;
  u32 ImportCount;
  u32 ImportOffset;
  u32 HeritageCount;
  u32 HeritageOffset;
  u8  GUID[16];
  
  friend FPackageFileIn& operator>>( FPackageFileIn& In, UPackageHeader& Header );
};

// Decides how packages are maintained during loading and idle states
enum EPkgLoadOpts
{
  PO_OpenOnLoad,  // Opens a package if objects are requested, closing it after ending load
  PO_KeepOpen,    // Keeps a package open as long as there are references to the package (not implemented)
  PO_LoadToMem,   // Loads the entire package to memory (not implemented)
};

/*-----------------------------------------------------------------------------
 * UPackage
 * A container for default object types
-----------------------------------------------------------------------------*/
class LIBUNR_API UPackage : public UObject
{
  DECLARE_NATIVE_CLASS( UPackage, UObject, CLASS_NoExport, Core )
  UPackage();
  
  virtual bool Load( const char* File );
  virtual bool Save( const char* File = NULL );
  virtual void Close();
 
  virtual FImport* GetImport( size_t Index );
  virtual FExport* GetExport( size_t Index );

  UPackageHeader* GetHeader();
  FNameEntry*     GetNameEntry( size_t Index );
  FNameEntry*     GetNameEntryByObjRef( int ObjRef );
  FExport*        GetExportByName( size_t Name );
  FExport*        GetClassExport( const char* ExportName );
  TArray<FNameEntry>& GetNameTable();
  TArray<FExport>& GetExportTable();
  TArray<FImport>& GetImportTable();
  u32             GetGlobalName( u32 PkgNameIdx );
  const char*     GetFilePath();
  const char*     GetFileName();
  FString*        GetFullObjName( FExport* ObjExp );
  size_t          GetPackageVer();
  void            LoadEditableTypes();

  size_t AddName( const char* Name );
  size_t FindName( const char* Name );
  
  // Name resolution
  const char* ResolveNameFromIdx( idx Index );
  const char* ResolveNameFromObjRef( int ObjRef );
  FName ResolveGlobalNameIdx( idx Index );
  FName ResolveGlobalNameObjRef( int ObjRef );

  // Accessors 
  FPackageFileIn* GetStream();

  static bool StaticInit();
  static void StaticExit( bool bCrashExit = false );
  static UPackage* StaticLoadPackage( const char* Filepath, bool bSearch = true );
  static UPackage* StaticCreatePackage( const char* Name );
  
  static TArray<UPackage*>* GetLoadedPackages();
  
  FString Path;
  bool bIntrinsicPackage;
  FNameEntry* NoneNameEntry;

protected:    
  TArray<FNameEntry> Names;
  TArray<FExport>    Exports;
  TArray<FImport>    Imports;
  FFileArchive* Stream;
  UPackageHeader Header;
  u32 NameTableStart; // The index at which this package's name table appears in the global name table
  UNativeModule* NativeModule; // May be NULL if there is no native component (or if package is intrinsic)

  // Global package variables
  static EPkgLoadOpts LoadOpts;
  static TArray<UPackage*>* Packages;
};

