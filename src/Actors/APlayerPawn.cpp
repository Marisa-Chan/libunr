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
 * APlayerPawn.cpp - Base player pawn functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Actors/APlayerPawn.h"
#include "Core/UClass.h"

APlayerPawn::APlayerPawn()
  : APawn()
{
}

APlayerPawn::~APlayerPawn()
{
}

ACamera::ACamera()
  : APlayerPawn()
{
}

ACamera::~ACamera()
{
}

IMPLEMENT_NATIVE_CLASS( ACamera );
IMPLEMENT_NATIVE_CLASS( APlayerPawn );

BEGIN_PROPERTY_LINK( ACamera, 0 )
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( APlayerPawn, 152 )
  LINK_NATIVE_PROPERTY( APlayerPawn, Player );
  LINK_NATIVE_PROPERTY( APlayerPawn, Password );
  LINK_NATIVE_PROPERTY( APlayerPawn, DodgeClickTimer );
  LINK_NATIVE_PROPERTY( APlayerPawn, DodgeClickTime );
  LINK_NATIVE_PROPERTY( APlayerPawn, Bob );
  LINK_NATIVE_PROPERTY( APlayerPawn, BobTime );
  LINK_NATIVE_PROPERTY( APlayerPawn, ShowFlags );
  LINK_NATIVE_PROPERTY( APlayerPawn, RendMap );
  LINK_NATIVE_PROPERTY( APlayerPawn, Misc1 );
  LINK_NATIVE_PROPERTY( APlayerPawn, Misc2 );
  LINK_NATIVE_PROPERTY( APlayerPawn, ViewTarget );
  LINK_NATIVE_PROPERTY( APlayerPawn, FlashScale );
  LINK_NATIVE_PROPERTY( APlayerPawn, FlashFog );
  LINK_NATIVE_PROPERTY( APlayerPawn, myHUD );
  LINK_NATIVE_PROPERTY( APlayerPawn, Scoring );
  LINK_NATIVE_PROPERTY( APlayerPawn, HUDType );
  LINK_NATIVE_PROPERTY( APlayerPawn, ScoringType );
  LINK_NATIVE_PROPERTY( APlayerPawn, DesiredFlashScale );
  LINK_NATIVE_PROPERTY( APlayerPawn, ConstantGlowScale );
  LINK_NATIVE_PROPERTY( APlayerPawn, InstantFlash );
  LINK_NATIVE_PROPERTY( APlayerPawn, DesiredFlashFog );
  LINK_NATIVE_PROPERTY( APlayerPawn, ConstantGlowFog );
  LINK_NATIVE_PROPERTY( APlayerPawn, InstantFog );
  LINK_NATIVE_PROPERTY( APlayerPawn, DesiredFOV );
  LINK_NATIVE_PROPERTY( APlayerPawn, DefaultFOV );
  LINK_NATIVE_PROPERTY( APlayerPawn, Song );
  LINK_NATIVE_PROPERTY( APlayerPawn, SongSection );
  LINK_NATIVE_PROPERTY( APlayerPawn, CdTrack );
  LINK_NATIVE_PROPERTY( APlayerPawn, Transition );
  LINK_NATIVE_PROPERTY( APlayerPawn, ShakeTimer );
  LINK_NATIVE_PROPERTY( APlayerPawn, ShakeMag );
  LINK_NATIVE_PROPERTY( APlayerPawn, ShakeVert );
  LINK_NATIVE_PROPERTY( APlayerPawn, MaxShake );
  LINK_NATIVE_PROPERTY( APlayerPawn, VertTimer );
  LINK_NATIVE_PROPERTY( APlayerPawn, CarcassType );
  LINK_NATIVE_PROPERTY( APlayerPawn, MyAutoAim );
  LINK_NATIVE_PROPERTY( APlayerPawn, Handedness );
  LINK_NATIVE_PROPERTY( APlayerPawn, JumpSound );
  LINK_NATIVE_PROPERTY( APlayerPawn, bAdmin );
  LINK_NATIVE_PROPERTY( APlayerPawn, bLookUpStairs );
  LINK_NATIVE_PROPERTY( APlayerPawn, bSnapToLevel );
  LINK_NATIVE_PROPERTY( APlayerPawn, bAlwaysMouseLook );
  LINK_NATIVE_PROPERTY( APlayerPawn, bKeyboardLook );
  LINK_NATIVE_PROPERTY( APlayerPawn, bWasForward );
  LINK_NATIVE_PROPERTY( APlayerPawn, bWasBack );
  LINK_NATIVE_PROPERTY( APlayerPawn, bWasLeft );
  LINK_NATIVE_PROPERTY( APlayerPawn, bWasRight );
  LINK_NATIVE_PROPERTY( APlayerPawn, bEdgeForward );
  LINK_NATIVE_PROPERTY( APlayerPawn, bEdgeBack );
  LINK_NATIVE_PROPERTY( APlayerPawn, bEdgeLeft );
  LINK_NATIVE_PROPERTY( APlayerPawn, bEdgeRight );
  LINK_NATIVE_PROPERTY( APlayerPawn, bIsCrouching );
  LINK_NATIVE_PROPERTY( APlayerPawn, bShakeDir );
  LINK_NATIVE_PROPERTY( APlayerPawn, bAnimTransition );
  LINK_NATIVE_PROPERTY( APlayerPawn, bIsTurning );
  LINK_NATIVE_PROPERTY( APlayerPawn, bFrozen );
  LINK_NATIVE_PROPERTY( APlayerPawn, bInvertMouse );
  LINK_NATIVE_PROPERTY( APlayerPawn, bShowScores );
  LINK_NATIVE_PROPERTY( APlayerPawn, bShowMenu );
  LINK_NATIVE_PROPERTY( APlayerPawn, bSpecialMenu );
  LINK_NATIVE_PROPERTY( APlayerPawn, bWokeUp );
  LINK_NATIVE_PROPERTY( APlayerPawn, bPressedJump );
  LINK_NATIVE_PROPERTY( APlayerPawn, bUpdatePosition );
  LINK_NATIVE_PROPERTY( APlayerPawn, bDelayedCommand );
  LINK_NATIVE_PROPERTY( APlayerPawn, bRising );
  LINK_NATIVE_PROPERTY( APlayerPawn, bReducedVis );
  LINK_NATIVE_PROPERTY( APlayerPawn, bCenterView );
  LINK_NATIVE_PROPERTY( APlayerPawn, bMaxMouseSmoothing );
  LINK_NATIVE_PROPERTY( APlayerPawn, bMouseZeroed );
  LINK_NATIVE_PROPERTY( APlayerPawn, bReadyToPlay );
  LINK_NATIVE_PROPERTY( APlayerPawn, bNoFlash );
  LINK_NATIVE_PROPERTY( APlayerPawn, bNoVoices );
  LINK_NATIVE_PROPERTY( APlayerPawn, bMessageBeep );
  LINK_NATIVE_PROPERTY( APlayerPawn, bZooming );
  LINK_NATIVE_PROPERTY( APlayerPawn, bSinglePlayer );
  LINK_NATIVE_PROPERTY( APlayerPawn, bJustFired );
  LINK_NATIVE_PROPERTY( APlayerPawn, bJustAltFired );
  LINK_NATIVE_PROPERTY( APlayerPawn, bIsTyping );
  LINK_NATIVE_PROPERTY( APlayerPawn, bFixedCamera );
  LINK_NATIVE_PROPERTY( APlayerPawn, MainFOV );
  LINK_NATIVE_PROPERTY( APlayerPawn, bConsoleCommandMessage );
  LINK_NATIVE_PROPERTY( APlayerPawn, ZoomLevel );
  LINK_NATIVE_PROPERTY( APlayerPawn, SpecialMenu );
  LINK_NATIVE_PROPERTY( APlayerPawn, DelayedCommand );
  LINK_NATIVE_PROPERTY( APlayerPawn, MouseSensitivity );
  LINK_NATIVE_ARRAY   ( APlayerPawn, WeaponPriority );
  LINK_NATIVE_PROPERTY( APlayerPawn, NetSpeed );
  LINK_NATIVE_PROPERTY( APlayerPawn, LanSpeed );
  LINK_NATIVE_PROPERTY( APlayerPawn, SmoothMouseX );
  LINK_NATIVE_PROPERTY( APlayerPawn, SmoothMouseY );
  LINK_NATIVE_PROPERTY( APlayerPawn, KbdAccel );
  LINK_NATIVE_PROPERTY( APlayerPawn, bMouseSmoothing );
  LINK_NATIVE_PROPERTY( APlayerPawn, MouseSmoothThreshold );
  LINK_NATIVE_PROPERTY( APlayerPawn, bNeverAutoSwitch );
  LINK_NATIVE_PROPERTY( APlayerPawn, bIgnoreMusicChange );
  LINK_NATIVE_PROPERTY( APlayerPawn, CameraRegion );
  LINK_NATIVE_PROPERTY( APlayerPawn, CrouchHeightPct );
  LINK_NATIVE_PROPERTY( APlayerPawn, bIsReducedCrouch );
  LINK_NATIVE_PROPERTY( APlayerPawn, CrouchCheckTime );
  LINK_NATIVE_PROPERTY( APlayerPawn, bDistanceFogEnabled );
  LINK_NATIVE_PROPERTY( APlayerPawn, bBlendingDistanceFog );
  LINK_NATIVE_PROPERTY( APlayerPawn, DistanceFogColor );
  LINK_NATIVE_PROPERTY( APlayerPawn, DistanceFogBlend );
  LINK_NATIVE_ARRAY   ( APlayerPawn, DistanceFogDistance );
  LINK_NATIVE_ARRAY   ( APlayerPawn, DistanceFogStart );
  LINK_NATIVE_ARRAY   ( APlayerPawn, DistanceFogBlendTimer );
  LINK_NATIVE_PROPERTY( APlayerPawn, DistanceFogOld );
  LINK_NATIVE_PROPERTY( APlayerPawn, FogDensity );
  LINK_NATIVE_PROPERTY( APlayerPawn, FogMode );
  LINK_NATIVE_PROPERTY( APlayerPawn, aBaseX );
  LINK_NATIVE_PROPERTY( APlayerPawn, aBaseY );
  LINK_NATIVE_PROPERTY( APlayerPawn, aBaseZ );
  LINK_NATIVE_PROPERTY( APlayerPawn, aMouseX );
  LINK_NATIVE_PROPERTY( APlayerPawn, aMouseY );
  LINK_NATIVE_PROPERTY( APlayerPawn, aForward );
  LINK_NATIVE_PROPERTY( APlayerPawn, aTurn );
  LINK_NATIVE_PROPERTY( APlayerPawn, aStrafe );
  LINK_NATIVE_PROPERTY( APlayerPawn, aUp );
  LINK_NATIVE_PROPERTY( APlayerPawn, aLookUp );
  LINK_NATIVE_PROPERTY( APlayerPawn, aExtra4 );
  LINK_NATIVE_PROPERTY( APlayerPawn, aExtra3 );
  LINK_NATIVE_PROPERTY( APlayerPawn, aExtra2 );
  LINK_NATIVE_PROPERTY( APlayerPawn, aExtra1 );
  LINK_NATIVE_PROPERTY( APlayerPawn, aExtra0 );
  LINK_NATIVE_PROPERTY( APlayerPawn, SavedMoves );
  LINK_NATIVE_PROPERTY( APlayerPawn, FreeMoves );
  LINK_NATIVE_PROPERTY( APlayerPawn, CurrentTimeStamp );
  LINK_NATIVE_PROPERTY( APlayerPawn, LastUpdateTime );
  LINK_NATIVE_PROPERTY( APlayerPawn, ServerTimeStamp );
  LINK_NATIVE_PROPERTY( APlayerPawn, TimeMargin );
  LINK_NATIVE_PROPERTY( APlayerPawn, MaxTimeMargin );
  LINK_NATIVE_ARRAY   ( APlayerPawn, ProgressMessage );
  LINK_NATIVE_ARRAY   ( APlayerPawn, ProgressColor );
  LINK_NATIVE_PROPERTY( APlayerPawn, ProgressTimeOut );
  LINK_NATIVE_PROPERTY( APlayerPawn, QuickSaveString );
  LINK_NATIVE_PROPERTY( APlayerPawn, NoPauseMessage );
  LINK_NATIVE_PROPERTY( APlayerPawn, ViewingFrom );
  LINK_NATIVE_PROPERTY( APlayerPawn, OwnCamera );
  LINK_NATIVE_PROPERTY( APlayerPawn, FailedView );
  LINK_NATIVE_PROPERTY( APlayerPawn, CantChangeNameMsg );
  LINK_NATIVE_PROPERTY( APlayerPawn, GameReplicationInfo );
  LINK_NATIVE_PROPERTY( APlayerPawn, ngWorldSecret );
  LINK_NATIVE_PROPERTY( APlayerPawn, bRepTargetViewRotation );
  LINK_NATIVE_PROPERTY( APlayerPawn, TargetViewRotation );
  LINK_NATIVE_PROPERTY( APlayerPawn, TargetEyeHeight );
  LINK_NATIVE_PROPERTY( APlayerPawn, TargetWeaponViewOffset );
  LINK_NATIVE_PROPERTY( APlayerPawn, CalcCameraLocation );
  LINK_NATIVE_PROPERTY( APlayerPawn, CalcCameraRotation );
  LINK_NATIVE_PROPERTY( APlayerPawn, LastCameraUpdate );
  LINK_NATIVE_PROPERTY( APlayerPawn, CustomPlayerStateInfo );
  LINK_NATIVE_PROPERTY( APlayerPawn, RealCrouchController );
  LINK_NATIVE_PROPERTY( APlayerPawn, ActiveLadder );
END_PROPERTY_LINK()

