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
 * ANavigationPoint.cpp - AI Pathing functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Actors/ANavigationPoint.h"
#include "Core/UClass.h"
#include "Core/UProperty.h"

ANavigationPoint::ANavigationPoint()
  : AActor()
{
}

ANavigationPoint::~ANavigationPoint()
{
}

void ANavigationPoint::PostDefaultLoad()
{
  Super::PostDefaultLoad();
  if ( !GSystem->bEnhancedRuntime )
  {
    UProperty* PropPrunedPaths   = FindProperty("PrunedPaths");
    UProperty* PropUpstreamPaths = FindProperty("upstreamPaths");
    UProperty* PropPaths         = FindProperty("Paths");

    // These can never be set by the user in the editor, but official Engine.u
    // does not mark these as editconst
    PropPrunedPaths->PropertyFlags   |= CPF_EditConst;
    PropUpstreamPaths->PropertyFlags |= CPF_EditConst;
    PropPaths->PropertyFlags         |= CPF_EditConst;
  }
}

AWarpZoneMarker::AWarpZoneMarker()
  : ANavigationPoint()
{
}

AWarpZoneMarker::~AWarpZoneMarker()
{
}

ALiftCenter::ALiftCenter()
  : ANavigationPoint()
{
}

ALiftCenter::~ALiftCenter()
{
}

ALiftExit::ALiftExit()
  : ANavigationPoint()
{
}

ALiftExit::~ALiftExit()
{
}

APlayerStart::APlayerStart()
  : ANavigationPoint()
{
}

APlayerStart::~APlayerStart()
{
}

ATeleporter::ATeleporter()
  : ANavigationPoint()
{
}

ATeleporter::~ATeleporter()
{
}

IMPLEMENT_NATIVE_CLASS( ANavigationPoint );
IMPLEMENT_NATIVE_CLASS( AWarpZoneMarker );
IMPLEMENT_NATIVE_CLASS( ALiftExit );
IMPLEMENT_NATIVE_CLASS( ALiftCenter );
IMPLEMENT_NATIVE_CLASS( APlayerStart );
IMPLEMENT_NATIVE_CLASS( ATeleporter );

BEGIN_PROPERTY_LINK( ANavigationPoint, 32 )
  LINK_NATIVE_PROPERTY( OwnerTeam );
  LINK_NATIVE_ARRAY   ( ProscribedPaths );
  LINK_NATIVE_ARRAY   ( ForcedPaths );
  LINK_NATIVE_ARRAY   ( UpstreamPaths );
  LINK_NATIVE_ARRAY   ( Paths );
  LINK_NATIVE_ARRAY   ( PrunedPaths );
  LINK_NATIVE_ARRAY   ( VisNoReachPaths );
  LINK_NATIVE_PROPERTY( VisitedWeight );
  LINK_NATIVE_PROPERTY( RouteCache );
  LINK_NATIVE_PROPERTY( BestPathWeight );
  LINK_NATIVE_PROPERTY( NextNavigationPoint );
  LINK_NATIVE_PROPERTY( NextOrdered );
  LINK_NATIVE_PROPERTY( PrevOrdered );
  LINK_NATIVE_PROPERTY( StartPath );
  LINK_NATIVE_PROPERTY( PreviousPath );
  LINK_NATIVE_PROPERTY( Cost );
  LINK_NATIVE_PROPERTY( ExtraCost );
  LINK_NATIVE_PROPERTY( PathDescription );
  LINK_NATIVE_PROPERTY( EditorData );
  LINK_NATIVE_PROPERTY( ForcedPathSize );
  LINK_NATIVE_PROPERTY( MaxPathDistance );
  LINK_NATIVE_PROPERTY( Taken );
  LINK_NATIVE_PROPERTY( bPlayerOnly );
  LINK_NATIVE_PROPERTY( bEndPoint );
  LINK_NATIVE_PROPERTY( bEndPointOnly );
  LINK_NATIVE_PROPERTY( bSpecialCost );
  LINK_NATIVE_PROPERTY( bOneWayPath );
  LINK_NATIVE_PROPERTY( bAutoBuilt );
  LINK_NATIVE_PROPERTY( bNoStrafeTo );
  LINK_NATIVE_PROPERTY( bNeverUseStrafing );
  LINK_NATIVE_PROPERTY( bTwoWay );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AWarpZoneMarker, 3 )
  LINK_NATIVE_PROPERTY( markedWarpZone );
  LINK_NATIVE_PROPERTY( TriggerActor );
  LINK_NATIVE_PROPERTY( TriggerActor2 );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ALiftExit, 5 )
  LINK_NATIVE_PROPERTY( LiftTag );
  LINK_NATIVE_PROPERTY( MyLift );
  LINK_NATIVE_PROPERTY( LiftTrigger );
  LINK_NATIVE_PROPERTY( RecommendedTrigger );
  LINK_NATIVE_PROPERTY( LastTriggerTime );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ALiftCenter, 8 )
  LINK_NATIVE_PROPERTY( LiftTag );
  LINK_NATIVE_PROPERTY( MyLift );
  LINK_NATIVE_PROPERTY( LiftTrigger );
  LINK_NATIVE_PROPERTY( RecommendedTrigger );
  LINK_NATIVE_PROPERTY( LastTriggerTime );
  LINK_NATIVE_PROPERTY( MaxZDiffAdd );
  LINK_NATIVE_PROPERTY( MaxDist2D );
  LINK_NATIVE_PROPERTY( LiftOffset );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( APlayerStart, 7 )
  LINK_NATIVE_PROPERTY( TeamNumber );
  LINK_NATIVE_PROPERTY( bSinglePlayerStart );
  LINK_NATIVE_PROPERTY( bCoopStart );
  LINK_NATIVE_PROPERTY( bEnabled );
  LINK_NATIVE_PROPERTY( bTeamOnlyStart );
  LINK_NATIVE_PROPERTY( bNonTeamOnlyStart );
  LINK_NATIVE_PROPERTY_ALIASED( bTeamOnly, bTeamOnlyStart ); // rune
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ATeleporter, 15 )
  LINK_NATIVE_PROPERTY( URL );
  LINK_NATIVE_PROPERTY( ProductRequired );
  LINK_NATIVE_PROPERTY( bChangesVelocity );
  LINK_NATIVE_PROPERTY( bChangesYaw );
  LINK_NATIVE_PROPERTY( bReversesX );
  LINK_NATIVE_PROPERTY( bReversesY );
  LINK_NATIVE_PROPERTY( bReversesZ );
  LINK_NATIVE_PROPERTY( bEnabled );
  LINK_NATIVE_PROPERTY( TargetVelocity );
  LINK_NATIVE_PROPERTY( TriggerActor );
  LINK_NATIVE_PROPERTY( TriggerActor2 );
  LINK_NATIVE_PROPERTY( LastUdpTime );
  LINK_NATIVE_PROPERTY( TeleportCount );
  LINK_NATIVE_PROPERTY( bBackup_Enabled );
  LINK_NATIVE_PROPERTY( LastFired );
END_PROPERTY_LINK()

