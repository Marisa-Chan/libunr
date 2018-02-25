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
 * UPackage.h - Object for keeping track of a single UE1 package
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#ifndef __UPACKAGE_H__
#define __UPACKAGE_H__

#include "FName.h"
#include "FString.h"
#include "TArray.h"

#define UE1_PKG_SIG 0x9e2a83c1

// FExport
// A data struct that contains information about a single object
// that a package will expose to outside sources
struct DLL_EXPORT FExport
{
  idx Class;
  idx Super;
  u32 Group;
  idx ObjectName;
  u32 ObjectFlags;
  idx SerialSize;
  idx SerialOffset;  
  
  friend FArchive& operator>>( FArchive& In, FExport& Export );
};

// FImport
// A data struct that contains information about a single object
// which is required for the complete loading of a package
struct DLL_EXPORT FImport
{
  idx ClassPackage;
  idx ClassName;
  int Package;
  idx ObjectName;
  
  friend FArchive& operator>>( FArchive& In, FImport& Import );
};

// UPackageHeader
// A data struct which contains information about a single package
// This information is always located at the top of a package file
struct DLL_EXPORT UPackageHeader
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
  
  friend FArchive& operator>>( FArchive& In, UPackageHeader& Header );
};

// Decides how packages are maintained during loading and idle states
enum EPkgLoadOpts
{
  PO_OpenOnLoad,  // Opens a package if objects are requested, closing it after ending load
  PO_KeepOpen,    // Keeps a package open as long as there are references to the package (not implemented)
  PO_LoadToMem,   // Loads the entire package to memory (not implemented)
};

// UPackage
// An object which keeps track of any information relating to a package
class DLL_EXPORT UPackage
{
  BLOCK_COPY_CTOR( UPackage );
  
public:
   UPackage();
  ~UPackage();
  
  virtual bool Load( const char* File );
  virtual bool Save( const char* File = NULL );
  virtual void Close();
  
  UPackageHeader* GetHeader();
  FNameEntry*     GetNameEntry( size_t Index );
  FImport*        GetImport( size_t Index );
  FExport*        GetExport( size_t Index );
  const char*     GetFilePath();
  const char*     GetFileName();
  
  // Name resolution
  const char* ResolveNameFromIdx( idx Index );
  const char* ResolveNameFromObjRef( int ObjRef );
  
  // Object reading
  bool BeginLoad();
  void EndLoad();
  bool ReadRawObject( idx Index, void* Buffer );

  // Object writing
  
protected:
  FString Path;
  FString Name;
  
  FArchive* FileStream;
  UPackageHeader Header;
  TArray<FNameEntry>* Names;
  TArray<FExport>*    Exports;
  TArray<FImport>*    Imports;

  // Global package variables
  static EPkgLoadOpts LoadOpts;
};

#endif
