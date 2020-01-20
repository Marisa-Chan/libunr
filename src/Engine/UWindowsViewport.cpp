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

#include "Engine/UWindowsClient.h"
#include "Engine/UWindowsViewport.h"

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

  DrawContext = GetDC( Window );

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
  case WM_KEYDOWN:
    This->Client->HandleInput( WParam, true );
    break;
  case WM_KEYUP:
    This->Client->HandleInput( WParam, false );
    break;
  case WM_DESTROY:
    DestroyWindow( Hwnd );
    break;
  case WM_PAINT:
    ValidateRect( Hwnd, NULL );
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