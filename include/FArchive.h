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
 * FArchive.h - Base FArchive class
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#ifndef __FARCHIVE_H__
#define __FARCHIVE_H__

#include "FUtil.h"

enum ESeekBase
{
  Begin,
  Cur,
  End,
};

/*-----------------------------------------------------------------------------
	FArchive
-----------------------------------------------------------------------------*/
class FArchive
{
public:
  FArchive() {};
  virtual ~FArchive() {};
  
  virtual size_t Read( void* Dest, size_t Len ) { return 0; }
  virtual size_t Write( void* Src, size_t Len ) { return 0; }
  virtual size_t Seek( size_t Off, ESeekBase Base ) { return 0; }
  virtual size_t Tell() { return 0; }
  
  friend FORCEINLINE FArchive& operator>>( FArchive& Ar, u8& b )     { Ar.Read( &b, 1 ); return Ar; }
  friend FORCEINLINE FArchive& operator>>( FArchive& Ar, u16& s )    { Ar.Read( &s, 2 ); return Ar; }
  friend FORCEINLINE FArchive& operator>>( FArchive& Ar, u32& d )    { Ar.Read( &d, 4 ); return Ar; }
  friend FORCEINLINE FArchive& operator>>( FArchive& Ar, u64& q )    { Ar.Read( &q, 8 ); return Ar; }
  friend FORCEINLINE FArchive& operator>>( FArchive& Ar, i8& b )     { Ar.Read( &b, 1 ); return Ar; }
  friend FORCEINLINE FArchive& operator>>( FArchive& Ar, i16& s )    { Ar.Read( &s, 2 ); return Ar; }
  friend FORCEINLINE FArchive& operator>>( FArchive& Ar, i32& d )    { Ar.Read( &d, 4 ); return Ar; }
  friend FORCEINLINE FArchive& operator>>( FArchive& Ar, i64& q )    { Ar.Read( &q, 8 ); return Ar; }
  friend FORCEINLINE FArchive& operator>>( FArchive& Ar, float& f )  { Ar.Read( &f, 4 ); return Ar; }
  friend FORCEINLINE FArchive& operator>>( FArchive& Ar, double& d ) { Ar.Read( &d, 8 ); return Ar; }
  
  friend FORCEINLINE FArchive& operator<<( FArchive& Ar, u8& b )     { Ar.Write( &b, 1 ); return Ar; }
  friend FORCEINLINE FArchive& operator<<( FArchive& Ar, u16& s )    { Ar.Write( &s, 2 ); return Ar; }
  friend FORCEINLINE FArchive& operator<<( FArchive& Ar, u32& d )    { Ar.Write( &d, 4 ); return Ar; }
  friend FORCEINLINE FArchive& operator<<( FArchive& Ar, u64& q )    { Ar.Write( &q, 8 ); return Ar; }
  friend FORCEINLINE FArchive& operator<<( FArchive& Ar, i8& b )     { Ar.Write( &b, 1 ); return Ar; }
  friend FORCEINLINE FArchive& operator<<( FArchive& Ar, i16& s )    { Ar.Write( &s, 2 ); return Ar; }
  friend FORCEINLINE FArchive& operator<<( FArchive& Ar, i32& d )    { Ar.Write( &d, 4 ); return Ar; }
  friend FORCEINLINE FArchive& operator<<( FArchive& Ar, i64& q )    { Ar.Write( &q, 8 ); return Ar; }
  friend FORCEINLINE FArchive& operator<<( FArchive& Ar, float& f )  { Ar.Write( &f, 4 ); return Ar; }
  friend FORCEINLINE FArchive& operator<<( FArchive& Ar, double& d ) { Ar.Write( &d, 8 ); return Ar; }
  friend FORCEINLINE FArchive& operator<<( FArchive& Ar, char* s )   { Ar.Write( s, strlen(s) ); return Ar; }
};

/*-----------------------------------------------------------------------------
	FCompactIndex
	https://wiki.beyondunreal.com/Legacy:Package_File_Format/Data_Details
-----------------------------------------------------------------------------*/
class DLL_EXPORT FCompactIndex
{
public:
  int Value;
  friend FArchive& operator>>( FArchive& Ar, FCompactIndex& Index );
  friend FArchive& operator<<( FArchive& Ar, FCompactIndex& Index );
};

#define CINDEX(val) (*(FCompactIndex*)&val)
#endif
