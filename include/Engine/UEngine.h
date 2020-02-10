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
 * UEngine.h - Base Engine Subsystem
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/USystem.h"
#include "Core/UPrimitive.h"
#include "Engine/UAudio.h"
#include "Engine/URender.h"

class UEngine;

// Input keys.
enum EInputKey
{
  /*00*/  IK_None, IK_LeftMouse, IK_RightMouse, IK_Cancel,
  /*04*/  IK_MiddleMouse, IK_Unknown05, IK_Unknown06, IK_Unknown07,
  /*08*/  IK_Backspace, IK_Tab, IK_Unknown0A, IK_Unknown0B,
  /*0C*/  IK_Unknown0C, IK_Enter, IK_Unknown0E, IK_Unknown0F,
  /*10*/  IK_Shift, IK_Ctrl, IK_Alt, IK_Pause,
  /*14*/  IK_CapsLock, IK_MouseButton4, IK_MouseButton5, IK_MouseButton6,
  /*18*/  IK_MouseButton7, IK_MouseButton8, IK_Unknown1A, IK_Escape,
  /*1C*/  IK_Unknown1C, IK_Unknown1D, IK_Unknown1E, IK_Unknown1F,
  /*20*/  IK_Space, IK_PageUp, IK_PageDown, IK_End,
  /*24*/  IK_Home, IK_Left, IK_Up, IK_Right,
  /*28*/  IK_Down, IK_Select, IK_Print, IK_Execute,
  /*2C*/  IK_PrintScrn, IK_Insert, IK_Delete, IK_Help,
  /*30*/  IK_0, IK_1, IK_2, IK_3,
  /*34*/  IK_4, IK_5, IK_6, IK_7,
  /*38*/  IK_8, IK_9, IK_Unknown3A, IK_Unknown3B,
  /*3C*/  IK_Unknown3C, IK_Unknown3D, IK_Unknown3E, IK_Unknown3F,
  /*40*/  IK_Unknown40, IK_A, IK_B, IK_C,
  /*44*/  IK_D, IK_E, IK_F, IK_G,
  /*48*/  IK_H, IK_I, IK_J, IK_K,
  /*4C*/  IK_L, IK_M, IK_N, IK_O,
  /*50*/  IK_P, IK_Q, IK_R, IK_S,
  /*54*/  IK_T, IK_U, IK_V, IK_W,
  /*58*/  IK_X, IK_Y, IK_Z, IK_Unknown5B,
  /*5C*/  IK_Unknown5C, IK_Unknown5D, IK_Unknown5E, IK_Unknown5F,
  /*60*/  IK_NumPad0, IK_NumPad1, IK_NumPad2, IK_NumPad3,
  /*64*/  IK_NumPad4, IK_NumPad5, IK_NumPad6, IK_NumPad7,
  /*68*/  IK_NumPad8, IK_NumPad9, IK_GreyStar, IK_GreyPlus,
  /*6C*/  IK_Separator, IK_GreyMinus, IK_NumPadPeriod, IK_GreySlash,
  /*70*/  IK_F1, IK_F2, IK_F3, IK_F4,
  /*74*/  IK_F5, IK_F6, IK_F7, IK_F8,
  /*78*/  IK_F9, IK_F10, IK_F11, IK_F12,
  /*7C*/  IK_F13, IK_F14, IK_F15, IK_F16,
  /*80*/  IK_F17, IK_F18, IK_F19, IK_F20,
  /*84*/  IK_F21, IK_F22, IK_F23, IK_F24,
  /*88*/  IK_Unknown88, IK_Unknown89, IK_Unknown8A, IK_Unknown8B,
  /*8C*/  IK_Unknown8C, IK_Unknown8D, IK_Unknown8E, IK_Unknown8F,
  /*90*/  IK_NumLock, IK_ScrollLock, IK_Unknown92, IK_Unknown93,
  /*94*/  IK_Unknown94, IK_Unknown95, IK_Unknown96, IK_Unknown97,
  /*98*/  IK_Unknown98, IK_Unknown99, IK_Unknown9A, IK_Unknown9B,
  /*9C*/  IK_Unknown9C, IK_Unknown9D, IK_Unknown9E, IK_Unknown9F,
  /*A0*/  IK_LShift, IK_RShift, IK_LControl, IK_RControl,
  /*A4*/  IK_UnknownA4, IK_UnknownA5, IK_UnknownA6, IK_UnknownA7,
  /*A8*/  IK_UnknownA8, IK_UnknownA9, IK_UnknownAA, IK_UnknownAB,
  /*AC*/  IK_UnknownAC, IK_UnknownAD, IK_UnknownAE, IK_UnknownAF,
  /*B0*/  IK_UnknownB0, IK_UnknownB1, IK_UnknownB2, IK_UnknownB3,
  /*B4*/  IK_UnknownB4, IK_UnknownB5, IK_UnknownB6, IK_UnknownB7,
  /*B8*/  IK_UnknownB8, IK_UnknownB9, IK_Semicolon, IK_Equals,
  /*BC*/  IK_Comma, IK_Minus, IK_Period, IK_Slash,
  /*C0*/  IK_Tilde, IK_UnknownC1, IK_UnknownC2, IK_UnknownC3,
  /*C4*/  IK_UnknownC4, IK_UnknownC5, IK_UnknownC6, IK_UnknownC7,
  /*C8*/  IK_Joy1, IK_Joy2, IK_Joy3, IK_Joy4,
  /*CC*/  IK_Joy5, IK_Joy6, IK_Joy7, IK_Joy8,
  /*D0*/  IK_Joy9, IK_Joy10, IK_Joy11, IK_Joy12,
  /*D4*/  IK_Joy13, IK_Joy14, IK_Joy15, IK_Joy16,
  /*D8*/  IK_UnknownD8, IK_UnknownD9, IK_UnknownDA, IK_LeftBracket,
  /*DC*/  IK_Backslash, IK_RightBracket, IK_SingleQuote, IK_UnknownDF,
  /*E0*/  IK_JoyX, IK_JoyY, IK_JoyZ, IK_JoyR,
  /*E4*/  IK_MouseX, IK_MouseY, IK_MouseZ, IK_MouseW,
  /*E8*/  IK_JoyU, IK_JoyV, IK_UnknownEA, IK_UnknownEB,
  /*EC*/  IK_MouseWheelUp, IK_MouseWheelDown, IK_Unknown10E, IK_Unknown10F,
  /*F0*/  IK_JoyPovUp, IK_JoyPovDown, IK_JoyPovLeft, IK_JoyPovRight,
  /*F4*/  IK_UnknownF4, IK_UnknownF5, IK_Attn, IK_CrSel,
  /*F8*/  IK_ExSel, IK_ErEof, IK_Play, IK_Zoom,
  /*FC*/  IK_NoName, IK_PA1, IK_OEMClear
};

/*-----------------------------------------------------------------------------
 * InputFunc
 * A function type which is called when a particular key is pressed/released
-----------------------------------------------------------------------------*/
typedef void (*InputFunc)(EInputKey Key, float DeltaTime, bool bKeyDown);

/*-----------------------------------------------------------------------------
 * MouseInputFunc
 * A function type which is called when an input device with two axes is moved
-----------------------------------------------------------------------------*/
typedef void (*AxisInputFunc)(float DeltaTime, int DeltaX, int DeltaY);

/*-----------------------------------------------------------------------------

 * UClient
 * An object representing a client which interacts with the engine
-----------------------------------------------------------------------------*/
class LIBUNR_API UClient : public UObject
{
  DECLARE_NATIVE_CLASS( UClient, UObject, CLASS_NoExport, Engine )

  UClient();

  virtual bool Init();
  virtual bool Exit();

  virtual void Tick( float DeltaTime ) {}
  virtual UViewport* OpenViewport( int InWidth = 0, int InHeight = 0 ) { return NULL; }
  virtual bool CloseViewport( UViewport* Viewport ) { return false; }
  virtual void HandleInput( int Key, bool bDown );
  virtual void HandleMouseInput( int XPos, int YPos );
  virtual void BindKeyInput( EInputKey Key, InputFunc Func );
  virtual void BindMouseInput( AxisInputFunc Func );

  UClass* ViewportClass;
  UEngine* Engine;
  UViewport* CurrentViewport;
  TArray<UViewport*> Viewports;

private:
  InputFunc InputFuncs[0xFF];
  AxisInputFunc MouseFunc;
};

/*-----------------------------------------------------------------------------
 * UEngine
 * The base engine class which enables basic elements of game and editor logic
-----------------------------------------------------------------------------*/
class LIBUNR_API UEngine : public USubsystem
{
  DECLARE_NATIVE_CLASS( UEngine, USubsystem, CLASS_Transient, Engine )
  EXPOSE_TO_USCRIPT()

  UEngine();

  virtual bool Init();
  virtual bool Exit();
  virtual void Tick( float DeltaTime );

  UClass* ClientClass;
  UClass* GameRenderDevice;
  UClass* AudioDevice;
  UClass* Console;
  UClass* NetworkDevice;
  UClass* Language;

  UPrimitive*      Cylinder; // but why?
  UClient*         Client;
  URenderDevice*   Render;
  UAudioSubsystem* Audio;

  int TickCycles;
  int GameCycles;
  int ClientCycles;
  int MinDesiredFramerate;

  int CacheSizeMegs;
  bool UseSound;

  float CurrentTickRate;
  float CurrentDeltaTime;

protected:
  UNativeModule* AudioModule;
  UNativeModule* RenderModule;
};

extern LIBUNR_API UEngine* GEngine;
