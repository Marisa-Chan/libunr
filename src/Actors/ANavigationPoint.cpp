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

BEGIN_PROPERTY_LINK( ANavigationPoint, 30 )
  LINK_NATIVE_PROPERTY( ANavigationPoint, OwnerTeam );
  LINK_NATIVE_ARRAY   ( ANavigationPoint, ProscribedPaths );
  LINK_NATIVE_ARRAY   ( ANavigationPoint, ForcedPaths );
  LINK_NATIVE_ARRAY   ( ANavigationPoint, UpstreamPaths );
  LINK_NATIVE_ARRAY   ( ANavigationPoint, Paths );
  LINK_NATIVE_ARRAY   ( ANavigationPoint, PrunedPaths );
  LINK_NATIVE_ARRAY   ( ANavigationPoint, VisNoReachPaths );
  LINK_NATIVE_PROPERTY( ANavigationPoint, VisitedWeight );
  LINK_NATIVE_PROPERTY( ANavigationPoint, RouteCache );
  LINK_NATIVE_PROPERTY( ANavigationPoint, BestPathWeight );
  LINK_NATIVE_PROPERTY( ANavigationPoint, NextNavigationPoint );
  LINK_NATIVE_PROPERTY( ANavigationPoint, NextOrdered );
  LINK_NATIVE_PROPERTY( ANavigationPoint, PrevOrdered );
  LINK_NATIVE_PROPERTY( ANavigationPoint, StartPath );
  LINK_NATIVE_PROPERTY( ANavigationPoint, PreviousPath );
  LINK_NATIVE_PROPERTY( ANavigationPoint, Cost );
  LINK_NATIVE_PROPERTY( ANavigationPoint, ExtraCost );
  LINK_NATIVE_PROPERTY( ANavigationPoint, PathDescription );
  LINK_NATIVE_PROPERTY( ANavigationPoint, EditorData );
  LINK_NATIVE_PROPERTY( ANavigationPoint, ForcedPathSize );
  LINK_NATIVE_PROPERTY( ANavigationPoint, MaxPathDistance );
  LINK_NATIVE_PROPERTY( ANavigationPoint, Taken );
  LINK_NATIVE_PROPERTY( ANavigationPoint, bPlayerOnly );
  LINK_NATIVE_PROPERTY( ANavigationPoint, bEndPoint );
  LINK_NATIVE_PROPERTY( ANavigationPoint, bEndPointOnly );
  LINK_NATIVE_PROPERTY( ANavigationPoint, bSpecialCost );
  LINK_NATIVE_PROPERTY( ANavigationPoint, bOneWayPath );
  LINK_NATIVE_PROPERTY( ANavigationPoint, bAutoBuilt );
  LINK_NATIVE_PROPERTY( ANavigationPoint, bNoStrafeTo );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AWarpZoneMarker, 3 )
  LINK_NATIVE_PROPERTY( AWarpZoneMarker, markedWarpZone );
  LINK_NATIVE_PROPERTY( AWarpZoneMarker, TriggerActor );
  LINK_NATIVE_PROPERTY( AWarpZoneMarker, TriggerActor2 );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ALiftExit, 5 )
  LINK_NATIVE_PROPERTY( ALiftExit, LiftTag );
  LINK_NATIVE_PROPERTY( ALiftExit, MyLift );
  LINK_NATIVE_PROPERTY( ALiftExit, LiftTrigger );
  LINK_NATIVE_PROPERTY( ALiftExit, RecommendedTrigger );
  LINK_NATIVE_PROPERTY( ALiftExit, LastTriggerTime );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ALiftCenter, 8 )
  LINK_NATIVE_PROPERTY( ALiftCenter, LiftTag );
  LINK_NATIVE_PROPERTY( ALiftCenter, MyLift );
  LINK_NATIVE_PROPERTY( ALiftCenter, LiftTrigger );
  LINK_NATIVE_PROPERTY( ALiftCenter, RecommendedTrigger );
  LINK_NATIVE_PROPERTY( ALiftCenter, LastTriggerTime );
  LINK_NATIVE_PROPERTY( ALiftCenter, MaxZDiffAdd );
  LINK_NATIVE_PROPERTY( ALiftCenter, MaxDist2D );
  LINK_NATIVE_PROPERTY( ALiftCenter, LiftOffset );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( APlayerStart, 4 )
  LINK_NATIVE_PROPERTY( APlayerStart, TeamNumber );
  LINK_NATIVE_PROPERTY( APlayerStart, bSinglePlayerStart );
  LINK_NATIVE_PROPERTY( APlayerStart, bCoopStart );
  LINK_NATIVE_PROPERTY( APlayerStart, bEnabled );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ATeleporter, 14 )
  LINK_NATIVE_PROPERTY( ATeleporter, URL );
  LINK_NATIVE_PROPERTY( ATeleporter, ProductRequired );
  LINK_NATIVE_PROPERTY( ATeleporter, bChangesVelocity );
  LINK_NATIVE_PROPERTY( ATeleporter, bChangesYaw );
  LINK_NATIVE_PROPERTY( ATeleporter, bReversesX );
  LINK_NATIVE_PROPERTY( ATeleporter, bReversesY );
  LINK_NATIVE_PROPERTY( ATeleporter, bReversesZ );
  LINK_NATIVE_PROPERTY( ATeleporter, bEnabled );
  LINK_NATIVE_PROPERTY( ATeleporter, TargetVelocity );
  LINK_NATIVE_PROPERTY( ATeleporter, TriggerActor );
  LINK_NATIVE_PROPERTY( ATeleporter, TriggerActor2 );
  LINK_NATIVE_PROPERTY( ATeleporter, LastUdpTime );
  LINK_NATIVE_PROPERTY( ATeleporter, TeleportCount );
  LINK_NATIVE_PROPERTY( ATeleporter, bBackup_Enabled );
END_PROPERTY_LINK()

