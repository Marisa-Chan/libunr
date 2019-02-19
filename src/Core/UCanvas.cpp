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
 * UCanvas.cpp - Canvas Functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UCanvas.h"

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

BEGIN_PROPERTY_LINK( UCanvas, 27 )
  LINK_NATIVE_PROPERTY( UCanvas, Font );
  LINK_NATIVE_PROPERTY( UCanvas, SpaceX );
  LINK_NATIVE_PROPERTY( UCanvas, SpaceY );
  LINK_NATIVE_PROPERTY( UCanvas, OrgX );
  LINK_NATIVE_PROPERTY( UCanvas, OrgY );
  LINK_NATIVE_PROPERTY( UCanvas, ClipX );
  LINK_NATIVE_PROPERTY( UCanvas, ClipY );
  LINK_NATIVE_PROPERTY( UCanvas, CurX );
  LINK_NATIVE_PROPERTY( UCanvas, CurY );
  LINK_NATIVE_PROPERTY( UCanvas, Z );
  LINK_NATIVE_PROPERTY( UCanvas, Style );
  LINK_NATIVE_PROPERTY( UCanvas, CurYL );
  LINK_NATIVE_PROPERTY( UCanvas, DrawColor );
  LINK_NATIVE_PROPERTY( UCanvas, bCenter );
  LINK_NATIVE_PROPERTY( UCanvas, bNoSmooth );
  LINK_NATIVE_PROPERTY( UCanvas, bUseOffset );
  LINK_NATIVE_PROPERTY( UCanvas, bUseFlatZ );
  LINK_NATIVE_PROPERTY( UCanvas, SizeX );
  LINK_NATIVE_PROPERTY( UCanvas, SizeY );
  LINK_NATIVE_PROPERTY( UCanvas, SmallFont );
  LINK_NATIVE_PROPERTY( UCanvas, MedFont );
  LINK_NATIVE_PROPERTY( UCanvas, BigFont );
  LINK_NATIVE_PROPERTY( UCanvas, LargeFont );
  LINK_NATIVE_PROPERTY( UCanvas, Viewport );
  LINK_NATIVE_PROPERTY( UCanvas, FramePtr );
  LINK_NATIVE_PROPERTY( UCanvas, RenderPtr );
  LINK_NATIVE_PROPERTY( UCanvas, RenderCoords );
END_PROPERTY_LINK()

