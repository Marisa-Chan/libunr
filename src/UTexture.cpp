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
 * UTexture.cpp - Native texture object type
 * See the 'Class Texture' in UT-Package-File-Format.pdf
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "UMusic.h"
#include "UPackage.h"
#include "UTexture.h"
#include "FArchiveFile.h"

UTexture::UTexture()
{
  BumpMap = NULL;
  DetailTexture = NULL;
  MacroTexture = NULL;
  
  Diffuse   = 1.f;
  Specular  = 0.f;
  Alpha     = 0.f;
  DrawScale = 1.f;
  Friction  = 0.f;
  MipMult   = 1.f;
  
  // USound* FootstepSound[6]
  // USound* HitSound;
  
  bHighColorQuality = true;
  bHighTextureQuality = true;
  bRealtime = false;
  bParametric = false;
  bRealtimeChanged = false;
  bHasComp = false;
  bFractical = false;
  
  AnimNext = NULL;
  AnimCurrent = NULL;
  PrimeCount = 0;
  PrimeCurrent = 0;
  MinFrameRate = 0.f;
  MaxFrameRate = 0.f;
  Accumulator = 0.f;
  
  CompFormat = TEXF_P8;
}

UTexture::~UTexture()
{
}

void UTexture::LoadFromPackage( FArchive& Ar )
{
  // Load UProperties
  // TODO: String comparison is really slow, implement faster property lookup
  UObject::ReadProperties( Ar );
  for (u64 i = 0; i < Properties.Size() && i != MAX_UINT64; i++) {
  }
}

bool UTexture::ExportToFile()
{
  return true;
}
