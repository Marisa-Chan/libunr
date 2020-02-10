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

#include "Util/FMacro.h"

#if defined LIBUNR_WIN32

#include "Engine/UWindowsClient.h"
#include "Engine/UWindowsViewport.h"
#include <Windowsx.h>

UWindowsViewport::UWindowsViewport()
  : UViewport()
{
}

UWindowsViewport::~UWindowsViewport()
{
}

bool UWindowsViewport::Init( int InWidth, int InHeight )
{
  if ( !GEngine->Render )
    return true;

  Super::Init( InWidth, InHeight );

  Handle = GetModuleHandle( NULL );

  // Set up window class
  memset( &WndClass, 0, sizeof( WndClass ));
  WndClass.cbSize = sizeof( WndClass );
  WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  WndClass.lpfnWndProc = StaticWndProc;
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

  u32 ExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
  u32 Style = WS_OVERLAPPEDWINDOW;

  // Ensure our window is sized for desired resolution
  RECT ClientSize;
  ClientSize.left = 0;
  ClientSize.right = Width;
  ClientSize.top = 0;
  ClientSize.bottom = Height;
  AdjustWindowRectEx( &ClientSize, Style, FALSE, ExStyle );

  // Create window
  Window = 
  CreateWindowEx
  (
    ExStyle,
    "UWindowsViewport",
    "OpenUE",
    Style,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    ClientSize.right-ClientSize.left,
    ClientSize.bottom-ClientSize.top,
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

  DrawContext = GetDC( Window );
  SetForegroundWindow( Window );

  if ( !GEngine->Render->InitViewport( this ) )
  {
    GLogf( LOG_ERR, "Failed to initialize windows viewport with renderer" );
    DestroyWindow( Window );
    return false;
  }

  return true;
}

bool UWindowsViewport::Exit()
{
  return DestroyWindow( Window );
}

void UWindowsViewport::Show()
{
  ShowWindow( Window, SW_SHOW );
}

void UWindowsViewport::Hide()
{
  ShowWindow( Window, SW_HIDE );
}

bool UWindowsViewport::Resize( int NewWidth, int NewHeight )
{
  // TODO
  return false;
}

LRESULT UWindowsViewport::StaticWndProc( HWND Hwnd, UINT Msg, WPARAM WParam, LPARAM LParam )
{
  UWindowsViewport* This = NULL;
  if ( Msg == WM_NCCREATE )
    SetWindowLongPtr( Hwnd, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT*)LParam)->lpCreateParams );
  else
    This = (UWindowsViewport*)GetWindowLongPtr( Hwnd, GWLP_USERDATA );

  switch ( Msg )
  {
  case WM_MOUSEMOVE:
    This->Client->HandleMouseInput( GET_X_LPARAM( LParam ), GET_Y_LPARAM( LParam ) );
    break;
  case WM_KEYDOWN:
    This->Client->HandleInput( WParam, true );
    break;
  case WM_KEYUP:
    This->Client->HandleInput( WParam, false );
    break;
  case WM_LBUTTONDOWN:
    This->Client->HandleInput( VK_LBUTTON, true );
    break;
  case WM_LBUTTONUP:
    This->Client->HandleInput( VK_LBUTTON, false );
    break;
  case WM_RBUTTONDOWN:
    This->Client->HandleInput( VK_RBUTTON, true );
    break;
  case WM_RBUTTONUP:
    This->Client->HandleInput( VK_RBUTTON, false );
    break;
  case WM_MBUTTONDOWN:
    This->Client->HandleInput( VK_MBUTTON, true );
    break;
  case WM_MBUTTONUP:
    This->Client->HandleInput( VK_MBUTTON, false );
    break;
  case WM_XBUTTONDOWN:
    if ( GET_XBUTTON_WPARAM( WParam ) == XBUTTON1 )
      This->Client->HandleInput( VK_XBUTTON1, true );
    else
      This->Client->HandleInput( VK_XBUTTON2, true );
    break;
  case WM_XBUTTONUP:
    if ( GET_XBUTTON_WPARAM( WParam ) == XBUTTON1 )
      This->Client->HandleInput( VK_XBUTTON1, false );
    else
      This->Client->HandleInput( VK_XBUTTON2, false );
    break;
  case WM_DESTROY:
    This->Client->CloseViewport( This );
    if ( This->Client->Viewports.Size() == 0 )
      GSystem->Exit( 0 );
    break;
  default:
    return DefWindowProc( Hwnd, Msg, WParam, LParam );
    break;
  }
  return 0;
}

#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( UWindowsViewport );

#endif //End win32 check
