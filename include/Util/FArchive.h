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
 * FArchive.h - Class for serializing bytes to and from any medium
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Util/FMacro.h"
#include "Util/FTypes.h"
#include "Util/FString.h"

#ifdef LIBUNR_BIG_ENDIAN
  #define conditional_bswap_16 bswap_16
  #define conditional_bswap_32 bswap_32
  #define conditional_bswap_64 bswap_64
#else
  #define conditional_bswap_16 (void)0;
  #define conditional_bswap_32 (void)0;
  #define conditional_bswap_64 (void)0;
#endif

enum ESeekBase
{
  Begin,
  Cur,
  End,
};

class DLL_EXPORT FArchive
{
public:
  FArchive() {};
  virtual ~FArchive() {};

  virtual int Open( const char* Filename ) { return -1; }
  virtual int Open( const FString& Filename ) { return -1; }
  virtual void Close() {}
  virtual size_t Read( void* Dest, size_t Len ) { return 0; }
  virtual size_t Write( void* Src, size_t Len ) { return 0; }
  virtual size_t Seek( size_t Off, ESeekBase Base ) { return 0; }
  virtual size_t Tell() { return 0; }
  
  friend FORCEINLINE FArchive& operator>>( FArchive& S, u8& b )
  { 
    S.Read( &b, 1 ); 
    return S; 
  }

  friend FORCEINLINE FArchive& operator>>( FArchive& S, u16& s )    
  { 
    S.Read( &s, 2 ); 
    conditional_bswap_16(s);
    return S; 
  }

  friend FORCEINLINE FArchive& operator>>( FArchive& S, u32& d )
  { 
    S.Read( &d, 4 ); 
    conditional_bswap_32(d);
    return S; 
  }

  friend FORCEINLINE FArchive& operator>>( FArchive& S, u64& q )    
  { 
    S.Read( &q, 8 ); 
    conditional_bswap_64(q);
    return S; 
  }

  friend FORCEINLINE FArchive& operator>>( FArchive& S, i8& b )     
  { 
    S.Read( &b, 1 ); 
    return S; 
  }

  friend FORCEINLINE FArchive& operator>>( FArchive& S, i16& s )    
  { 
    S.Read( &s, 2 );
    conditional_bswap_16(s);
    return S; 
  }

  friend FORCEINLINE FArchive& operator>>( FArchive& S, i32& d )    
  { 
    S.Read( &d, 4 ); 
    conditional_bswap_32(d);
    return S; 
  }

  friend FORCEINLINE FArchive& operator>>( FArchive& S, i64& q )    
  { 
    S.Read( &q, 8 ); 
    conditional_bswap_64(q);
    return S; 
  }

  friend FORCEINLINE FArchive& operator>>( FArchive& S, float& f )  
  { 
    S.Read( &f, 4 );
    conditional_bswap_32(f);
    return S; 
  }
  
  friend FORCEINLINE FArchive& operator>>( FArchive& S, double& d ) 
  { 
    S.Read( &d, 8 ); 
    conditional_bswap_64(d);
    return S; 
  }
  
  friend FORCEINLINE FArchive& operator<<( FArchive& S, u8& b )
  { 
    S.Write( &b, 1 ); 
    return S; 
  }

  friend FORCEINLINE FArchive& operator<<( FArchive& S, u16& s )
  {
    u16 s2 = s;
    conditional_bswap_16(s2);
    S.Write( &s2, 2 ); 
    return S; 
  }

  friend FORCEINLINE FArchive& operator<<( FArchive& S, u32& d )    
  { 
    u32 d2 = d;
    conditional_bswap_32(d2);
    S.Write( &d2, 4 ); 
    return S; 
  }

  friend FORCEINLINE FArchive& operator<<( FArchive& S, u64& q )
  { 
    u64 q2 = q;
    conditional_bswap_64(q2);
    S.Write( &q2, 8 ); 
    return S; 
  }

  friend FORCEINLINE FArchive& operator<<( FArchive& S, i8& b ) 
  { 
    S.Write( &b, 1 ); 
    return S; 
  }

  friend FORCEINLINE FArchive& operator<<( FArchive& S, i16& s )
  { 
    i16 s2 = s;
    conditional_bswap_16(s2);
    S.Write( &s2, 2 ); 
    return S; 
  }

  friend FORCEINLINE FArchive& operator<<( FArchive& S, i32& d )
  { 
    i32 d2 = d;
    conditional_bswap_32(d2);
    S.Write( &d2, 4 ); 
    return S; 
  }

  friend FORCEINLINE FArchive& operator<<( FArchive& S, i64& q )
  { 
    i64 q2 = q;
    conditional_bswap_64(q2);
    S.Write( &q2, 8 ); 
    return S; 
  }

  friend FORCEINLINE FArchive& operator<<( FArchive& S, float& f )  
  { 
    float f2 = f;
    conditional_bswap_32(f2);
    S.Write( &f2, 4 ); 
    return S; 
  }

  friend FORCEINLINE FArchive& operator<<( FArchive& S, double& d ) 
  { 
    double d2 = d;
    conditional_bswap_64(d2);
    S.Write( &d2, 8 ); 
    return S; 
  }
};

