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
 * FString.cpp - string helper implementation
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include <stdlib.h>
#include "Util/FMacro.h"
#include "Util/FString.h"
#include "Core/UPackage.h"

/*-----------------------------------------------------------------------------
 * FString
-----------------------------------------------------------------------------*/
FString::FString() 
{
  NumElements = 0;
  NumReserved = 0;
  Array = NULL;
}

FString::FString( const FString& Str )
{
  NumElements = Str.NumElements;
  NumReserved = Str.NumElements * 2;
  Array = (char*)malloc( NumReserved );
  memset( Array, 0, NumReserved );
}

FString::FString( const FString& Str, size_t Pos, size_t Len )
{
  NumElements = 0;
  NumReserved = Len * 2;
  Array = (char*)malloc( NumReserved );
  Append( Str, Pos, Len );
}

FString::FString( const char* s )
{
  NumElements = strlen( s );
  NumReserved = NumElements * 2;
  Array = (char*)malloc( NumReserved );
  memcpy( Array, s, NumElements + 1 );
}

FString::FString( const char* s, size_t n )
{
  NumElements = n;
  NumReserved = n * 2;
  Array = (char*)malloc( NumReserved );
  memcpy( Array, s, n + 1 );
}

FString::FString( size_t n, char c )
{
  NumElements = n;
  NumReserved = n * 2;
  Array = (char*)malloc( NumReserved );
  memset( Array, c, NumElements );
  Array[NumElements] = '\0';
}

FString::FString( int I )
{
  static char Buf[12];
  char* Ptr = Buf + 12;
  *--Ptr = '\0';

  bool Neg = (I < 0);

  do
  {
    *--Ptr = '0' - (I % 10);
    I /= 10;
  } while ( I );

  if ( Neg )
    *--Ptr = '-';

  NumElements = 0;
  NumReserved = 0;
  Array = NULL;
  Assign( Ptr );
}

FString::FString( i64 I )
{
  static char Buf[22];
  char* Ptr = Buf + 22;
  *--Ptr = '\0';

  bool Neg = (I < 0);

  do
  {
    *--Ptr = '0' - (I % 10);
    I /= 10;
  } while ( I );

  if ( Neg )
    *--Ptr = '-';

  NumElements = 0;
  NumReserved = 0;
  Array = NULL;
  Assign( Ptr );
}

FString::FString( u32 U )
{
  static char Buf[12];
  char* Ptr = Buf + 12;
  *--Ptr = '\0';

  do
  {
    *--Ptr = '0' - (U % 10);
    U /= 10;
  } while ( U );

  NumElements = 0;
  NumReserved = 12;
  Array = NULL;
  Assign( Ptr );
}

FString::FString( u64 U )
{
  static char Buf[22];
  char* Ptr = Buf + 22;
  *--Ptr = '\0';

  do
  {
    *--Ptr = '0' - (U % 10);
    U /= 10;
  } while ( U );

  NumElements = 0;
  NumReserved = 0;
  Array = NULL;
  Assign( Ptr );
}

FString::FString( float F )
{
  Resize( 64 );
  sprintf( Array, "%0.6f", F );
  NumElements = strlen( Array );
}

FString::FString( double D )
{
  Resize( 64 );
  sprintf( Array, "%0.12lf", D );
  NumElements = strlen( Array );
}

FString::FString( bool B )
{
  const char* Str = (B) ? "true" : "false";
  NumElements = 0;
  NumReserved = 6;
  Array = (char*)malloc( NumReserved );
  Append( Str );
}

FString::~FString()
{
  free( Array );
  Array = NULL;
}

void FString::Resize( size_t n )
{
  Reserve( n );

  if ( n > NumElements )
    memset( &Array[NumElements], 0, (n - NumElements) );

  NumElements = n;
}

void FString::Resize( size_t n, char c )
{
  Reserve( n );

  if ( n > NumElements )
    memset( &Array[NumElements], c, (n - NumElements) );

  NumElements = n;
}

void FString::Reserve( size_t n )
{
  // Call destructors if needed
  if ( n < NumElements )
  {
    NumElements = n;
    Array[NumElements - 1] = '\0';
  }

  char* NewArray = NULL;
  if ( n == 0 )
  {
    free( Array );
  }
  else
  {
    NewArray = (char*)realloc( Array, n );
    if ( NewArray == NULL )
    {
      // throw an error here if we fail
      return;
    }
  }

  Array = NewArray;
  NumReserved = n;
}

void FString::Reclaim()
{
  Reserve( NumElements );
}

FString& FString::Append( const FString& Str )
{
  return Append( Str.Array, Str.NumElements );
}

FString& FString::Append( const FString& Str, size_t SubPos, size_t SubLen )
{
  if ( SubLen == MAX_SIZE )
    SubLen = Str.NumElements - SubPos;

  return Append( &Str.Array[SubPos], SubLen );
}

FString& FString::Append( const char* s )
{
  return Append( s, strlen( s ) );
}

FString& FString::Append( const char* s, size_t n )
{
  if ( !Array || n + 1 > NumReserved - NumElements )
    Reserve( (NumReserved + n) * 2 );

  for ( int i = 0; i < n; i++ )
    Array[i + NumElements] = s[i];

  NumElements += n;
  Array[NumElements] = '\0';

  return *this;
}

FString& FString::Append( size_t n, char c )
{
  Resize( NumElements + n, c );
  return *this;
}

void FString::PushBack( char c )
{
  if ( !Array || NumElements+1 == NumReserved )
    Reserve( (NumReserved * 2) + 4 );

  Array[NumElements] = c;
  Array[++NumElements] = '\0';
}

FString& FString::Assign( const FString& Str )
{
  return Assign( Str.Array, Str.NumElements );
}

FString& FString::Assign( const FString& Str, size_t SubPos, size_t SubLen )
{
  if ( SubLen == MAX_SIZE )
    SubLen = Str.NumElements - SubPos;

  return Assign( &Str.Array[SubPos], SubLen );
}
FString& FString::Assign( const char* s )
{
  return Assign( s, strlen( s ) );
}

FString& FString::Assign( const char* s, size_t n )
{
  if ( n + 1 > NumReserved )
    Reserve( n * 2 );

  memcpy( Array, s, n + 1 );
  NumElements = n;

  return *this;
}

FString& FString::Assign( size_t n, char c )
{
  Resize( n );
  memset( Array, c, NumElements );
  return *this;
}

FString& FString::Insert( const FString& Str, size_t Pos )
{
  return Insert( Str.Array, Str.NumElements, Pos );
}

FString& FString::Insert( const FString& Str, size_t Pos, size_t SubPos, size_t SubLen )
{
  if ( SubLen == MAX_SIZE )
    SubLen = Str.NumElements - SubPos;

  if ( NumElements + SubLen + 1 >= NumReserved )
    Reserve( (NumElements + SubLen) * 2 );

  memmove( &Array[Pos + SubLen], &Array[Pos], NumElements - Pos );
  NumElements += SubLen;
  memcpy( &Array[Pos], &Str.Array[SubPos], SubLen );
  Array[NumElements] = '\0';

  return *this;
}

FString& FString::Insert( const char* s, size_t Pos )
{
  return Insert( s, strlen( s ), Pos );
}

FString& FString::Insert( const char* s, size_t n, size_t Pos )
{
  if ( NumElements + n + 1 >= NumReserved )
    Reserve( (NumElements + n) * 2 );

  memmove( &Array[Pos + n], &Array[Pos], NumElements - Pos );
  NumElements += n;
  memcpy( &Array[Pos], s, n );
  Array[NumElements] = '\0';

  return *this;
}

FString& FString::Insert( size_t Pos, size_t n, char c )
{
  if ( NumElements + n + 1 >= NumReserved )
    Reserve( (NumElements + n) * 2 );

  memmove( &Array[Pos + n], &Array[Pos], NumElements - Pos );
  NumElements += n;
  memset( &Array[Pos], c, n );
  Array[NumElements] = '\0';

  return *this;
}

FString& FString::Erase( size_t Pos, size_t Len )
{
  if ( Len == MAX_SIZE )
  {
    NumElements = Pos;
  }
  else
  {
    NumElements -= Len;
    if ( Pos < NumElements )
      memmove( &Array[Pos], &Array[Pos + Len], NumElements - Len );
  }

  Array[NumElements] = '\0';
  return *this;
}

void FString::Clear()
{
  NumElements = 0;
  Array[0] = '\0';
}

void FString::Swap( FString& Str )
{
  char* TmpArray = Array;
  size_t TmpElements = NumElements;
  size_t TmpReserved = NumReserved;

  Array = Str.Array;
  NumElements = Str.NumElements;
  NumReserved = Str.NumReserved;

  Str.Array = TmpArray;
  Str.NumElements = TmpElements;
  Str.NumReserved = TmpReserved;
}

void FString::PopBack()
{
  Array[--NumElements] = '\0';
}

FString& FString::Replace( size_t Pos, size_t Len, const char* s )
{
  Len = MIN( Len, strlen( s ) );

  if ( Pos + Len > NumElements )
    Len = NumElements - Pos;

  memcpy( &Array[Pos], s, Len );
  return *this;
}

FString& FString::Replace( size_t Pos, size_t Len, const char* s, size_t n )
{
  Len = MIN( MIN( Len, n ), strlen( s ) );

  if ( Pos + Len > NumElements )
    Len = NumElements - Pos;

  memcpy( &Array[Pos], s, Len );
  return *this;
}

FString& FString::Replace( size_t Pos, size_t Len, size_t n, char c )
{
  Len = MIN( Len, n );

  if ( Pos + Len > NumElements )
    Len = NumElements - Pos;

  for ( int i = Pos; i < Len; i++ )
    Array[i] = c;

  return *this;
}

size_t FString::Find( const FString& Str, size_t Pos )
{
  char* Result = &Array[Pos];
  char* StrPtr = Str.Array;

  int Len = Str.Length();
  int Matched = 0;
  do
  {
    Matched = (*Result == *StrPtr) ? Matched + 1 : 0;
    StrPtr = (Matched) ? StrPtr + 1 : Str.Array;
  } while ( *Result++ );

  if ( Matched == Len )
    return MAX_SIZE;

  return (size_t)(Result - Array);
}

size_t FString::Find( const char* s, size_t Pos )
{
  char* Result = &Array[Pos];
  const char* StrPtr = s;

  int Len = strlen( s );
  int Matched = 0;
  do
  {
    Matched = (*Result == *StrPtr) ? Matched + 1 : 0;
    StrPtr = (Matched) ? StrPtr + 1 : s;
  } while ( *Result++ );

  if ( Matched == Len )
    return MAX_SIZE;

  return (size_t)(Result - Array);
}

size_t FString::Find( const char* s, size_t Pos, size_t n )
{
  char* Result = &Array[Pos];
  const char* StrPtr = s;

  int Len = strlen( s );
  int Matched = 0;
  do
  {
    Matched = (*Result == *StrPtr) ? Matched + 1 : 0;
    StrPtr = (Matched) ? StrPtr + 1 : s;
  } while ( *Result++ && (size_t)StrPtr < (size_t)(s + n + 1) );

  if ( Matched == Len )
    return MAX_SIZE;

  return (size_t)(Result - Array);
}

size_t FString::Find( char c, size_t Pos )
{
  char* Result = &Array[Pos];
  while ( *Result )
  {
    if ( *Result == c )
      return (size_t)(Result - Array);
    Result++;
  }

  return MAX_SIZE;
}

size_t FString::RFind( const FString& Str, size_t Pos )
{
  if ( Pos == MAX_SIZE )
    Pos = NumElements - 1;

  char* Result = &Array[Pos];
  const char* StrPtr = &Str.Back();

  int Len = Str.Length();
  int Matched = 0;
  do
  {
    Matched = (*Result == *StrPtr) ? Matched + 1 : 0;
    StrPtr = (Matched) ? StrPtr - 1 : Str.Array;
  } while ( Result >= Array );

  if ( Matched == Len )
    return MAX_SIZE;

  return (size_t)(Result - Array);
}

size_t FString::RFind( const char* s, size_t Pos )
{
  if ( Pos == MAX_SIZE )
    Pos = NumElements - 1;

  int Len = strlen( s );
  char* Result = &Array[Pos];
  const char* StrPtr = &s[Len - 1];

  int Matched = 0;
  do
  {
    Matched = (*Result == *StrPtr) ? Matched + 1 : 0;
    StrPtr = (Matched) ? StrPtr - 1 : &s[Len - 1];
  } while ( Result >= Array );

  if ( Matched == Len )
    return MAX_SIZE;

  return (size_t)(Result - Array);
}
size_t FString::RFind( const char* s, size_t Pos, size_t n )
{
  if ( Pos == MAX_SIZE )
    Pos = NumElements - 1;

  int Len = strlen( s );
  char* Result = &Array[Pos];
  const char* StrPtr = &s[Len - 1];

  int Matched = 0;
  do
  {
    Matched = (*Result == *StrPtr) ? Matched + 1 : 0;
    StrPtr = (Matched) ? StrPtr - 1 : &s[Len - 1];
  } while ( *Result-- && (size_t)StrPtr > (size_t)(&s[Len - 1] - n) ); // needs testing

  if ( Matched == Len )
    return MAX_SIZE;

  return (size_t)(Result - Array);
}

size_t FString::RFind( char c, size_t Pos )
{
  if ( Pos == MAX_SIZE )
    Pos = NumElements - 1;

  char* Result = &Array[Pos];
  while ( Result >= Array )
  {
    if ( *Result == c )
      return (size_t)(Result - Array);
    Result--;
  }

  return MAX_SIZE;
}

size_t FString::FindFirstOf( const FString& Str, size_t Pos )
{
  char* Result = &Array[Pos];
  while ( *Result )
  {
    for ( int i = 0; i < Str.Length(); i++ )
    {
      if ( *Result == Str.Array[i] )
        return (size_t)(Result - Array);
    }
    Result++;
  }

  return MAX_SIZE;
}

size_t FString::FindFirstOf( const char* s, size_t Pos )
{
  size_t Len = strlen( s );
  char* Result = &Array[Pos];
  while ( *Result )
  {
    for ( int i = 0; i < Len; i++ )
    {
      if ( *Result == s[i] )
        return (size_t)(Result - Array);
    }
    Result++;
  }

  return MAX_SIZE;
}

size_t FString::FindFirstOf( const char* s, size_t Pos, size_t n )
{
  size_t Len = MIN( n, strlen( s ) );
  char* Result = &Array[Pos];
  while ( *Result )
  {
    for ( int i = 0; i < Len; i++ )
    {
      if ( *Result == s[i] )
        return (size_t)(Result - Array);
    }
    Result++;
  }

  return MAX_SIZE;
}

size_t FString::FindFirstOf( char c, size_t Pos )
{
  return Find( c, Pos );
}

size_t FString::FindLastOf( const FString& Str, size_t Pos )
{
  if ( Pos == MAX_SIZE )
    Pos = NumElements - 1;

  char* Result = &Array[Pos];
  while ( Result >= Array )
  {
    for ( int i = 0; i < Str.Length(); i++ )
    {
      if ( *Result == Str.Array[i] )
        return (size_t)(Result - Array);
    }
    Result--;
  }

  return MAX_SIZE;
}

size_t FString::FindLastOf( const char* s, size_t Pos )
{
  if ( Pos == MAX_SIZE )
    Pos = NumElements - 1;

  size_t Len = strlen( s );
  char* Result = &Array[Pos];
  while ( Result >= Array )
  {
    for ( int i = 0; i < Len; i++ )
    {
      if ( *Result == s[i] )
        return (size_t)(Result - Array);
    }
    Result--;
  }

  return MAX_SIZE;
}

size_t FString::FindLastOf( const char* s, size_t Pos, size_t n )
{
  if ( Pos == MAX_SIZE )
    Pos = NumElements - 1;

  size_t Len = MIN( n, strlen( s ) );
  char* Result = &Array[Pos];
  while ( Result >= Array )
  {
    for ( int i = 0; i < Len; i++ )
    {
      if ( *Result == s[i] )
        return (size_t)(Result - Array);
    }
    Result--;
  }

  return MAX_SIZE;
}

size_t FString::FindLastOf( char c, size_t Pos )
{
  if ( Pos == MAX_SIZE )
    Pos = NumElements - 1;

  char* Result = &Array[Pos];
  while ( Result >= Array )
  {
    if ( *Result == c )
      return (size_t)(Result - Array);
    Result--;
  }

  return MAX_SIZE;
}

size_t FString::FindFirstNotOf( const FString& Str, size_t Pos )
{
  char* Result = &Array[Pos];
  while ( *Result )
  {
    for ( int i = 0; i < Str.Length(); i++ )
    {
      if ( *Result != Str.Array[i] )
        return (size_t)(Result - Array);
    }
    Result++;
  }

  return MAX_SIZE;
}

size_t FString::FindFirstNotOf( const char* s, size_t Pos )
{
  size_t Len = strlen( s );
  char* Result = &Array[Pos];
  while ( *Result )
  {
    for ( int i = 0; i < Len; i++ )
    {
      if ( *Result != s[i] )
        return (size_t)(Result - Array);
    }
    Result++;
  }

  return MAX_SIZE;
}

size_t FString::FindFirstNotOf( const char* s, size_t Pos, size_t n )
{
  size_t Len = MIN( n, strlen( s ) );
  char* Result = &Array[Pos];
  while ( *Result )
  {
    for ( int i = 0; i < Len; i++ )
    {
      if ( *Result != s[i] )
        return (size_t)(Result - Array);
    }
    Result++;
  }

  return MAX_SIZE;
}

size_t FString::FindFirstNotOf( char c, size_t Pos )
{
  char* Result = &Array[Pos];
  while ( *Result )
  {
    if ( *Result != c )
      return (size_t)(Result - Array);
    Result++;
  }

  return MAX_SIZE;
}

size_t FString::FindLastNotOf( const FString& Str, size_t Pos )
{
  if ( Pos == MAX_SIZE )
    Pos = NumElements - 1;

  char* Result = &Array[Pos];
  while ( Result >= Array )
  {
    for ( int i = 0; i < Str.Length(); i++ )
    {
      if ( *Result != Str.Array[i] )
        return (size_t)(Result - Array);
    }
    Result--;
  }

  return MAX_SIZE;
}

size_t FString::FindLastNotOf( const char* s, size_t Pos )
{
  if ( Pos == MAX_SIZE )
    Pos = NumElements - 1;

  size_t Len = strlen( s );
  char* Result = &Array[Pos];
  while ( Result >= Array )
  {
    for ( int i = 0; i < Len; i++ )
    {
      if ( *Result != s[i] )
        return (size_t)(Result - Array);
    }
    Result--;
  }

  return MAX_SIZE;
}

size_t FString::FindLastNotOf( const char* s, size_t Pos, size_t n )
{
  if ( Pos == MAX_SIZE )
    Pos = NumElements - 1;

  size_t Len = MIN( n, strlen( s ) );
  char* Result = &Array[Pos];
  while ( Result >= Array )
  {
    for ( int i = 0; i < Len; i++ )
    {
      if ( *Result != s[i] )
        return (size_t)(Result - Array);
    }
    Result--;
  }

  return MAX_SIZE;
}

size_t FString::FindLastNotOf( char  c, size_t Pos )
{
  if ( Pos == MAX_SIZE )
    Pos = NumElements - 1;

  char* Result = &Array[Pos];
  while ( Result >= Array )
  {
    if ( *Result != c )
      return (size_t)(Result - Array);
    Result--;
  }

  return MAX_SIZE;
}

FString FString::Substr( size_t Pos, size_t Len ) const
{
  if ( Len == MAX_SIZE )
    Len = NumElements - Pos;

  return FString( *this, Pos, Len );
}

void FString::ReplaceChars( char Old, char New )
{
  for ( int i = 0; i < NumElements; i++ )
  {
    if ( Array[i] == Old )
      Array[i] = New;
  }
}

FString& FString::operator+=( const FString& Str )
{
  return Append( Str );
}

FString& FString::operator+=( const char* s )
{
  return Append( s );
}

FString& FString::operator+=( char c )
{
  PushBack( c );
  return *this;
}

FString& FString::operator+=( FName Name )
{
  return Append( Name.Data() );
}

FString operator+( const FString& lhs, const FString& rhs )
{
  FString Out = lhs;
  return Out.Append( rhs );
}

FString operator+( const FString& lhs, const char* rhs )
{
  FString Out = lhs;
  return Out.Append( rhs );
}

FString operator+( const FString& lhs, char rhs )
{
  FString Out = lhs;
  return Out.Append( rhs );
}

FString operator+( const FString& lhs, FName rhs )
{
  FString Out = lhs;
  return Out.Append( rhs.Data() );
}

bool operator==( const FString& lhs, const FString& rhs )
{
  if ( lhs.NumElements != rhs.NumElements )
    return false;

  return (memcmp( lhs.Array, rhs.Array, lhs.NumElements ) == 0);
}

bool operator!=( const FString& lhs, const FString& rhs )
{
  if ( lhs.NumElements != rhs.NumElements )
    return true;

  return (memcmp( lhs.Array, rhs.Array, lhs.NumElements ) != 0);
}

LIBUNR_API FPackageFileIn& operator>>( FPackageFileIn& In, FString& Str )
{
  idx Size = 0;
  In >> CINDEX( Size );

  if ( Size > 0 )
  {
    Str.Reserve( Size-1 );
    
    // Slow but secure
    for ( int i = 0; i < Size; i++ )
    {
      char C = '\0';
      In.Read( &C, 1 );

      if ( C == '\0' )
        break;

      Str += C;
    }
  }
  else if ( Size < 0 )
  {
    // Some packages store in unicode by storing a negative
    // length. This is an indicator for unicode text.
    // Will this actually butcher the text if it has non-ascii
    // representable characters?
    Size = -Size;

    wchar_t* TextBuf = new wchar_t[Size];
    In.Read( TextBuf, (Size) * 2 );

    char* MbsText = new char[Size];
    wcstombs( MbsText, TextBuf, Size );

    Str.Assign( MbsText );

    delete TextBuf;
    delete MbsText;
  }

  return In;
}

LIBUNR_API FPackageFileOut& operator<<( FPackageFileOut& Out, FString& Str )
{
  size_t Length = Str.Length();
  Out << CINDEX( Length );
  Out.Write( (void*)Str.Data(), Length+1 );
  return Out;
}

//#ifndef LIBUNR_WIN32
static const char StricmpTable[] =
{
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
  0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
  0x60, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
};

int stricmp ( const char* str1, const char* str2 )
{  
  if (!str1 || !str2)
    return 0;
  
  while (1)
  { 
    if (StricmpTable[*str1] != StricmpTable[*str2])
      return StricmpTable[*str1] < StricmpTable[*str2] ? -1 : 1;
    
    if (StricmpTable[*str1] == '\0')
      break;

    str1++;
    str2++;
  }
  
  return 0;
}

int strnicmp( const char* str1, const char* str2, size_t count )
{
  if (!str1 || !str2)
    return 0;
  
  while (count)
  { 
    if (StricmpTable[*str1] != StricmpTable[*str2])
      return StricmpTable[*str1] < StricmpTable[*str2] ? -1 : 1;
    
    if (StricmpTable[*str1] == '\0')
      break;

    str1++;
    str2++;
    count--;
  }
  
  return 0;

}
//#endif

char* strupper( const char* str )
{
  size_t len = strlen( str );
  char* strUp = new char[len+1];
      
  for (size_t i = 0; i < len && i != MAX_SIZE; i++)
    strUp[i] = toupper( str[i] );
  
  strUp[len] = '\0';
  return strUp;
}

char* GetDateString( const char* Fmt )
{
  static char DateString[32];
  
  time_t Now;
  struct tm* TimeInfo = {0};
  
  time( &Now );
  TimeInfo = localtime( &Now );
  
  if (strftime( DateString, 32, Fmt, TimeInfo ) == 0)
    return NULL;
  else
    return DateString;
}

