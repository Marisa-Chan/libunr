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
 * ATimeDemo.h - Time demo handler class (Unreal only)
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Actors/AActor.h"

class ATimeDemo : public AInfo
{
  DECLARE_NATIVE_CLASS( ATimeDemo, AInfo, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ATimeDemo();

  int FileAr; // Unused now

  float TimePassed;
  float TimeDilation;

  float StartTime;
  float LastSecTime;
  float LastCycleTime;
  float LastFrameTime;
  float SquareSum;

  int FrameNum;
  int FrameLastSecond;
  int FrameLastCycle;
  int CycleCount;
  int QuitAfterCycles;

  FString* CycleMessage;
  FString* CycleResult;

  bool bSaveToFile;
  bool bFirstFrame;

  float LastSec;
  float MinFPS;
  float MaxFPS;

  class AInterpolationPoint* OldPoint;
  class ATimeDemoInterpolationPoint* NewPoint;

  class UConsole* Console;
};

