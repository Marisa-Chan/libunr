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
#if defined LIBUNR_POSIX

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
  
  m_Window = XCreateSimpleWindow( m_Display, RootWindow( m_Display, m_DefaultScreen ), 0, 0, InWidth, InHeight, 1, BlackPixel( m_Display, m_Screen ), WhitePixel( m_Display, m_Screen ) );
  
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

bool UX11Viewport::Show()
{
   XMapWindow( m_Display, m_Window );
}

bool UX11Viewport::Hide()
{
   XUnmapWindow( m_Display, m_Window );
}

bool UX11Viewport::Resize( int NewWidth, int NewHeight )
{
  XMoveResizeWindow( m_Display, m_Window, 0, 0, InWidth, InHeight );
}

#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( UX11Viewport );

#endif //End POSIX check
