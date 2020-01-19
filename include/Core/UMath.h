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
 * UMath.h - Structs and functions for mathematical operations
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include <float.h>
#include <math.h>
#include "Util/FTypes.h"
#include "Core/UPackage.h"

// Floating point math functions
inline bool FltEqual( float A, float B )
{
  return fabsf( A - B ) <= FLT_MIN;
}

/*-----------------------------------------------------------------------------
 * FVector
 * A 3D floating point coordinate
-----------------------------------------------------------------------------*/
struct LIBUNR_API FVector
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

/*-----------------------------------------------------------------------------
 * FVector
 * A 4D floating point coordinate
-----------------------------------------------------------------------------*/
struct LIBUNR_API FPlane : public FVector
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

/*-----------------------------------------------------------------------------
 * FQuat
 * A floating point quaternion
-----------------------------------------------------------------------------*/
struct LIBUNR_API FQuat
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

/*-----------------------------------------------------------------------------
 * FRotator
 * A 3D rotation descriptor
-----------------------------------------------------------------------------*/
struct LIBUNR_API FRotator
{
  int Pitch;
  int Yaw;
  int Roll;

  FRotator()
  {
    Pitch = 0;
    Yaw = 0;
    Roll = 0;
  }

  FRotator( int InPitch, int InYaw, int InRoll )
  {
    Pitch = InPitch;
    Yaw = InYaw;
    Roll = InRoll;
  }

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

/*-----------------------------------------------------------------------------
 * FBox
 * A 3D rectangular volume
-----------------------------------------------------------------------------*/
struct LIBUNR_API FBox
{
  FVector Min;
  FVector Max;
  u8      IsValid;

  bool IsZero()
  {
    FVector Sub = Max - Min;
    return (Sub.X == 0 && Sub.Y == 0 && Sub.Z == 0);
  }

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

/*-----------------------------------------------------------------------------
 * FBox2D
 * A 2D rectangular area
-----------------------------------------------------------------------------*/
struct LIBUNR_API FBox2D
{
  float X, Y;
  float Width, Height;

  bool IsZero()
  {
    return (Width == 0 && Height == 0);
  }

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FBox2D& Box )
  {
    In >> Box.X;
    In >> Box.Y;
    In >> Box.Width;
    In >> Box.Height;
    return In;
  }

  friend FPackageFileOut& operator<<( FPackageFileOut& Out, FBox2D& Box )
  {
    Out << Box.X;
    Out << Box.Y;
    Out << Box.Width;
    Out << Box.Height;
    return Out;
  }
};

/*-----------------------------------------------------------------------------
 * FBoxInt2D
 * An integer based 2D rectangular area
-----------------------------------------------------------------------------*/
struct LIBUNR_API FBoxInt2D
{
  int X, Y;
  int Width, Height;

  FBoxInt2D()
  {
    memset( this, 0, sizeof( FBoxInt2D ) );
  }

  FBoxInt2D( int InX, int InY, int InWidth, int InHeight )
  {
    X = InX;
    Y = InY;
    Width = InWidth;
    Height = InHeight;
  }

  bool IsZero()
  {
    return (Width == 0 && Height == 0);
  }

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FBoxInt2D& Box )
  {
    In >> Box.X;
    In >> Box.Y;
    In >> Box.Width;
    In >> Box.Height;
    return In;
  }

  friend FPackageFileOut& operator<<( FPackageFileOut& Out, FBoxInt2D& Box )
  {
    Out << Box.X;
    Out << Box.Y;
    Out << Box.Width;
    Out << Box.Height;
    return Out;
  }
};

/*-----------------------------------------------------------------------------
 * FSphere
 * A floating point 3D sphere
-----------------------------------------------------------------------------*/
struct LIBUNR_API FSphere : public FPlane
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

/*-----------------------------------------------------------------------------
 * FCoords
 * A 3D floating point coordinate space
-----------------------------------------------------------------------------*/
struct LIBUNR_API FCoords
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

/*-----------------------------------------------------------------------------
 * FScale
 * A 3D floating point scaling vector
-----------------------------------------------------------------------------*/
struct LIBUNR_API FScale
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

/*-----------------------------------------------------------------------------
 * FMatrix4x4
 * A 4x4 floating point matrix
-----------------------------------------------------------------------------*/
struct LIBUNR_API FMatrix4x4
{
  float Data[4][4];

  FMatrix4x4()
  {
    memset( Data, 0, sizeof( Data ) );
  }
  FMatrix4x4( float* InData )
  {
    memcpy( Data, InData, sizeof( Data ) );
  }

  friend FPackageFileIn& operator>>( FPackageFileIn& In, FMatrix4x4& Mat )
  {
    for ( int i = 0; i < 4; i++ )
    {
      In >> Mat.Data[i][0];
      In >> Mat.Data[i][1];
      In >> Mat.Data[i][2];
      In >> Mat.Data[i][3];
    }
    return In;
  }

  friend FPackageFileOut& operator<<( FPackageFileOut& Out, FMatrix4x4& Mat )
  {
    for ( int i = 0; i < 4; i++ )
    {
      Out << Mat.Data[i][0];
      Out << Mat.Data[i][1];
      Out << Mat.Data[i][2];
      Out << Mat.Data[i][3];
    }
    return Out;
  }
};

