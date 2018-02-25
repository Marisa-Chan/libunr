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
 * FArchiveFile.h - File based FArchive interfaces
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#ifndef __FARCHIVEFILE_H__
#define __FARCHIVEFILE_H__

#include "FArchive.h"
#include "FString.h"

/*-----------------------------------------------------------------------------
	FArchiveFile
-----------------------------------------------------------------------------*/
class DLL_EXPORT FArchiveFile : public FArchive
{  
public:
  FArchiveFile();
  virtual ~FArchiveFile();
  
  virtual bool Open( const char* Filename );
  virtual bool Open( const FString& Filename );
  virtual void Close();
  
  virtual size_t Read( void* Dest, size_t Len );
  virtual size_t Write( void* Src, size_t Len );
  virtual size_t Seek( size_t Off, ESeekBase Base );
  virtual size_t Tell();
  
protected:
  FILE* Stream;
};

/*-----------------------------------------------------------------------------
	FArchiveFileIn
-----------------------------------------------------------------------------*/
class DLL_EXPORT FArchiveFileIn : public FArchiveFile
{
public:
  virtual bool Open( const char* Filename );
  virtual bool Open( const FString& Filename );
};

/*-----------------------------------------------------------------------------
	FArchiveFileOut
-----------------------------------------------------------------------------*/
class DLL_EXPORT FArchiveFileOut : public FArchiveFile
{
public:
  // what about appending files?
  virtual bool Open( const char* Filename );
  virtual bool Open( const FString& Filename );
};
#endif
