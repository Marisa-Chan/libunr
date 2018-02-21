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
 * FMemory.cpp - Class for memory based operations
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "FMemory.h"

void* FMemory::Malloc( size_t Num )
{
  if ( Num )
    return malloc( Num );
    
  return NULL;
}

void* FMemory::Realloc( void* Ptr, size_t Num )
{
  if ( Num && Ptr )
    return realloc( Ptr, Num );
    
  // Memory leak will incur if invalid parameters are passed to Realloc
  return NULL;
}

void* FMemory::Calloc( size_t Num, size_t Size )
{
  if ( Num && Size )
    return calloc( Num, Size );
    
  return NULL;
}

void FMemory::Free( void* Ptr )
{
  if ( Ptr )
    free( Ptr );
}

// TODO: wrap 64-bit copy code in preprocessor if statement
void* FMemory::Copy( void* Dest, size_t DestLen, const void* Src, size_t Num )
{
  if ( !Dest || !Src || !Num || DestLen < Num )
    return NULL;

    
  u64* Dest64 = ( u64* )Dest;
  const u64* Src64 = (const u64* )Src;

#if defined LIBUNR_64BIT
  while ( Num > 8 )
  {
    *Dest64++ = *Src64++;
    Num -= 8;
  }
#endif

  u32* Dest32 = ( u32* )Dest64;
  const u32* Src32 = ( u32* )Dest64;
  
  while ( Num > 4 )
  {
    *Dest32++ = *Src32++;
    Num -= 4;
  }
  
  // No reason to check if we can copy two bytes in one write
  // At best, there will only be one 2-byte transfer that ever
  // occurs
  
  u8* Dest8 = ( u8* )Dest32;
  const u8* Src8 = ( u8* )Src32;
  
  while ( Num > 0 )
  {
    *Dest8++ = *Src8++;
    Num--;
  }
  
  return Dest;
}

void* FMemory::Move( void* Dest, size_t DestLen, const void* Src, size_t Num )
{
  // do we need to check twice?
  if ( !Dest || !Src || !Num || DestLen < Num )
    return NULL;
    
  void* Temp = FMemory::Malloc( DestLen );
  FMemory::Copy( Temp, DestLen, Src, Num );
  FMemory::Copy( Dest, DestLen, Temp, DestLen );
  FMemory::Free( Temp );
  
  return Dest;
}

// do we really need to return an int?
// need architecture-specific preprocessor stuff here too...
bool FMemory::Compare( const void* Ptr1, const void* Ptr2, size_t Num )
{
  const u64* Ptr1_64 = ( const u64* )Ptr1;
  const u64* Ptr2_64 = ( const u64* )Ptr2;
  
#if defined LIBUNR_64BIT
  while ( Num > 8 )
  {
    if ( *Ptr1_64++ != *Ptr2_64++ )
      return false;
      
    Num -= 8;
  }
#endif

  const u32* Ptr1_32 = ( const u32* )Ptr1_64;
  const u32* Ptr2_32 = ( const u32* )Ptr2_64;
  
  while ( Num > 4 )
  {
    if ( *Ptr1_32++ != *Ptr2_32++ )
      return false;
      
    Num -= 4;
  }
  
  const u8* Ptr1_8 = ( const u8* )Ptr1_8;
  const u8* Ptr2_8 = ( const u8* )Ptr2_8;
  
  while ( Num > 0 )
  {
    if ( *Ptr1_8++ != *Ptr2_8++ )
      return false;
      
    Num--;
  }
  
  return true;
}

const void* Search( const void* Ptr, u8 Value, size_t Num )
{
  const u8* Ptr8 = ( const u8* )Ptr;
  
  while ( Num > 0 )
  {
    if ( *Ptr8 == Value )
      return Ptr8;
      
    Ptr8++;
    Num--;
  }
  
  return NULL;
}

void* Search( void* Ptr, u8 Value, size_t Num )
{
  u8* Ptr8 = ( u8* )Ptr;
  
  while ( Num > 0 )
  {
    if ( *Ptr8 == Value )
      return Ptr8;
      
    Ptr8++;
    Num--;
  }
  
  return NULL;
}

//========================================================================
// EOF
//========================================================================
