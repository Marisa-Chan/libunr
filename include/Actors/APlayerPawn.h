/*========================================================================*\
|*  This file is part of libunr.                                          *|
|*                                                                        *|
|*  libunr is free software: you can redistribute it and/or modify        *|
|*  it under the terms of the GNU General Public License as published by  *|
|*  the Free Software Foundation, either version 3 of the License, or     *|
|*  (at your option) any later version.                                   *|
|*                                                                        *|
|*  libunr is distributed in the hope that it will be useful,             *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of        *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *|
|*  GNU General Public License for more details.                          *|
|*                                                                        *|
|*  You should have received a copy of the GNU General Public License     *|
|*  along with libunr.  If not, see <http://www.gnu.org/licenses/>.       *|
|*                                                                        *|
\*========================================================================*/

/*========================================================================
 * APlayerPawn.h - Base player pawn class
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Actors/APawn.h"
#include "Core/UPlayer.h"

class DLL_EXPORT APlayerPawn : public APawn
{
  DECLARE_NATIVE_CLASS( APlayerPawn, APawn, 0, Engine )
  EXPOSE_TO_USCRIPT()

  APlayerPawn();

  UPlayer* Player;
  String*  Password;

  float DodgeClickTimer;
  float DodgeClickTime;
  float Bob;
  float BobTime;

  int ShowFlags;
  int RendMap;
  int Misc1;
  int Misc2;

  AActor* ViewTarget;
  FVector FlashScale;
  FVector FlashFog;

  class AHUD* myHUD;
  class AScoreBoard* Scoring;
  UClass* HUDType;
  UClass* ScoringType;

  float   DesiredFlashScale, ConstantGlowScale, InstantFlash;
  FVector DesiredFlashFog,   ConstantGlowFog,   InstantFog;
  float DesiredFOV;
  float DefaultFOV;

  UMusic* Song;
  u8      SongSection;
  u8      CdTrack;
  EMusicTransition Transition;

  float   ShakeTimer;
  float   ShakeMag;
  float   ShakeVert;
  float   MaxShake;
  float   VertTimer;
  UClass* CarcassType;
  float   MyAutoAim;
  float   Handedness;
  USound* JumpSound;

  bool bAdmin;
  bool bLookUpStairs;
  bool bSnapToLevel;
  bool bAlwaysMouseLook;
  bool bKeyboardLook;
  bool bWasForward;
  bool bWasBack;
  bool bWasLeft;
  bool bWasRight;
  bool bEdgeForward;
  bool bEdgeBack;
  bool bEdgeLeft;
  bool bEdgeRight;
  bool bIsCrouching;
  bool bShakeDir;
  bool bAnimTransition;
  bool bIsTurning;
  bool bFrozen;
  bool bInvertMouse;
  bool bShowScores;
  bool bShowMenu;
  bool bSpecialMenu;
  bool bWokeUp;
  bool bPressedJump;
  bool bUpdatePosition;
  bool bDelayedCommand;
  bool bRising;
  bool bReducedVis;
  bool bCenterView;
  bool bMaxMouseSmoothing;
  bool bMouseZeroed;
  bool bReadyToPlay;
  bool bNoFlash;
  bool bNoVoices;
  bool bMessageBeep;
  bool bZooming;
  bool bSinglePlayer;
  bool bJustFired;
  bool bJustAltFired;
  bool bIsTyping;
  bool bFixedCamera;
  float MainFOV;
  bool bConsoleCommandMessage;
  float ZoomLevel;

  UClass* SpecialMenu;
  String* DelayedCommand;
  float   MouseSensitivity;

  idx WeaponPriority[30];

  int NetSpeed, LanSpeed;
  float SmoothMouseX, SmoothMouseY, KbdAccel;
  bool bMouseSmoothing;
  float MouseSmoothThreshold;

  bool bNeverAutoSwitch;
  bool bIgnoreMusicChange;
  PointRegion CameraRegion;
  float CrouchHeightPct;
  bool bIsReducedCrouch;
  float CrouchCheckTime;
  bool bDistanceFogEnabled, bBlendingDistanceFog;
  FPlane DistanceFogColor, DistanceFogBlend;
  float DistanceFogDistance[2];
  float DistanceFogStart[2];
  float DistanceFogBlendTimer[2];
  AZoneInfo* DistanceFogOld;
  float FogDensity;
  int FogMode;

  float aBaseX, aBaseY, aBaseZ,
        aMouseX, aMouseY,
        aForward, aTurn, aStrafe, aUp,
        aLookUp, aExtra4, aExtra3, aExtra2,
        aExtra1, aExtra0;

  class ASavedMove* SavedMoves;
  class ASavedMove* FreeMoves;
  float CurrentTimeStamp;
  float LastUpdateTime;
  float ServerTimeStamp;
  float TimeMargin;
  float MaxTimeMargin;

  String* ProgressMessage[5];
  FColor  ProgressColor[5];
  float   ProgressTimeOut;

  String* QuickSaveString;
  String* NoPauseMessage;
  String* ViewingFrom;
  String* OwnCamera;
  String* FailedView;
  String* CantChangeNameMsg;

  AGameReplicationInfo* GameReplicationInfo;

  String* ngWorldSecret;

  bool     bRepTargetViewRotation;
  FRotator TargetViewRotation;
  float    TargetEyeHeight;
  FVector  TargetWeaponViewOffset;

  FVector  CalcCameraLocation;
  FRotator CalcCameraRotation;
  float    LastCameraUpdate;

  class ACustomPlayerStateInfo* CustomPlayerStateInfo;
  class ARealCrouchController*  RealCrouchController;
  class ALadderTrigger*         ActiveLadder;
};

class ACamera : public APlayerPawn
{
  DECLARE_NATIVE_CLASS( ACamera, APlayerPawn, CLASS_Transient, Engine )
  EXPOSE_TO_USCRIPT()

  ACamera();
};

