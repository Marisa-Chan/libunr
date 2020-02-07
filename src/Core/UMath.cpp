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

#if defined _MSC_VER
  #include <intrin.h>
#elif defined __GNUG__
  #include <immintrin.h>
#endif

FMatrix4x4& FMatrix4x4::operator*=( FMatrix4x4& A )
{
  FMatrix4x4 Result;
  for ( int i = 0; i < 4; i++ )
  {
    for ( int j = 0; j < 4; j++ )
    {
      Result.Data[i][0] += Data[i][0] * A.Data[j][0];
      Result.Data[i][1] += Data[i][1] * A.Data[j][1];
      Result.Data[i][2] += Data[i][2] * A.Data[j][2];
      Result.Data[i][3] += Data[i][3] * A.Data[j][3];
    }
  }

  memcpy( this->Data, Result.Data, sizeof( float ) * 16 );
  return *this;
}

FVector operator*( FMatrix4x4& A, FVector& B )
{
  FVector Result;
  for ( int i = 0; i < 3; i++ )
  {
    Result.X += A.Data[i][0] * B.X;
    Result.Y += A.Data[i][1] * B.Y;
    Result.Z += A.Data[i][2] * B.Z;
  }
  return Result;
}

float FVector::VSize()
{
  float X2 = X * X;
  float Y2 = Y * Y;
  float Z2 = Z * Z;
  return sqrtf( X2 + Y2 + Z2 );
}

float FVector::Dot( FVector& A )
{
  float x = X * A.X;
  float y = Y * A.Y;
  float z = Z * A.Z;
  return x + y + z;
}

void FVector::Cross( FVector& B )
{
  float NewX = (Y * B.Z) - (Z * B.Y);
  float NewY = (Z * B.X) - (X * B.Z);
  float NewZ = (X * B.Y) - (Y * B.X);

  X = NewX;
  Y = NewY;
  Z = NewZ;
}

void FVector::Normalize()
{
  float Mag = 1.0f / VSize();
  X *= Mag;
  Y *= Mag;
  Z *= Mag;
}

// Treat vector as a positional vector and get a translation matrix
void FVector::GetTranslationMatrix( FMatrix4x4& Mat )
{
  memset( &Mat, 0, sizeof( Mat ) );
  Mat.Data[0][0] = 1.0f;
  Mat.Data[0][3] = X;
  Mat.Data[1][1] = 1.0f;
  Mat.Data[1][3] = Y;
  Mat.Data[2][2] = 1.0f;
  Mat.Data[2][3] = Z;
  Mat.Data[3][3] = 1.0f;
}

// Treat vector as a scaling parameter and get a scale matrix
void FVector::GetScaleMatrix( FMatrix4x4& Mat )
{
  memset( &Mat, 0, sizeof( Mat ) );
  Mat.Data[0][0] = X;
  Mat.Data[1][1] = Y;
  Mat.Data[2][2] = Z;
  Mat.Data[3][3] = 1.0f;
}

LIBUNR_API FVector& operator+=( FVector& A, FVector& B )
{
  A.X += B.X;
  A.Y += B.Y;
  A.Z += B.Z;
  return A;
}

LIBUNR_API FVector& operator-=( FVector& A, FVector& B )
{
  A.X -= B.X;
  A.Y -= B.Y;
  A.Z -= B.Z;
  return A;
}

LIBUNR_API FVector& operator*=( FVector& A, float B )
{
  A.X *= B;
  A.Y *= B;
  A.Z *= B;
  return A;
}

LIBUNR_API FVector operator+( FVector& A, FVector& B )
{
  FVector Y;
  Y.X = A.X + B.X;
  Y.Y = A.Y + B.Y;
  Y.Z = A.Z + B.Z;
  return Y;
}

LIBUNR_API FVector operator-( FVector& A, FVector& B )
{
  FVector Y;
  Y.X = A.X - B.X;
  Y.Y = A.Y - B.Y;
  Y.Z = A.Z - B.Z;
  return Y;
}

void FRotator::GetRadians( FVector& Out )
{
  // Convert rotator coordinates to radians
  // 16384 rotation units = 90 degree turn, 16384 / 90 = 182.0444444 rotation units per degree
  // Therefore, (Rot * 90) / 16384 = Rot in degrees -> Degrees to radians = (angle) * (PI/180)
  // Simplify -> 90/180 = 1/2 -> ((Rot*PI)/16384) * (1/2) -> (Rot*PI) / 32768
  // Use multiplication for speed, so 1/32768 = 0.000030517578125

  #define UU_ROT_TO_RAD(angle) (((double)angle*PI) * 0.000030517578125)

  Out.X = UU_ROT_TO_RAD( Roll );
  Out.Y = UU_ROT_TO_RAD( Yaw );
  Out.Z = UU_ROT_TO_RAD( Pitch );
}

void FRotator::GetMatrix( FMatrix4x4& Out )
{
  FMatrix4x4 Tmp;

  FVector Rads;
  GetRadians( Rads );

  float cx = cosf( Rads.X );
  float sx = sinf( Rads.X );
  float cy = cosf( Rads.Y );
  float sy = sinf( Rads.Y );
  float cz = cosf( Rads.Z );
  float sz = sinf( Rads.Z );

  // Set up Rx
  Out.Data[0][0] = 1.0f;
  Out.Data[1][1] = cx;
  Out.Data[1][2] = -sx;
  Out.Data[2][1] = sx;
  Out.Data[2][2] = cx;
  Out.Data[3][3] = 1.0f;

  // Set up Ry
  Tmp.Data[0][0] = cy;
  Tmp.Data[0][2] = sy;
  Tmp.Data[1][1] = 1.0f;
  Tmp.Data[2][0] = -sy;
  Tmp.Data[2][2] = cy;
  Tmp.Data[3][3] = 1.0f;

  // Rx * Ry
  Out *= Tmp;

  // Set up Rz
  Tmp.Data[0][0] = cz;
  Tmp.Data[0][1] = -sz;
  Tmp.Data[0][2] = 0.0f;
  Tmp.Data[1][0] = sz;
  Tmp.Data[1][1] = cz;
  Tmp.Data[2][0] = 0.0f;
  Tmp.Data[2][2] = 1.0f;
  Tmp.Data[3][3] = 1.0f;

  // (Rx * Ry) * Rz
  Out *= Tmp;
}

void FRotator::GetAxes( FVector& X, FVector& Y, FVector& Z )
{
  // Set up unit vectors
  FVector UnitX = FVector( 0, 0, 1 );
  FVector UnitY = FVector( 0, 1, 0 );
  FVector UnitZ = FVector( 1, 0, 0 );

  // Get rotation matrix
  FMatrix4x4 Mat;
  GetMatrix( Mat );

  // Transform each vector
  X = Mat * UnitX;
  Y = Mat * UnitY;
  Z = Mat * UnitZ;
}
