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
#include "Core/UPackage.h"

struct FVector
{
  float X;
  float Y;
  float Z;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FVector& Vector )
  {
    In >> Vector.X >> Vector.Y >> Vector.Z;
  }

  friend FPackageFileOut& operator<<( FPackageFileOut& Out, FVector& Vector )
  {
    return Out << Vector.X << Vector.Y << Vector.Z;
  }
};

struct FPlane : public FVector
{
  float W;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FPlane& Plane )
  {
    return In >> (FVector&)Plane >> Plane.W;
  }

  friend FPackageFileOut& operator<<( FPackageFileOut& Out, FPlane& Plane )
  {
    return Out << (FVector&)Plane << Plane.W;
  }
};

struct FQuat
{
  float X;
  float Y;
  float Z;
  float W;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FQuat& Quat )
  {
    return In >> Quat.X >> Quat.Y >> Quat.Z >> Quat.W;
  }

  friend FPackageFileOut& operator<<( FPackageFileOut& Out, FQuat& Quat )
  {
    return Out << Quat.X << Quat.Y << Quat.Z << Quat.W;
  }
};

struct FRotator
{
  int Pitch;
  int Yaw;
  int Roll;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FRotator& Rotator )
  {
    return In >> Rotator.Pitch >> Rotator.Yaw >> Rotator.Roll;
  }

  friend FPackageFileOut& operator<<( FPackageFileOut& Out, FRotator& Rotator )
  {
    return Out << Rotator.Pitch << Rotator.Yaw << Rotator.Roll;
  }
};

struct FBox
{
  FVector Min;
  FVector Max;
  u8      IsValid;

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FBox& Box )
  {
    return In >> Box.Min >> Box.Max >> IsValid;
  }

  friend FPackageFileOut& operator<<( FPackageFileOut& Out, FBox& Box )
  {
    return Out << Box.Min << Box.Max << IsValid;
  }
};

struct FSphere : public FPlane
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

struct FCoords
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

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FScale& Scale )
  {
    return In >> Scale >> SheerRate >> (u8)SheerAxis;
  }

  friend FPackageFileOut& operator<<( FPackageFileOut& Out, FScale& Scale )
  {
    return Out << Scale << SheerRate << (u8)SheerAxis;
  }
};

