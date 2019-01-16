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
 * ULevel.h - Level class
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/UClass.h"
#include "Core/UEngine.h"
#include "Core/UModel.h"
#include "Core/UNet.h"

class DLL_EXPORT FReachSpec
{
public:
  FReachSpec();

  friend FPackageFileIn& operator>>( FPackageFileIn& Ar, FReachSpec& ReachSpec );
  friend FPackageFileOut& operator<<( FPackageFileOut& Ar, FReachSpec& ReachSpec );

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
  FURL();

  String Protocol;
  String Host;
  String Map;
  String Portal;
  Array<String> Op;
  int    Port;
  bool   bValid;

  friend FPackageFileIn& operator>>( FPackageFileIn& Ar, FURL& URL );
};

class ULevelBase : public UObject
{
  DECLARE_NATIVE_ABSTRACT_CLASS( ULevelBase, UObject, CLASS_NoExport, Engine )
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
  Map<String,String> TravelInfo;
};

