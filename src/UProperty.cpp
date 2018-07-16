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
 * UProperty.cpp - Objects that describe properties inside of objects
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "UProperty.h"
#include "UPackage.h"

u8 UProperty::PropertySizes[8] = { 1, 2, 4, 12, 16, 1, 2, 4 };

// TODO: Create variable in Object to check for uninitialized properties?
UProperty::UProperty()
{
  ArrayDim = 0;
  ElementSize = 0;
  PropertyFlags = 0;
  Category = 0;
	Offset = 0;
}

UProperty::UProperty( int InNameIdx )
{
  NameIdx = InNameIdx;
  ArrayDim = 0;
  ElementSize = 0;
  PropertyFlags = 0;
  Category = 0;
  Offset = 0;
}

UProperty::~UProperty()
{
}

void UProperty::LoadFromPackage( FPackageFileIn& In )
{
  Super::LoadFromPackage( In );
  
  In >> ArrayDim;
  In >> ElementSize;
  In >> PropertyFlags;
  In >> CINDEX( Category );
  
  if ( PropertyFlags & CPF_Net )
    In >> ReplicationOffset;
  
  if ( PropertyFlags & CPF_Native )
    Offset = GetNativeOffset( Outer->Name, Name );
  // else
    // no idea how thats gonna work yet
    // probably have a pool of UScript properties for one object and just grab the next free object there
}

u32 UProperty::GetNativeOffset( const char* ClassName, const char* PropName )
{
  FNativePropertyList* NativePropList;
  size_t ClassHash = Fnv1aHashString( ClassName );
  for ( size_t i = 0; i < NativePropertyLists->Size() && i != MAX_SIZE; i++ )
  {
    NativePropList = &NativePropertyLists->Data()[i]; // Can you take the address of an overloaded operator[] ?
    if ( ClassHash == NativePropList->Hash )
      break;
  }
  
  size_t PropHash = Fnv1aHashString( PropName );
  for ( size_t i = 0; i < NativePropList->Num; i++ )
  {
    if ( NativePropList->Properties[i].Hash == PropHash )
      return NativePropList->Properties[i].Offset;
  }
  
  return MAX_UINT32;
}

void UByteProperty::LoadFromPackage( FPackageFileIn& In )
{
  Super::LoadFromPackage( In );
 
	idx EnumType = 0;
  In >> CINDEX( EnumType );
  if ( EnumType )
    Enum = (UEnum*)UPackage::StaticLoadObject( Pkg, EnumType, UEnum::StaticClass() );
}

void UIntProperty::LoadFromPackage( FPackageFileIn& In )
{
  Super::LoadFromPackage( In );
}

void UBoolProperty::LoadFromPackage( FPackageFileIn& In )
{
  Super::LoadFromPackage( In );
}

void UFloatProperty::LoadFromPackage( FPackageFileIn& In )
{
  Super::LoadFromPackage( In );
}

void UObjectProperty::LoadFromPackage( FPackageFileIn& In )
{
  Super::LoadFromPackage( In );
}

