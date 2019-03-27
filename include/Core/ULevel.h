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
 * ULevel.h - Level class
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include <libxstl/XMap.h>
#include "Core/UClass.h"
#include "Core/UEngine.h"
#include "Core/UModel.h"
#include "Core/UNet.h"

struct DLL_EXPORT FReachSpec
{
  friend FPackageFileIn& operator>>( FPackageFileIn& In, FReachSpec& RS );

  int Distance;
  AActor* Start;
  AActor* End;
  int CollisionRadius;
  int CollisionHeight;
  int ReachFlags;
  u8  bPruned;
};

struct DLL_EXPORT FURL
{
  friend FPackageFileIn& operator>>( FPackageFileIn& In, FURL& URL );

  FString Protocol;
  FString Host;
  FString Map;
  FString Portal;
  Array<FString> Op;
  int    Port;
  bool   bValid;
};

class ULevelBase : public UObject
{
  DECLARE_NATIVE_CLASS( ULevelBase, UObject, CLASS_Abstract | CLASS_NoExport, Engine )
  ULevelBase();

  Array<AActor*> Actors;

  virtual void Load();

  UNetDriver* NetDriver;
  UEngine*    Engine;
  FURL        URL;
  UNetDriver* DemoRecDriver; // do we need this?
};

class ULevel : public ULevelBase
{
  DECLARE_NATIVE_CLASS( ULevel, ULevelBase, CLASS_NoExport, Engine )
  EXPORTABLE()
  ULevel();

  virtual void Load();

  Array<FReachSpec> ReachSpecs;
  UModel* Model;
  UTextBuffer* TextBlocks[16];
  double TimeSeconds;
  Map<FString,FString> TravelInfo;
};

