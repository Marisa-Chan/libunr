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
 * UMath.h - Structs and functions for mathematical operations
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Util/FTypes.h"
#include "Core/UPackage.h"

// Floating point math functions
inline bool FltEqual( float A, float B )
{
  return fabsf( A - B ) <= FLT_MIN;
}

struct DLL_EXPORT FVector
{
  float X;
  float Y;
  float Z;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FVector& Vector )
  {
    In >> Vector.X; 
    In >> Vector.Y;
    In >> Vector.Z;
    return In;
  }

  friend FPackageFileOut& operator<<( FPackageFileOut& Out, FVector& Vector )
  {
    Out << Vector.X;
    Out << Vector.Y;
    Out << Vector.Z;
    return Out;
  }

  friend void operator+=( FVector& A, FVector& B );
  friend FVector operator+( FVector& A, FVector& B );
  friend FVector operator-( FVector& A, FVector& B );
};

struct DLL_EXPORT FPlane : public FVector
{
  float W;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FPlane& Plane )
  {
    In >> (FVector&)Plane;
    In >> Plane.W;
    return In;
  }

  friend FPackageFileOut& operator<<( FPackageFileOut& Out, FPlane& Plane )
  {
    Out << (FVector&)Plane;
    Out << Plane.W;
    return Out;
  }
};

struct DLL_EXPORT FQuat
{
  float X;
  float Y;
  float Z;
  float W;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FQuat& Quat )
  {
    In >> Quat.X;
    In >> Quat.Y; 
    In >> Quat.Z; 
    In >> Quat.W;
    return In;
  }

  friend FPackageFileOut& operator<<( FPackageFileOut& Out, FQuat& Quat )
  {
    Out << Quat.X; 
    Out << Quat.Y;
    Out << Quat.Z;
    Out << Quat.W;
    return Out;
  }
};

struct DLL_EXPORT FRotator
{
  int Pitch;
  int Yaw;
  int Roll;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FRotator& Rotator )
  {
    In >> Rotator.Pitch;
    In >> Rotator.Yaw;
    In >> Rotator.Roll;
    return In;
  }

  friend FPackageFileOut& operator<<( FPackageFileOut& Out, FRotator& Rotator )
  {
    Out << Rotator.Pitch;
    Out << Rotator.Yaw;
    Out << Rotator.Roll;
    return Out;
  }
};

struct DLL_EXPORT FBox
{
  FVector Min;
  FVector Max;
  u8      IsValid;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FBox& Box )
  {
    In >> Box.Min;
    In >> Box.Max;
    In >> Box.IsValid;
    return In;
  }

  friend FPackageFileOut& operator<<( FPackageFileOut& Out, FBox& Box )
  {
    Out << Box.Min;
    Out << Box.Max;
    Out << Box.IsValid;
    return Out;
  }
};

struct DLL_EXPORT FSphere : public FPlane
{
  friend FPackageFileIn& operator>>( FPackageFileIn& In, FSphere& Sphere )
  {
    if ( In.Ver <= PKG_VER_UN_200 )
      return In >> (FVector&)Sphere;
    else
      return In >> (FPlane&)Sphere;
  }

  friend FPackageFileOut& operator<<( FPackageFileOut& Out, FSphere& Sphere )
  {
    if ( Out.Ver <= PKG_VER_UN_200 )
      return Out << (FVector&)Sphere;
    else
      return Out << (FPlane&)Sphere;
  }
};

struct DLL_EXPORT FCoords
{
  FVector Origin;
  FVector XAxis;
  FVector YAxis;
  FVector ZAxis;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FCoords& Coords )
  {
    return In >> Coords.Origin >> Coords.XAxis >> Coords.YAxis >> Coords.ZAxis;
  }

  friend FPackageFileOut& operator<<( FPackageFileOut& Out, FCoords& Coords )
  {
    return Out << Coords.Origin << Coords.XAxis << Coords.YAxis << Coords.ZAxis;
  }
};

struct DLL_EXPORT FScale
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

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FScale& Scale )
  {
    In >> Scale.Scale;
    In >> Scale.SheerRate;
    In >> (u8&)Scale.SheerAxis;
    return In;
  }

  friend FPackageFileOut& operator<<( FPackageFileOut& Out, FScale& Scale )
  {
    Out << Scale.Scale;
    Out << Scale.SheerRate;
    Out << (u8&)Scale.SheerAxis;
    return Out;
  }
};

