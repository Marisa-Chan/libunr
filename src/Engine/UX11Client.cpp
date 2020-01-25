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
 * UX11Client.cpp
 *
 * written by Jesse 'Hyzoran' Kowalik
 *========================================================================
*/
#if defined LIBUNR_POSIX

#include "Engine/UX11Client.h"

UX11Client::UX11Client()
  : UClient()
{
  m_Display = NULL;
}

UX11Client::~UX11Client()
{}

bool UX11Client::Init()
{
  m_Display = XOpenDisplay( NULL );
  if (!m_Display)
  {
    return false;
  }
  m_DefaultDisplay = DefaultScreen( m_Display );
}

bool UX11Client::Exit()
{
  bool Result = true;

  // Close all viewports
  // Close all viewports
  for ( int i = 0; i < Viewports.Size(); i++ )
  {
    UX11Viewport* ViewPort = (UX11Viewport*)Viewports[i];
    Result &= UX11Viewport->Exit();
  }
  
  XCloseDisplay( m_Display );
  return Result;
}

UViewport* UX11Client::OpenViewport( int Width, int Height )
{
  UX11Viewport* NewViewport = new UX11Viewport();
  if ( !NewViewport->Init( Width, Height ) )
  {
    GLogf( LOG_ERR, "Failed to create X11 viewport" );
    delete NewViewport;
    return NULL;
  }

  Viewports.PushBack( NewViewport );
  CurrentViewport = NewViewport;
  return NewViewport;
}

bool UX11Client::CloseViewport( UViewport* Viewport )
{
  bool Result = false;

  for ( int i = 0; i < Viewports.Size(); i++ )
  {
    if ( Viewports[i] == Viewport )
    {
      Result = Viewport->Exit();
      Viewports.Erase( i );
      if ( Viewports.Size() == 0 )
        CurrentViewport = NULL;
      else
        CurrentViewport = Viewports[0];
    }
  }

  return Result;
}

void UX11Client::HandleInput( int Key, bool bDown )
{
}

void UX11Client::Tick( float DeltaTime )
{
  XEvent event;

  while(1) 
  {
     XNextEvent( m_Display, &event);

     // Handle Windows Close Event
     if(event.type==ClientMessage)
        break;
  }
}

#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( UX11Client );

#endif //End POSIX check
