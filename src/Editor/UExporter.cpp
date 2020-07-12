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
#include "Core/UProperty.h"
#include "Engine/USkelMesh.h"
#include "Actors/ABrush.h"
#include "Actors/APlayerPawn.h"

/*-----------------------------------------------------------------------------
 * UExporter
-----------------------------------------------------------------------------*/
bool UExporter::ExportObject( UObject* Obj, const char* Dir, const char* Type, int OptArg )
{
  if ( !Obj )
  {
    GLogf( LOG_WARN, "Tried to export NULL object" );
    return false;
  }

  if ( !Dir )
  {
    GLogf( LOG_WARN, "NULL directory path specified when exporting '%s'", Obj->Name.Data() );
    return false;
  }

  if ( Obj->Class == UClass::StaticClass() )
    return UClassExporter::ExportObject( (UClass*)Obj, Dir, Type );
  else if ( Obj->Class == ULevel::StaticClass() )
    return ULevelExporter::ExportObject( (ULevel*)Obj, Dir, Type );
  else if ( Obj->Class->ClassIsA( UMesh::StaticClass() ) )
    return UMeshExporter::ExportObject( (UMesh*)Obj, Dir, Type, OptArg );
  else if ( Obj->Class == UMusic::StaticClass() )
    return UMusicExporter::ExportObject( (UMusic*)Obj, Dir, Type );
  else if ( Obj->Class == UPolys::StaticClass() )
    return UPolysExporter::ExportObject( (UPolys*)Obj, Dir, Type );
  else if ( Obj->Class == USound::StaticClass() )
    return USoundExporter::ExportObject( (USound*)Obj, Dir, Type );
  else if ( Obj->Class == UTextBuffer::StaticClass() )
    return UTextBufferExporter::ExportObject( (UTextBuffer*)Obj, Dir, Type );
  else if ( Obj->Class == UPalette::StaticClass() )
    return UPaletteExporter::ExportObject( (UPalette*)Obj, Dir, Type );
  else if ( Obj->Class == UTexture::StaticClass() )
    return UTextureExporter::ExportObject( (UTexture*)Obj, Dir, Type );
  else if ( Obj->Class->ClassIsA( UFractalTexture::StaticClass() ) )
    return UFractalTextureExporter::ExportObject( (UFractalTexture*)Obj, Dir, Type );
  else if ( Obj->Class == UFont::StaticClass() )
    return UFontExporter::ExportObject( (UFont*)Obj, Dir, Type );
  else
    GLogf( LOG_ERR, "Object '%s' has unexportable type '%s'", Obj->Name.Data(), Obj->Class->Name.Data() );

  return false;
}

/*-----------------------------------------------------------------------------
 * UClassExporter
-----------------------------------------------------------------------------*/
bool UClassExporter::ExportObject( UClass* Obj, const char* Dir, const char* Type )
{
  FString Filename( Dir );
#if defined LIBUNR_WIN32
  Filename.ReplaceChars( '\\', '/' );
#endif
  if ( Filename.Back() != '/' )
    Filename += '/';

  Filename += Obj->Name.Data();
  Filename += ".uc"; // Scripts won't get exported to any other type

  GLogf( LOG_INFO, "Exporting %s.uc", Obj->Name.Data() );

  // Write script text
  FFileArchiveOut* Out = new FFileArchiveOut();
  if ( Out->Open( Filename ) != 0 )
  {
    GLogf( LOG_WARN, "Failed to export script to file '%s'", Filename.Data() );
    return false;
  }
  Out->Write( Obj->ScriptText->Text->Data(), Obj->ScriptText->Text->Size() );

  // Write default properties
  FString ValueBuf;
  Out->Printf( "defaultproperties\r\n{\r\n" );
  for ( UField* Iter = Obj->Default->Field; Iter != NULL; Iter = Iter->Next )
  {
    if ( Iter->IsA( UProperty::StaticClass() ) )
    {
      UProperty* PropIter = (UProperty*)Iter;
      if ( UNLIKELY( PropIter->Offset & 0x80000000 ) )
      {
        GLogf( LOG_WARN, "Bad offset for property '%s' (Offset = %x)",
          PropIter->Name.Data(), PropIter->Offset );
        continue;
      }

      for ( int i = 0; i < PropIter->ArrayDim; i++ )
      {
        if ( PropIter->ObjectFlags & RF_TagExp )
        {
          // Get default property from super class
          UObject* DefObj = (PropIter->Outer == Obj) ? NULL : Obj->SuperClass->Default;
          PropIter->GetText( ValueBuf, Obj->Default, DefObj, i );

          if ( ValueBuf.Length() > 0 )
          {
            if ( PropIter->Class == UArrayProperty::StaticClass() )
            {
              Out->Write( ValueBuf.Data(), ValueBuf.Length() );
              ValueBuf.Erase();
              continue;
            }

            if ( PropIter->ArrayDim > 1 )
              Out->Printf( "\t%s(%i)=%s\r\n", PropIter->Name.Data(), i, ValueBuf.Data() );
            else
              Out->Printf( "\t%s=%s\r\n", PropIter->Name.Data(), ValueBuf.Data() );

            ValueBuf.Erase();
          }
        }
      }
    }
  }
  Out->Printf( "}\r\n" );

  Out->Close();
  delete Out;
  return true;
}

/*-----------------------------------------------------------------------------
 * ULevelExporter
-----------------------------------------------------------------------------*/
bool ULevelExporter::ExportObject( ULevel* Obj, const char* Dir, const char* Type )
{
  FString Filename( Dir );
#if defined LIBUNR_WIN32
  Filename.ReplaceChars( '\\', '/' );
#endif
  if ( Filename.Back() != '/' )
    Filename += "/";

  Filename += Obj->Pkg->Name;
  Filename += ".t3d"; // Levels won't get exported to any other type

  FFileArchiveOut* Out = new FFileArchiveOut();
  if ( Out->Open( Filename ) != 0 )
  {
    GLogf( LOG_WARN, "Failed to export level to file '%s'", Filename.Data() );
    return false;
  }

  // Temporarily change package name to MyLevel
  FName PkgName = Obj->Pkg->Name;
  Obj->Pkg->Name = FName( "MyLevel" );

  // Write beginning map
  Out->Printf( "Begin Map\r\n" );

  // Loop through all actors
  FString ValueBuf;
  for ( size_t i = 0; i < Obj->Actors.Size(); i++ )
  {
    AActor* Actor = Obj->Actors[i];

    // Don't export unnecessary actors
    if ( Actor->Class == ACamera::StaticClass() ||
      (Actor->Class->ClassFlags & CLASS_NoUserCreate) )
      continue;

    // Write begin actor
    Out->Printf( "Begin Actor Class=%s Name=%s\r\n", Actor->Class->Name.Data(), Actor->Name.Data() );

    // Write actor properties
    for ( UField* Iter = Actor->Field; Iter != NULL; Iter = Iter->Next )
    {
      UProperty* Prop = SafeCast<UProperty>( Iter );
      if ( Prop != NULL )
      {
        if ( UNLIKELY( Prop->Offset & 0x80000000 ) )
        {
          GLogf( LOG_WARN, "Bad offset for property '%s' in Actor '%s' (Offset = %x)",
            Prop->Name.Data(), Actor->Name.Data(), Prop->Offset );
          continue;
        }

        if ( !(Prop->PropertyFlags & (CPF_Const | CPF_EditConst)) || (Prop->PropertyFlags & (CPF_Edit | CPF_ExportObject)) )
        {
          for ( int j = 0; j < Prop->ArrayDim; j++ )
          {
            // Get default property of this class
            Prop->GetText( ValueBuf, Actor, Actor->Class->Default, j );

            if ( ValueBuf.Length() > 0 )
            {
              if ( Prop->Class == UArrayProperty::StaticClass() )
              {
                Out->Write( ValueBuf.Data(), ValueBuf.Length() );
                ValueBuf.Erase();
                continue;
              }
              else if ( Prop->Name == ABrush::StaticClass()->Name )
              {
                ABrush* Brush = (ABrush*)Actor;
                Brush->ExportToLevelText( Out );
              }
              else if ( Prop->Class == UObjectProperty::StaticClass() &&
                ((UObjectProperty*)Prop)->ObjectType->ClassIsA( ANavigationPoint::StaticClass() ) )
              {
                ValueBuf.Erase();
                break;
              }

              if ( Prop->ArrayDim > 1 )
                Out->Printf( "\t%s(%i)=%s\r\n", Prop->Name.Data(), j, ValueBuf.Data() );
              else
                Out->Printf( "\t%s=%s\r\n", Prop->Name.Data(), ValueBuf.Data() );

              ValueBuf.Erase();
            }
          }
        }
      }
    }

    // Write end actor
    Out->Write( (char*)"End Actor\r\n", 11 );
  }

  // Write end map
  Out->Write( (char*)"End Map\r\n", 9 );
  Out->Close();
  delete Out;

  // Restore package name
  Obj->Pkg->Name = PkgName;
  return true;
}

/*-----------------------------------------------------------------------------
 * UMeshExporter
-----------------------------------------------------------------------------*/
bool UMeshExporter::ExportObject( UMesh* Obj, const char* Dir, const char* Type, int Frame )
{
  if ( Obj->Class == ULodMesh::StaticClass() )
  {
    if ( stricmp( Type, "3d" ) == 0 )
      return ExportLodMesh3d( (ULodMesh*)Obj, Dir );
    else if ( stricmp( Type, "obj" ) == 0 )
      return ExportLodMeshObj( (ULodMesh*)Obj, Dir, Frame );
    else
      GLogf( LOG_ERR, "Unknown lod mesh export type '%s'", Type );
  }
  else if ( Obj->Class == UMesh::StaticClass() )
  {
    if ( stricmp( Type, "3d" ) == 0 )
      return ExportOldMesh3d( Obj, Dir );
    else if ( stricmp( Type, "obj" ) == 0 )
      return ExportOldMeshObj( Obj, Dir, Frame );
    else
      GLogf( LOG_ERR, "Unknown legacy mesh export type '%s'", Type );
  }
  else if ( Obj->Class == USkeletalMesh::StaticClass() )
  {
    return ExportSkeletalMesh( (USkeletalMesh*)Obj, Dir );
  }
  else
  {
    GLogf( LOG_ERR, "Mesh '%s' is of unknown type '%s'", Obj->Name.Data(), Obj->Class->Name.Data() );
  }
  return false;
}

bool UMeshExporter::ExportLodMesh3d( ULodMesh* Obj, const char* Dir )
{
  FFileArchiveOut Out;
  FString Filename( Dir );
#if defined LIBUNR_WIN32
  Filename.ReplaceChars( '\\', '/' );
#endif
  if ( Filename.Back() != '/' )
    Filename += '/';

  Filename += Obj->Name.Data();

  // Export data file
  FString DataFileName( Filename );
  DataFileName += "_d.3d";

  if ( Out.Open( DataFileName ) != 0 )
  {
    GLogf( LOG_WARN, "Failed to export LOD mesh to data file '%s'", DataFileName.Data() );
    return false;
  }

  FVertexDataHeader DataHdr;
  memset( &DataHdr, 0, sizeof( DataHdr ) ); // Everything else goes unused
  DataHdr.NumPolygons = (u16)Obj->Faces.Size();
  DataHdr.NumVertices = Obj->FrameVerts;

  Out.Write( &DataHdr, sizeof( DataHdr ) );

  FVertexDataTri TriOut;
  for ( int i = 0; i < DataHdr.NumPolygons; i++ )
  {
    // Maybe inline this so it's a bit neater
    FLodWedge TriWedges[3];
    TriWedges[0] = Obj->Wedges[Obj->Faces[i].WedgeIndex[0]];
    TriWedges[1] = Obj->Wedges[Obj->Faces[i].WedgeIndex[1]];
    TriWedges[2] = Obj->Wedges[Obj->Faces[i].WedgeIndex[2]];

    TriOut.Vertex[0] = TriWedges[0].VertexIndex;
    TriOut.Vertex[1] = TriWedges[1].VertexIndex;
    TriOut.Vertex[2] = TriWedges[2].VertexIndex;
    TriOut.VertexUV[0][0] = TriWedges[0].S;
    TriOut.VertexUV[1][0] = TriWedges[1].S;
    TriOut.VertexUV[2][0] = TriWedges[2].S;
    TriOut.VertexUV[0][1] = TriWedges[0].T;
    TriOut.VertexUV[1][1] = TriWedges[1].T;
    TriOut.VertexUV[2][1] = TriWedges[2].T;

    // Mesh flags inside of a package use EPolyFlags
    // Mesh flags in a _d.3d file use something else
    TriOut.Type = 0;
    FLodMaterial Mat = Obj->Materials[Obj->Faces[i].MaterialIndex];

    // Weapon triangles are only in the special faces array
    if ( Obj->SpecialFaces.Size() == 1 && i == Obj->Faces.Size() - 1 )
      TriOut.Type = JMT_WeaponTriangle;
    else if ( Mat.Flags & PF_TwoSided )
    {
      if ( Mat.Flags & PF_Modulated )
        TriOut.Type = JMT_ModulatedTwoSided;
      else if ( Mat.Flags & PF_Masked )
        TriOut.Type = JMT_MaskedTwoSided;
      else if ( Mat.Flags & PF_Translucent )
        TriOut.Type = JMT_Translucent;
      else
        TriOut.Type = JMT_TwoSided;
    }
    TriOut.Color = 127; // ???
    TriOut.TexNum = Mat.TextureIndex;
    TriOut.Flags = 0; // ???

    Out.Write( &TriOut, sizeof( TriOut ) );
  }

  Out.Close();
  FString AnivFileName( Filename );
  AnivFileName += "_a.3d";

  if ( Out.Open( AnivFileName ) != 0 )
  {
    GLogf( LOG_WARN, "Failed to export LOD mesh to aniv file '%s'", AnivFileName.Data() );
    return false;
  }

  FVertexAnivHeader AnivHdr;
  AnivHdr.NumFrames = Obj->AnimFrames;
  AnivHdr.FrameSize = Obj->FrameVerts * 4;
  Out.Write( &AnivHdr, sizeof( AnivHdr ) );

  uint32_t VtxOut;
  for ( u32 i = 0; i < Obj->AnimFrames; i++ )
  {
    FORTIFY_LOOP_SLOW( i, MAX_UINT32 );
    for ( u32 j = 0; j < Obj->FrameVerts; j++ )
    {
      FORTIFY_LOOP_SLOW( i, MAX_UINT32 );
      u32 Index = (i * Obj->FrameVerts) + j;
      FMeshVert Vtx = Obj->Verts[Index];
      VtxOut = ((int)(-Vtx.X * 8.0) & 0x7ff) | (((int)(-Vtx.Y * 8.0) & 0x7ff) << 11) | (((int)(Vtx.Z * 4.0) & 0x3ff) << 22);
      Out.Write( &VtxOut, sizeof( VtxOut ) );
    }
  }

  Out.Close();

  return true;
}

bool UMeshExporter::ExportLodMeshObj( ULodMesh* Obj, const char* Dir, int Frame )
{
  FFileArchiveOut Out;
  FString Filename( Dir );
#if defined LIBUNR_WIN32
  Filename.ReplaceChars( '\\', '/' );
#endif
  if ( Filename.Back() != '/' )
    Filename += '/';

  Filename += Obj->Name.Data();

  // Export obj mesh

  FString ObjFileName = Filename;
  ObjFileName += "_";
  ObjFileName += FString( Frame );
  ObjFileName += ".obj";

  if ( Out.Open( ObjFileName ) != 0 )
  {
    GLogf( LOG_WARN, "Failed to export LOD mesh to obj file '%s'", ObjFileName.Data() );
    return false;
  }

  Out.Printf( "# OBJ File generated with libunr LODMesh exporter\n" );
  Out.Printf( "mtllib %s.mtl\n", Obj->Name.Data() );
  Out.Printf( "o %s\n", Obj->Name.Data() );

  // Vertex coordinates
  size_t FirstVert = (size_t)Frame * Obj->FrameVerts;
  for ( u32 i = 0; i < Obj->FrameVerts; i++ )
  {
    FORTIFY_LOOP_SLOW( i, MAX_UINT32 );
    FMeshVert Vtx = Obj->Verts[FirstVert + i];
    Out.Printf( "v %.6f %.6f %.6f\n", -Vtx.X, -Vtx.Y, -Vtx.Z );
  }

  // Vertex UVs
  for ( int i = 0; i < Obj->Wedges.Size(); i++ )
  {
    FLodWedge Wedge = Obj->Wedges[i];
    Out.Printf( "vt %.6f %.6f\n", Wedge.S / 255.0, (255 - Wedge.T) / 255.0 );
  }

  // Vertex Normals
  FVector* VertNormals = new FVector[Obj->FrameVerts];
  memset( VertNormals, 0, sizeof( FVector ) * Obj->FrameVerts );

  FVector FaceNormals[2];
  FLodWedge TriWedges[2][3];
  int v[2][3];
  int i = 0;

  if ( Obj->Faces.Size() & 1 )
  {
    TriWedges[0][0] = Obj->Wedges[Obj->Faces[0].WedgeIndex[0]];
    TriWedges[0][1] = Obj->Wedges[Obj->Faces[0].WedgeIndex[1]];
    TriWedges[0][2] = Obj->Wedges[Obj->Faces[0].WedgeIndex[2]];

    v[0][0] = TriWedges[0][0].VertexIndex;
    v[0][1] = TriWedges[0][1].VertexIndex;
    v[0][2] = TriWedges[0][2].VertexIndex;

    FVector U = *(FVector*)&Obj->Verts[v[0][0]] - *(FVector*)&Obj->Verts[v[0][1]];
    FVector V = *(FVector*)&Obj->Verts[v[0][2]] - *(FVector*)&Obj->Verts[v[0][1]];

    FaceNormals[0].X = (U.Y * V.Z) - (U.Z * V.Y);
    FaceNormals[0].Y = (U.Z * V.X) - (U.X * V.Z);
    FaceNormals[0].Z = (U.X * V.Y) - (U.Y * V.X);

    VertNormals[v[0][0]] += FaceNormals[0];
    VertNormals[v[0][1]] += FaceNormals[0];
    VertNormals[v[0][2]] += FaceNormals[0];

    i++;
  }
  for ( ; i < Obj->Faces.Size(); i += 2 )
  {
    // Loop unrolling, woo
    TriWedges[0][0] = Obj->Wedges[Obj->Faces[i].WedgeIndex[0]];
    TriWedges[0][1] = Obj->Wedges[Obj->Faces[i].WedgeIndex[1]];
    TriWedges[0][2] = Obj->Wedges[Obj->Faces[i].WedgeIndex[2]];

    TriWedges[1][0] = Obj->Wedges[Obj->Faces[i + 1].WedgeIndex[0]];
    TriWedges[1][1] = Obj->Wedges[Obj->Faces[i + 1].WedgeIndex[1]];
    TriWedges[1][2] = Obj->Wedges[Obj->Faces[i + 1].WedgeIndex[2]];

    v[0][0] = TriWedges[0][0].VertexIndex;
    v[0][1] = TriWedges[0][1].VertexIndex;
    v[0][2] = TriWedges[0][2].VertexIndex;

    v[1][0] = TriWedges[1][0].VertexIndex;
    v[1][1] = TriWedges[1][1].VertexIndex;
    v[1][2] = TriWedges[1][2].VertexIndex;

    FVector U1 = *(FVector*)&Obj->Verts[FirstVert + v[0][0]] - *(FVector*)&Obj->Verts[FirstVert + v[0][1]];
    FVector V1 = *(FVector*)&Obj->Verts[FirstVert + v[0][2]] - *(FVector*)&Obj->Verts[FirstVert + v[0][1]];

    FVector U2 = *(FVector*)&Obj->Verts[FirstVert + v[1][0]] - *(FVector*)&Obj->Verts[FirstVert + v[1][1]];
    FVector V2 = *(FVector*)&Obj->Verts[FirstVert + v[1][2]] - *(FVector*)&Obj->Verts[FirstVert + v[1][1]];

    FaceNormals[0].X = (U1.Y * V1.Z) - (U1.Z * V1.Y);
    FaceNormals[0].Y = (U1.Z * V1.X) - (U1.X * V1.Z);
    FaceNormals[0].Z = (U1.X * V1.Y) - (U1.Y * V1.X);

    FaceNormals[1].X = (U2.Y * V2.Z) - (U2.Z * V2.Y);
    FaceNormals[1].Y = (U2.Z * V2.X) - (U2.X * V2.Z);
    FaceNormals[1].Z = (U2.X * V2.Y) - (U2.Y * V2.X);

    VertNormals[v[0][0]] += FaceNormals[0];
    VertNormals[v[0][1]] += FaceNormals[0];
    VertNormals[v[0][2]] += FaceNormals[0];

    VertNormals[v[1][0]] += FaceNormals[1];
    VertNormals[v[1][1]] += FaceNormals[1];
    VertNormals[v[1][2]] += FaceNormals[1];
  }
  for ( u32 i = 0; i < Obj->FrameVerts; i++ )
  {
    FORTIFY_LOOP_SLOW( i, MAX_UINT32 );
    FVector Normal = VertNormals[i];
    if ( Normal.X == 0 && Normal.Y == 0 && Normal.Z == 0 )
      continue;

    float Norm = sqrtf( (Normal.X * Normal.X) + (Normal.Y * Normal.Y) + (Normal.Z * Normal.Z) );

    Normal.X /= Norm;
    Normal.Y /= Norm;
    Normal.Z /= Norm;

    Out.Printf( "vn %.6f %.6f %.6f\n", Normal.X, Normal.Y, Normal.Z );
  }
  delete[] VertNormals;

  // Polygons
  FName TexName = 0;
  FName OldName = 0;
  for ( int i = 0; i < Obj->Faces.Size(); i++ )
  {
    FLodFace Face = Obj->Faces[i];

    FLodWedge TriWedges[3];
    TriWedges[0] = Obj->Wedges[Face.WedgeIndex[0]];
    TriWedges[1] = Obj->Wedges[Face.WedgeIndex[1]];
    TriWedges[2] = Obj->Wedges[Face.WedgeIndex[2]];

    // Change mtl assignment
    int MaterialIndex = Face.MaterialIndex;
    TexName = Obj->Textures[Obj->Materials[Face.MaterialIndex].TextureIndex]->Name;
    if ( TexName != OldName )
      Out.Printf( "usemtl %s\n", TexName.Data() );
    OldName = TexName;

    Out.Printf( "f %i/%i/%i %i/%i/%i %i/%i/%i\n",
      TriWedges[0].VertexIndex + 1, Face.WedgeIndex[0] + 1, TriWedges[0].VertexIndex + 1,
      TriWedges[1].VertexIndex + 1, Face.WedgeIndex[1] + 1, TriWedges[1].VertexIndex + 1,
      TriWedges[2].VertexIndex + 1, Face.WedgeIndex[2] + 1, TriWedges[2].VertexIndex + 1
    );
  }

  Out.Close();

  FString MtlFileName = Filename;
  MtlFileName += ".mtl";

  if ( Out.Open( MtlFileName ) != 0 )
  {
    GLogf( LOG_WARN, "Failed to generate MTL file for LOD mesh '%s'", Obj->Name.Data() );
    return false;
  }

  Out.Printf( "# MTL File generated with libunr LODMesh exporter\n" );
  Out.Printf( "# Materials: %i\n", Obj->Materials.Size() );

  for ( int i = 0; i < Obj->Materials.Size(); i++ )
  {
    Out.Printf( "newmtl %s\n", Obj->Textures[Obj->Materials[i].TextureIndex]->Name.Data() );
    Out.Printf( "Ns 0\n" );
    Out.Printf( "Ka %.6f %.6f %.6f\n", 0.0f, 0.0f, 0.0f ); /// ???
    Out.Printf( "Kd %.1f %.1f %.1f\n", 0.1f, 0.1f, 0.1f ); /// ???
    Out.Printf( "Ks %.1f %.1f %.1f\n", 0.1f, 0.1f, 0.1f ); /// ???
    Out.Printf( "d 1\n" );
    Out.Printf( "illum 1\n\n" );
  }

  Out.Close();
  return true;
}

bool UMeshExporter::ExportOldMesh3d( UMesh* Obj, const char* Dir )
{
  FFileArchiveOut Out;
  FString Filename( Dir );
#if defined LIBUNR_WIN32
  Filename.ReplaceChars( '\\', '/' );
#endif
  if ( Filename.Back() != '/' )
    Filename += '/';

  Filename += Obj->Name.Data();

  // Export data file
  FString DataFileName( Filename );
  DataFileName += "_d.3d";

  if ( Out.Open( DataFileName ) != 0 )
  {
    GLogf( LOG_WARN, "Failed to export LOD mesh to data file '%s'", DataFileName.Data() );
    return false;
  }

  FVertexDataHeader DataHdr;
  memset( &DataHdr, 0, sizeof( DataHdr ) ); // Everything else goes unused
  DataHdr.NumPolygons = (u16)Obj->Tris.Size();
  DataHdr.NumVertices = Obj->FrameVerts;

  Out.Write( &DataHdr, sizeof( DataHdr ) );

  FVertexDataTri TriOut;
  for ( int i = 0; i < DataHdr.NumPolygons; i++ )
  {
    TriOut.Vertex[0] = Obj->Tris[i].Vertex[0];
    TriOut.Vertex[1] = Obj->Tris[i].Vertex[1];
    TriOut.Vertex[2] = Obj->Tris[i].Vertex[2];
    TriOut.VertexUV[0][0] = Obj->Tris[i].VertexUV[0][0];
    TriOut.VertexUV[1][0] = Obj->Tris[i].VertexUV[1][0];
    TriOut.VertexUV[2][0] = Obj->Tris[i].VertexUV[2][0];
    TriOut.VertexUV[0][1] = Obj->Tris[i].VertexUV[0][1];
    TriOut.VertexUV[1][1] = Obj->Tris[i].VertexUV[1][1];
    TriOut.VertexUV[2][1] = Obj->Tris[i].VertexUV[2][1];

    // Weapon triangles are only in the special faces array
    if ( Obj->Tris[i].Flags & PF_TwoSided )
    {
      if ( Obj->Tris[i].Flags & PF_Modulated )
        TriOut.Type = JMT_ModulatedTwoSided;
      else if ( Obj->Tris[i].Flags & PF_Masked )
        TriOut.Type = JMT_MaskedTwoSided;
      else if ( Obj->Tris[i].Flags & PF_Translucent )
        TriOut.Type = JMT_Translucent;
      else
        TriOut.Type = JMT_TwoSided;
    }
    else
    {
      TriOut.Type = JMT_Normal;
    }
    TriOut.Color = 127; // ???
    TriOut.TexNum = Obj->Tris[i].TextureIndex;
    TriOut.Flags = 0; // ???

    Out.Write( &TriOut, sizeof( TriOut ) );
  }

  Out.Close();
  FString AnivFileName( Filename );
  AnivFileName += "_a.3d";

  if ( Out.Open( AnivFileName ) != 0 )
  {
    GLogf( LOG_WARN, "Failed to export LOD mesh to aniv file '%s'", AnivFileName.Data() );
    return false;
  }

  FVertexAnivHeader AnivHdr;
  AnivHdr.NumFrames = Obj->AnimFrames;
  AnivHdr.FrameSize = Obj->FrameVerts * 4;
  Out.Write( &AnivHdr, sizeof( AnivHdr ) );

  uint32_t VtxOut;
  for ( u32 i = 0; i < Obj->AnimFrames; i++ )
  {
    FORTIFY_LOOP_SLOW( i, MAX_UINT32 );
    for ( u32 j = 0; j < Obj->FrameVerts; j++ )
    {
      FORTIFY_LOOP_SLOW( j, MAX_UINT32 );
      size_t Index = ((size_t)i * (size_t)Obj->FrameVerts) + (size_t)j;
      FMeshVert Vtx = Obj->Verts[Index];
      VtxOut = ((int)(-Vtx.X * 8.0) & 0x7ff) | (((int)(-Vtx.Y * 8.0) & 0x7ff) << 11) | (((int)(Vtx.Z * 4.0) & 0x3ff) << 22);
      Out.Write( &VtxOut, sizeof( VtxOut ) );
    }
  }

  Out.Close();

  return true;
}

bool UMeshExporter::ExportOldMeshObj( UMesh* Obj, const char* Dir, int Frame )
{
  GLogf( LOG_WARN, "UMeshExporter::ExportOldMeshObj() not implemented!" );
  return true;
}

bool UMeshExporter::ExportSkeletalMesh( USkeletalMesh* Obj, const char* Dir )
{
  GLogf( LOG_ERR, "Skeletal mesh export not implemented! (%s)", Obj->Name.Data() );
  return true;

  /*
  if ( stricmp( Type, "psk" ) != 0 )
  {
    GLogf( LOG_ERR, "Can't export skeletal mesh to file type '%s'", Type );
    return false;
  }

  // Open psk file
  FStringFilePath Filename( Dir, Name.Data(), Type );
  FFileArchiveOut Out;
  if ( Out.Open( Filename ) != 0 )
  {
    GLogf( LOG_WARN, "Failed to export skeletal mesh to file '%s'", Filename.Data() );
    return false;
  }

  FChunkHeaderOut ChunkHdr;

  // Write PSK header
  strcpy( ChunkHdr.ChunkId, "ACTRHEAD" );
  ChunkHdr.TypeFlags = PSK_PSA_TYPE_FLAGS;
  ChunkHdr.DataSize = 0;
  ChunkHdr.DataCount = 0;

  Out << ChunkHdr;

  // Write points
  strcpy( ChunkHdr.ChunkId, "PNTS0000" );
  ChunkHdr.DataSize = sizeof(FVector);
  ChunkHdr.DataCount = Points.Size();

  Out << ChunkHdr;
  Out << Points;

  // Write wedges
  strcpy( ChunkHdr.ChunkId, "VTXW0000" );
  ChunkHdr.DataSize = sizeof(FWedgeChunk);
  ChunkHdr.DataCount = Wedges.Size();

  Out << ChunkHdr;

  for ( int i = 0; i < ExtWedges.Size(); i++ )
  {
    FWedgeChunk Wedge( Wedges[i] );
    Out << Wedge;
  }

  // Write faces
  strcpy( ChunkHdr.ChunkId, "FACE0000" );
  ChunkHdr.DataSize = sizeof(FFaceChunk);
  ChunkHdr.DataCount = Faces.Size();

  Out << ChunkHdr;

  for ( int i = 0; i < Faces.Size(); i++ )
  {
    FFaceChunk Face( Faces[i] );
    Out << Face;
  }

  // Write materials
  strcpy( ChunkHdr.ChunkId, "MATT0000" );
  ChunkHdr.DataSize = sizeof(FMaterialChunk);
  ChunkHdr.DataCount = Materials.Size();

  Out << ChunkHdr;

  for ( int i = 0; i < Materials.Size(); i++ )
  {
    FMaterialChunk Mat( Materials[i] );
    Out << Mat;
  }

  // Write bone data
  strcpy( ChunkHdr.ChunkId, "REFSKELT" );
  ChunkHdr.DataSize = sizeof(FBoneChunk);
  ChunkHdr.DataCount = Refs.Size();

  Out << ChunkHdr;

  for ( int i = 0; i < Refs.Size(); i++ )
  {
    FBoneChunk Bone( Refs[i] );
    Out << Bone;
  }

  // Write weight data
  strcpy( ChunkHdr.ChunkId, "RAWWEIGHTS" );
  ChunkHdr.DataSize = sizeof(FWeightChunk);
  ChunkHdr.DataCount = Weights.Size();

  Out << ChunkHdr;

  for ( int i = 0; i < Weights.Size(); i++ )
  {
    Out << Weights[i];
  }

  // Close file
  Out.Close();
*/
}

bool UMeshExporter::ExportSkeletalAnim( UAnimation* Obj, const char* Dir )
{
  GLogf( LOG_ERR, "Skeletal anim export not implemented! (%s)", Obj->Name.Data() );
  return true;

  /*
  if ( stricmp( Type, "psa" ) != 0 )
  {
    GLogf( LOG_ERR, "Can't export skeletal animation to file type '%s'", Type );
    return false;
  }

  // Open psa file
  FStringFilePath Filename( Dir, Name.Data(), Type );
  FFileArchiveOut Out = FFileArchiveOut();
  if ( Out.Open( Filename ) != 0 )
  {
    GLogf( LOG_WARN, "Failed to export skeletal animation to file '%s'", Filename.Data() );
    return false;
  }

  FChunkHeaderOut ChunkHdr;

  // Write PSA header
  strcpy( ChunkHdr.ChunkId, "ANIMHEAD" );
  ChunkHdr.TypeFlags = PSK_PSA_TYPE_FLAGS;
  ChunkHdr.DataSize = 0;
  ChunkHdr.DataCount = 0;

  Out << ChunkHdr;

  // Write bones
  strcpy( ChunkHdr.ChunkId, "BONENAMES" );
  ChunkHdr.DataSize = sizeof(FBoneChunk);
  ChunkHdr.DataCount = RefBones.Size();

  Out << ChunkHdr;

  for ( int i = 0; i < RefBones.Size(); i++ )
  {
    // TODO
  }

  // Write anim info
  strcpy( ChunkHdr.ChunkId, "ANIMINFO" );
  ChunkHdr.DataSize = sizeof(FAnimInfoChunk);
  ChunkHdr.DataCount = AnimSeqs.Size();

  Out << ChunkHdr;

  for ( int i = 0; i < AnimSeqs.Size(); i++ )
  {
    // TODO
  }

  // Write anim keys
  strcpy( ChunkHdr.ChunkId, "ANIMKEYS" );
  ChunkHdr.DataSize = sizeof(FAnimKeyChunk);
*/
}

/*-----------------------------------------------------------------------------
 * UMusicExporter
-----------------------------------------------------------------------------*/
bool UMusicExporter::ExportObject( UMusic* Obj, const char* Dir, const char* Type )
{
  const char* Ext = Obj->MusicType.Data();

  // Set up filename
  FString Filename( Dir );
#if defined LIBUNR_WIN32
  Filename.ReplaceChars( '\\', '/' );
#endif
  if ( Filename.Back() != '/' )
    Filename += "/";

  Filename += Obj->Name;
  Filename += ".";
  Filename += Ext;

  GLogf( LOG_INFO, "Exporting %s.%s", Obj->Name.Data(), Ext );

  // Open file
  FFileArchiveOut* Out = new FFileArchiveOut();
  if ( Out->Open( Filename ) != 0 )
  {
    GLogf( LOG_WARN, "Failed to export music to file '%s'", Filename.Data() );
    return false;
  }

  // Write
  Out->Write( Obj->ChunkData, Obj->ChunkSize );

  // Close
  Out->Close();
  delete Out;
  return true;
}

/*-----------------------------------------------------------------------------
 * UPolysExporter
-----------------------------------------------------------------------------*/
bool UPolysExporter::ExportObject( UPolys* Obj, const char* Dir, const char* Type )
{
  GLogf( LOG_ERR, "UPolysExporter::ExportObject stub" );
  return false;
}

/*-----------------------------------------------------------------------------
 * USoundExporter
-----------------------------------------------------------------------------*/
bool USoundExporter::ExportObject( USound* Obj, const char* Dir, const char* Type )
{
  // TODO: Support exporting to a number of different formats
  const char* Ext = Obj->SoundFormat.Data();

  // Set up filename
  FString Filename( Dir );
#if defined LIBUNR_WIN32
  Filename.ReplaceChars( '\\', '/' );
#endif
  if ( Filename.Back() != '/' )
    Filename += '/';

  Filename += Obj->Name;
  Filename += ".";
  Filename += Ext;

  GLogf( LOG_INFO, "Exporting %s.%s", Obj->Name.Data(), Ext );

  // Open file
  FFileArchiveOut* Out = new FFileArchiveOut();
  if ( Out->Open( Filename ) != 0 )
  {
    GLogf( LOG_WARN, "Failed to export sound to wav file '%s'", Filename.Data() );
    return false;
  }

  // Write
  Out->Write( Obj->SoundData, Obj->SoundSize );

  // Close
  Out->Close();
  delete Out;
  return true;
}

/*-----------------------------------------------------------------------------
 * UTextBufferExporter
-----------------------------------------------------------------------------*/
bool UTextBufferExporter::ExportObject( UTextBuffer* Obj, const char* Dir, const char* Type )
{
  // Set up filename
  FString Filename( Dir );
#if defined LIBUNR_WIN32
  Filename.ReplaceChars( '\\', '/' );
#endif
  if ( Filename.Back() != '/' )
    Filename += '/';

  Filename += Obj->Name;
  Filename += ".txt";

  GLogf( LOG_INFO, "Exporting %s.txt", Obj->Name.Data() );

  // Open file
  FFileArchiveOut* Out = new FFileArchiveOut();
  if ( Out->Open( Filename ) != 0 )
  {
    GLogf( LOG_WARN, "Failed to export text buffer file '%s'", Filename.Data() );
    return false;
  }

  // Write
  Out->Write( Obj->Text->Data(), Obj->Text->Size() );

  // Close
  Out->Close();
  delete Out;
  return true;
}

/*-----------------------------------------------------------------------------
 * UPaletteExporter
-----------------------------------------------------------------------------*/
bool UPaletteExporter::ExportObject( UPalette* Obj, const char* Dir, const char* Type )
{
  GLogf( LOG_ERR, "UPaletteExporter::ExportObject stub" );
  return false;
}

/*-----------------------------------------------------------------------------
 * UTextureExporter
-----------------------------------------------------------------------------*/
bool UTextureExporter::ExportObject( UTexture* Obj, const char* Dir, const char* Type )
{
  // TODO: Export textures besides TEXF_P8 (S3TC, etc etc)
  // TODO: Export to formats besides .bmp
  FString Filename( Dir );
#if defined LIBUNR_WIN32
  Filename.ReplaceChars( '\\', '/' );
#endif
  if ( Filename.Back() != '/' )
    Filename += '/';

  Filename += Obj->Name;
  Filename += ".";
  if ( Type == NULL )
    Type = "bmp";

  else if ( strnicmp( Type, "bmp", 3 ) != 0 )
  {
    GLogf( LOG_WARN, "Can't export texture to file type '%s'", Type );
    return false;
  }
  Filename += Type;

  GLogf( LOG_INFO, "Exporting %s.%s", Obj->Name.Data(), Type );

  FFileArchiveOut* Out = new FFileArchiveOut();
  if ( Out->Open( Filename ) != 0 )
  {
    GLogf( LOG_WARN, "Failed to export texture to bitmap file '%s'", Filename.Data() );
    return false;
  }

  u32 Zero32 = 0;
  u32 bfSize = 0;
  u32 bfOffBits = 0;

  // Write BMP header
  Out->Write( (char*)"BM", 2 ); // magic number
  *Out << Zero32;        // come back to write the file size later
  *Out << Zero32;        // reserved
  *Out << Zero32;        // come back to write the pixel data

  // 8-bit paletted textures (standard format for UE1 games)
  if ( Obj->Format == TEXF_P8 )
  {
    u32 biSize = 40;
    u16 biPlanes = 1;
    u16 biBitCount = 8;

    *Out << biSize;     // biSize
    *Out << Obj->USize;      // biWidth
    *Out << Obj->VSize;      // biHeight
    *Out << biPlanes;   // biPlanes
    *Out << biBitCount; // biBitCount
    *Out << Zero32;     // biCompression
    *Out << Zero32;     // biSizeImage
    *Out << Zero32;     // biXPelsPerMeter
    *Out << Zero32;     // biYPelsPerMeter
    *Out << Zero32;     // biClrUsed
    *Out << Zero32;     // biClrImportant

    // Write color table
    u8 Alpha = 0;
    for ( int i = 0; i < 256; i++ )
    {
      *Out << Obj->Palette->Colors[i].B;
      *Out << Obj->Palette->Colors[i].G;
      *Out << Obj->Palette->Colors[i].R;
      *Out << Alpha;
    }

    bfOffBits = (u32)Out->Tell();

    // Just export the first mipmap since that's probably what the user is going to want anyway
    // if there's some need for exporting choice mipmaps, implement it later
    u8* Bitmap = Obj->Mips[0].DataArray.Data();

    // We need to write each row from bottom to top
    for ( u32 i = Obj->VSize; i > 0; i-- )
    {
      for ( u32 j = 0; j < Obj->USize; j++ )
      {
        FORTIFY_LOOP_SLOW( j, 8192 );
        *Out << Bitmap[((i - 1) * Obj->USize) + j];
      }
    }

    bfSize = (u32)Out->Tell();

    // Go back and write header info
    Out->Seek( 2, Begin );
    *Out << bfSize;

    Out->Seek( 10, Begin );
    *Out << bfOffBits;

    Out->Close();
    return true;
  }
  else if ( Obj->Format == TEXF_DXT5 )
  {

  }

  return false;
}

/*-----------------------------------------------------------------------------
 * UFractalTextureExporter
-----------------------------------------------------------------------------*/
bool UFractalTextureExporter::ExportObject( UFractalTexture* Obj, const char* Dir, const char* Type )
{
  GLogf( LOG_ERR, "UFractalTextureExporter::ExportObject stub" );
  return false;
}

/*-----------------------------------------------------------------------------
 * UFontExporter
-----------------------------------------------------------------------------*/
bool UFontExporter::ExportObject( UFont* Obj, const char* Dir, const char* Type )
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
