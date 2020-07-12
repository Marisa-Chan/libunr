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
 * UEditorEngine.h - Editor engine environment
 *
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Engine/UEngine.h"
#include "Engine/ULevel.h"

// What does this even do?
class LIBUNR_API UTransBuffer : public UObject
{
  DECLARE_NATIVE_CLASS( UTransBuffer, UObject, CLASS_NoExport, Editor )
  UTransBuffer() {}
};

/*-----------------------------------------------------------------------------
 * UEditorEngine
 * Engine implementation meant for the editor
-----------------------------------------------------------------------------*/
class LIBUNR_API UEditorEngine : public UEngine
{
  DECLARE_NATIVE_CLASS( UEditorEngine, UEngine, CLASS_Transient, Editor )
  EXPOSE_TO_USCRIPT()

  UEditorEngine();
  virtual bool Init();
  virtual bool Exit();
  virtual void Tick( float DeltaTime );

  ULevel* Level;
  UModel* TempModel;
  UTexture* CurrentTexture;
  UClass* CurrentClass;
  UTransBuffer* Trans; //?
  UTextBuffer* Results;

  UTexture* MenuUp, MenuDn;
  UTexture* CollOn, CollOff;
  UTexture* PlyrOn, PlyrOff;
  UTexture* LiteOn, LiteOff;
  UTexture* BackPrev;

  UTexture* Bad, Bkgnd, BkgndHi;

  bool bFastRebuild, bBootstrapping;

  int AutoSaveIndex;
  int AutoSaveCount, Mode, ClickFlags;
  float MovementSpeed;
  UPackage* PackageContext;
  FVector AddLocation;
  FPlane AddPlane;

  TArray<UObject*>* Tools;
  UClass* BrowseClass;

  bool GridEnabled;
  bool SnapVertices;
  bool AffectRegion;
  bool TextureLock;
  bool SelectionLock;
  float SnapDistance;
  FVector GridSize;

  bool RotGridEnabled;
  FRotator RotGridSize;

  float FovAngleDegrees;
  bool GodMode;
  bool FreeMeshView;
  bool AutoSave;
  int  AutosaveTimeMinutes;
  bool AskSave;
  FString* GameCommandLine;
  TArray<FString*>* EditPackages;
  bool AlwaysPermanentBrush;

  FColor C_WorldBox;
  FColor C_GroundPlane;
  FColor C_GroundHighlight;
  FColor C_BrushWire;
  FColor C_Pivot;
  FColor C_Select;
  FColor C_Current;
  FColor C_AddWire;
  FColor C_SubtractWire;
  FColor C_GreyWire;
  FColor C_BrushVertex;
  FColor C_BrushSnap;
  FColor C_Invalid;
  FColor C_ActorWire;
  FColor C_ActorHiWire;
  FColor C_Black;
  FColor C_White;
  FColor C_Mask;
  FColor C_SemiSolidWire;
  FColor C_NonSolidWire;
  FColor C_WireBackground;
  FColor C_WireGridAxis;
  FColor C_ActorArrow;
  FColor C_ScaleBox;
  FColor C_ScaleBoxHi;
  FColor C_ZoneWire;
  FColor C_Mover;
  FColor C_OrthoBackground;
};
