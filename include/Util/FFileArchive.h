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
 * FFileArchive.h - Class for serializing bytes to and from files
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#ifndef _FFILEARCHIVE_H_
#define _FFILEARCHIVE_H_

#pragma once
#include "Util/FArchive.h"

/*-----------------------------------------------------------------------------
 * FFileArchive
 * Provides generic interface for reading and writing to a file
-----------------------------------------------------------------------------*/
class LIBUNR_API FFileArchive : public FArchive
{
public:
  FFileArchive();
  virtual ~FFileArchive();

  virtual int Open( const char* Filename );
  virtual int Open( const FString& Filename );
  virtual void Close();
  
  virtual size_t Read( void* Dest, size_t Len );
  virtual size_t Write( void* Src, size_t Len );
  virtual size_t Seek( ssize_t Off, ESeekBase Base );
  virtual char   Peek();
  virtual size_t Tell();
  virtual bool Eof();
  virtual void Flush();

  virtual size_t Printf( const char* Str, ... );

protected:
  FILE* File;
};

/*-----------------------------------------------------------------------------
 * FFileArchiveIn
 * Provides interface to read from a file
-----------------------------------------------------------------------------*/
class LIBUNR_API FFileArchiveIn : public FFileArchive
{
public:
  virtual int Open( const char* Filename );
  virtual int Open( const FString& Filename );
};

/*-----------------------------------------------------------------------------
 * FFileArchiveOut
 * Provides interface to write to a file
-----------------------------------------------------------------------------*/
class LIBUNR_API FFileArchiveOut : public FFileArchive
{
public:
  virtual int Open( const char* Filename );
  virtual int Open( const FString& Filename );
};

/*-----------------------------------------------------------------------------
 * FStringFilePath
 * An FString variant that ensures consistent state for a file path
-----------------------------------------------------------------------------*/
class LIBUNR_API FStringFilePath : public FString
{
public:
  FStringFilePath( const char* InDir, const char* InName, const char* InExt=NULL );
  FStringFilePath( const char* Path );
  ~FStringFilePath()
  {
    if ( Dir )
      free( Dir );

    if ( Name )
      free( Name );

    if ( Ext )
      free( Ext );
  }

  const char* GetDir();
  const char* GetName();
  const char* GetExt();

protected:
  char* Dir;
  char* Name;
  char* Ext;
};

#endif
