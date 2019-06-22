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
  return false;
}

bool ULodMesh::ExportToFile( const char* Dir, const char* Type, int Frame )
{
  if ( stricmp( Type, "U3D" ) == 0 )
    return ExportUnreal3DMesh( Dir, Frame );
  else if ( stricmp( Type, "OBJ" ) == 0 )
    return ExportObjMesh( Dir, Frame );
  else
    Logf( LOG_WARN, "Unknown LODMesh export extension '%s'", Type );

  return false;
}

IMPLEMENT_NATIVE_CLASS( ULodMesh );

