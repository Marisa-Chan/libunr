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
 * UEditorEngine.cpp - Editor engine environment
 *
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Editor/UEditorEngine.h"

UTransBuffer::~UTransBuffer()
{
}

UEditorEngine::UEditorEngine()
  : UEngine()
{

}

UEditorEngine::~UEditorEngine()
{
}

bool UEditorEngine::Init()
{
  return Super::Init();
}

bool UEditorEngine::Exit()
{
  return Super::Exit();
}

void UEditorEngine::Tick( float DeltaTime )
{
  Super::Tick( DeltaTime );
}

IMPLEMENT_NATIVE_CLASS( UTransBuffer );
IMPLEMENT_NATIVE_CLASS( UEditorEngine );

BEGIN_PROPERTY_LINK( UEditorEngine, 75 )
  LINK_NATIVE_PROPERTY( Level );
  LINK_NATIVE_PROPERTY( TempModel );
  LINK_NATIVE_PROPERTY( CurrentTexture );
  LINK_NATIVE_PROPERTY( CurrentClass );
  LINK_NATIVE_PROPERTY( Trans );
  LINK_NATIVE_PROPERTY( Results );
  LINK_NATIVE_PROPERTY( MenuUp );
  LINK_NATIVE_PROPERTY( MenuDn );
  LINK_NATIVE_PROPERTY( CollOn );
  LINK_NATIVE_PROPERTY( CollOff );
  LINK_NATIVE_PROPERTY( PlyrOn );
  LINK_NATIVE_PROPERTY( PlyrOff );
  LINK_NATIVE_PROPERTY( LiteOn );
  LINK_NATIVE_PROPERTY( LiteOff );
  LINK_NATIVE_PROPERTY( BackPrev );
  LINK_NATIVE_PROPERTY( Bad );
  LINK_NATIVE_PROPERTY( Bkgnd );
  LINK_NATIVE_PROPERTY( BkgndHi );
  LINK_NATIVE_PROPERTY( bFastRebuild );
  LINK_NATIVE_PROPERTY( bBootstrapping );
  LINK_NATIVE_PROPERTY( AutoSaveIndex );
  LINK_NATIVE_PROPERTY( AutoSaveCount );
  LINK_NATIVE_PROPERTY( Mode );
  LINK_NATIVE_PROPERTY( ClickFlags );
  LINK_NATIVE_PROPERTY( MovementSpeed );
  LINK_NATIVE_PROPERTY( PackageContext );
  LINK_NATIVE_PROPERTY( AddLocation );
  LINK_NATIVE_PROPERTY( AddPlane );
  LINK_NATIVE_PROPERTY( Tools );
  LINK_NATIVE_PROPERTY( BrowseClass );
  LINK_NATIVE_PROPERTY( GridEnabled );
  LINK_NATIVE_PROPERTY( SnapVertices );
  LINK_NATIVE_PROPERTY( AffectRegion );
  LINK_NATIVE_PROPERTY( TextureLock );
  LINK_NATIVE_PROPERTY( SelectionLock );
  LINK_NATIVE_PROPERTY( SnapDistance );
  LINK_NATIVE_PROPERTY( GridSize );
  LINK_NATIVE_PROPERTY( RotGridEnabled );
  LINK_NATIVE_PROPERTY( RotGridSize );
  LINK_NATIVE_PROPERTY( FovAngleDegrees );
  LINK_NATIVE_PROPERTY( GodMode );
  LINK_NATIVE_PROPERTY( FreeMeshView );
  LINK_NATIVE_PROPERTY( AutoSave );
  LINK_NATIVE_PROPERTY( AutosaveTimeMinutes );
  LINK_NATIVE_PROPERTY( AskSave );
  LINK_NATIVE_PROPERTY( GameCommandLine );
  LINK_NATIVE_PROPERTY( EditPackages );
  LINK_NATIVE_PROPERTY( AlwaysPermanentBrush );
  LINK_NATIVE_PROPERTY( C_WorldBox );
  LINK_NATIVE_PROPERTY( C_GroundPlane );
  LINK_NATIVE_PROPERTY( C_GroundHighlight );
  LINK_NATIVE_PROPERTY( C_BrushWire );
  LINK_NATIVE_PROPERTY( C_Pivot );
  LINK_NATIVE_PROPERTY( C_Select );
  LINK_NATIVE_PROPERTY( C_Current );
  LINK_NATIVE_PROPERTY( C_AddWire );
  LINK_NATIVE_PROPERTY( C_SubtractWire );
  LINK_NATIVE_PROPERTY( C_GreyWire );
  LINK_NATIVE_PROPERTY( C_BrushVertex );
  LINK_NATIVE_PROPERTY( C_BrushSnap );
  LINK_NATIVE_PROPERTY( C_Invalid );
  LINK_NATIVE_PROPERTY( C_ActorWire );
  LINK_NATIVE_PROPERTY( C_ActorHiWire );
  LINK_NATIVE_PROPERTY( C_Black );
  LINK_NATIVE_PROPERTY( C_White );
  LINK_NATIVE_PROPERTY( C_Mask );
  LINK_NATIVE_PROPERTY( C_SemiSolidWire );
  LINK_NATIVE_PROPERTY( C_NonSolidWire );
  LINK_NATIVE_PROPERTY( C_WireBackground );
  LINK_NATIVE_PROPERTY( C_WireGridAxis );
  LINK_NATIVE_PROPERTY( C_ActorArrow );
  LINK_NATIVE_PROPERTY( C_ScaleBox );
  LINK_NATIVE_PROPERTY( C_ScaleBoxHi );
  LINK_NATIVE_PROPERTY( C_ZoneWire );
  LINK_NATIVE_PROPERTY( C_Mover );
  LINK_NATIVE_PROPERTY( C_OrthoBackground );
  LINK_USELESS_PROPERTY( NotifyVtbl );
  LINK_USELESS_PROPERTY( ConstraintsVtbl );
  LINK_USELESS_PROPERTY( Pad );
END_PROPERTY_LINK()