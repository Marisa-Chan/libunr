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
 * libunr.h - Master libunr header file, do not use internally
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once 

#include "Util/FMacro.h"
#include "Util/FTypes.h"
#include "Util/FArchive.h"
#include "Util/FFileArchive.h"
#include "Util/FLogFile.h"
#include "Util/FConfig.h"
#include "Util/FString.h"
#include "Util/TArray.h"
#include "Util/TMap.h"
#include "Util/TQueue.h"
#include "Util/TStack.h"

#include "Core/UClass.h"
#include "Core/ULocale.h"
#include "Core/UMath.h"
#include "Core/UObject.h"
#include "Core/UPackage.h"
#include "Core/UPrimitive.h"
#include "Core/UProperty.h"
#include "Core/UScript.h"
#include "Core/USystem.h"

#include "Engine/UAudio.h"
#include "Engine/UCanvas.h"
#include "Engine/UConsole.h"
#include "Engine/UEngine.h"
#include "Engine/ULevel.h"
#include "Engine/UMesh.h"
#include "Engine/UModel.h"
#include "Engine/UMusic.h"
#include "Engine/UNet.h"
#include "Engine/UPlayer.h"
#include "Engine/URender.h"
#include "Engine/USkelAnim.h"
#include "Engine/USound.h"
#include "Engine/UTexture.h"
#include "Engine/UViewport.h"

#include "Actors/AActor.h"
#include "Actors/ABrush.h"
#include "Actors/ADecal.h"
#include "Actors/ADynamicZoneInfo.h"
#include "Actors/AGameInfo.h"
#include "Actors/AHUD.h"
#include "Actors/AInventory.h"
#include "Actors/ANavigationPoint.h"
#include "Actors/APawn.h"
#include "Actors/AProjector.h"
#include "Actors/AReplicationInfo.h"
#include "Actors/ASkyZoneInfo.h"
#include "Actors/AStatLog.h"
#include "Actors/AWeapon.h"
#include "Actors/AZoneInfo.h"

