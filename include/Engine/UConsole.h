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
 * UConsole.h - Console object class
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/UObject.h"

class UTexture;
class UFont;
class UViewport;
class APlayerReplicationInfo;
class ATimeDemo;

class LIBUNR_API UConsole : public UObject
{
  DECLARE_NATIVE_CLASS( UConsole, UObject, 0, Engine )
  EXPOSE_TO_USCRIPT()

  UConsole();

  int vtblOut; // ???

  UViewport* Viewport;
  int HistoryTop, HistoryBot, HistoryCur;
  FString* TypedStr;
  FString* History[16];
  int Scrollback, NumLines, TopLine, TextLines;
  float MsgTime;
  FString* MsgText[64];
  FString* MsgPRINames[64];
  idx MsgType[64];
  APlayerReplicationInfo* MsgPlayer[64];
  float MsgTick[64];
  int BorderSize;
  int ConsoleLines, BorderLines, BorderPixels;
  float ConsolePos, ConsoleDest;
  float FrameX, FrameY;
  UTexture* ConBackground;
  UTexture* Border;
  bool bNoStuff, bTyping, bTimeDemo;
  ATimeDemo* TimeDemo;
  bool bNoDrawWorld;
  int TypingOffset;
  u8 GlobalConsoleKey;
  u8 GlobalWindowKey;

  FString* LoadingMessage;
  FString* SavingMessage;
  FString* ConnectingMessage;
  FString* PausedMessage;
  FString* PrecachingMessage;
  bool bValidKeyEvent;

  // Input system states.
  enum EInputAction
  {
    IST_None,    // Not performing special input processing.
    IST_Press,   // Handling a keypress or button press.
    IST_Hold,    // Handling holding a key or button.
    IST_Release, // Handling a key or button release.
    IST_Axis,    // Handling analog axis movement.
  };

  // UT99 Vars
  float MsgTickTime;
  bool  bStartTimeDemo;
  bool  bRestartTimeDemo;
  bool  bSaveTimeDemoToFile;
  float StartTime;
  float ExtraTime;
  float LastFrameTime;
  float LastSecondStartTime;
  int   FrameCount;
  int   LastSecondFrameCount;
  float MinFPS;
  float MaxFPS;
  float LastSecFPS;
  UFont* TimeDemoFont;
  FString* FrameRateText;
  FString* AvgText;
  FString* LastSecText;
  FString* MinText;
  FString* MaxText;
  FString* FpsText;
  FString* SecondsText;
  FString* FramesText;
};

