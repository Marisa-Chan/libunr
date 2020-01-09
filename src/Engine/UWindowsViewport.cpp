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
 * UWindowsViewport.cpp - A viewport implemented in WinAPI
 *
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Engine/UWindowsViewport.h"

UWindowsViewport::UWindowsViewport()
  : UViewport()
{
}

UWindowsViewport::~UWindowsViewport()
{
}

bool UWindowsViewport::Init()
{
  Super::Init();

  // Get hInstance handle
  Handle = GetModuleHandle( NULL );

  // Set up window class
  WndClass.style = CS_HREDRAW | CS_VREDRAW;
  WndClass.lpfnWndProc = StaticWndProc;
  WndClass.cbClsExtra = 0;
  WndClass.cbWndExtra = 0;
  WndClass.hInstance = Handle;
  WndClass.hIcon = NULL; // TODO: Put OpenUE icon here
  WndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
  WndClass.hbrBackground = NULL;
  WndClass.lpszMenuName = NULL;
  WndClass.lpszClassName = "UWindowsViewport";

  WndClassAtom = RegisterClassEx( &WndClass );
  if ( !WndClassAtom )
  {
    GLogf( LOG_CRIT, "Failed to create windows viewport, RegisterClassEx() failed (%d)", GetLastError() );
    return false;
  }

  Window = 
  CreateWindowEx
  (
    0,
    "UWindowsViewport",
    "OpenUE",
    WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    Width,
    Height,
    NULL,
    NULL,
    Handle,
    this
  );

  if ( !Window )
  {
    GLogf( LOG_CRIT, "Failed to create windows viewport, CreateWindowEx() failed (%d)", GetLastError() );
    return false;
  }

  return true;
}

#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( UWindowsViewport );