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
 * ULevel.h - Level class
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Util/FString.h"
#include "Util/TMap.h"
#include "Core/UClass.h"
#include "Engine/UEngine.h"
#include "Engine/UModel.h"
#include "Engine/UNet.h"

struct LIBUNR_API FReachSpec
{
  friend LIBUNR_API FPackageFileIn& operator>>( FPackageFileIn& In, FReachSpec& RS );

  int Distance;
  AActor* Start;
  AActor* End;
  int CollisionRadius;
  int CollisionHeight;
  int ReachFlags;
  u8  bPruned;
};

struct LIBUNR_API FURL
{
  friend LIBUNR_API FPackageFileIn& operator>>( FPackageFileIn& In, FURL& URL );

  FString Protocol;
  FString Host;
  FString Map;
  FString Portal;
  TArray<FString> Op;
  int    Port;
  bool   bValid;
};

class LIBUNR_API ULevelBase : public UObject
{
  DECLARE_NATIVE_CLASS( ULevelBase, UObject, CLASS_Abstract | CLASS_NoExport, Engine )
  ULevelBase();

  TArray<AActor*> Actors;

  virtual void Load();

  UNetDriver* NetDriver;
  UEngine*    Engine;
  FURL        URL;
  UNetDriver* DemoRecDriver; // do we need this?

  bool bDeletedBrushwork;
};

class LIBUNR_API ULevel : public ULevelBase
{
  DECLARE_NATIVE_CLASS( ULevel, ULevelBase, CLASS_NoExport, Engine )
  ULevel();

  virtual void Load();
  virtual bool ExportToFile( const char* Dir, const char* Type );

  TArray<FReachSpec> ReachSpecs;
  UModel* Model;
  UTextBuffer* TextBlocks[16];
  double TimeSeconds;
  TMap<FString,FString> TravelInfo;
};

