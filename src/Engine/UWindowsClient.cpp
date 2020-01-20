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

#include "Engine/UWindowsViewport.h"
#include "Engine/UWindowsClient.h"

UWindowsClient::UWindowsClient()
  : UClient()
{
  hInstance = NULL;
}

UWindowsClient::~UWindowsClient()
{
}

bool UWindowsClient::Init()
{
  hInstance = GetModuleHandle( NULL );
  return true;
}

bool UWindowsClient::Exit()
{
  bool Result = true;

  // Close all viewports
  for ( int i = 0; i < Viewports.Size(); i++ )
  {
    UWindowsViewport* WindowsViewport = (UWindowsViewport*)Viewports[i];
    Result &= WindowsViewport->Exit();
  }

  return Result;
}

UViewport* UWindowsClient::OpenViewport( int Width, int Height )
{
  UWindowsViewport* NewViewport = new UWindowsViewport();
  if ( !NewViewport->Init( Width, Height ) )
  {
    GLogf( LOG_ERR, "Failed to create windows viewport" );
    delete NewViewport;
    return NULL;
  }

  Viewports.PushBack( NewViewport );
  CurrentViewport = NewViewport;
  return NewViewport;
}

bool UWindowsClient::CloseViewport( UViewport* Viewport )
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

void UWindowsClient::HandleInput( int Key, bool bDown )
{
  // TODO
  // Convert windows input to universal input enum
}

void UWindowsClient::Tick( float DeltaTime )
{
  MSG Msg;

  // Dispatch one incoming message for each viewport
  for ( int i = 0; i < Viewports.Size(); i++ )
  {
    UWindowsViewport* Viewport = (UWindowsViewport*)Viewports[i];
    if ( GetMessage( &Msg, Viewport->Window, 0, 0 ) )
    {
      TranslateMessage( &Msg );
      DispatchMessage( &Msg );
    }
  }
}

#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( UWindowsClient );