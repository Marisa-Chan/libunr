/*========================================================================*\
|*  This file is part of libunr.                                          *|
|*                                                                        *|
|*  libunr is free software: you can redistribute it and/or modify        *|
|*  it under the terms of the GNU General Public License as published by  *|
|*  the Free Software Foundation, either version 3 of the License, or     *|
|*  (at your option) any later version.                                   *|
|*                                                                        *|
|*  libunr is distributed in the hope that it will be useful,             *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of        *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *|
|*  GNU General Public License for more details.                          *|
|*                                                                        *|
|*  You should have received a copy of the GNU General Public License     *|
|*  along with libunr.  If not, see <http://www.gnu.org/licenses/>.       *|
|*                                                                        *|
\*========================================================================*/

/*========================================================================
 * UModel.cpp - Model functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Core/UModel.h"
#include "Actors/ABrush.h"
#include "Actors/AZoneInfo.h"

DLL_EXPORT FPackageFileIn& operator>>( FPackageFileIn& In, FPoly& P )
{
  In >> CINDEX( P.NumVertices );

  // TODO: Make a macro to check for stuff like this
  if ( P.NumVertices < 3 || P.NumVertices > MAX_VERTICES )
  {
    Logf( LOG_CRIT, "NumVertices < 3 || NumVertices > MAX_VERTICES" );
    GSystem->Exit( -1 );
  }

  In >> P.Base;
  In >> P.Normal;
  In >> P.TextureU;
  In >> P.TextureV;
  for ( int i = 0; i < P.NumVertices; i++ )
    In >> P.Vertex[i];

  In >> P.PolyFlags;
  
  idx ObjRef;

  In >> CINDEX( ObjRef );
  P.Brush = (ABrush*)UObject::StaticLoadObject( In.Pkg, ObjRef, ABrush::StaticClass(), NULL );
  
  In >> CINDEX( ObjRef );
  P.Texture = (UTexture*)UObject::StaticLoadObject( In.Pkg, ObjRef, UTexture::StaticClass(), NULL );

  In >> CINDEX( P.ItemName );
  In >> CINDEX( P.iLink );
  In >> CINDEX( P.iBrushPoly );
  In >> P.PanU;
  In >> P.PanV;
  return In;
}

DLL_EXPORT FPackageFileIn& operator>>( FPackageFileIn& In, FBspNode& BN )
{
  In >> BN.Plane;
  In >> BN.ZoneMask;
  In >> BN.NodeFlags;
  In >> CINDEX( BN.iVertPool );
  In >> CINDEX( BN.iSurf ); 
  In >> CINDEX( BN.iBack );
  In >> CINDEX( BN.iFront );
  In >> CINDEX( BN.iPlane );
  In >> CINDEX( BN.iCollisionBound ); 
  In >> CINDEX( BN.iRenderBound );
  In >> BN.iZone[0];
  In >> BN.iZone[1];
  In >> BN.NumVertices;
  In >> BN.iLeaf[0];
  In >> BN.iLeaf[1];
  return In;
}

DLL_EXPORT FPackageFileIn& operator>>( FPackageFileIn& In, FBspSurf& BS )
{
  idx ObjRef;
  In >> CINDEX( ObjRef );
  BS.Texture = (UTexture*)UObject::StaticLoadObject( In.Pkg, ObjRef, UTexture::StaticClass(), NULL ); 

  In >> BS.PolyFlags;
  In >> CINDEX( BS.pBase );
  In >> CINDEX( BS.vNormal );
  In >> CINDEX( BS.vTextureU );
  In >> CINDEX( BS.vTextureV );
  In >> CINDEX( BS.iLightMap );
  In >> CINDEX( BS.iBrushPoly );
  In >> BS.PanU;
  In >> BS.PanV;
  
  In >> CINDEX( ObjRef );
  BS.Brush = (ABrush*)UObject::StaticLoadObject( In.Pkg, ObjRef, ABrush::StaticClass(), NULL );
  return In;
}

DLL_EXPORT FPackageFileIn& operator>>( FPackageFileIn& In, FZoneProperties& ZP )
{
  idx ObjRef;
  In >> CINDEX( ObjRef );
  ZP.ZoneInfo = (AZoneInfo*)UObject::StaticLoadObject( In.Pkg, ObjRef, AZoneInfo::StaticClass(), NULL );

  In >> ZP.LastRenderTime;
  In >> ZP.Connectivity;
  In >> ZP.Visibility;

  return In;
}

UVectors::UVectors()
  : UObject()
{
}

UVectors::~UVectors()
{
}

void UVectors::Load()
{
  if ( Outer->IsA( UModel::StaticClass() ) )
  {
    Super::Load();
    Data = &((UModel*)Outer)->Vectors;

    int Num, Max;
    *PkgFile >> Num;
    *PkgFile >> Max;
    Data->Resize( Num );

    for ( int i = 0; i < Num; i++ )
      *PkgFile >> (*Data)[i];
  }
}

UBspNodes::UBspNodes()
  : UObject()
{
}

UBspNodes::~UBspNodes()
{
}

void UBspNodes::Load()
{
  if ( Outer->IsA( UModel::StaticClass() ) )
  {
    Super::Load();
    UModel* Model = (UModel*)Outer;
    Data = &Model->Nodes;

    int Num, Max;
    *PkgFile >> Num;
    *PkgFile >> Max;
    Data->Resize( Num );

    for ( int i = 0; i < Num; i++ )
      *PkgFile >> (*Data)[i];

    idx NumZones;
    *PkgFile >> CINDEX( NumZones );
    Model->NumZones = NumZones;
    for ( int i = 0; i != MAX_ZONES && i < NumZones; i++ )
      *PkgFile >> Model->Zones[i];
  }
}

UBspSurfs::UBspSurfs()
  : UObject()
{
}

UBspSurfs::~UBspSurfs()
{
}

void UBspSurfs::Load()
{
  if ( Outer->IsA( UModel::StaticClass() ) )
  {
    Super::Load();
    Data = &((UModel*)Outer)->Surfs;

    int Num, Max;
    *PkgFile >> Num;
    *PkgFile >> Max;
    Data->Resize( Num );

    for ( int i = 0; i < Num; i++ )
      *PkgFile >> (*Data)[i];
  }
}

UVerts::UVerts()
  : UObject()
{
}

UVerts::~UVerts()
{
}

void UVerts::Load()
{
  if ( Outer->IsA( UModel::StaticClass() ) )
  {
    Super::Load();
    Data = &((UModel*)Outer)->Verts;

    int Num, Max;
    *PkgFile >> Num;
    *PkgFile >> Max;
    Data->Resize( Num );

    for ( int i = 0; i < Num; i++ )
      *PkgFile >> (*Data)[i];
  }
}

UPolys::UPolys()
  : UObject()
{
}

UPolys::~UPolys()
{
}

void UPolys::Load()
{
  Super::Load();
  int Num, Max;
  *PkgFile >> Num;
  *PkgFile >> Max;
  Element.Resize( Num );

  for ( int i = 0; i < Num; i++ )
    *PkgFile >> Element[i];
}

UModel::UModel()
  : UPrimitive()
{
}

UModel::~UModel()
{
}

void UModel::Load()
{
  Super::Load();

  if ( PkgFile->Ver < PKG_VER_UT_400 )
  {
    idx ObjRef;

    *PkgFile >> CINDEX( ObjRef );
    UVectors* OldVectors = (UVectors*)LoadObject( ObjRef, UVectors::StaticClass(), this );
    delete OldVectors;

    *PkgFile >> CINDEX( ObjRef );
    UVectors* OldPoints = (UVectors*)LoadObject( ObjRef, UVectors::StaticClass(), this );
    delete OldPoints;

    *PkgFile >> CINDEX( ObjRef );
    UBspNodes* OldNodes = (UBspNodes*)LoadObject( ObjRef, UBspNodes::StaticClass(), this );
    delete OldNodes;

    *PkgFile >> CINDEX( ObjRef );
    UBspSurfs* OldSurfs = (UBspSurfs*)LoadObject( ObjRef, UBspSurfs::StaticClass(), this );
    delete OldSurfs;

    *PkgFile >> CINDEX( ObjRef );
    UVerts* OldVerts = (UVerts*)LoadObject( ObjRef, UVerts::StaticClass(), this );
    delete OldVerts;

    *PkgFile >> CINDEX( ObjRef );
    Polys = (UPolys*)LoadObject( ObjRef, UPolys::StaticClass(), this );
  }
  else
  {
    idx Num;

    *PkgFile >> CINDEX( Num );
    Vectors.Resize( Num );
    for ( int i = 0; i < Num; i++ )
      *PkgFile >> Vectors[i];

    *PkgFile >> CINDEX( Num );
    Points.Resize( Num );
    for ( int i = 0; i < Num; i++ )
      *PkgFile >> Points[i];

    *PkgFile >> CINDEX( Num );
    Nodes.Resize( Num );
    for ( int i = 0; i < Num; i++ )
      *PkgFile >> Nodes[i];

    *PkgFile >> CINDEX( Num );
    Surfs.Resize( Num );
    for ( int i = 0; i < Num; i++ )
      *PkgFile >> Surfs[i];

    *PkgFile >> CINDEX( Num );
    Verts.Resize( Num );
    for ( int i = 0; i < Num; i++ )
      *PkgFile >> Verts[i];

    *PkgFile >> NumSharedSides;
    *PkgFile >> NumZones;
    for ( int i = 0; i < NumZones; i++ )
      *PkgFile >> Zones[i];

    idx ObjRef;
    *PkgFile >> CINDEX( ObjRef );
    Polys = (UPolys*)LoadObject( ObjRef, UPolys::StaticClass(), this );
  }
}

IMPLEMENT_NATIVE_CLASS( UVectors );
IMPLEMENT_NATIVE_CLASS( UBspNodes );
IMPLEMENT_NATIVE_CLASS( UBspSurfs );
IMPLEMENT_NATIVE_CLASS( UVerts );
IMPLEMENT_NATIVE_CLASS( UPolys );
IMPLEMENT_NATIVE_CLASS( UModel );

