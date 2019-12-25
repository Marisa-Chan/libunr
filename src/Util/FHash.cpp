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
 * FHash.h - SuperFastHash implementation
 *
 * Original implementation by Paul Hsieh
 * http://www.azillionmonkeys.com/qed/hash.html
 *
 * adapted to libunr by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Util/FHash.h"

/*-----------------------------------------------------------------------------
 * SuperFastHash implementation
 -----------------------------------------------------------------------------*/

#define GET_16_BITS(d) (*((const uint16_t *) (d)))

u32 SuperFastHash( const char* Data, size_t Len )
{
  u32 Hash = 0;
  u32 Tmp;
  size_t Rem;

  if ( Len == 0 || Data == NULL ) return 0;

  Rem = Len & 3;
  Len >>= 2;

  /* Main loop */
  for ( ; Len > 0; Len-- )
  {
    Hash += GET_16_BITS( Data );
    Tmp = (GET_16_BITS( Data + 2 ) << 11) ^ Hash;
    Hash = (Hash << 16) ^ Tmp;
    Data += 2 * sizeof( uint16_t );
    Hash += Hash >> 11;
  }

  /* Handle end cases */
  switch ( Rem )
  {
  case 3:	Hash += GET_16_BITS( Data );
    Hash ^= Hash << 16;
    Hash ^= Data[sizeof( uint16_t )] << 18;
    Hash += Hash >> 11;
    break;
  case 2:	Hash += GET_16_BITS( Data );
    Hash ^= Hash << 11;
    Hash += Hash >> 17;
    break;
  case 1: Hash += *Data;
    Hash ^= Hash << 10;
    Hash += Hash >> 1;
  }

  /* Force "avalanching" of final 127 bits */
  Hash ^= Hash << 3;
  Hash += Hash >> 5;
  Hash ^= Hash << 4;
  Hash += Hash >> 17;
  Hash ^= Hash << 25;
  Hash += Hash >> 6;

  return Hash;
}

/*-----------------------------------------------------------------------------
 * Case insensitive SuperFastHash for strings
 -----------------------------------------------------------------------------*/

#define GET_16_BITS_I(d) ((((u32)tolower(((const u8 *)(d))[1])) << 8)\
                       +(u32)tolower(((const u8 *)(d))[0]) )

u32 SuperFastHashString( const char* Text, size_t Len )
{
  if ( Text == NULL )
    return -1;

  u32 Hash = 0;
  u32 Tmp;
  size_t Rem;

  if ( Len == 0 )
  {
    Len = strlen( Text );
    if ( Len == 0 )
      return -1;
  }

  Rem = Len & 3;
  Len >>= 2;

  /* Main loop */
  for ( ; Len > 0; Len-- )
  {
    Hash += GET_16_BITS_I( Text );
    Tmp = (GET_16_BITS_I( Text + 2 ) << 11) ^ Hash;
    Hash = (Hash << 16) ^ Tmp;
    Text += 2 * sizeof( u16 );
    Hash += Hash >> 11;
  }

  /* Handle end cases */
  switch ( Rem )
  {
  case 3:
    Hash += GET_16_BITS_I( Text );
    Hash ^= Hash << 16;
    Hash ^= Text[sizeof( u16 )] << 18;
    Hash += Hash >> 11;
    break;
  case 2:
    Hash += GET_16_BITS_I( Text );
    Hash ^= Hash << 11;
    Hash += Hash >> 17;
    break;
  case 1:
    Hash += *Text;
    Hash ^= Hash << 10;
    Hash += Hash >> 1;
  }

  /* Force "avalanching" of final 127 bits */
  Hash ^= Hash << 3;
  Hash += Hash >> 5;
  Hash ^= Hash << 4;
  Hash += Hash >> 17;
  Hash ^= Hash << 25;
  Hash += Hash >> 6;

  return Hash;
}