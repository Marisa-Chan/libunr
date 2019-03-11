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
  LINK_NATIVE_PROPERTY( ABrush, CsgOper );
  LINK_NATIVE_PROPERTY( ABrush, UnusedLightMesh );
  LINK_NATIVE_PROPERTY( ABrush, PostPivot );
  LINK_NATIVE_PROPERTY( ABrush, MainScale );
  LINK_NATIVE_PROPERTY( ABrush, PostScale );
  LINK_NATIVE_PROPERTY( ABrush, TempScale );
  LINK_NATIVE_PROPERTY( ABrush, BrushColor );
  LINK_NATIVE_PROPERTY( ABrush, PolyFlags );
  LINK_NATIVE_PROPERTY( ABrush, bColored );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AMover, 63 )
  LINK_NATIVE_PROPERTY( AMover, MoverEncroachType );
  LINK_NATIVE_PROPERTY( AMover, MoverGlideType );
  LINK_NATIVE_PROPERTY( AMover, bUseShortestRotation );
  LINK_NATIVE_PROPERTY( AMover, BumpType );
  LINK_NATIVE_PROPERTY( AMover, KeyNum );
  LINK_NATIVE_PROPERTY( AMover, PrevKeyNum );
  LINK_NATIVE_PROPERTY( AMover, NumKeys );
  LINK_NATIVE_PROPERTY( AMover, WorldRaytraceKey );
  LINK_NATIVE_PROPERTY( AMover, BrushRaytraceKey );
  LINK_NATIVE_PROPERTY( AMover, MoveTime );
  LINK_NATIVE_PROPERTY( AMover, StayOpenTime );
  LINK_NATIVE_PROPERTY( AMover, OtherTime );
  LINK_NATIVE_PROPERTY( AMover, EncroachDamage );
  LINK_NATIVE_PROPERTY( AMover, bTriggerOnceOnly );
  LINK_NATIVE_PROPERTY( AMover, bSlave );
  LINK_NATIVE_PROPERTY( AMover, bUseTriggered );
  LINK_NATIVE_PROPERTY( AMover, bDamageTriggered );
  LINK_NATIVE_PROPERTY( AMover, bDynamicLightMover );
  LINK_NATIVE_PROPERTY( AMover, bDirectionalPushOff );
  LINK_NATIVE_PROPERTY( AMover, bAdvancedCamUpdate );
  LINK_NATIVE_PROPERTY( AMover, bUseGoodCollision );
  LINK_NATIVE_PROPERTY( AMover, PlayerBumpEvent );
  LINK_NATIVE_PROPERTY( AMover, BumpEvent );
  LINK_NATIVE_PROPERTY( AMover, SavedTrigger );
  LINK_NATIVE_PROPERTY( AMover, DamageThreshold );
  LINK_NATIVE_PROPERTY( AMover, NumTriggerEvents );
  LINK_NATIVE_PROPERTY( AMover, Leader );
  LINK_NATIVE_PROPERTY( AMover, Follower );
  LINK_NATIVE_PROPERTY( AMover, ReturnGroup );
  LINK_NATIVE_PROPERTY( AMover, DelayTime );
  LINK_NATIVE_PROPERTY( AMover, OpeningSound );
  LINK_NATIVE_PROPERTY( AMover, OpenedSound );
  LINK_NATIVE_PROPERTY( AMover, ClosingSound );
  LINK_NATIVE_PROPERTY( AMover, ClosedSound );
  LINK_NATIVE_PROPERTY( AMover, MoveAmbientSound );
  LINK_NATIVE_ARRAY   ( AMover, KeyPos );
  LINK_NATIVE_ARRAY   ( AMover, KeyRot );
  LINK_NATIVE_PROPERTY( AMover, BasePos );
  LINK_NATIVE_PROPERTY( AMover, OldPos );
  LINK_NATIVE_PROPERTY( AMover, OldPrePivot );
  LINK_NATIVE_PROPERTY( AMover, SavedPos );
  LINK_NATIVE_PROPERTY( AMover, BaseRot );
  LINK_NATIVE_PROPERTY( AMover, OldRot );
  LINK_NATIVE_PROPERTY( AMover, SavedRot );
  LINK_NATIVE_PROPERTY( AMover, NotifyLightMaps );
  LINK_NATIVE_PROPERTY( AMover, MyMarker );
  LINK_NATIVE_PROPERTY( AMover, TriggerActor );
  LINK_NATIVE_PROPERTY( AMover, TriggerActor2 );
  LINK_NATIVE_PROPERTY( AMover, WaitingPawn );
  LINK_NATIVE_PROPERTY( AMover, bOpening );
  LINK_NATIVE_PROPERTY( AMover, bDelaying );
  LINK_NATIVE_PROPERTY( AMover, bPlayerOnly );
  LINK_NATIVE_PROPERTY( AMover, RecommendedTrigger );
  LINK_NATIVE_PROPERTY( AMover, SimOldPos );
  LINK_NATIVE_PROPERTY( AMover, SimOldRotPitch );
  LINK_NATIVE_PROPERTY( AMover, SimOldRotYaw );
  LINK_NATIVE_PROPERTY( AMover, SimOldRotRoll );
  LINK_NATIVE_PROPERTY( AMover, SimInterpolate );
  LINK_NATIVE_PROPERTY( AMover, RealPosition );
  LINK_NATIVE_PROPERTY( AMover, RealRotation );
  LINK_NATIVE_PROPERTY( AMover, ServerUpdate );
  LINK_NATIVE_PROPERTY( AMover, ClientUpdate );
  LINK_NATIVE_PROPERTY( AMover, LNextKeyNum );
END_PROPERTY_LINK()

