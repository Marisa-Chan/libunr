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
 * AProjector.h - Projects textures onto surfaces
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "AActor.h"

class DLL_EXPORT AProjector : public AActor
{
  DECLARE_NATIVE_CLASS( AProjector, AActor, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AProjector();

  void AttachPrjDecal();
  void DeattachPrjDecal();
  void AttachActor( AActor* Other );
  void DeattachActor( AActor* Other );
  void DeattachAllActors();

  FPlane FrustrumPlanes[6];
  TArray<AActor*> DecalActors;
  TArray<int>     DecalNodes;
  FBox           Box;
  FVector        VisBox[8];
  void*          TexData;

  UTexture* ProjectTexture;
  float MaxDistance, ProjectorScale;
  ERenderStyle ProjectStyle;
  u8 FOV;

  bool bProjectActors;
  bool bProjectBSPBackfaces;
  bool bProjectMeshBackfaces;
  bool bProjectBSP;
  bool bGradualFade;
  bool bUseBetterActorAttach;
  bool bHasAttached;
  bool bProjecting;
};

