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
 * UMath.cpp - Math functions
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UMath.h"

void operator+=( FVector& A, FVector& B )
{
  A.X += B.X;
  A.Y += B.Y;
  A.Z += B.Z;
}

FVector operator+( FVector& A, FVector& B )
{
  FVector Y;
  Y.X = A.X + B.X;
  Y.Y = A.Y + B.Y;
  Y.Z = A.Z + B.Z;
  return Y;
}

FVector operator-( FVector& A, FVector& B )
{
  FVector Y;
  Y.X = A.X - B.X;
  Y.Y = A.Y - B.Y;
  Y.Z = A.Z - B.Z;
  return Y;
}

