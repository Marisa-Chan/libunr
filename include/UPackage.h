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

#pragma once

#include "Array.h"
#include "FileStream.h"
#include "String.h"

#include "UObject.h"

#define UE1_PKG_SIG 0x9e2a83c1

using namespace xstl;

/*-----------------------------------------------------------------------------
 * FPackageFileIn
 * Keeps track of package specifics when reading a package from a file
-----------------------------------------------------------------------------*/
class FPackageFileIn : public FileStreamIn
{
public:
  int Ver;
};

/*-----------------------------------------------------------------------------
 * FPackageFileOut
 * Keeps track of package specifics when writing a package to a file
-----------------------------------------------------------------------------*/
class FPackageFileOut : public FileStreamOut
{
public:
  int Ver;
};

/*-----------------------------------------------------------------------------
 * FCompactIndex
 * https://wiki.beyondunreal.com/Legacy:Package_File_Format/Data_Details
-----------------------------------------------------------------------------*/
class DLL_EXPORT FCompactIndex
{
public:
  int Value;
  friend FPackageFileIn& operator>> ( FPackageFileIn& Ar,  FCompactIndex& Index );
  friend FPackageFileOut& operator<<( FPackageFileOut& Ar, FCompactIndex& Index );
};

#define CINDEX(val) (*(FCompactIndex*)&val)

/*-----------------------------------------------------------------------------
 * FExport
 * A data struct that contains information about a single object
 * that a package will expose to outside sources
-----------------------------------------------------------------------------*/
struct DLL_EXPORT FExport
{
  idx Class;
  idx Super;
  u32 Group;
  idx ObjectName;
  u32 ObjectFlags;
  idx SerialSize;
  idx SerialOffset;  
  
  void Read( FPackageFileIn& Pkg );
};

/*-----------------------------------------------------------------------------
 * FImport
 * A data struct that contains information about a single object
 * which is required for the complete loading of a package
-----------------------------------------------------------------------------*/
struct DLL_EXPORT FImport
{
  idx ClassPackage;
  idx ClassName;
  int Package;
  idx ObjectName;
  
  void Read( FPackageFileIn& Pkg );
};

/*-----------------------------------------------------------------------------
 * UPackageHeader
 * A data struct which contains information about a single package
 * This information is always located at the top of a package file
-----------------------------------------------------------------------------*/
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
 * An object which keeps track of any information relating to a package
-----------------------------------------------------------------------------*/
class UPackage : public UObject
{
  DECLARE_CLASS( UPackage, UObject, 0 )
  UPackage();
  
  virtual bool Load( const char* File );
  virtual bool Save( const char* File = NULL );
  virtual void Close();
  
  UPackageHeader* GetHeader();
  FNameEntry*     GetNameEntry( size_t Index );
  FImport*        GetImport( size_t Index );
  FExport*        GetExport( size_t Index );
  const char*     GetFilePath();
  const char*     GetFileName();
  int             GetPackageVer();
  
  int FindName( const char* Name );
  
  // Name resolution
  const char* ResolveNameFromIdx( idx Index );
  const char* ResolveNameFromObjRef( int ObjRef );
  FExport*    ResolveExportFromName( int Name );
  
  // Object reading
  bool LoadObject( UObject** Obj, const char* ObjName );
  
  // Accessors 
  String& GetPackageName();
  
protected:
    
  bool BeginLoad( FExport* Export );
  void EndLoad();
  
  String Name;
  String Path;
  FileStream* Stream;
  UPackageHeader Header;
  Array<FNameEntry>* Names;
  Array<FExport>*    Exports;
  Array<FImport>*    Imports;

  // Global package variables
  static EPkgLoadOpts LoadOpts;
};

/*-----------------------------------------------------------------------------
 * UObjectManager
 * Manages the loading and saving of any objects from a package
-----------------------------------------------------------------------------*/
class UObjectManager
{
public:
  UObjectManager();
  ~UObjectManager();
  
  bool LoadPkg( const char* Filepath );
  
  template <class T> T* CreateObject( const char* PkgName, const char* ObjName, const char* ClassName = NULL )
  {
    UPackage* Pkg = NULL;
    char* RealPkgName;
    const char* ExtSeperator = strchr( PkgName, '.' );
    
    if (ExtSeperator != NULL)
    {
      size_t ExtOffset = (size_t)PtrSubtract( ExtSeperator, PkgName );
      RealPkgName = strdup( PkgName );
      RealPkgName[ExtOffset] = '\0';
    }
    else
    {
      RealPkgName = (char*)PkgName;
    }
    
    for ( int i = 0; i < Packages->Size(); i++ )
    {
      Pkg = Packages->Data()[i];
      if ( Pkg->GetPackageName() == RealPkgName )
        break;
    }
    
    if (Pkg == NULL)
      return NULL;
    
    T* Obj = new T();
    if ( !Pkg->LoadObject( (UObject**)&Obj, ObjName ) )
    {
      if (Obj)
        delete Obj;
      return NULL;
    }
    
    if ( RealPkgName != PkgName )
      free( RealPkgName );
    
    return Obj;
  }
  
private:
  Array<UPackage*>* Packages;
};

