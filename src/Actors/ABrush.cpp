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
 * ABrush.h - Brush actor functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Core/UModel.h"
#include "Actors/ABrush.h"

ABrush::ABrush()
  : AActor()
{
}

ABrush::~ABrush()
{
}

AMover::AMover()
  : ABrush()
{
}

AMover::~AMover()
{
}

bool ABrush::ExportToFile( const char* Dir, const char* Type )
{
  return false;
}

void ABrush::ExportToLevelText( FileStreamOut* Out )
{
  Out->Printf( "\r\n\tBegin Brush Name=%s\r\n", Brush->Name );
  Brush->Polys->ExportToLevelText( Out );
  Out->Printf( "\tEnd Brush\r\n" );
}

IMPLEMENT_NATIVE_CLASS( ABrush );
IMPLEMENT_NATIVE_CLASS( AMover );

BEGIN_PROPERTY_LINK( ABrush, 9 )
  LINK_NATIVE_PROPERTY( CsgOper );
  LINK_NATIVE_PROPERTY( UnusedLightMesh );
  LINK_NATIVE_PROPERTY( PostPivot );
  LINK_NATIVE_PROPERTY( MainScale );
  LINK_NATIVE_PROPERTY( PostScale );
  LINK_NATIVE_PROPERTY( TempScale );
  LINK_NATIVE_PROPERTY( BrushColor );
  LINK_NATIVE_PROPERTY( PolyFlags );
  LINK_NATIVE_PROPERTY( bColored );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AMover, 63 )
  LINK_NATIVE_PROPERTY( MoverEncroachType );
  LINK_NATIVE_PROPERTY( MoverGlideType );
  LINK_NATIVE_PROPERTY( bUseShortestRotation );
  LINK_NATIVE_PROPERTY( BumpType );
  LINK_NATIVE_PROPERTY( KeyNum );
  LINK_NATIVE_PROPERTY( PrevKeyNum );
  LINK_NATIVE_PROPERTY( NumKeys );
  LINK_NATIVE_PROPERTY( WorldRaytraceKey );
  LINK_NATIVE_PROPERTY( BrushRaytraceKey );
  LINK_NATIVE_PROPERTY( MoveTime );
  LINK_NATIVE_PROPERTY( StayOpenTime );
  LINK_NATIVE_PROPERTY( OtherTime );
  LINK_NATIVE_PROPERTY( EncroachDamage );
  LINK_NATIVE_PROPERTY( bTriggerOnceOnly );
  LINK_NATIVE_PROPERTY( bSlave );
  LINK_NATIVE_PROPERTY( bUseTriggered );
  LINK_NATIVE_PROPERTY( bDamageTriggered );
  LINK_NATIVE_PROPERTY( bDynamicLightMover );
  LINK_NATIVE_PROPERTY( bDirectionalPushOff );
  LINK_NATIVE_PROPERTY( bAdvancedCamUpdate );
  LINK_NATIVE_PROPERTY( bUseGoodCollision );
  LINK_NATIVE_PROPERTY( PlayerBumpEvent );
  LINK_NATIVE_PROPERTY( BumpEvent );
  LINK_NATIVE_PROPERTY( SavedTrigger );
  LINK_NATIVE_PROPERTY( DamageThreshold );
  LINK_NATIVE_PROPERTY( NumTriggerEvents );
  LINK_NATIVE_PROPERTY( Leader );
  LINK_NATIVE_PROPERTY( Follower );
  LINK_NATIVE_PROPERTY( ReturnGroup );
  LINK_NATIVE_PROPERTY( DelayTime );
  LINK_NATIVE_PROPERTY( OpeningSound );
  LINK_NATIVE_PROPERTY( OpenedSound );
  LINK_NATIVE_PROPERTY( ClosingSound );
  LINK_NATIVE_PROPERTY( ClosedSound );
  LINK_NATIVE_PROPERTY( MoveAmbientSound );
  LINK_NATIVE_ARRAY   ( KeyPos );
  LINK_NATIVE_ARRAY   ( KeyRot );
  LINK_NATIVE_PROPERTY( BasePos );
  LINK_NATIVE_PROPERTY( OldPos );
  LINK_NATIVE_PROPERTY( OldPrePivot );
  LINK_NATIVE_PROPERTY( SavedPos );
  LINK_NATIVE_PROPERTY( BaseRot );
  LINK_NATIVE_PROPERTY( OldRot );
  LINK_NATIVE_PROPERTY( SavedRot );
  LINK_NATIVE_PROPERTY( NotifyLightMaps );
  LINK_NATIVE_PROPERTY( MyMarker );
  LINK_NATIVE_PROPERTY( TriggerActor );
  LINK_NATIVE_PROPERTY( TriggerActor2 );
  LINK_NATIVE_PROPERTY( WaitingPawn );
  LINK_NATIVE_PROPERTY( bOpening );
  LINK_NATIVE_PROPERTY( bDelaying );
  LINK_NATIVE_PROPERTY( bPlayerOnly );
  LINK_NATIVE_PROPERTY( RecommendedTrigger );
  LINK_NATIVE_PROPERTY( SimOldPos );
  LINK_NATIVE_PROPERTY( SimOldRotPitch );
  LINK_NATIVE_PROPERTY( SimOldRotYaw );
  LINK_NATIVE_PROPERTY( SimOldRotRoll );
  LINK_NATIVE_PROPERTY( SimInterpolate );
  LINK_NATIVE_PROPERTY( RealPosition );
  LINK_NATIVE_PROPERTY( RealRotation );
  LINK_NATIVE_PROPERTY( ServerUpdate );
  LINK_NATIVE_PROPERTY( ClientUpdate );
  LINK_NATIVE_PROPERTY( LNextKeyNum );
END_PROPERTY_LINK()

