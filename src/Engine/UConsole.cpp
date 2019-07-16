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
#include "Engine/UConsole.h"

UConsole::UConsole()
  : UObject()
{
}

UConsole::~UConsole()
{
}

IMPLEMENT_NATIVE_CLASS( UConsole );

BEGIN_PROPERTY_LINK( UConsole, 63 )
  LINK_NATIVE_PROPERTY( vtblOut );
  LINK_NATIVE_PROPERTY( Viewport );
  LINK_NATIVE_PROPERTY( HistoryTop );
  LINK_NATIVE_PROPERTY( HistoryBot );
  LINK_NATIVE_PROPERTY( HistoryCur );
  LINK_NATIVE_PROPERTY( TypedStr );
  LINK_NATIVE_ARRAY   ( History );
  LINK_NATIVE_PROPERTY( Scrollback );
  LINK_NATIVE_PROPERTY( NumLines );
  LINK_NATIVE_PROPERTY( TopLine );
  LINK_NATIVE_PROPERTY( TextLines );
  LINK_NATIVE_PROPERTY( MsgTime );
  LINK_NATIVE_ARRAY   ( MsgText );
  LINK_NATIVE_ARRAY   ( MsgPRINames );
  LINK_NATIVE_ARRAY   ( MsgType );
  LINK_NATIVE_ARRAY   ( MsgPlayer );
  LINK_NATIVE_ARRAY   ( MsgTick );
  LINK_NATIVE_PROPERTY( BorderSize );
  LINK_NATIVE_PROPERTY( ConsoleLines );
  LINK_NATIVE_PROPERTY( BorderLines );
  LINK_NATIVE_PROPERTY( BorderPixels );
  LINK_NATIVE_PROPERTY( ConsolePos );
  LINK_NATIVE_PROPERTY( ConsoleDest );
  LINK_NATIVE_PROPERTY( FrameX );
  LINK_NATIVE_PROPERTY( FrameY );
  LINK_NATIVE_PROPERTY( ConBackground );
  LINK_NATIVE_PROPERTY( Border );
  LINK_NATIVE_PROPERTY( bNoStuff );
  LINK_NATIVE_PROPERTY( bTyping );
  LINK_NATIVE_PROPERTY( bTimeDemo );
  LINK_NATIVE_PROPERTY( TimeDemo );
  LINK_NATIVE_PROPERTY( bNoDrawWorld );
  LINK_NATIVE_PROPERTY( TypingOffset );
  LINK_NATIVE_PROPERTY( GlobalConsoleKey );
  LINK_NATIVE_PROPERTY( GlobalWindowKey );
  LINK_NATIVE_PROPERTY( LoadingMessage );
  LINK_NATIVE_PROPERTY( SavingMessage );
  LINK_NATIVE_PROPERTY( ConnectingMessage );
  LINK_NATIVE_PROPERTY( PausedMessage );
  LINK_NATIVE_PROPERTY( PrecachingMessage );
  LINK_NATIVE_PROPERTY( bValidKeyEvent );
  LINK_NATIVE_PROPERTY( MsgTickTime );
  LINK_NATIVE_PROPERTY( bStartTimeDemo );
  LINK_NATIVE_PROPERTY( bRestartTimeDemo );
  LINK_NATIVE_PROPERTY( bSaveTimeDemoToFile );
  LINK_NATIVE_PROPERTY( StartTime );
  LINK_NATIVE_PROPERTY( ExtraTime );
  LINK_NATIVE_PROPERTY( LastFrameTime );
  LINK_NATIVE_PROPERTY( LastSecondStartTime );
  LINK_NATIVE_PROPERTY( FrameCount );
  LINK_NATIVE_PROPERTY( LastSecondFrameCount );
  LINK_NATIVE_PROPERTY( MinFPS );
  LINK_NATIVE_PROPERTY( MaxFPS );
  LINK_NATIVE_PROPERTY( LastSecFPS );
  LINK_NATIVE_PROPERTY( TimeDemoFont );
  LINK_NATIVE_PROPERTY( FrameRateText );
  LINK_NATIVE_PROPERTY( AvgText );
  LINK_NATIVE_PROPERTY( LastSecText );
  LINK_NATIVE_PROPERTY( MinText );
  LINK_NATIVE_PROPERTY( MaxText );
  LINK_NATIVE_PROPERTY( FpsText );
  LINK_NATIVE_PROPERTY( SecondsText );
  LINK_NATIVE_PROPERTY( FramesText );
END_PROPERTY_LINK()

