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
 * ANavigationPoint.h - AI Pathing Base Class
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Actors/AActor.h"

class LIBUNR_API ANavigationPoint : public AActor
{
  DECLARE_NATIVE_CLASS( ANavigationPoint, AActor, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ANavigationPoint();
  virtual void PostDefaultLoad();

  // NavigationPoint variables
  idx OwnerTeam;
  idx ProscribedPaths[4];
  idx ForcedPaths[4];

  int UpstreamPaths[16];
  int Paths[16];
  int PrunedPaths[16];
  ANavigationPoint* VisNoReachPaths[16];
  int VisitedWeight;
  AActor* RouteCache;
  int BestPathWeight;
  ANavigationPoint* NextNavigationPoint;
  ANavigationPoint* NextOrdered;
  ANavigationPoint* PrevOrdered;
  ANavigationPoint* StartPath;
  ANavigationPoint* PreviousPath;
  int Cost;
  int ExtraCost;

  u8 PathDescription;
  void* EditorData;

  int ForcedPathSize;
  float MaxPathDistance;

  enum PathBuildingType
  {
    PATHING_Normal,
    PATHING_Proscribe,
    PATHING_Force,
    PATHING_Special,
  };

  // Flags
  bool Taken;
  bool bPlayerOnly;
  bool bEndPoint;
  bool bEndPointOnly;
  bool bSpecialCost;
  bool bOneWayPath;
  bool bAutoBuilt;
  bool bNoStrafeTo;

  // UT99 Variables
  bool bNeverUseStrafing;
  bool bTwoWay;
};

class LIBUNR_API AWarpZoneMarker : public ANavigationPoint
{
  DECLARE_NATIVE_CLASS( AWarpZoneMarker, ANavigationPoint, CLASS_NoUserCreate, Engine )
  EXPOSE_TO_USCRIPT()

  AWarpZoneMarker();

  class AWarpZoneInfo* markedWarpZone;
  AActor* TriggerActor;
  AActor* TriggerActor2;
};

class LIBUNR_API ALiftExit : public ANavigationPoint
{
  DECLARE_NATIVE_CLASS( ALiftExit, ANavigationPoint, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ALiftExit();

  idx LiftTag;
  class AMover* MyLift;
  idx LiftTrigger;
  class ATrigger* RecommendedTrigger;
  float LastTriggerTime;
};

class LIBUNR_API ALiftCenter : public ANavigationPoint
{
  DECLARE_NATIVE_CLASS( ALiftCenter, ANavigationPoint, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ALiftCenter();

  idx LiftTag;
  class AMover* MyLift;
  idx LiftTrigger;
  class ATrigger* RecommendedTrigger;
  float LastTriggerTime;
  float MaxZDiffAdd;
  float MaxDist2D;
  FVector LiftOffset;
};

class LIBUNR_API APlayerStart : public ANavigationPoint
{
  DECLARE_NATIVE_CLASS( APlayerStart, ANavigationPoint, 0, Engine )
  EXPOSE_TO_USCRIPT()

  APlayerStart();

  u8   TeamNumber;
  bool bSinglePlayerStart;
  bool bCoopStart;
  bool bEnabled;
  
  // Deus Ex Variables
  bool bTeamOnlyStart;
  bool bNonTeamOnlyStart;
};

class LIBUNR_API ATeleporter : public ANavigationPoint
{
  DECLARE_NATIVE_CLASS( ATeleporter, ANavigationPoint, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ATeleporter();

  FString* URL;
  
  idx ProductRequired;

  bool bChangesVelocity;
  bool bChangesYaw;
  bool bReversesX;
  bool bReversesY;
  bool bReversesZ;

  bool bEnabled;

  FVector TargetVelocity;

  AActor* TriggerActor;
  AActor* TriggerActor2;

  float LastUdpTime;
  u8    TeleportCount;
  bool  bBackup_Enabled;

  float LastFired;
};

