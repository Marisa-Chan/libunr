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
 * ULodMesh.cpp - "Level of Detail" Mesh functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Core/UModel.h"
#include "Core/ULodMesh.h"

ULodMesh::ULodMesh()
  : UMesh()
{
}

ULodMesh::~ULodMesh()
{
}

void ULodMesh::Load()
{
  Super::Load();
  FPackageFileIn& In = *PkgFile;

  idx CollapsePointCount;
  idx FaceLevelCount;
  idx FaceCount;
  idx CollapseWedgeCount;
  idx WedgeCount;
  idx MaterialCount;
  idx SpecialFaceCount;
  idx ReMapAnimVertCount;

  In >> CINDEX( CollapsePointCount );
  CollapsePoints.Resize( CollapsePointCount );
  In.Read( CollapsePoints.Data(), CollapsePointCount * sizeof(u16) );

  In >> CINDEX( FaceLevelCount );
  FaceLevels.Resize( FaceLevelCount );
  In.Read( FaceLevels.Data(), FaceLevelCount * sizeof(u16) );

  In >> CINDEX( FaceCount );
  Faces.Resize( FaceCount );
  In.Read( Faces.Data(), FaceCount * sizeof(FLodFace) );

  In >> CINDEX( CollapseWedgeCount );
  CollapseWedges.Resize( CollapseWedgeCount );
  In.Read( CollapseWedges.Data(), CollapseWedgeCount * sizeof(u16) );

  In >> CINDEX( WedgeCount );
  Wedges.Resize( WedgeCount );
  In.Read( Wedges.Data(), WedgeCount * sizeof(FLodWedge) );

  In >> CINDEX( MaterialCount );
  Materials.Resize( MaterialCount );
  In.Read( Materials.Data(), MaterialCount * sizeof(FLodMaterial) );

  In >> CINDEX( SpecialFaceCount );
  SpecialFaces.Resize( SpecialFaceCount );
  In.Read( SpecialFaces.Data(), SpecialFaceCount * sizeof(FLodFace) );

  In >> ModelVerts >> SpecialVerts;
  In >> MeshScaleMax >> LODHysteresis;
  In >> LODStrength >> LODMinVerts;
  In >> LODMorph >> LODZDisplace;

  In >> CINDEX( ReMapAnimVertCount );
  ReMapAnimVerts.Resize( ReMapAnimVertCount );
  In.Read( ReMapAnimVerts.Data(), ReMapAnimVertCount * sizeof(u16) );

  In >> OldFrameVerts;
};

bool ULodMesh::ExportUnreal3DMesh( const char* Dir, int Frame )
{
  FileStreamOut Out;
  FString Filename( Dir );
#if defined LIBUNR_WIN32
  Filename.ReplaceChars( '\\', '/' );
#endif
  if ( Filename.Back() != '/' )
    Filename += '/';

  Filename += Name.Data();

  // Export data file
  FString DataFileName( Filename );
  DataFileName += "_d.3d";
  
  if ( Out.Open( DataFileName ) != 0 )
  {
    Logf( LOG_WARN, "Failed to export LOD mesh to data file '%s'", DataFileName.Data() );
    return false;
  }

  FVertexDataHeader DataHdr;
  xstl::Set( &DataHdr, 0, sizeof(DataHdr) ); // Everything else goes unused
  DataHdr.NumPolygons = Faces.Size();
  DataHdr.NumVertices = ModelVerts;
  
  Out.Write( &DataHdr, sizeof(DataHdr) );

  FVertexDataTri TriOut;
  for ( int i = 0; i < DataHdr.NumPolygons; i++ )
  {
    FLodWedge TriWedges[3];
    TriWedges[0] = Wedges[Faces[i].WedgeIndex[0]];
    TriWedges[1] = Wedges[Faces[i].WedgeIndex[1]];
    TriWedges[2] = Wedges[Faces[i].WedgeIndex[2]];

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
    FLodMaterial Mat = Materials[Faces[i].MaterialIndex];

    // Weapon triangles are invisible, so maybe that's the only thing the engine
    // checks for? Might need to look into this some more...
    if ( Mat.Flags & PF_Invisible && SpecialVerts > 0 )
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

    Out.Write( &TriOut, sizeof(TriOut) );
  }

  Out.Close();
  FString AnivFileName( Filename );
  AnivFileName += "_a.3d";

  if ( Out.Open( AnivFileName ) != 0 )
  {
    Logf( LOG_WARN, "Failed to export LOD mesh to aniv file '%s'", AnivFileName.Data() );
    return false;
  }
  
  FVertexAnivHeader AnivHdr;
  AnivHdr.NumFrames = AnimFrames;
  AnivHdr.FrameSize = FrameVerts * 4;
  Out.Write( &AnivHdr, sizeof(AnivHdr) );

  uint32_t VtxOut;
  for ( int i = 0; i < AnimFrames; i++ )
  {
    for ( int j = 0; j < FrameVerts; j++ )
    {
      FMeshVert Vtx = Verts[(i*FrameVerts)+j];
      VtxOut = ((int)(-Vtx.X * 8.0) & 0x7ff) | (((int)(-Vtx.Y * 8.0) & 0x7ff) << 11) | (((int)(Vtx.Z * 4.0) & 0x3ff) << 22);
      Out.Write( &VtxOut, sizeof(VtxOut) );
    }
  }

  Out.Close();

  return true;
}

bool ULodMesh::ExportObjMesh( const char* Dir, int Frame )
{
  FileStreamOut Out;
  FString Filename( Dir );
#if defined LIBUNR_WIN32
  Filename.ReplaceChars( '\\', '/' );
#endif
  if ( Filename.Back() != '/' )
    Filename += '/';

  Filename += Name.Data();

  // Export obj mesh

  FString ObjFileName = Filename;
  ObjFileName += "_";
  ObjFileName += FString(Frame);
  ObjFileName += ".obj";

  if ( Out.Open( ObjFileName ) != 0 )
  {
    Logf( LOG_WARN, "Failed to export LOD mesh to obj file '%s'", ObjFileName.Data() );
    return false;
  }
  
  Out.Printf("# OBJ File generated with libunr LODMesh exporter\n");
  Out.Printf("mtllib %s.mtl\n", Name.Data());
  Out.Printf("o %s\n", Name.Data());

  // Vertex coordinates
  int FirstVert = Frame * FrameVerts;
  for ( int i = 0; i < FrameVerts; i++ )
  {
    FMeshVert Vtx = Verts[FirstVert+i];
    Out.Printf("v %.6f %.6f %.6f\n", -Vtx.X, -Vtx.Y, -Vtx.Z);
  }

  // Vertex UVs
  for ( int i = 0; i < Wedges.Size(); i++ )
  {
    FLodWedge Wedge = Wedges[i];
    Out.Printf("vt %.6f %.6f\n", Wedge.S/255.0, (255-Wedge.T)/255.0); 
  }

  // Vertex Normals
  FVector* VertNormals = new FVector[FrameVerts];
  xstl::Set( VertNormals, 0, sizeof(FVector)*FrameVerts );

  {
    FVector FaceNormals[2];
    FLodWedge TriWedges[2][3];
    int v[2][3];
    int i = 0;

    if ( Faces.Size() & 1 )
    {
      TriWedges[0][0] = Wedges[Faces[0].WedgeIndex[0]];
      TriWedges[0][1] = Wedges[Faces[0].WedgeIndex[1]];
      TriWedges[0][2] = Wedges[Faces[0].WedgeIndex[2]];
  
      v[0][0] = TriWedges[0][0].VertexIndex;
      v[0][1] = TriWedges[0][1].VertexIndex;
      v[0][2] = TriWedges[0][2].VertexIndex;
  
      FVector U = *(FVector*)&Verts[v[0][0]] - *(FVector*)&Verts[v[0][1]];
      FVector V = *(FVector*)&Verts[v[0][2]] - *(FVector*)&Verts[v[0][1]];
  
      FaceNormals[0].X = (U.Y * V.Z) - (U.Z * V.Y);
      FaceNormals[0].Y = (U.Z * V.X) - (U.X * V.Z);
      FaceNormals[0].Z = (U.X * V.Y) - (U.Y * V.X);
  
      VertNormals[v[0][0]] += FaceNormals[0];
      VertNormals[v[0][1]] += FaceNormals[0];
      VertNormals[v[0][2]] += FaceNormals[0];

      i++;
    }
    for ( ; i < Faces.Size(); i+=2 )
    {
      // Loop unrolling, woo
      TriWedges[0][0] = Wedges[Faces[i].WedgeIndex[0]];
      TriWedges[0][1] = Wedges[Faces[i].WedgeIndex[1]];
      TriWedges[0][2] = Wedges[Faces[i].WedgeIndex[2]];
  
      TriWedges[1][0] = Wedges[Faces[i+1].WedgeIndex[0]];
      TriWedges[1][1] = Wedges[Faces[i+1].WedgeIndex[1]];
      TriWedges[1][2] = Wedges[Faces[i+1].WedgeIndex[2]];
  
      v[0][0] = TriWedges[0][0].VertexIndex;
      v[0][1] = TriWedges[0][1].VertexIndex;
      v[0][2] = TriWedges[0][2].VertexIndex;
  
      v[1][0] = TriWedges[1][0].VertexIndex;
      v[1][1] = TriWedges[1][1].VertexIndex;
      v[1][2] = TriWedges[1][2].VertexIndex;
  
      FVector U1 = *(FVector*)&Verts[FirstVert+v[0][0]] - *(FVector*)&Verts[FirstVert+v[0][1]];
      FVector V1 = *(FVector*)&Verts[FirstVert+v[0][2]] - *(FVector*)&Verts[FirstVert+v[0][1]];
  
      FVector U2 = *(FVector*)&Verts[FirstVert+v[1][0]] - *(FVector*)&Verts[FirstVert+v[1][1]];
      FVector V2 = *(FVector*)&Verts[FirstVert+v[1][2]] - *(FVector*)&Verts[FirstVert+v[1][1]];
 
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
    for ( int i = 0; i < FrameVerts; i++ )
    {
      FVector Normal = VertNormals[i];
      if ( Normal.X == 0 && Normal.Y == 0 && Normal.Z == 0 )
        continue;

      float Norm = sqrtf((Normal.X * Normal.X) + (Normal.Y * Normal.Y) + (Normal.Z * Normal.Z));

      Normal.X /= Norm;
      Normal.Y /= Norm;
      Normal.Z /= Norm;

      Out.Printf("vn %.6f %.6f %.6f\n", Normal.X, Normal.Y, Normal.Z);
    }
    delete VertNormals;
  }

  // Polygons
  FName TexName = 0;
  FName OldName = 0;
  for ( int i = 0; i < Faces.Size(); i++ )
  {
    FLodFace Face = Faces[i];

    FLodWedge TriWedges[3];
    TriWedges[0] = Wedges[Face.WedgeIndex[0]];
    TriWedges[1] = Wedges[Face.WedgeIndex[1]];
    TriWedges[2] = Wedges[Face.WedgeIndex[2]];

    // Change mtl assignment
    int MaterialIndex = Face.MaterialIndex;
    TexName = Textures[Materials[Face.MaterialIndex].TextureIndex]->Name;
    if ( TexName != OldName )
      Out.Printf("usemtl %s\n", TexName.Data());
    OldName = TexName;

    Out.Printf("f %i/%i/%i %i/%i/%i %i/%i/%i\n", 
      TriWedges[0].VertexIndex+1, Face.WedgeIndex[0]+1, TriWedges[0].VertexIndex+1,
      TriWedges[1].VertexIndex+1, Face.WedgeIndex[1]+1, TriWedges[1].VertexIndex+1,
      TriWedges[2].VertexIndex+1, Face.WedgeIndex[2]+1, TriWedges[2].VertexIndex+1
    );
  }

  Out.Close();

  FString MtlFileName = Filename;
  MtlFileName += ".mtl";

  if ( Out.Open( MtlFileName ) != 0 )
  {
    Logf( LOG_WARN, "Failed to generate MTL file for LOD mesh '%s'", Name.Data() );
    return false;
  }
 
  Out.Printf("# MTL File generated with libunr LODMesh exporter\n");
  Out.Printf("# Materials: %i\n", Materials.Size());

  for ( int i = 0; i < Materials.Size(); i++ )
  {
    Out.Printf("newmtl %s\n", Textures[Materials[i].TextureIndex]->Name.Data());
    Out.Printf("Ns 0\n");
    Out.Printf("Ka %.6f %.6f %.6f\n", 0.0f, 0.0f, 0.0f); /// ???
    Out.Printf("Kd %.1f %.1f %.1f\n", 0.1f, 0.1f, 0.1f); /// ???
    Out.Printf("Ks %.1f %.1f %.1f\n", 0.1f, 0.1f, 0.1f); /// ???
    Out.Printf("d 1\n");
    Out.Printf("illum 1\n\n");
  }

  Out.Close();
  return true;
}

bool ULodMesh::ExportToFile( const char* Dir, const char* Type )
{
  return ULodMesh::ExportToFile( Dir, Type, -1 );
}

bool ULodMesh::ExportToFile( const char* Dir, const char* Type, int Frame )
{
  if ( Type == NULL )
    Type = "U3D";

  Logf( LOG_INFO, "Exporting %s.3d", Name.Data() );

  if ( stricmp( Type, "U3D" ) == 0 )
  {
    return ExportUnreal3DMesh( Dir, Frame );
  }
  else if ( stricmp( Type, "OBJ" ) == 0 )
  {
    if ( Frame < 0 )
    {
      for ( int i = 0; i < AnimFrames; i++ )
      {
        if ( !ExportObjMesh( Dir, i ) )
          return false;
      }
      return true;
    }
    return ExportObjMesh( Dir, Frame );
  }
  else
  {
    Logf( LOG_WARN, "Unknown LODMesh export extension '%s'", Type );
  }

  return false;
}

IMPLEMENT_NATIVE_CLASS( ULodMesh );
