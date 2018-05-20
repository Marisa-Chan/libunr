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

//#include "FLog.h"
#include "UMusic.h"
#include "UPackage.h"
#include "UProperty.h"
#include "UTexture.h"

UBitmap::UBitmap()
{
  Format = TEXF_P8;
  UBits = 0;
  VBits = 0;
  USize = 0;
  VSize = 0;
  UClamp = 0;
  VClamp = 0;
  InternalTime[0] = 0;
  InternalTime[1] = 0;
}

UBitmap::~UBitmap()
{
}

UTexture::UTexture()
  : UBitmap()
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

void UTexture::LoadFromPackage( FPackageFileIn& Ar )
{
  // Load UProperties
  // TODO: String comparison is really slow, implement faster property lookup
  UObject::ReadProperties( Ar );
  for (u64 i = 0; i < Properties.Size() && i != MAX_UINT64; i++) {
    if (strncmp( Pkg->ResolveNameFromIdx( Properties[i]->Name ), "DetailTexture", NAME_LEN ) == 0) {
      Logf( LOG_WARN, "UTexture->DetailTexture loading not implemented" );
      DetailTexture = NULL;
    }
  }
}

bool UTexture::ExportToFile()
{
  return true;
}
