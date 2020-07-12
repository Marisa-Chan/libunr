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
 * UFactory.cpp - Object import helper code
 *
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Editor/UFactory.h"

/*-----------------------------------------------------------------------------
 * UFactory
-----------------------------------------------------------------------------*/
UObject* UFactory::CreateObject( UPackage* Pkg, UClass* Class, const char* Name )
{
  if ( Class == UClass::StaticClass() )
    return UClassFactory::CreateObject( Pkg, Name );
  else if ( Class == ULevel::StaticClass() )
    return ULevelFactory::CreateObject( Pkg, Name );
  else if ( Class == UMesh::StaticClass() )
    return UMeshFactory::CreateObject( Pkg, Name );
  else if ( Class == UMusic::StaticClass() )
    return UMusicFactory::CreateObject( Pkg, Name );
  else if ( Class == UPolys::StaticClass() )
    return UPolysFactory::CreateObject( Pkg, Name );
  else if ( Class == USound::StaticClass() )
    return USoundFactory::CreateObject( Pkg, Name );
  else if ( Class == UTextBuffer::StaticClass() )
    return UTextBufferFactory::CreateObject( Pkg, Name );
  else if ( Class == UPalette::StaticClass() )
    return UPaletteFactory::CreateObject( Pkg, Name );
  else if ( Class == UTexture::StaticClass() )
    return UTextureFactory::CreateObject( Pkg, Name );
  else if ( Class == UFractalTexture::StaticClass() )
    return UFractalTextureFactory::CreateObject( Pkg, Name );
  else if ( Class == UFont::StaticClass() )
    return UFontFactory::CreateObject( Pkg, Name );
  else
    GLogf( LOG_ERR, "Can't create object of type '%s' via factory", Class->Name.Data() );
 
  return NULL;
}

UObject* UFactory::ImportObject( UPackage* Pkg, UClass* Class, const char* Path )
{
  if ( Class == UClass::StaticClass() )
    return UClassFactory::ImportObject( Pkg, Path );
  else if ( Class == ULevel::StaticClass() )
    return ULevelFactory::ImportObject( Pkg, Path );
  else if ( Class == UMesh::StaticClass() )
    return UMeshFactory::ImportObject( Pkg, Path );
  else if ( Class == UMusic::StaticClass() )
    return UMusicFactory::ImportObject( Pkg, Path );
  else if ( Class == UPolys::StaticClass() )
    return UPolysFactory::ImportObject( Pkg, Path );
  else if ( Class == USound::StaticClass() )
    return USoundFactory::ImportObject( Pkg, Path );
  else if ( Class == UTextBuffer::StaticClass() )
    return UTextBufferFactory::ImportObject( Pkg, Path );
  else if ( Class == UPalette::StaticClass() )
    return UPaletteFactory::ImportObject( Pkg, Path );
  else if ( Class == UTexture::StaticClass() )
    return UTextureFactory::ImportObject( Pkg, Path );
  else if ( Class == UFractalTexture::StaticClass() )
    return UFractalTextureFactory::ImportObject( Pkg, Path );
  else if ( Class == UFont::StaticClass() )
    return UFontFactory::ImportObject( Pkg, Path );
  else
    GLogf( LOG_ERR, "Can't import object of type '%s' via factory", Class->Name.Data() );

  return NULL;
}

/*-----------------------------------------------------------------------------
 * UClassFactory
-----------------------------------------------------------------------------*/
UClass* UClassFactory::CreateObject( UPackage* Pkg, const char* Name )
{
  GLogf( LOG_ERR, "UClassFactory::CreateObject stub" );
  return NULL;
}

UClass* UClassFactory::ImportObject( UPackage* Pkg, const char* Path )
{
  GLogf( LOG_ERR, "UClassFactory::ImportObject stub" );
  return NULL;
}

/*-----------------------------------------------------------------------------
 * ULevelFactory
-----------------------------------------------------------------------------*/
ULevel* ULevelFactory::CreateObject( UPackage* Pkg, const char* Name )
{
  GLogf( LOG_ERR, "ULevelFactory::CreateObject stub" );
  return NULL;
}

ULevel* ULevelFactory::ImportObject( UPackage* Pkg, const char* Path )
{
  GLogf( LOG_ERR, "ULevelFactory::ImportObject stub" );
  return NULL;
}

/*-----------------------------------------------------------------------------
 * UMeshFactory
-----------------------------------------------------------------------------*/
UMesh* UMeshFactory::CreateObject( UPackage* Pkg, const char* Name )
{
  GLogf( LOG_ERR, "UMeshFactory::CreateObject stub" );
  return NULL;
}

UMesh* UMeshFactory::ImportObject( UPackage* Pkg, const char* Path )
{
  GLogf( LOG_ERR, "UMeshFactory::ImportObject stub" );
  return NULL;
}

/*-----------------------------------------------------------------------------
 * UMusicFactory
-----------------------------------------------------------------------------*/
UMusic* UMusicFactory::CreateObject( UPackage* Pkg, const char* Name )
{
  GLogf( LOG_ERR, "UMusicFactory::CreateObject stub" );
  return NULL;
}

UMusic* UMusicFactory::ImportObject( UPackage* Pkg, const char* Path )
{
  GLogf( LOG_ERR, "UMusicFactory::ImportObject stub" );
  return NULL;
}

/*-----------------------------------------------------------------------------
 * UPolysFactory
-----------------------------------------------------------------------------*/
UPolys* UPolysFactory::CreateObject( UPackage* Pkg, const char* Name )
{
  GLogf( LOG_ERR, "UPolysFactory::CreateObject stub" );
  return NULL;
}

UPolys* UPolysFactory::ImportObject( UPackage* Pkg, const char* Path )
{
  GLogf( LOG_ERR, "UPolysFactory::ImportObject stub" );
  return NULL;
}

/*-----------------------------------------------------------------------------
 * USoundFactory
-----------------------------------------------------------------------------*/
USound* USoundFactory::CreateObject( UPackage* Pkg, const char* Name )
{
  GLogf( LOG_ERR, "USoundFactory::CreateObject stub" );
  return NULL;
}

USound* USoundFactory::ImportObject( UPackage* Pkg, const char* Path )
{
  GLogf( LOG_ERR, "USoundFactory::ImportObject stub" );
  return NULL;
}

/*-----------------------------------------------------------------------------
 * UTextBufferFactory
-----------------------------------------------------------------------------*/
UTextBuffer* UTextBufferFactory::CreateObject( UPackage* Pkg, const char* Name )
{
  GLogf( LOG_ERR, "UTextBufferFactory::CreateObject stub" );
  return NULL;
}

UTextBuffer* UTextBufferFactory::ImportObject( UPackage* Pkg, const char* Path )
{
  GLogf( LOG_ERR, "UTextBufferFactory::ImportObject stub" );
  return NULL;
}

/*-----------------------------------------------------------------------------
 * UPaletteFactory
-----------------------------------------------------------------------------*/
UPalette* UPaletteFactory::CreateObject( UPackage* Pkg, const char* Name )
{
  GLogf( LOG_ERR, "UPaletteFactory::CreateObject stub" );
  return NULL;
}

UPalette* UPaletteFactory::ImportObject( UPackage* Pkg, const char* Path )
{
  GLogf( LOG_ERR, "UPaletteFactory::ImportObject stub" );
  return NULL;
}

/*-----------------------------------------------------------------------------
 * UTextureFactory
-----------------------------------------------------------------------------*/
UTexture* UTextureFactory::CreateObject( UPackage* Pkg, const char* Name )
{
  GLogf( LOG_ERR, "UTextureFactory::CreateObject stub" );
  return NULL;
}

UTexture* UTextureFactory::ImportObject( UPackage* Pkg, const char* Path )
{
  GLogf( LOG_ERR, "UTextureFactory::ImportObject stub" );
  return NULL;
}

/*-----------------------------------------------------------------------------
 * UFractalTextureFactory
-----------------------------------------------------------------------------*/
UFractalTexture* UFractalTextureFactory::CreateObject( UPackage* Pkg, const char* Name )
{
  GLogf( LOG_ERR, "UFractalTextureFactory::CreateObject stub" );
  return NULL;
}

UFractalTexture* UFractalTextureFactory::ImportObject( UPackage* Pkg, const char* Path )
{
  GLogf( LOG_ERR, "UFractalTextureFactory::ImportObject stub" );
  return NULL;
}

/*-----------------------------------------------------------------------------
 * UFontFactory
-----------------------------------------------------------------------------*/
UFont* UFontFactory::CreateObject( UPackage* Pkg, const char* Name )
{
  GLogf( LOG_ERR, "UFontFactory::CreateObject stub" );
  return NULL;
}

UFont* UFontFactory::ImportObject( UPackage* Pkg, const char* Path )
{
  GLogf( LOG_ERR, "UFontFactory::ImportObject stub" );
  return NULL;
}

IMPLEMENT_NATIVE_CLASS( UFactory );
IMPLEMENT_NATIVE_CLASS( UClassFactory );
IMPLEMENT_NATIVE_CLASS( ULevelFactory );
IMPLEMENT_NATIVE_CLASS( UMeshFactory );
IMPLEMENT_NATIVE_CLASS( UMusicFactory );
IMPLEMENT_NATIVE_CLASS( UPolysFactory );
IMPLEMENT_NATIVE_CLASS( USoundFactory );
IMPLEMENT_NATIVE_CLASS( UTextBufferFactory );
IMPLEMENT_NATIVE_CLASS( UPaletteFactory );
IMPLEMENT_NATIVE_CLASS( UTextureFactory );
IMPLEMENT_NATIVE_CLASS( UFractalTextureFactory );
IMPLEMENT_NATIVE_CLASS( UFontFactory );

