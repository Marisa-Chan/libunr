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
 * UClass.cpp - Objects that describe classes of Unreal objects
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "UClass.h"
#include "UPackage.h"

// UField
UField::~UField()
{
  SuperField->RefCnt--;
  Next->RefCnt--;
}

void UField::LoadFromPackage( FPackageFileIn& In )
{
  Super::LoadFromPackage( In );
  
  idx SuperIdx;
  idx NextIdx;
  
  In >> CINDEX( SuperIdx );
  In >> CINDEX( NextIdx );
  
  if ( SuperIdx )
    SuperField = (UField*)UPackage::StaticLoadObject( Pkg, SuperIdx );
  
  if ( NextIdx )
    Next = (UField*)UPackage::StaticLoadObject( Pkg, NextIdx );
}

// UStruct
UStruct::~UStruct()
{
}

void UStruct::LoadFromPackage( FPackageFileIn& In )
{
  Super::LoadFromPackage( In );
  
  idx ScriptTextIdx;
  idx ChildIdx;
  idx FriendlyNameIdx;
  
  In >> CINDEX( ScriptTextIdx );
  In >> CINDEX( ChildIdx );
  In >> CINDEX( FriendlyNameIdx );
  
//   // If we aren't an editor, then we don't need the script text
//   if ( USystem::bIsEditor )
    ScriptText = (UTextBuffer*)UPackage::StaticLoadObject( Pkg, ScriptTextIdx );
    
  Children = (UField*)UPackage::StaticLoadObject( Pkg, ChildIdx );
  FriendlyName = Pkg->ResolveNameFromIdx( FriendlyNameIdx );
  In >> Line;
  In >> TextPos;
  In >> ScriptSize;
  
  ScriptCode = UObject::LoadScriptCode( In, ScriptSize );
}

// UState
UState::~UState()
{
}

void UState::LoadFromPackage( FPackageFileIn& In )
{
  Super::LoadFromPackage( In );
  
  In >> ProbeMask;
  In >> IgnoreMask;
  In >> LabelTableOffset;
  In >> StateFlags;
}

// UClass
UClass::UClass()
{
  
}

UClass::UClass( u32 Flags )
{
  ClassFlags = Flags;
}

UClass::~UClass()
{
}

void UClass::LoadFromPackage( FPackageFileIn& In )
{
  Super::LoadFromPackage( In );
  
  In >> ClassFlags;
  In.Read( ClassGuid, sizeof( ClassGuid ) );
  
  idx DepCount;
  In >> CINDEX( DepCount );
  Dependencies.Reserve( DepCount );
  
  FDependency Dep;
  for ( int i = 0; i < DepCount; i++ )
  {
    idx DepObjRef;
    In >> CINDEX( DepObjRef );
    
    if ( stricmp( Pkg->ResolveNameFromObjRef( DepObjRef ), FriendlyName ) )
      Dep.Class = (UClass*)UPackage::StaticLoadObject( Pkg, DepObjRef );
    
    In >> Dep.Deep;
    In >> Dep.ScriptTextCRC;
    
    Dependencies.PushBack( Dep );
  }
  
  // I don't actually know what these are for, so just load their values until we know what we need them for
  idx NumPkgImports;
  In >> CINDEX( NumPkgImports );
  PackageImports.Reserve( NumPkgImports );
  
  for ( int i = 0; i < NumPkgImports; i++ )
  {
    idx PkgImport;
    In >> CINDEX( PkgImport );
    PackageImports.PushBack( PkgImport );
  }
  
  if ( In.Ver >= PKG_VER_UN_220 - 1 ) // package version 62 ??
  {
    idx ClassWithinIdx;
    idx ClassConfigNameIdx;
    
    In >> CINDEX( ClassWithinIdx );
    In >> CINDEX( ClassConfigNameIdx );
    
    ClassWithin = (UClass*)UPackage::StaticLoadObject( Pkg, ClassWithinIdx );
    ClassConfigName = Pkg->ResolveNameFromIdx( ClassConfigNameIdx );
  }
  
  ReadProperties( In );
}

bool UClass::IsNative()
{
  // NOTE: this is NOT checking class flags
  return (Flags & RF_Native) != 0;
}
