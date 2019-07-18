/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU Affero General Public License as           *|
|*  published by the free Software Foundation, either version 3 of the       *|
|*  License, or (at your option) any later version.                          *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU Affero General Public License for more details.                      *|
|*                                                                           *|
|*  You should have received a copy of the GNU Affero General Public License *|
|*  along with this program.  If not, see <https://www.gnu.org/licenses/>.   *|
\*===========================================================================*/

/*========================================================================
 * FFileArchiveOut.cpp - FFileArchiveOut implementation
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include <stdarg.h>
#include "Util/FFileArchive.h"

/*-----------------------------------------------------------------------------
  FFileArchive
-----------------------------------------------------------------------------*/
FFileArchive::FFileArchive() 
{
  File = NULL;
}

FFileArchive::~FFileArchive() 
{ 
  Close();
}

int FFileArchive::Open( const char* Filename ) 
{ 
  return -1;
}

int FFileArchive::Open( const FString& Filename ) 
{ 
  return -1; 
}

void FFileArchive::Close()
{
  if (File)
  {
    fclose( File );
    File = NULL;
  }
}

size_t FFileArchive::Read( void* Dest, size_t Len )
{
  return fread( Dest, 1, Len, File );
}

size_t FFileArchive::Write( void* Src, size_t Len )
{
  return fwrite( Src, 1, Len, File );
}

size_t FFileArchive::Printf( const char* Str, ... )
{
  va_list vl;
  va_start( vl, Str );
  size_t Num = vfprintf( File, Str, vl );
  va_end( vl );
  return Num;
}

size_t FFileArchive::Seek( ssize_t Off, ESeekBase Base )
{
  fseeko( File, Off, (int)Base );
  return ftello( File );
}

char FFileArchive::Peek()
{
  char Char = (char)fgetc( File );
  ungetc( Char, File );
  return Char;
}

size_t FFileArchive::Tell()
{
  return ftello( File );
}

bool FFileArchive::Eof()
{
  return feof( File );
}

void FFileArchive::Flush()
{
  fflush( File );
}

/*-----------------------------------------------------------------------------
  FFileArchiveIn
-----------------------------------------------------------------------------*/
int FFileArchiveIn::Open( const char* Filename )
{
  File = fopen( Filename, "rb" );
  return (File == NULL) ? errno : 0;
}

int FFileArchiveIn::Open( const FString& Filename )
{
  File = fopen( Filename.Data(), "rb" );
  return (File == NULL) ? errno : 0;
}

/*-----------------------------------------------------------------------------
  FFileArchiveOut
-----------------------------------------------------------------------------*/
int FFileArchiveOut::Open( const char* Filename )
{
  File = fopen( Filename, "wb" );
  return (File == NULL) ? errno : 0;
}

int FFileArchiveOut::Open( const FString& Filename )
{
  File = fopen( Filename.Data(), "wb" );
  return (File == NULL) ? errno : 0;
}

