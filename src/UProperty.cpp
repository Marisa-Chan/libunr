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
 * UProperty.cpp - Objects that describe properties inside of objects
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "UProperty.h"

u8 UProperty::PropertySizes[8] = { 1, 2, 4, 12, 16, 1, 2, 4 };

UProperty::~UProperty()
{
}

UByteProperty::~UByteProperty()
{
}

UIntProperty::~UIntProperty()
{
}

UBoolProperty::~UBoolProperty()
{
}

UFloatProperty::~UFloatProperty()
{
}

UObjectProperty::~UObjectProperty()
{
}

UNameProperty::~UNameProperty()
{
}

UStringProperty::~UStringProperty()
{
}

UClassProperty::~UClassProperty()
{
}

UArrayProperty::~UArrayProperty()
{
}

UStructProperty::~UStructProperty()
{
}

UVectorProperty::~UVectorProperty()
{
}

URotatorProperty::~URotatorProperty()
{
}

UAsciiStrProperty::~UAsciiStrProperty()
{
}

UMapProperty::~UMapProperty()
{
}

UFixedArrayProperty::~UFixedArrayProperty()
{
}
