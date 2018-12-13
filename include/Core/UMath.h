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
 * UMath.h - Structs and functions for mathematical operations
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/FUtil.h"

struct FVector
{
  float X;
  float Y;
  float Z;
};

struct FPlane : public FVector
{
  float W;
};

struct FQuat
{
  float X;
  float Y;
  float Z;
  float W;
};

struct FRotator
{
  int Pitch;
  int Yaw;
  int Roll;
};

struct FBox
{
  FVector Min;
  FVector Max;
  u8      IsValid;
};

struct FSphere : public FPlane
{
};

struct FCoords
{
  FVector Origin;
  FVector XAxis;
  FVector YAxis;
  FVector ZAxis;
};

struct FScale
{
  FVector Scale;
  float   SheerRate;
  enum ESheerAxis
  {
    SHEER_None,
    SHEER_XY,
    SHEER_XZ,
    SHEER_YX,
    SHEER_YZ,
    SHEER_ZX,
    SHEER_ZY
  };

  ESheerAxis SheerAxis;
};

