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
 * ACameraPoint.h - Deus Ex CameraPoint actor
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Actors/AActor.h"

class DLL_EXPORT ACameraPoint : public AKeypoint
{
  DECLARE_NATIVE_CLASS( ACameraPoint, AKeypoint, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ACameraPoint();

  enum ECameraCommand
  {
    CAMCMD_Move,
    CAMCMD_Push,
    CAMCMD_Fov,
    CAMCMD_Tilt,
    CAMCMD_Pan,
    CAMCMD_Roll,
    CAMCMD_Wait,
    CAMCMD_Trigger
  };

  ECameraCommand Cmd;
  float Value;
  FName EventName;
  float TimeSmooth;
  float TimeWaitPost;
  bool  bParallel;
  bool  bRandom;
  int   RandomCount;
  int   PostRandomNum;

  int SequenceNum;
  ACameraPoint* NextPoint;
  ACameraPoint* PrevPoint;
  float CurTime;
  FVector   StartLoc;
  FRotator  StartRot;
  float     StartFov;
  FVector   EndLoc;
  FRotator  EndRot;
  float     EndFov;
  APlayerPawn* Player;
  bool bTickReady;
  bool bFirstRandom;
  int RandomRemain;
  ACameraPoint* ContinuePoint;
};

