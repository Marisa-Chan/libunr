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
 * UGesture.h - 2D pattern matching for mouse input
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include "Core/UObject.h"
#include "Core/UMath.h"

class LIBUNR_API UGesture : public UObject
{
  DECLARE_NATIVE_CLASS( UGesture, UObject, 0, Engine );
  EXPOSE_TO_USCRIPT();

  UGesture();

  float CompareGesture( TArray<FVector>* InMousePoints, float Accuracy );
  float CompareGesturePoint( FVector InMousePoint, float Accuracy );

  TArray<FVector>* Points;
  TArray<int>*     Segments;
};

