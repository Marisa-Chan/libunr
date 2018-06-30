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
#include "Memory.h"
#include "UObject.h"
#include "UClass.h"
#include "UProperty.h"
#include "UPackage.h"

FNativePropertyList::FNativePropertyList( size_t InHash, size_t InNum )
{
  Hash = InHash;
  Num = InNum;
  Added = 0;
  Properties = new FNativePropertyLink[Num];
}

void FNativePropertyList::AddProperty( const char* Name, u32 Offset )
{
  if ( LIKELY( Added < Num ) )
  {
    Properties[Added].Hash = Fnv1aHashString( Name );
    Properties[Added].Offset = Offset;
    Added++;
  }
}

void UObject::StaticLinkNativeProperties()
{
  if ( StaticInitNativePropList( 5 ) )
  {
    LINK_NATIVE_ARRAY( UObject, ObjectInternal );
    LINK_NATIVE_PROPERTY( UObject, Outer );
    LINK_NATIVE_PROPERTY( UObject, Flags );
    LINK_NATIVE_PROPERTY( UObject, Name );
    LINK_NATIVE_PROPERTY( UObject, Class );
  }
}

UClass* UObject::StaticAllocateClass( u32 Flags )
{
  return new UClass( Flags );
}

FPackageFileIn& operator>>( FPackageFileIn& Ar, UObject& Obj )
{
  Obj.LoadFromPackage( Ar );
  return Ar;
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
  Flags = 0;
  Outer = NULL;
  Class = ObjectClass;
  Pkg = NULL;
  RefCnt = 1;
  Properties = new FPropertyHash();
}

UObject::~UObject()
{
  delete Properties;
}

bool UObject::ExportToFile()
{
  return false;
}

void UObject::LoadFromPackage( FPackageFileIn& Ar )
{
  if ( Flags & RF_HasStack )
  {
    // Load stack info
  }
  
  if ( !ObjectClass.IsA( UClass::StaticClass() ) )
  {
    // Load properties
  }
  
  return;
}

void UObject::SetProperties( UPackage* InPkg, int InExpIdx, int InNameIdx )
{
  Pkg = InPkg;
  ExpIdx = InExpIdx;
  NameIdx = InNameIdx;
  Name = Pkg->ResolveNameFromIdx( NameIdx );
}

void UObject::ReadPropertyList( FPackageFileIn& In )
{
  
}
