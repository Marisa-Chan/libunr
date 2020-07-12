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
 * UExporter.cpp - Object export helper code
 *
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Editor/UExporter.h"

/*-----------------------------------------------------------------------------
 * UExporter
-----------------------------------------------------------------------------*/
bool UExporter::ExportObject( UObject* Obj, const char* Path, const char* Type )
{
  if ( !Obj )
  {
    GLogf( LOG_WARN, "Tried to export NULL object" );
    return false;
  }

  if ( !Path )
  {
    GLogf( LOG_WARN, "NULL Path specified when exporting '%s'", Obj->Name.Data() );
    return false;
  }

  if ( Obj->Class == UClass::StaticClass() )
    return UClassExporter::ExportObject( (UClass*)Obj, Path, Type );
  else if ( Obj->Class == ULevel::StaticClass() )
    return ULevelExporter::ExportObject( (ULevel*)Obj, Path, Type );
  else if ( Obj->Class == UMesh::StaticClass() )
    return UMeshExporter::ExportObject( (UMesh*)Obj, Path, Type );
  else if ( Obj->Class == UMusic::StaticClass() )
    return UMusicExporter::ExportObject( (UMusic*)Obj, Path, Type );
  else if ( Obj->Class == UPolys::StaticClass() )
    return UPolysExporter::ExportObject( (UPolys*)Obj, Path, Type );
  else if ( Obj->Class == USound::StaticClass() )
    return USoundExporter::ExportObject( (USound*)Obj, Path, Type );
  else if ( Obj->Class == UTextBuffer::StaticClass() )
    return UTextBufferExporter::ExportObject( (UTextBuffer*)Obj, Path, Type );
  else if ( Obj->Class == UPalette::StaticClass() )
    return UPaletteExporter::ExportObject( (UPalette*)Obj, Path, Type );
  else if ( Obj->Class == UTexture::StaticClass() )
    return UTextureExporter::ExportObject( (UTexture*)Obj, Path, Type );
  else if ( Obj->Class == UFractalTexture::StaticClass() )
    return UFractalTextureExporter::ExportObject( (UFractalTexture*)Obj, Path, Type );
  else if ( Obj->Class == UFont::StaticClass() )
    return UFontExporter::ExportObject( (UFont*)Obj, Path, Type );
  else
    GLogf( LOG_ERR, "Object '%s' has unexportable type '%s'", Obj->Name.Data(), Obj->Class->Name.Data() );

  return false;
}

/*-----------------------------------------------------------------------------
 * UClassExporter
-----------------------------------------------------------------------------*/
bool UClassExporter::ExportObject( UClass* Obj, const char* Path, const char* Type )
{
  GLogf( LOG_ERR, "UClassExporter::ExportObject stub" );
  return false;
}

/*-----------------------------------------------------------------------------
 * ULevelExporter
-----------------------------------------------------------------------------*/
bool ULevelExporter::ExportObject( ULevel* Obj, const char* Path, const char* Type )
{
  GLogf( LOG_ERR, "ULevelExporter::ExportObject stub" );
  return false;
}

/*-----------------------------------------------------------------------------
 * UMeshExporter
-----------------------------------------------------------------------------*/
bool UMeshExporter::ExportObject( UMesh* Obj, const char* Path, const char* Type )
{
  GLogf( LOG_ERR, "UMeshExporter::ExportObject stub" );
  return false;
}

/*-----------------------------------------------------------------------------
 * UMusicExporter
-----------------------------------------------------------------------------*/
bool UMusicExporter::ExportObject( UMusic* Obj, const char* Path, const char* Type )
{
  GLogf( LOG_ERR, "UMusicExporter::ExportObject stub" );
  return false;
}

/*-----------------------------------------------------------------------------
 * UPolysExporter
-----------------------------------------------------------------------------*/
bool UPolysExporter::ExportObject( UPolys* Obj, const char* Path, const char* Type )
{
  GLogf( LOG_ERR, "UPolysExporter::ExportObject stub" );
  return false;
}

/*-----------------------------------------------------------------------------
 * USoundExporter
-----------------------------------------------------------------------------*/
bool USoundExporter::ExportObject( USound* Obj, const char* Path, const char* Type )
{
  GLogf( LOG_ERR, "USoundExporter::ExportObject stub" );
  return false;
}

/*-----------------------------------------------------------------------------
 * UTextBufferExporter
-----------------------------------------------------------------------------*/
bool UTextBufferExporter::ExportObject( UTextBuffer* Obj, const char* Path, const char* Type )
{
  GLogf( LOG_ERR, "UTextBufferExporter::ExportObject stub" );
  return false;
}

/*-----------------------------------------------------------------------------
 * UPaletteExporter
-----------------------------------------------------------------------------*/
bool UPaletteExporter::ExportObject( UPalette* Obj, const char* Path, const char* Type )
{
  GLogf( LOG_ERR, "UPaletteExporter::ExportObject stub" );
  return false;
}

/*-----------------------------------------------------------------------------
 * UTextureExporter
-----------------------------------------------------------------------------*/
bool UTextureExporter::ExportObject( UTexture* Obj, const char* Path, const char* Type )
{
  GLogf( LOG_ERR, "UTextureExporter::ExportObject stub" );
  return false;
}

/*-----------------------------------------------------------------------------
 * UFractalTextureExporter
-----------------------------------------------------------------------------*/
bool UFractalTextureExporter::ExportObject( UFractalTexture* Obj, const char* Path, const char* Type )
{
  GLogf( LOG_ERR, "UFractalTextureExporter::ExportObject stub" );
  return false;
}

/*-----------------------------------------------------------------------------
 * UFontExporter
-----------------------------------------------------------------------------*/
bool UFontExporter::ExportObject( UFont* Obj, const char* Path, const char* Type )
{
  GLogf( LOG_ERR, "UFontExporter::ExportObject stub" );
  return false;
}

IMPLEMENT_NATIVE_CLASS( UExporter );
IMPLEMENT_NATIVE_CLASS( UClassExporter );
IMPLEMENT_NATIVE_CLASS( ULevelExporter );
IMPLEMENT_NATIVE_CLASS( UMeshExporter );
IMPLEMENT_NATIVE_CLASS( UMusicExporter );
IMPLEMENT_NATIVE_CLASS( UPolysExporter );
IMPLEMENT_NATIVE_CLASS( USoundExporter );
IMPLEMENT_NATIVE_CLASS( UTextBufferExporter );
IMPLEMENT_NATIVE_CLASS( UPaletteExporter );
IMPLEMENT_NATIVE_CLASS( UTextureExporter );
IMPLEMENT_NATIVE_CLASS( UFractalTextureExporter );
IMPLEMENT_NATIVE_CLASS( UFontExporter );
