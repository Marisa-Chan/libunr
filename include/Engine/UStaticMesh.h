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
 * UStaticMesh.h - 227 Static Mesh Class
 * 
 * written by Adam 'Xaleros' Smith
 * StaticMesh definitions from 227i public headers
 *========================================================================
*/

#pragma once
#include "Engine/UMesh.h"

enum EStaticMeshFlags
{
  SMFL_Masked      = 0x0001,
  SMFL_Translucent = 0x0002,
  SMFL_AlphaBlend  = 0x0004,
  SMFL_Modulated   = 0x0008,
  SMFL_MeshEnvMap  = 0x0010,
  SMFL_Invisible   = 0x0020,
  SMFL_NoCollision = 0x0040,
  SMFL_TwoSided    = 0x0080,
  SMFL_Unlit       = 0x0100
};

class LIBUNR_API UStaticMesh : public UMesh
{
  DECLARE_NATIVE_CLASS( UStaticMesh, UMesh, CLASS_NoExport, Engine )
  UStaticMesh();

  // TODO:
};

