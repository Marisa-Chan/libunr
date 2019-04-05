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
 * ADecal.h - Decals for projectiles, gibs, etc
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Actors/AActor.h"

class DLL_EXPORT ADecal : public AActor
{
  DECLARE_NATIVE_CLASS( ADecal, AActor, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ADecal();

  int  MultiDecalLevel;
  bool bAttachPanningSurfs;
  bool bAttachUnlitSurfs;
  bool bBloodyDecal;
  Array<int>* SurfList;
  float LastRenderedTime;

  UTexture* AttachDecal( float TraceDistance, FVector* DecalDir = NULL );
  void DetachDecal();
};

