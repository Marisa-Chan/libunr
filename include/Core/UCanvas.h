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
 * UCanvas.h - Canvas, draws elements in a 2D manner on the screen
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/UMath.h"
#include "Core/UTexture.h"
#include "Core/UViewport.h"
#include "Actors/AActor.h"

class UCanvas : public UObject
{
  DECLARE_NATIVE_CLASS( UCanvas, UObject, 0, Engine )
  EXPOSE_TO_USCRIPT()

  UCanvas();

  // Modifiable properties
  UFont* Font;
  float  SpaceX, SpaceY;
  float  OrgX,   OrgY;
  float  ClipX,  ClipY;
  float  CurX,   CurY;
  float  Z;
  u8     Style;
  float  CurYL;
  FColor DrawColor;
  bool   bCenter;
  bool   bNoSmooth;
  bool   bUseOffset;
  bool   bUseFlatZ;
  int    SizeX,  SizeY;

  // Stock Fonts
  UFont* SmallFont;
  UFont* MedFont;
  UFont* BigFont;
  UFont* LargeFont;

  // Internal
  UViewport* Viewport;
  int FramePtr;
  int RenderPtr; // this and FramePtr are probably not useful anymore
  FCoords RenderCoords;

  // Functions
  void StrLen( String* Str, float* XL, float* YL );
  void DrawText( String* Text, bool CR = false, int PolyFlags = 0 );
  void DrawTile( UTexture* Tex, float XL, float YL, 
      float U, float V, 
      float UL, float VL, 
      int PolyFlags = 0 );

  void DrawActor( AActor* A, bool WireFrame, bool ClearZ = false );

  void DrawTileClipped( UTexture* Tex, float XL, float YL,
      float U, float V,
      float UL, float VL,
      int PolyFlags = 0 );

  void DrawTextClipped( String* Text, bool bCheckHotKey = false, int PolyFlags = 0 );
  void TextSize( String* Str, float* XL, float* YL ); // same as StrLen?
  void DrawClippedActor( AActor* A, bool WireFrame, int X, int Y, int XB, int YB, bool ClearZ = false );
  void DrawTileStretched( UTexture* Tex, int X1, int Y1, int X2, int Y2 );
  void DrawPortal( int X, int Y, int Width, int Height, AActor* CamActor, 
      FVector& CamLocation, FRotator& CamRotation, int FOV = -1, bool ClearZ = false );

  // 227 functions
  void Draw2DLine( FColor& Color, FVector& Start, FVector& End );
  void Draw3DLine( FColor& Color, FVector& Start, FVector& End );
  FVector WorldToScreen( FVector& WorldPos, float* ZDistance );
  FVector ScreenToWorld( FVector& ScreenPos );
  void DrawPathNetwork( bool bOnlyWalkable, float* MaxDistance );
  void DrawCircle( FColor& Col, int LineFlags, FVector& Position, float Radius );
  void DrawBox( FColor& Col, int LineFlags, FVector& Start, FVector& End );
  FCoords GetCameraCoords();
  void SetTile3DOffset( bool bEnable, FVector* Offset = NULL, FRotator* RotOffset = NULL,
      bool bFlatZ = false, float Scale = 1.0 );
};

