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
 * UExporter.h - Object export helpers
 *
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include "Editor/UEditorEngine.h"

/*-----------------------------------------------------------------------------
 * UExporter
 * Handles exporting of objects from packages
-----------------------------------------------------------------------------*/
class LIBUNR_API UExporter : public UObject
{
  DECLARE_ABSTRACT_CLASS( UExporter, UObject, CLASS_NoExport, Editor )
  static bool ExportObject( UObject* Obj, const char* Path, const char* Type );
};

/*-----------------------------------------------------------------------------
 * UClassExporter
 * Exports class objects
-----------------------------------------------------------------------------*/
class LIBUNR_API UClassExporter : public UExporter
{
  DECLARE_ABSTRACT_CLASS( UClassExporter, UExporter, CLASS_NoExport, Editor )
  static bool ExportObject( UClass* Obj, const char* Path, const char* Type );
};

/*-----------------------------------------------------------------------------
 * ULevelExporter
 * Exports level objects
-----------------------------------------------------------------------------*/
class LIBUNR_API ULevelExporter : public UExporter
{
  DECLARE_ABSTRACT_CLASS( ULevelExporter, UExporter, CLASS_NoExport, Editor )
  static bool ExportObject( ULevel* Obj, const char* Path, const char* Type );
};

/*-----------------------------------------------------------------------------
 * UMeshExporter
 * Exports mesh objects
-----------------------------------------------------------------------------*/
class LIBUNR_API UMeshExporter : public UExporter
{
  DECLARE_ABSTRACT_CLASS( UMeshExporter, UExporter, CLASS_NoExport, Editor )
  static bool ExportObject( UMesh* Obj, const char* Path, const char* Type );
};

/*-----------------------------------------------------------------------------
 * UMusicExporter
 * Exports music objects
-----------------------------------------------------------------------------*/
class LIBUNR_API UMusicExporter : public UExporter
{
  DECLARE_ABSTRACT_CLASS( UMusicExporter, UExporter, CLASS_NoExport, Editor )
  static bool ExportObject( UMusic* Obj, const char* Path, const char* Type );
};

/*-----------------------------------------------------------------------------
 * UPolysExporter
 * Exports music objects
-----------------------------------------------------------------------------*/
class LIBUNR_API UPolysExporter : public UExporter
{
  DECLARE_ABSTRACT_CLASS( UPolysExporter, UExporter, CLASS_NoExport, Editor )
  static bool ExportObject( UPolys* Obj, const char* Path, const char* Type );
};

/*-----------------------------------------------------------------------------
 * USoundExporter
 * Exports sound objects
-----------------------------------------------------------------------------*/
class LIBUNR_API USoundExporter : public UExporter
{
  DECLARE_ABSTRACT_CLASS( USoundExporter, UExporter, CLASS_NoExport, Editor )
  static bool ExportObject( USound* Obj, const char* Path, const char* Type );
};

/*-----------------------------------------------------------------------------
 * UTextBufferExporter
 * Exports text objects
-----------------------------------------------------------------------------*/
class LIBUNR_API UTextBufferExporter : public UExporter
{
  DECLARE_ABSTRACT_CLASS( UTextBufferExporter, UExporter, CLASS_NoExport, Editor )
  static bool ExportObject( UTextBuffer* Obj, const char* Path, const char* Type );
};

/*-----------------------------------------------------------------------------
 * UPaletteExporter
 * Exports palette objects
-----------------------------------------------------------------------------*/
class LIBUNR_API UPaletteExporter : public UExporter
{
  DECLARE_ABSTRACT_CLASS( UPaletteExporter, UExporter, CLASS_NoExport, Editor )
  static bool ExportObject( UPalette* Obj, const char* Path, const char* Type );
};

/*-----------------------------------------------------------------------------
 * UTextureExporter
 * Exports texture objects
-----------------------------------------------------------------------------*/
class LIBUNR_API UTextureExporter : public UExporter
{
  DECLARE_ABSTRACT_CLASS( UTextureExporter, UExporter, CLASS_NoExport, Editor )
  static bool ExportObject( UTexture* Obj, const char* Path, const char* Type );
};

/*-----------------------------------------------------------------------------
 * UFractalTextureExporter
 * Exports fractal texture objects
-----------------------------------------------------------------------------*/
class LIBUNR_API UFractalTextureExporter : public UExporter
{
  DECLARE_ABSTRACT_CLASS( UFractalTextureExporter, UExporter, CLASS_NoExport, Editor )
  static bool ExportObject( UFractalTexture* Obj, const char* Path, const char* Type );
};

/*-----------------------------------------------------------------------------
 * UFontExporter
 * Exports font objects
-----------------------------------------------------------------------------*/
class LIBUNR_API UFontExporter : public UExporter
{
  DECLARE_ABSTRACT_CLASS( UFontExporter, UExporter, CLASS_NoExport, Editor )
  static bool ExportObject( UFont* Obj, const char* Path, const char* Type );
};
