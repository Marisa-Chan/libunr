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

#include "Core/UProperty.h"
#include "Core/UPackage.h"
#include "Core/UObjGetSet.h"

u8 UProperty::PropertySizes[8] = { 1, 2, 4, 12, 16, 1, 2, 4 };

// For reusing pointers for different contexts
// 
// WARNING: This implies that the application cannot access memory addresses
// higher than 0x80000000 in the 32bit case or 0x800000000000000 in the 64 bit case
//
// This gets the upper-most byte of an address and returns it.
// Additionally, this also masks out said byte from the address
static inline int ConvertPtrForPropFmt( void** Ptr )
{
  size_t mask = ((size_t)0xf << (ARCH_BITS-1));
  int t = ((size_t)*Ptr) & mask;
  *Ptr = (void*)(((size_t)*Ptr) & ~mask);
  return t;
}

#define PTR_ARRAY 0xAA
#define PTR_MAP   0xAB

template<class T> static inline T GetValueFromArray( Array<T>* A, UProperty* Prop, int Idx )
{
  
}

template<class T> static inline T GetValueFromSrc( void* Src, UProperty* Prop, int Idx )
{
  void* RealSrc = Src;
  int   Type = ConvertPtrForPropFmt( &RealSrc );

  switch( Type )
  {
    case PTR_ARRAY:
      return GetValueFromArray<T>( (Array<T>*)RealSrc, Prop, Idx );
    case PTR_MAP:
      break; // TODO
    default:
      return ((UObject*)RealSrc)->GetProperty<T>( Prop, Idx );
  }
  
  // Shouldn't ever get here
  return (T)0;
}

// TODO: Create variable in Object to check for uninitialized properties?
UProperty::UProperty()
  : UField()
{
  ArrayDim = 0;
  ElementSize = 0;
  PropertyFlags = 0;
  Category = 0;
  Offset = 0;
}

UProperty::UProperty( int InNameIdx )
  : UField()
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

void UProperty::Load()
{
  Super::Load();
  
  *PkgFile >> ArrayDim;
  *PkgFile >> ElementSize;
  *PkgFile >> PropertyFlags;
  *PkgFile >> CINDEX( Category );
  
  if ( PropertyFlags & CPF_Net )
    *PkgFile >> ReplicationOffset;
  
  if ( Outer->Flags & RF_Native )
    Offset = GetNativeOffset( Outer->Name, Name );

  if ( Outer->Class == UClass::StaticClass() && PropertyFlags & CPF_GlobalConfig )
    GlobalClass = (UClass*)Outer;
}

u32 UProperty::GetNativeOffset( const char* ClassName, const char* PropName )
{
  FNativePropertyList* NativePropList = NULL;
  u32 Offset = MAX_UINT32;

  FHash ClassHash = FnvHashString( ClassName );
  for ( size_t i = 0; i < NativePropertyLists->Size() && i != MAX_SIZE; i++ )
  {
    // Can you take the address of an overloaded operator[] ?
    NativePropList = NativePropertyLists->Data()[i];    
    if ( ClassHash == NativePropList->Hash )
      break;
  }
 
  char* UpperPropName = strupper( PropName );
  FHash PropHash = FnvHashString( UpperPropName );
  for ( size_t i = 0; i < NativePropList->Num; i++ )
  {
    if ( NativePropList->Properties[i].Hash == PropHash )
    {
      Offset = NativePropList->Properties[i].Offset;
      break;
    }
  }
 
  xstl::Free( UpperPropName );
  return Offset;
}

size_t UProperty::FormatForScript( UPackage* ScrPkg, char* Buf, size_t BufLen, void* Src, int Idx )
{
  xstl::Set( Buf, 0, BufLen );
  return 0;
}

void UByteProperty::Load()
{
  Super::Load();
 
  idx EnumType = 0;
  *PkgFile >> CINDEX( EnumType );
  if ( EnumType )
    Enum = (UEnum*)LoadObject( EnumType, UEnum::StaticClass(), Outer );

  ElementSize = 1;
}

size_t UByteProperty::FormatForScript( UPackage* ScrPkg, char* Buf, size_t BufRemain, void* Src, int Idx )
{
  u8 Value = GetValueFromSrc<u8>( Src, this, Idx );
  char RBuf[64] = { 0 };
  int RBufLen = 0;

  if ( Enum == NULL )
    RBufLen = snprintf( RBuf, sizeof( RBuf ), "%i\r\n", Value );
  else
    RBufLen = snprintf( RBuf, sizeof( RBuf ), "%s\r\n", Enum->Names[Value] );
 
  if ( RBufLen <= BufRemain )
    strncat( Buf, RBuf, RBufLen );

  return RBufLen;
}

void UIntProperty::Load()
{
  Super::Load();
  ElementSize = 4;
}

size_t UIntProperty::FormatForScript( UPackage* ScrPkg, char* Buf, size_t BufRemain, void* Src, int Idx )
{ 
  char RBuf[32] = { 0 };
  int RBufLen = snprintf( RBuf, sizeof( RBuf ), "%i\r\n", 
    GetValueFromSrc<int>( Src, this, Idx ) );
  
  if ( RBufLen <= BufRemain )
    strncat( Buf, RBuf, RBufLen );

  return RBufLen;
}

void UBoolProperty::Load()
{
  Super::Load();
  ElementSize = sizeof( bool );
}

size_t UBoolProperty::FormatForScript( UPackage* ScrPkg, char* Buf, size_t BufRemain, void* Src, int Idx )
{
  bool Value = GetValueFromSrc<bool>( Src, this, Idx );
  if ( Value && 5 <= BufRemain )
  {
    strncat( Buf, "true\r\n", 5 );
    return 5;
  }
  else if ( Value && 6 <= BufRemain )
  {
    strncat( Buf, "false\r\n", 6 );
    return 6;
  }
}

void UFloatProperty::Load()
{
  Super::Load();
  ElementSize = 4;
}

size_t UFloatProperty::FormatForScript( UPackage* ScrPkg, char* Buf, size_t BufRemain, void* Src, int Idx )
{
  char RBuf[16] = { 0 };
  size_t RBufLen = snprintf( RBuf, RBufLen, "%.6f\r\n",
    GetValueFromSrc<float>( Src, this, Idx ) );

  if ( RBufLen <= BufRemain )
    strncat( Buf, RBuf, RBufLen );

  return RBufLen;
}

void UNameProperty::Load()
{
  Super::Load();
  ElementSize = 4;
}

size_t UNameProperty::FormatForScript( UPackage* ScrPkg, char* Buf, size_t BufRemain, void* Src, int Idx )
{
  char* NameData = Pkg->GetNameEntry( GetValueFromSrc<idx>( Src, this, Idx ) )->Data;
  size_t NameLen = strlen( NameData ) + 1;

  if ( NameLen <= BufRemain )
  {
    strncat( Buf, NameData, NameLen-1 );
    strncat( Buf, "\r\n", 1 );
  }

  return NameLen;
}

void UStrProperty::Load()
{
  Super::Load();
  ElementSize = sizeof( char* );
}

size_t UStrProperty::FormatForScript( UPackage* ScrPkg, char* Buf, size_t BufRemain, void* Src, int Idx )
{
  char* StrData = GetValueFromSrc<char*>( Src, this, Idx );
  size_t Len    = strlen( StrData ) + 1;

  if ( Len <= BufRemain )
  {
    strncat( Buf, "\"", 1 );
    strncat( Buf, StrData, Len-1 );
    strncat( Buf, "\"\r\n", 3 );
  }

  return Len;
}

void UStringProperty::Load()
{
  Super::Load();
}

size_t UStringProperty::FormatForScript( UPackage* ScrPkg, char* Buf, size_t BufRemain, void* Src, int Idx )
{
  Logf( LOG_WARN, "Defaut property of type 'StringProperty' being written to script" );
  return 0;
}

void UObjectProperty::Load()
{
  Super::Load();
  ElementSize = sizeof( UObject* );

  idx ObjTypeIdx = 0;
  *PkgFile >> CINDEX( ObjTypeIdx );
  ObjectType = (UClass*)LoadObject( ObjTypeIdx, UClass::StaticClass(), Outer );
}

size_t UObjectProperty::FormatForScript( UPackage* ScrPkg, char* Buf, size_t BufRemain, void* Src, int Idx )
{
  char RBuf[64] = { 0 };
  size_t RBufLen = 0;
  
  UObject* Value = GetValueFromSrc<UObject*>( Src, this, Idx );
  if ( Value == NULL )
    RBufLen = snprintf( RBuf, sizeof( RBuf ), "None\r\n" );
  else
    RBufLen = snprintf( RBuf, sizeof( RBuf ), "%s'%s'\r\n", 
      ObjectType->Name, ScrPkg->GetFullObjName( Value->Name ) );

  if ( RBufLen <= BufRemain )
    strncat( Buf, RBuf, RBufLen );

  return RBufLen;
}

void UClassProperty::Load()
{
  Super::Load();
  ElementSize = sizeof( UClass* );

  idx ClassIdx = 0;
  *PkgFile >> CINDEX( ClassIdx );
  ClassObj = (UClass*)LoadObject( ClassIdx, UClass::StaticClass(), Outer );
}

size_t UClassProperty::FormatForScript( UPackage* ScrPkg, char* Buf, size_t BufLen, void* Src, int Idx )
{
  return Super::FormatForScript( Buf, BufLen, Src, Idx );
}

void UStructProperty::Load()
{
  Super::Load();

  idx StructIdx = 0;
  *PkgFile >> CINDEX( StructIdx );
  Struct = (UStruct*)LoadObject( StructIdx, UStruct::StaticClass(), Outer );

  ElementSize = Struct->StructSize;
}

size_t UStructProperty::FormatForScript( UPackage* ScrPkg, char* Buf, size_t BufLen, void* Src, int Idx )
{
  size_t SubBufLen = 1;
  char SubBuf[256] = { 0 };
  SubBuf[0] = '(';

  for ( UField* ChildIter = Struct->Children; ChildIter != NULL; ChildIter = ChildIter->Next )
  {
    if ( ChildIter->IsA( UProperty::StaticClass() ) )
    {
      char SubBuf2[32] = { 0 };
      size_t SubBuf2Len = 0;
        
      UProperty* ChildProp = (UProperty*)ChildIter;
      for ( int i = 0; i < ChildProp->ArrayDim; i++ )
      {
        SubBuf2Len = ChildProp->FormatForScript( SubBuf2, sizeof( SubBuf2 ), 
            PtrAdd( Src, Offset ), i );
        SubBufLen += SubBuf2Len;
        strncat( SubBuf, SubBuf2, 
          SubBuf2Len > sizeof( SubBuf2 ) ? sizeof( SubBuf2 ) : SubBuf2Len );

        if ( i < ChildProp->ArrayDim - 1 )
        {
          strcat( SubBuf, "," );
          SubBufLen++;
        }
      }
    }
  }

  strcat( SubBuf, ")\r\n" );
  if ( SubBufLen <= BufLen )
    strncpy( Buf, SubBuf, SubBufLen );

  return SubBufLen;
}

void UArrayProperty::Load()
{
  Super::Load();

  idx InnerIdx;
  *PkgFile >> CINDEX( InnerIdx );
  Inner = (UProperty*)LoadObject( InnerIdx, NULL, Outer );
}

size_t UArrayProperty::FormatForScript( UPackage* ScrPkg, char* Buf, size_t BufRemain, void* Src, int Idx )
{
  /*for (   )
  {
    char SubBuf[256] = { 0 };
    size_t SubBufLen = snprintf( SubBuf, sizeof( SubBuf ), "\t%s(%i)="
  }*/
  return 0;
}

void UFixedArrayProperty::Load()
{
  Super::Load();
  Logf( LOG_CRIT, "Go pop '%s' in UTPT and see how to load a FixedArrayProperty", Pkg->Name );
  exit( -1 ); // <- can we not do this
}

size_t UFixedArrayProperty::FormatForScript( UPackage* ScrPkg, char* Buf, size_t BufRemain, void* Src, int Idx )
{
  Logf( LOG_WARN, "Defaut property of type 'FixedArrayProperty' being written to script" );
  return 0;
}

void UMapProperty::Load()
{
  Super::Load();
  Logf( LOG_CRIT, "Go pop '%s' in UTPT and see how to load a MapProperty", Pkg->Name );
  exit( -1 ); // <- can we not do this
}

size_t UMapProperty::FormatForScript( UPackage* ScrPkg, char* Buf, size_t BufRemain, void* Src, int Idx )
{
  Logf( LOG_WARN, "Defaut property of type 'MapProperty' being written to script" );
  return 0;
}

UByteProperty::~UByteProperty()
{}

UIntProperty::~UIntProperty()
{}

UBoolProperty::~UBoolProperty()
{}

UFloatProperty::~UFloatProperty()
{}

UObjectProperty::~UObjectProperty()
{
  if ( ObjectType != NULL )
    ObjectType->DelRef();
}

UNameProperty::~UNameProperty()
{}

UStringProperty::~UStringProperty()
{}

UClassProperty::~UClassProperty()
{}

UArrayProperty::~UArrayProperty()
{
  if ( Inner != NULL )
    Inner->DelRef();
}

UStructProperty::~UStructProperty()
{
  if ( Struct != NULL )
    Struct->DelRef();
}

UStrProperty::~UStrProperty()
{}

UMapProperty::~UMapProperty()
{}

UFixedArrayProperty::~UFixedArrayProperty()
{}

