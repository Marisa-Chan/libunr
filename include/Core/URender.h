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
 * URender.h - Rendering classes
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/USystem.h"

class APlayerPawn;

class DLL_EXPORT URenderIterator : public UObject
{
  DECLARE_NATIVE_CLASS( URenderIterator, UObject, 0, Engine )
  EXPOSE_TO_USCRIPT()

  URenderIterator();

  // Leaving out some structs here until they're actually needed
  // struct ActorBuffer
  // {
  //   u8 Padding[564];
  // };
  //
  // struct ActorNode
  // {
  //   ActorBuffer ActorProxy;
  //   ActorNode*  NextNode;
  // };

  int MaxItems;
  int Index;
  APlayerPawn* Observer;
  void* Frame;
};

class DLL_EXPORT URenderDevice : public USubsystem
{
  DECLARE_NATIVE_CLASS( URenderDevice, USubsystem, CLASS_NoExport, Engine )

  URenderDevice();

  // TODO:
};

class DLL_EXPORT URenderBase : public USubsystem
{
  DECLARE_NATIVE_CLASS( URenderBase, USubsystem, CLASS_NoExport, Engine )

  URenderBase();

  URenderDevice* RenderDevice;
  // TODO:
};

class DLL_EXPORT UStaticLightData : public UObject
{
  DECLARE_NATIVE_CLASS( UStaticLightData, UObject, CLASS_NoExport, Engine )
  UStaticLightData();

  // TODO:
};


