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
 * USound.h - Native sound object type
 * See the 'Class Sound' in UT-Package-File-Format.pdf
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include "UObject.h"

enum ESoundSlot
{
	SLOT_None,
	SLOT_Misc,
	SLOT_Pain,
	SLOT_Interact,
	SLOT_Ambient,
	SLOT_Talk,
	SLOT_Interface,
};

class DLL_EXPORT USound : public UObject
{
  DECLARE_NATIVE_CLASS( USound, UObject, CLASS_NoExport | CLASS_SafeReplace, Engine )
  EXPORTABLE()
  USound();

  virtual void Load();
  
  int SoundFormat;
  u32 OffsetNext;  // PackageVersion >= 63
  idx SoundSize;
  u8* SoundData;
};
