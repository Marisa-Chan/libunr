/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU Affero General Public License as           *|
|*  published by the Free Software Foundation, either version 3 of the       *|
|*  License, or (at your option) any later version.                          *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU Affero General Public License for more details.                      *|
|*                                                                           *|
|*  You should have received a copy of the GNU Affero General Public License *|
|*  along with this program.  If not, see <https://www.gnu.org/licenses/>.   *|
\*===========================================================================*/

/*========================================================================
 * UConsole.cpp - Console functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Core/UPackage.h"
#include "Core/UConsole.h"

UConsole::UConsole()
  : UObject()
{
}

UConsole::~UConsole()
{
}

IMPLEMENT_NATIVE_CLASS( UConsole );

BEGIN_PROPERTY_LINK( UConsole, 41 )
  LINK_NATIVE_PROPERTY( UConsole, vtblOut );
  LINK_NATIVE_PROPERTY( UConsole, Viewport );
  LINK_NATIVE_PROPERTY( UConsole, HistoryTop );
  LINK_NATIVE_PROPERTY( UConsole, HistoryBot );
  LINK_NATIVE_PROPERTY( UConsole, HistoryCur );
  LINK_NATIVE_PROPERTY( UConsole, TypedStr );
  LINK_NATIVE_ARRAY   ( UConsole, History );
  LINK_NATIVE_PROPERTY( UConsole, Scrollback );
  LINK_NATIVE_PROPERTY( UConsole, NumLines );
  LINK_NATIVE_PROPERTY( UConsole, TopLine );
  LINK_NATIVE_PROPERTY( UConsole, TextLines );
  LINK_NATIVE_PROPERTY( UConsole, MsgTime );
  LINK_NATIVE_ARRAY   ( UConsole, MsgText );
  LINK_NATIVE_ARRAY   ( UConsole, MsgPRINames );
  LINK_NATIVE_ARRAY   ( UConsole, MsgType );
  LINK_NATIVE_ARRAY   ( UConsole, MsgPlayer );
  LINK_NATIVE_ARRAY   ( UConsole, MsgTick );
  LINK_NATIVE_PROPERTY( UConsole, BorderSize );
  LINK_NATIVE_PROPERTY( UConsole, ConsoleLines );
  LINK_NATIVE_PROPERTY( UConsole, BorderLines );
  LINK_NATIVE_PROPERTY( UConsole, BorderPixels );
  LINK_NATIVE_PROPERTY( UConsole, ConsolePos );
  LINK_NATIVE_PROPERTY( UConsole, ConsoleDest );
  LINK_NATIVE_PROPERTY( UConsole, FrameX );
  LINK_NATIVE_PROPERTY( UConsole, FrameY );
  LINK_NATIVE_PROPERTY( UConsole, ConBackground );
  LINK_NATIVE_PROPERTY( UConsole, Border );
  LINK_NATIVE_PROPERTY( UConsole, bNoStuff );
  LINK_NATIVE_PROPERTY( UConsole, bTyping );
  LINK_NATIVE_PROPERTY( UConsole, bTimeDemo );
  LINK_NATIVE_PROPERTY( UConsole, TimeDemo );
  LINK_NATIVE_PROPERTY( UConsole, bNoDrawWorld );
  LINK_NATIVE_PROPERTY( UConsole, TypingOffset );
  LINK_NATIVE_PROPERTY( UConsole, GlobalConsoleKey );
  LINK_NATIVE_PROPERTY( UConsole, GlobalWindowKey );
  LINK_NATIVE_PROPERTY( UConsole, LoadingMessage );
  LINK_NATIVE_PROPERTY( UConsole, SavingMessage );
  LINK_NATIVE_PROPERTY( UConsole, ConnectingMessage );
  LINK_NATIVE_PROPERTY( UConsole, PausedMessage );
  LINK_NATIVE_PROPERTY( UConsole, PrecachingMessage );
  LINK_NATIVE_PROPERTY( UConsole, bValidKeyEvent );
END_PROPERTY_LINK()

