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
 * glm was a massive help for some of these
 * https://glm.g-truc.net/0.9.9/index.html
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

// The legendary inverse square root from Quake III
float rsqrt( float n )
{
  long i;
  float x2, y;
  const float threehalfs = 1.5F;

  x2 = n * 0.5F;
  y = n;
  i = *(long*)&y;						           // evil floating point bit level hacking
  i = 0x5f3759df - (i >> 1);           // what the fuck?
  y = *(float*)&i;
  y = y * (threehalfs - (x2 * y * y)); // 1st iteration
  return y;
}

FMatrix4x4& FMatrix4x4::operator*=( FMatrix4x4& A )
{
  FMatrix4x4 Result;

  for ( int i = 0; i < 4; i++ )
  {
    Result.Data[i][0] = (Data[i][0] * A.Data[0][0]) + (Data[i][1] * A.Data[1][0]) + (Data[i][2] * A.Data[2][0]) + (Data[i][3] * A.Data[3][0]);
    Result.Data[i][1] = (Data[i][0] * A.Data[0][1]) + (Data[i][1] * A.Data[1][1]) + (Data[i][2] * A.Data[2][1]) + (Data[i][3] * A.Data[3][1]);
    Result.Data[i][2] = (Data[i][0] * A.Data[0][2]) + (Data[i][1] * A.Data[1][2]) + (Data[i][2] * A.Data[2][2]) + (Data[i][3] * A.Data[3][2]);
    Result.Data[i][3] = (Data[i][0] * A.Data[0][3]) + (Data[i][1] * A.Data[1][3]) + (Data[i][2] * A.Data[2][3]) + (Data[i][3] * A.Data[3][3]);
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

LIBUNR_API float Dot( FVector& A, FVector& B )
{
  float x = A.X * B.X;
  float y = A.Y * B.Y;
  float z = A.Z * B.Z;
  return x + y + z;
}

LIBUNR_API FVector Cross( FVector& A, FVector& B )
{
  float NewX = (A.Y * B.Z) - (A.Z * B.Y);
  float NewY = (A.Z * B.X) - (A.X * B.Z);
  float NewZ = (A.X * B.Y) - (A.Y * B.X);

  return FVector( NewX, NewY, NewZ );
}

LIBUNR_API FVector Normalize( FVector& V )
{
  float X2 = V.X * V.X;
  float Y2 = V.Y * V.Y;
  float Z2 = V.Z * V.Z;
  float Mag = rsqrt( X2 + Y2 + Z2 );
  return FVector( V.X * Mag, V.Y * Mag, V.Z * Mag );
}

// Treat vector as a positional vector and get a translation matrix
void FVector::GetTranslationMatrix( FMatrix4x4& Mat )
{
  memset( &Mat, 0, sizeof( Mat ) );
  Mat.Data[0][0] = 1.0f;
  Mat.Data[1][1] = 1.0f;
  Mat.Data[2][2] = 1.0f;
  Mat.Data[3][0] = X;
  Mat.Data[3][1] = Y;
  Mat.Data[3][2] = Z;
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

LIBUNR_API FVector operator+( FVector A, FVector B )
{
  FVector Y;
  Y.X = A.X + B.X;
  Y.Y = A.Y + B.Y;
  Y.Z = A.Z + B.Z;
  return Y;
}

LIBUNR_API FVector operator-( FVector A, FVector B )
{
  FVector Y;
  Y.X = A.X - B.X;
  Y.Y = A.Y - B.Y;
  Y.Z = A.Z - B.Z;
  return Y;
}

LIBUNR_API FVector operator-( FVector& V )
{
  return FVector( -V.X, -V.Y, -V.Z );
}

LIBUNR_API FVector operator*( FVector A, float B )
{
  return FVector( A.X * B, A.Y * B, A.Z * B );
}

FPlane::FPlane()
{
  memset( this, 0, sizeof( FPlane ) );
}

FPlane::FPlane( FVector V )
  : FVector( V )
{
  W = 1.0;
}

int FPlane::FindBoxSide( FBox& Box )
{
  int Side = 0;
  float Dist[2];
  FVector Corners[2];

  // Get true min/max corners in case they're backwards
  for ( int i = 0; i < 3; i++ )
  {
    if ( V[i] < 0.0f )
    {
      Corners[0].V[i] = Box.Min.V[i];
      Corners[1].V[i] = Box.Max.V[i];
    }
    else
    {
      Corners[1].V[i] = Box.Min.V[i];
      Corners[0].V[i] = Box.Max.V[i];
    }
  }

  // Get point orientations
  Dist[0] = Dot( Corners[0], *this );
  Dist[1] = Dot( Corners[1], *this );

  if ( Dist[0] >= 0.0f )
    Side = ORIENT_FRONT;
  if ( Dist[1] < 0.0f )
    Side |= ORIENT_BACK;

  return Side;
}

LIBUNR_API float Dot( FVector& V, FPlane& P )
{
  return ((V.X * P.X) + (V.Y * P.Y) + (V.Z * P.Z)) - P.W;
}

FVector FRotator::GetDegrees()
{
  // Convert rotator coordinates to degrees
  // 16384 rotation units = 90 degree turn, 16384 / 90 = 182.0444444 rotation units per degree
  #define UU_ROT_TO_DEG(angle) (((double)angle * 90.0) / 16384.0)
  return FVector( UU_ROT_TO_DEG( Pitch ), UU_ROT_TO_DEG( Yaw ), UU_ROT_TO_DEG( Roll ) );
}

FVector FRotator::GetRadians()
{
  // Convert rotator coordinates to radians
  // 16384 rotation units = 90 degree turn, 16384 / 90 = 182.0444444 rotation units per degree
  // Therefore, (Rot * 90) / 16384 = Rot in degrees -> Degrees to radians = (angle) * (PI/180)
  // Simplify -> 90/180 = 1/2 -> ((Rot*PI)/16384) * (1/2) -> (Rot*PI) / 32768
  // Use multiplication for speed, so 1/32768 = 0.000030517578125

  #define UU_ROT_TO_RAD(angle) (((double)angle*PI) * 0.000030517578125)

  return FVector( UU_ROT_TO_RAD( Pitch ), UU_ROT_TO_RAD( Yaw ), UU_ROT_TO_RAD( Roll ) );
}

void FRotator::GetMatrix( FMatrix4x4& Out )
{
  FMatrix4x4 Tmp;

  FVector Rads = GetRadians();

  // See glm::eulerAngleYZX for original implementation
  // Here, pitch and yaw were negated to match expected rotation direction compared to UE1
  float c1 = cos( Rads.Y );
  float s1 = sin( Rads.Y );
  float c2 = cos( Rads.Z );
  float s2 = sin( Rads.Z );
  float c3 = cos( -Rads.X );
  float s3 = sin( -Rads.X );

  Out.Data[0][0] = c1 * c2;
  Out.Data[0][1] = s2;
  Out.Data[0][2] = -c2 * s1;
  Out.Data[0][3] = 0;
  Out.Data[1][0] = s1 * s3 - c1 * c3 * s2;
  Out.Data[1][1] = c2 * c3;
  Out.Data[1][2] = c1 * s3 + c3 * s1 * s2;
  Out.Data[1][3] = 0;
  Out.Data[2][0] = c3 * s1 + c1 * s2 * s3;
  Out.Data[2][1] = -c2 * s3;
  Out.Data[2][2] = c1 * c3 - s1 * s2 * s3;
  Out.Data[2][3] = 0;
  Out.Data[3][0] = 0;
  Out.Data[3][1] = 0;
  Out.Data[3][2] = 0;
  Out.Data[3][3] = 1;
}

void FRotator::GetAxes( FVector& Fwd, FVector& Right, FVector& Up )
{
  // Get rotation in radians
  FVector Rads = GetRadians();
  Rads.X = -Rads.X;
  float cp, sp, cy, sy, cr, sr;

  cp = cos( Rads.X );
  cy = cos( Rads.Y );
  cr = cos( Rads.Z );
  sp = sin( Rads.X );
  sy = sin( Rads.Y );
  sr = sin( Rads.Z );

  Fwd.X = cp * cy;
  Fwd.Y = cp * sy;
  Fwd.Z = -sp;

  Right.X = (-sr * sp * cy) - (cr * -sy);
  Right.Y = (-sr * sp * sy) - (cr * cy);
  Right.Z = -sr * cp;

  Up.X = (cr * sp * cy) - (sr * -sy);
  Up.Y = (cr * sp * sy) - (sr * cy);
  Up.Z = cr * cp;
}
