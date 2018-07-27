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
 * UStaticClassInit.cpp - Initializes the object class for all native objects
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "UObject.h"
#include "UClass.h"
#include "UMusic.h"
#include "UProperty.h"
#include "USound.h"
#include "UTexture.h"

#define INIT_CLASS(cls) cls::StaticInitializeClass();

bool InitNativeClasses()
{
  bool Result = true;
  Result &= UObject::StaticClassInit();
    Result &= UField::StaticClassInit();
      Result &= UConst::StaticClassInit();
      Result &= UEnum::StaticClassInit();
      Result &= UProperty::StaticClassInit();
        Result &= UByteProperty::StaticClassInit();
        Result &= UIntProperty::StaticClassInit();
        Result &= UBoolProperty::StaticClassInit();
        Result &= UFloatProperty::StaticClassInit();
        Result &= UObjectProperty::StaticClassInit();
          Result &= UClassProperty::StaticClassInit();
        Result &= UNameProperty::StaticClassInit();
        Result &= UStringProperty::StaticClassInit();
        Result &= UArrayProperty::StaticClassInit();
        Result &= UStructProperty::StaticClassInit();
        Result &= UStrProperty::StaticClassInit();
        Result &= UMapProperty::StaticClassInit();
        Result &= UFixedArrayProperty::StaticClassInit();
      Result &= UStruct::StaticClassInit();
        Result &= UFunction::StaticClassInit();
        Result &= UState::StaticClassInit();
          Result &= UClass::StaticClassInit();
    Result &= UMusic::StaticClassInit();
    Result &= UTexture::StaticClassInit();
    Result &= USound::StaticClassInit();
    Result &= UPackage::StaticClassInit();
  return Result;
}

IMPLEMENT_CLASS( UObject );
  IMPLEMENT_CLASS( UField );
    IMPLEMENT_CLASS( UConst );
    IMPLEMENT_CLASS( UEnum );
    IMPLEMENT_CLASS( UProperty );
      IMPLEMENT_CLASS( UByteProperty );
      IMPLEMENT_CLASS( UIntProperty );
      IMPLEMENT_CLASS( UBoolProperty );
      IMPLEMENT_CLASS( UFloatProperty );
      IMPLEMENT_CLASS( UObjectProperty );
        IMPLEMENT_CLASS( UClassProperty );
      IMPLEMENT_CLASS( UNameProperty );
      IMPLEMENT_CLASS( UStringProperty );
      IMPLEMENT_CLASS( UArrayProperty );
      IMPLEMENT_CLASS( UStructProperty );
      IMPLEMENT_CLASS( UStrProperty );
      IMPLEMENT_CLASS( UMapProperty );
      IMPLEMENT_CLASS( UFixedArrayProperty );
    IMPLEMENT_CLASS( UStruct );
      IMPLEMENT_CLASS( UFunction );
      IMPLEMENT_CLASS( UState );
        IMPLEMENT_CLASS( UClass );
  IMPLEMENT_CLASS( UMusic );
  IMPLEMENT_CLASS( UTexture );
  IMPLEMENT_CLASS( USound );
  IMPLEMENT_CLASS( UPackage );


