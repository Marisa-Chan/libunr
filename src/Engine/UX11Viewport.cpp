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
 * UX11Viewport.cpp
 *
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/

#include "Util/FMacro.h"

#if defined LIBUNR_POSIX

#include "Core/UClass.h"
#include "Core/UPackage.h"

#include "Engine/UX11Client.h"
#include "Engine/UX11Viewport.h"

UX11Viewport::UX11Viewport()
  : UViewport()
{
}

UX11Viewport::~UX11Viewport()
{}

bool UX11Viewport::Init( int InWidth, int InHeight )
{
  if ( !GEngine->Render )
    return true;

  Super::Init( InWidth, InHeight );
  
  m_Display = ((UX11Client*)Client)->m_Display;
  auto screen = ((UX11Client*)Client)->m_DefaultScreen;

  m_Window = XCreateSimpleWindow( m_Display, RootWindow( m_Display, screen ), 0, 0, Width, Height, 1, BlackPixel( m_Display, screen ), WhitePixel( m_Display, screen ) );

  if ( !GEngine->Render->InitViewport( this ) )
  {
    GLogf( LOG_ERR, "Failed to initialize X11 viewport with renderer" );
    Exit();
    return false;
  }
  
  return true;
}

bool UX11Viewport::Exit()
{
  XDestroyWindow( m_Display, m_Window );
  return true;
}

void UX11Viewport::Show()
{
   XMapWindow( m_Display, m_Window );
}

void UX11Viewport::Hide()
{
   XUnmapWindow( m_Display, m_Window );
}

bool UX11Viewport::Resize( int NewWidth, int NewHeight )
{
  XMoveResizeWindow( m_Display, m_Window, 0, 0, NewWidth, NewHeight );
}

IMPLEMENT_NATIVE_CLASS( UX11Viewport );

#endif //End POSIX check
