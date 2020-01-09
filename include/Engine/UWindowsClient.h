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
 * UWindowsClient.h - A windows client to interact with the engine
 *
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Engine/UEngine.h"

class UWindowsClient : public UClient
{
  DECLARE_NATIVE_CLASS( UWindowsClient, UClient, CLASS_NoExport | CLASS_Config, WinDrv );

  UWindowsClient();

  virtual bool Init();
  virtual bool Exit();

  virtual UViewport* OpenViewport( int Width, int Height );
  virtual bool CloseViewport( UViewport* Viewport );
  virtual void Tick( float DeltaTime );
};