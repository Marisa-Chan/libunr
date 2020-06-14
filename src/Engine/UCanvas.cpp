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
 * UCanvas.cpp - Canvas Functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Engine/UCanvas.h"
#include "Engine/UEngine.h"

UCanvas::UCanvas()
  : UObject()
{
  Font = NULL;
  SpaceX = 0.0f;
  SpaceY = 0.0f;
  OrgX = 0.0f;
  OrgY = 0.0f;
  ClipX = 0.0f;
  ClipY = 0.0f;
  CurX = 0.0f;
  CurY = 0.0f;
  Z = 0.0f;
  Style = 0;
  CurYL = 0.0f;
  DrawColor.Data = 0xffffffff;
  bCenter = false;
  bNoSmooth = false;
  bUseOffset = false;
  bUseFlatZ = false;
  SizeX = 0;
  SizeY = 0;
  SmallFont = NULL;
  MedFont = NULL;
  BigFont = NULL;
  LargeFont = NULL;
  Viewport = NULL;
  FramePtr = 0;
  RenderPtr = 0;
}

UCanvas::~UCanvas()
{
  if ( Font )
    Font->DelRef();
  
  if ( SmallFont )
    SmallFont->DelRef();

  if ( MedFont )
    MedFont->DelRef();

  if ( BigFont )
    BigFont->DelRef();

  if ( LargeFont )
    LargeFont->DelRef();
}

bool UCanvas::Init()
{
  // TODO: Check that these loaded
  // Load each font
  SmallFont = (UFont*)StaticLoadObject( Pkg, "SmallFont", UFont::StaticClass(), NULL );
  MedFont   = (UFont*)StaticLoadObject( Pkg, "MedFont",   UFont::StaticClass(), NULL );
  BigFont   = (UFont*)StaticLoadObject( Pkg, "BigFont",   UFont::StaticClass(), NULL );
  LargeFont = (UFont*)StaticLoadObject( Pkg, "LargeFont", UFont::StaticClass(), NULL );
  Viewport = (UViewport*)Outer;

  ClipX = (float)Viewport->Width;
  ClipY = (float)Viewport->Height;

  return true;
}

void UCanvas::StrLen( FString* Str, float* XL, float* YL )
{
   // TODO
}

void UCanvas::DrawText( FString* Text, bool CR, int PolyFlags )
{
  // TODO: Implement CR (carriage return) functionality
  FBoxInt2D Dim( (int)CurX, (int)CurY, (int)ClipX, (int)ClipY );
  GEngine->Render->DrawText( Font, Dim, *Text, PolyFlags );
}

void UCanvas::DrawTile( UTexture* Tex, float XL, float YL, float U, float V, float UL, float VL, int PolyFlags )
{
  FBoxInt2D Dim( (int)CurX, (int)CurY, (int)CurX + (int)XL, (int)CurY + (int)YL );
  FRotator Rot;
  GEngine->Render->DrawTile( Tex, Dim, Rot, U, V, UL, VL, PolyFlags );
}

void UCanvas::DrawTileClipped( UTexture* Tex, float XL, float YL, float U, float V, float UL, float VL, int PolyFlags )
{
  FBoxInt2D Dim( (int)CurX, (int)CurY, (int)CurX + (int)XL, (int)CurY + (int)YL );
  FRotator Rot;
  GEngine->Render->DrawTile( Tex, Dim, Rot, U, V, UL, VL, PolyFlags );
}

#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( UCanvas );

BEGIN_PROPERTY_LINK( UCanvas, 31 )
  LINK_NATIVE_PROPERTY( Font );
  LINK_NATIVE_PROPERTY( SpaceX );
  LINK_NATIVE_PROPERTY( SpaceY );
  LINK_NATIVE_PROPERTY( OrgX );
  LINK_NATIVE_PROPERTY( OrgY );
  LINK_NATIVE_PROPERTY( ClipX );
  LINK_NATIVE_PROPERTY( ClipY );
  LINK_NATIVE_PROPERTY( CurX );
  LINK_NATIVE_PROPERTY( CurY );
  LINK_NATIVE_PROPERTY( Z );
  LINK_NATIVE_PROPERTY( Style );
  LINK_NATIVE_PROPERTY( CurYL );
  LINK_NATIVE_PROPERTY( DrawColor );
  LINK_NATIVE_PROPERTY( bCenter );
  LINK_NATIVE_PROPERTY( bNoSmooth );
  LINK_NATIVE_PROPERTY( bUseOffset );
  LINK_NATIVE_PROPERTY( bUseFlatZ );
  LINK_NATIVE_PROPERTY( SizeX );
  LINK_NATIVE_PROPERTY( SizeY );
  LINK_NATIVE_PROPERTY( SmallFont );
  LINK_NATIVE_PROPERTY( MedFont );
  LINK_NATIVE_PROPERTY( BigFont );
  LINK_NATIVE_PROPERTY( LargeFont );
  LINK_NATIVE_PROPERTY( Viewport );
  LINK_NATIVE_PROPERTY( FramePtr );
  LINK_NATIVE_PROPERTY( RenderPtr );
  LINK_NATIVE_PROPERTY( RenderCoords );
  LINK_NATIVE_PROPERTY( AlphaScale );
  LINK_NATIVE_PROPERTY( RuneMedFont );
  LINK_NATIVE_PROPERTY( CredsFont );
  LINK_NATIVE_PROPERTY( ButtonFont );
END_PROPERTY_LINK()

