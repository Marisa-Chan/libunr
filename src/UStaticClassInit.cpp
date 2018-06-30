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

bool InitNativeClasses();
{
  bool Result = true;
  Result &= UObject::StaticInit();
    Result &= UField::StaticInit();
      Result &= UConst::StaticInit();
      Result &= UEnum::StaticInit();
      Result &= UProperty::StaticInit();
        Result &= UByteProperty::StaticInit();
        Result &= UIntProperty::StaticInit();
        Result &= UBoolProperty::StaticInit();
        Result &= UFloatProperty::StaticInit();
        Result &= UObjectProperty::StaticInit();
          Result &= UClassProperty::StaticInit();
        Result &= UNameProperty::StaticInit();
        Result &= UStringProperty::StaticInit();
        Result &= UArrayProperty::StaticInit();
        Result &= UStructProperty::StaticInit();
        Result &= UVectorProperty::StaticInit();
        Result &= URotatorProperty::StaticInit();
        Result &= UAsciiStrProperty::StaticInit();
        Result &= UMapProperty::StaticInit();
        Result &= UFixedArrayProperty::StaticInit();
      Result &= UStruct::StaticInit();
        Result &= UFunction::StaticInit();
        Result &= UState::StaticInit();
          Result &= UClass::StaticInit();
    Result &= UMusic::StaticInit();
    Result &= UTexture::StaticInit();
    Result &= USound::StaticInit();
    Result &= UPackage::StaticInit();
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
      IMPLEMENT_CLASS( UVectorProperty );
      IMPLEMENT_CLASS( URotatorProperty );
      IMPLEMENT_CLASS( UAsciiStrProperty );
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


