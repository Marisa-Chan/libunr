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
 * UFactory.h - Object import helpers
 *
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include "Editor/UEditorEngine.h"

/*-----------------------------------------------------------------------------
 * UFactory
 * Constructs new objects for packages by importing assets
-----------------------------------------------------------------------------*/
class LIBUNR_API UFactory : public UObject
{
  DECLARE_ABSTRACT_CLASS( UFactory, UObject, CLASS_NoExport, Editor )
  static UObject* CreateObject( UPackage* Pkg, UClass* Class, const char* Name );
  static UObject* ImportObject( UPackage* Pkg, UClass* Class, const char* Path );
};

/*-----------------------------------------------------------------------------
 * UClassFactory
 * Creates class objects
-----------------------------------------------------------------------------*/
class LIBUNR_API UClassFactory : public UFactory
{
  DECLARE_ABSTRACT_CLASS( UClassFactory, UFactory, CLASS_NoExport, Editor )
  static UClass* CreateObject( UPackage* Pkg, const char* Name );
  static UClass* ImportObject( UPackage* Pkg, const char* Path );
};

/*-----------------------------------------------------------------------------
 * ULevelFactory
 * Creates level objects
-----------------------------------------------------------------------------*/
class LIBUNR_API ULevelFactory : public UFactory
{
  DECLARE_ABSTRACT_CLASS( ULevelFactory, UFactory, CLASS_NoExport, Editor )
  static ULevel* CreateObject( UPackage* Pkg, const char* Name );
  static ULevel* ImportObject( UPackage* Pkg, const char* Path );
};

/*-----------------------------------------------------------------------------
 * UMeshFactory
 * Creates model objects
-----------------------------------------------------------------------------*/
class LIBUNR_API UMeshFactory : public UFactory
{
  DECLARE_ABSTRACT_CLASS( UMeshFactory, UFactory, CLASS_NoExport, Editor )
  static UMesh* CreateObject( UPackage* Pkg, const char* Name );
  static UMesh* ImportObject( UPackage* Pkg, const char* Path );
};

/*-----------------------------------------------------------------------------
 * UMusicFactory
 * Creates music objects
-----------------------------------------------------------------------------*/
class LIBUNR_API UMusicFactory : public UFactory
{
  DECLARE_ABSTRACT_CLASS( UMusicFactory, UFactory, CLASS_NoExport, Editor )
  static UMusic* CreateObject( UPackage* Pkg, const char* Name );
  static UMusic* ImportObject( UPackage* Pkg, const char* Path );
};

/*-----------------------------------------------------------------------------
 * UPolysFactory
 * Creates music objects
-----------------------------------------------------------------------------*/
class LIBUNR_API UPolysFactory : public UFactory
{
  DECLARE_ABSTRACT_CLASS( UPolysFactory, UFactory, CLASS_NoExport, Editor )
  static UPolys* CreateObject( UPackage* Pkg, const char* Name );
  static UPolys* ImportObject( UPackage* Pkg, const char* Path );
};

/*-----------------------------------------------------------------------------
 * USoundFactory
 * Creates sound objects
-----------------------------------------------------------------------------*/
class LIBUNR_API USoundFactory : public UFactory
{
  DECLARE_ABSTRACT_CLASS( USoundFactory, UFactory, CLASS_NoExport, Editor )
  static USound* CreateObject( UPackage* Pkg, const char* Name );
  static USound* ImportObject( UPackage* Pkg, const char* Path );
};

/*-----------------------------------------------------------------------------
 * UTextBufferFactory
 * Creates text objects
-----------------------------------------------------------------------------*/
class LIBUNR_API UTextBufferFactory : public UFactory
{
  DECLARE_ABSTRACT_CLASS( UTextBufferFactory, UFactory, CLASS_NoExport, Editor )
  static UTextBuffer* CreateObject( UPackage* Pkg, const char* Name );
  static UTextBuffer* ImportObject( UPackage* Pkg, const char* Path );
};

/*-----------------------------------------------------------------------------
 * UPaletteFactory
 * Creates palette objects
-----------------------------------------------------------------------------*/
class LIBUNR_API UPaletteFactory : public UFactory
{
  DECLARE_ABSTRACT_CLASS( UPaletteFactory, UFactory, CLASS_NoExport, Editor )
  static UPalette* CreateObject( UPackage* Pkg, const char* Name );
  static UPalette* ImportObject( UPackage* Pkg, const char* Path );
};

/*-----------------------------------------------------------------------------
 * UTextureFactory
 * Creates texture objects
-----------------------------------------------------------------------------*/
class LIBUNR_API UTextureFactory : public UFactory
{
  DECLARE_ABSTRACT_CLASS( UTextureFactory, UFactory, CLASS_NoExport, Editor )
  static UTexture* CreateObject( UPackage* Pkg, const char* Name );
  static UTexture* ImportObject( UPackage* Pkg, const char* Path );
};

/*-----------------------------------------------------------------------------
 * UFractalTextureFactory
 * Creates fractal texture objects
-----------------------------------------------------------------------------*/
class LIBUNR_API UFractalTextureFactory : public UFactory
{
  DECLARE_ABSTRACT_CLASS( UFractalTextureFactory, UFactory, CLASS_NoExport, Editor )
  static UFractalTexture* CreateObject( UPackage* Pkg, const char* Name );
  static UFractalTexture* ImportObject( UPackage* Pkg, const char* Path );
};

/*-----------------------------------------------------------------------------
 * UFontFactory
 * Creates font objects
-----------------------------------------------------------------------------*/
class LIBUNR_API UFontFactory : public UFactory
{
  DECLARE_ABSTRACT_CLASS( UFontFactory, UFactory, CLASS_NoExport, Editor )
  static UFont* CreateObject( UPackage* Pkg, const char* Name );
  static UFont* ImportObject( UPackage* Pkg, const char* Path );
};
