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
 * UWindowsViewport.h - A viewport implemented in WinAPI
 *
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include <Windows.h>
#include "Engine/UViewport.h"

class LIBUNR_API UWindowsViewport : public UViewport
{
  DECLARE_NATIVE_CLASS( UWindowsViewport, UViewport, CLASS_NoExport, WinDrv )
  UWindowsViewport();

  virtual bool Init();
  virtual bool Exit() { return true; }
  virtual void Show() {}
  virtual void Hide() {}
  virtual bool Resize( int NewWidth, int NewHeight ) { return true; }

  // Viewport properties
  HINSTANCE Handle;
  HWND Window;
  HDC  DrawContext;
  WNDCLASSEX WndClass;
  ATOM WndClassAtom;

protected:
  static LRESULT StaticWndProc( HWND Hwnd, UINT Msg, WPARAM WParam, LPARAM LParam ) { return 0; }
};
