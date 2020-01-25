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
 * UX11Client.h - X11 Client Implementation.
 *
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/

#pragma once

#include <X11/Xlib.h>

#include "Engine/UEngine.h"

class LIBUNR_API UX11Client : public UClient
{
  DECLARE_NATIVE_CLASS(  UX11Client, UClient, CLASS_NoExport | CLASS_Config, XDrv )

  UX11Client();

  virtual bool Init();
  virtual bool Exit();

  virtual UViewport* OpenViewport( int InWidth = 0, int InHeight = 0 );
  virtual bool CloseViewport( UViewport* Viewport );
  virtual void HandleInput( int Key, bool bDown );
  virtual void Tick( float DeltaTime );
  
  Display* m_Display;
  int m_DefaultScreen;
};
