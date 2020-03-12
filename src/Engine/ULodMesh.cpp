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
 * ULodMesh.cpp - "Level of Detail" Mesh functionality
 * 
 * written by Adam 'Xaleros' Smith
 * with a good deal of help from this source file of UModel
 * https://github.com/gildor2/UModel/blob/master/Unreal/UnMesh1.cpp
 *========================================================================
*/

#include "Util/FString.h"
#include "Core/UClass.h"
#include "Engine/UModel.h"
#include "Engine/ULodMesh.h"

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
 
  // Merge special faces into regular mesh
  for ( int i = 0; i < SpecialFaceCount; i++ )
  {
    // 227 seems to change the material type to 0x81, I guess
    // for clearly identifying weapon triangles. Can't rely on this
    // behavior as it isn't seen anywhere else. Check also for an
    // extremely large MaterialIndex, which seems to occur in UT
    if ( SpecialFaces[i].MaterialIndex == 0x81 || SpecialFaces[i].MaterialIndex >= Materials.Size() )
      SpecialFaces[i].MaterialIndex = 0;

    // Separate weapon triangle wedges
    FLodWedge SpecialWedge;
    
    SpecialWedge.VertexIndex = 0;
    Wedges.PushBack( SpecialWedge );
    SpecialFaces[i].WedgeIndex[0] = Wedges.Size() - 1;

    SpecialWedge.VertexIndex = 1;
    Wedges.PushBack( SpecialWedge );
    SpecialFaces[i].WedgeIndex[1] = Wedges.Size() - 1;

    SpecialWedge.VertexIndex = 2;
    Wedges.PushBack( SpecialWedge );
    SpecialFaces[i].WedgeIndex[2] = Wedges.Size() - 1;
  }

  Faces.Append( SpecialFaces );

  In >> ModelVerts >> SpecialVerts;
  In >> MeshScaleMax >> LODHysteresis;
  In >> LODStrength >> LODMinVerts;
  In >> LODMorph >> LODZDisplace;

  // TODO: We will probably have to make use of this somehow
  In >> CINDEX( ReMapAnimVertCount );
  ReMapAnimVerts.Resize( ReMapAnimVertCount );
  In.Read( ReMapAnimVerts.Data(), ReMapAnimVertCount * sizeof(u16) );

  In >> OldFrameVerts;
  
  if ( SpecialFaces.Size() > 0 )
  {
    if ( SpecialFaces.Size() > 1 )
      GLogf( LOG_WARN, "SpecialFaces.Size() > 1 for LodMesh '%s', export may be incorrect!", Name.Data() );

    // Ugh, need to remap all of the wedges for special vertices
    // Behavior observed in UModel source
    for ( int i = 0; i < Wedges.Size() - (SpecialFaces.Size()*3); i++ )
      Wedges[i].VertexIndex += SpecialVerts;
  }
}

bool ULodMesh::ExportUnreal3DMesh( const char* Dir, int Frame ) 
{
  FFileArchiveOut Out;
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
    GLogf( LOG_WARN, "Failed to export LOD mesh to data file '%s'", DataFileName.Data() );
    return false;
  }

  FVertexDataHeader DataHdr;
  memset( &DataHdr, 0, sizeof(DataHdr) ); // Everything else goes unused
  DataHdr.NumPolygons = Faces.Size();
  DataHdr.NumVertices = FrameVerts;
  
  Out.Write( &DataHdr, sizeof(DataHdr) );

  FVertexDataTri TriOut;
  for ( int i = 0; i < DataHdr.NumPolygons; i++ )
  {
    // Maybe inline this so it's a bit neater
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

    // Weapon triangles are only in the special faces array
    if ( SpecialFaces.Size() == 1 && i == Faces.Size()-1 )
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
    GLogf( LOG_WARN, "Failed to export LOD mesh to aniv file '%s'", AnivFileName.Data() );
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
  FFileArchiveOut Out;
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
    GLogf( LOG_WARN, "Failed to export LOD mesh to obj file '%s'", ObjFileName.Data() );
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
  memset( VertNormals, 0, sizeof(FVector)*FrameVerts );

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
    delete[] VertNormals;
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
    GLogf( LOG_WARN, "Failed to generate MTL file for LOD mesh '%s'", Name.Data() );
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

IMPLEMENT_NATIVE_CLASS( ULodMesh );

