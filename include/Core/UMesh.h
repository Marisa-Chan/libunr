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
 * UMesh.h - Mesh classes
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/UPrimitive.h"

class AActor;

class DLL_EXPORT UMesh : public UPrimitive
{
  DECLARE_NATIVE_CLASS( UMesh, UPrimitive, CLASS_NoExport, Engine )

  UMesh();

  bool bCurvyMesh;
  // TODO:
};

class DLL_EXPORT UAnimationNotify : public UObject
{
  DECLARE_NATIVE_CLASS( UAnimationNotify, UObject, 0, Engine )
  EXPOSE_TO_USCRIPT()

  UAnimationNotify();

  struct AnimNotify
  {
    idx AnimName;
    idx FunctionName;
    int KeyFrame;
    EAnimNotifyEval NotifyEval;
    bool bCallOncePerLoop;
    bool bCalculatedFrame;
    bool bAlreadyCalled;
    int NumFrames;
    float CallKey;
  };

  AnimNotify AnimationNotify[255];
  int NumNotifies;
  AActor* Owner;
  bool bInitialized, bErrorOccured;
};

