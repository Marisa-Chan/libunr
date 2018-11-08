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
 * UMesh.h - Mesh classes
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "UObject.h"
#include "UMath.h"

class AActor;

class UMesh : public UObject
{
  DECLARE_NATIVE_CLASS( UMesh, UObject, CLASS_NoExport, Engine )

  UMesh();

  // TODO:
};

class UAnimationNotify : public UObject
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


