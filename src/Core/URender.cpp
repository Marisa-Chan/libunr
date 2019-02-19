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
 * URender.cpp - Rendering Functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Core/UPackage.h"
#include "Core/URender.h"
//#include "APlayerPawn.h"

URenderIterator::URenderIterator()
  : UObject()
{
  MaxItems = -1;
  Index = -1;
  Observer = NULL;
  Frame = NULL;
}

URenderIterator::~URenderIterator()
{
  //if ( Observer )
  //  Observer->DelRef();
}

URenderDevice::URenderDevice()
  : USubsystem()
{
}

URenderDevice::~URenderDevice()
{
}

URenderBase::URenderBase()
  : USubsystem()
{
  RenderDevice = NULL;
}

URenderBase::~URenderBase()
{
  if ( RenderDevice )
    delete RenderDevice;
}

UStaticLightData::UStaticLightData()
  : UObject()
{
}

UStaticLightData::~UStaticLightData()
{
}

IMPLEMENT_NATIVE_CLASS( URenderBase );
IMPLEMENT_NATIVE_CLASS( URenderDevice );
IMPLEMENT_NATIVE_CLASS( URenderIterator );
IMPLEMENT_NATIVE_CLASS( UStaticLightData );

BEGIN_PROPERTY_LINK( URenderIterator, 4 )
  LINK_NATIVE_PROPERTY( URenderIterator, MaxItems );
  LINK_NATIVE_PROPERTY( URenderIterator, Index );
  LINK_NATIVE_PROPERTY( URenderIterator, Observer );
  LINK_NATIVE_PROPERTY( URenderIterator, Frame );
END_PROPERTY_LINK()

