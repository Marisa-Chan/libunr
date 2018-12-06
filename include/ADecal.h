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
 * ADecal.h - Decals for projectiles, gibs, etc
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "AActor.h"

class ADecal : public AActor
{
  DECLARE_NATIVE_CLASS( ADecal, AActor, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ADecal();

  int  MultiDecalLevel;
  bool bAttachPanningSurfs;
  bool bAttachUnlitSurfs;
  Array<int>* SurfList;

  UTexture* AttachDecal( float TraceDistance, FVector* DecalDir = NULL );
  void DetachDecal();
};

