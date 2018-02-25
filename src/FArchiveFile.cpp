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
 * FArchiveFile.cpp - Implementation of FArchiveFile family
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "FArchiveFile.h"

/*-----------------------------------------------------------------------------
	FArchiveFile
-----------------------------------------------------------------------------*/
FArchiveFile::FArchiveFile() 
{
  Stream = NULL;
}

FArchiveFile::~FArchiveFile() 
{ 
  Close();
}

bool FArchiveFile::Open( const char* Filename ) 
{ 
  return false;
}

bool FArchiveFile::Open( const FString& Filename ) 
{ 
  return false; 
}

void FArchiveFile::Close()
{
  if (Stream)
  {
    fclose( Stream );
    Stream = NULL;
  }
}

size_t FArchiveFile::Read( void* Dest, size_t Len )
{
  return fread( Dest, 1, Len, Stream );
}

size_t FArchiveFile::Write( void* Src, size_t Len )
{
  return fwrite( Src, 1, Len, Stream );
}

size_t FArchiveFile::Seek( size_t Off, ESeekBase Base )
{
  fseeko( Stream, Off, (int)Base );
  return ftello( Stream );
}

size_t FArchiveFile::Tell()
{
  return ftello( Stream );
}

/*-----------------------------------------------------------------------------
	FArchiveFileIn
-----------------------------------------------------------------------------*/
bool FArchiveFileIn::Open( const char* Filename )
{
  return (Stream = fopen( Filename, "rb" )) != NULL;
}

bool FArchiveFileIn::Open( const FString& Filename )
{
  return (Stream = fopen( Filename.Data(), "rb" )) != NULL;
}

/*-----------------------------------------------------------------------------
	FArchiveFileOut
-----------------------------------------------------------------------------*/
bool FArchiveFileOut::Open( const char* Filename )
{
  return (Stream = fopen( Filename, "wb" )) != NULL;
}

bool FArchiveFileOut::Open( const FString& Filename )
{
  return (Stream = fopen( Filename.Data(), "wb" )) != NULL;
}
