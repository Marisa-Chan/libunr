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
 * UViewport.cpp - Base viewport code
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Util/FConfig.h"
#include "Engine/UViewport.h"

UViewport::UViewport()
  : UPlayer()
{
  Width = -1;
  Height = -1;
  BitsPerPixel = 0;
}

UViewport::~UViewport()
{
}

bool UViewport::Init()
{
  // Read config settings
  Width = GLibunrConfig->ReadInt32( "Engine.Viewport", "Width", 0, 1024 ); // TODO: Default to native res
  Height = GLibunrConfig->ReadInt32( "Engine.Viewport", "Height", 0, 768 );
  BitsPerPixel = GLibunrConfig->ReadUInt8( "Engine.Viewport", "BitsPerPixel", 0, 24 ); // Usually ignored
  return true;
}

bool UViewport::Exit()
{
  return false;
}

void UViewport::Show()
{

}

void UViewport::Hide()
{

}

#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( UViewport );

