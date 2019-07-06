/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU Affero General Public License as           *|
|*  published by the Free Software Foundation, either version 3 of the       *|
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
 * FFileArchive.h - Class for serializing bytes to and from files
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include "Core/FArchive.h"

/*-----------------------------------------------------------------------------
 * FFileArchive
 * Provides generic interface for reading and writing to a file
-----------------------------------------------------------------------------*/
class DLL_EXPORT FFileArchive : public FArchive
{
public:
  virtual int Open( const char* Filename );
  virtual int Open( const String& Filename );
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
class DLL_EXPORT FFileArchiveIn : public FFileArchive
{
public:
  virtual int Open( const char* Filename );
  virtual int Open( const String& Filename );
};

/*-----------------------------------------------------------------------------
 * FFileArchiveOut
 * Provides interface to write to a file
-----------------------------------------------------------------------------*/
class DLL_EXPORT FFileArchiveOut : public FFileArchive
{
public:
  virtual int Open( const char* Filename );
  virtual int Open( const String& Filename );
};

