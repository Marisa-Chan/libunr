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
 * ANavigationPoint.h - AI Pathing Base Class
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Actors/AActor.h"

class ANavigationPoint : public AActor
{
  DECLARE_NATIVE_CLASS( ANavigationPoint, AActor, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ANavigationPoint();

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
};

class APlayerStart : public ANavigationPoint
{
  DECLARE_NATIVE_CLASS( APlayerStart, ANavigationPoint, 0, Engine )
  EXPOSE_TO_USCRIPT()

  APlayerStart();

  u8   TeamNumber;
  bool bSinglePlayerStart;
  bool bCoopStart;
  bool bEnabled;
};

class ATeleporter : public ANavigationPoint
{
  DECLARE_NATIVE_CLASS( ATeleporter, ANavigationPoint, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ATeleporter();

  String* URL;
  
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
};
