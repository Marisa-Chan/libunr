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
  FString*  Password;

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
  FVector CurrentFlashFog;

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
  float   LandBob;
  float   AppliedBob;

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
  bool bBadConnectionAlert;
  bool bJumpStatus;
  bool bUpdating;
  bool bCheatsEnabled;
  float MainFOV;
  bool bConsoleCommandMessage;
  float ZoomLevel;

  UClass* SpecialMenu;
  FString* DelayedCommand;
  float   MouseSensitivity;

  FName WeaponPriority[50];

  int NetSpeed, LanSpeed;
  float SmoothMouseX, SmoothMouseY, KbdAccel;
  bool bMouseSmoothing;
  float MouseSmoothThreshold;
  float BorrowedMouseX;
  float BorrowedMouseY;
  float MouseZeroTime;

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

  int DemoViewPitch;
  int DemoViewYaw;

  float aBaseX, aBaseY, aBaseZ,
        aMouseX, aMouseY,
        aForward, aTurn, aStrafe, aUp,
        aLookUp, aExtra4, aExtra3, aExtra2,
        aExtra1, aExtra0;

  class ASavedMove* SavedMoves;
  class ASavedMove* FreeMoves;
  class ASavedMove* PendingMove;
  float CurrentTimeStamp;
  float LastUpdateTime;
  float ServerTimeStamp;
  float TimeMargin;
  float MaxTimeMargin;
  float ClientUpdateTime;
  float LastPlaySound;
  float LastMessageWindow;

  FString* ProgressMessage[5];
  FColor  ProgressColor[5];
  float   ProgressTimeOut;

  FString* QuickSaveString;
  FString* NoPauseMessage;
  FString* ViewingFrom;
  FString* OwnCamera;
  FString* FailedView;
  FString* CantChangeNameMsg;

  AGameReplicationInfo* GameReplicationInfo;

  FString* ngWorldSecret;
  bool ngSecretSet;
  bool ReceivedSecretChecksum;

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

  // Deus Ex Variables
  AActor* AdditionalViews[3];
  float TurnRateAdjuster;

  // Rune Variables
  class ADebugHUD* myDebugHUD;
  float HudTranslucency;
  float AtrophyTimer;
  bool bBloodLust;
  FVector PolyColorAdjust;
  FVector DesiredPolyColorAdjust;
  bool bClientSideAlpha;
  float ClientSideAlphaScale;
  u8 OldStyle;
  float OldScale;
  AWeapon* StowSpot[3];
  bool bJustSpawned;
  float LevelFadeAlpha;
  u8 oldPhysics;
};

class ACamera : public APlayerPawn
{
  DECLARE_NATIVE_CLASS( ACamera, APlayerPawn, CLASS_Transient, Engine )
  EXPOSE_TO_USCRIPT()

  ACamera();
};

