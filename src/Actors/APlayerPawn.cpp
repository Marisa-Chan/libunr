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

BEGIN_PROPERTY_LINK( APlayerPawn, 193 )
  LINK_NATIVE_PROPERTY( Player );
  LINK_NATIVE_PROPERTY( Password );
  LINK_NATIVE_PROPERTY( DodgeClickTimer );
  LINK_NATIVE_PROPERTY( DodgeClickTime );
  LINK_NATIVE_PROPERTY( Bob );
  LINK_NATIVE_PROPERTY( BobTime );
  LINK_NATIVE_PROPERTY( ShowFlags );
  LINK_NATIVE_PROPERTY( RendMap );
  LINK_NATIVE_PROPERTY( Misc1 );
  LINK_NATIVE_PROPERTY( Misc2 );
  LINK_NATIVE_PROPERTY( ViewTarget );
  LINK_NATIVE_PROPERTY( FlashScale );
  LINK_NATIVE_PROPERTY( FlashFog );
  LINK_NATIVE_PROPERTY( CurrentFlashFog );
  LINK_NATIVE_PROPERTY( myHUD );
  LINK_NATIVE_PROPERTY( Scoring );
  LINK_NATIVE_PROPERTY( HUDType );
  LINK_NATIVE_PROPERTY( ScoringType );
  LINK_NATIVE_PROPERTY( DesiredFlashScale );
  LINK_NATIVE_PROPERTY( ConstantGlowScale );
  LINK_NATIVE_PROPERTY( InstantFlash );
  LINK_NATIVE_PROPERTY( DesiredFlashFog );
  LINK_NATIVE_PROPERTY( ConstantGlowFog );
  LINK_NATIVE_PROPERTY( InstantFog );
  LINK_NATIVE_PROPERTY( DesiredFOV );
  LINK_NATIVE_PROPERTY( DefaultFOV );
  LINK_NATIVE_PROPERTY( Song );
  LINK_NATIVE_PROPERTY( SongSection );
  LINK_NATIVE_PROPERTY( CdTrack );
  LINK_NATIVE_PROPERTY( Transition );
  LINK_NATIVE_PROPERTY( ShakeTimer );
  LINK_NATIVE_PROPERTY( ShakeMag );
  LINK_NATIVE_PROPERTY( ShakeVert );
  LINK_NATIVE_PROPERTY( MaxShake );
  LINK_NATIVE_PROPERTY( VertTimer );
  LINK_NATIVE_PROPERTY( CarcassType );
  LINK_NATIVE_PROPERTY( MyAutoAim );
  LINK_NATIVE_PROPERTY( Handedness );
  LINK_NATIVE_PROPERTY( JumpSound );
  LINK_NATIVE_PROPERTY( LandBob );
  LINK_NATIVE_PROPERTY( AppliedBob );
  LINK_NATIVE_PROPERTY( bAdmin );
  LINK_NATIVE_PROPERTY( bLookUpStairs );
  LINK_NATIVE_PROPERTY( bSnapToLevel );
  LINK_NATIVE_PROPERTY( bAlwaysMouseLook );
  LINK_NATIVE_PROPERTY( bKeyboardLook );
  LINK_NATIVE_PROPERTY( bWasForward );
  LINK_NATIVE_PROPERTY( bWasBack );
  LINK_NATIVE_PROPERTY( bWasLeft );
  LINK_NATIVE_PROPERTY( bWasRight );
  LINK_NATIVE_PROPERTY( bEdgeForward );
  LINK_NATIVE_PROPERTY( bEdgeBack );
  LINK_NATIVE_PROPERTY( bEdgeLeft );
  LINK_NATIVE_PROPERTY( bEdgeRight );
  LINK_NATIVE_PROPERTY( bIsCrouching );
  LINK_NATIVE_PROPERTY( bShakeDir );
  LINK_NATIVE_PROPERTY( bAnimTransition );
  LINK_NATIVE_PROPERTY( bIsTurning );
  LINK_NATIVE_PROPERTY( bFrozen );
  LINK_NATIVE_PROPERTY( bInvertMouse );
  LINK_NATIVE_PROPERTY( bShowScores );
  LINK_NATIVE_PROPERTY( bShowMenu );
  LINK_NATIVE_PROPERTY( bSpecialMenu );
  LINK_NATIVE_PROPERTY( bWokeUp );
  LINK_NATIVE_PROPERTY( bPressedJump );
  LINK_NATIVE_PROPERTY( bUpdatePosition );
  LINK_NATIVE_PROPERTY( bDelayedCommand );
  LINK_NATIVE_PROPERTY( bRising );
  LINK_NATIVE_PROPERTY( bReducedVis );
  LINK_NATIVE_PROPERTY( bCenterView );
  LINK_NATIVE_PROPERTY( bMaxMouseSmoothing );
  LINK_NATIVE_PROPERTY( bMouseZeroed );
  LINK_NATIVE_PROPERTY( bReadyToPlay );
  LINK_NATIVE_PROPERTY( bNoFlash );
  LINK_NATIVE_PROPERTY( bNoVoices );
  LINK_NATIVE_PROPERTY( bMessageBeep );
  LINK_NATIVE_PROPERTY( bZooming );
  LINK_NATIVE_PROPERTY( bSinglePlayer );
  LINK_NATIVE_PROPERTY( bJustFired );
  LINK_NATIVE_PROPERTY( bJustAltFired );
  LINK_NATIVE_PROPERTY( bIsTyping );
  LINK_NATIVE_PROPERTY( bFixedCamera );
  LINK_NATIVE_PROPERTY( bBadConnectionAlert );
  LINK_NATIVE_PROPERTY( bJumpStatus );
  LINK_NATIVE_PROPERTY( bUpdating );
  LINK_NATIVE_PROPERTY( bCheatsEnabled );
  LINK_NATIVE_PROPERTY( MainFOV );
  LINK_NATIVE_PROPERTY( bConsoleCommandMessage );
  LINK_NATIVE_PROPERTY( ZoomLevel );
  LINK_NATIVE_PROPERTY( SpecialMenu );
  LINK_NATIVE_PROPERTY( DelayedCommand );
  LINK_NATIVE_PROPERTY( MouseSensitivity );
  LINK_NATIVE_ARRAY   ( WeaponPriority );
  LINK_NATIVE_PROPERTY( NetSpeed );
  LINK_NATIVE_PROPERTY( LanSpeed );
  LINK_NATIVE_PROPERTY( SmoothMouseX );
  LINK_NATIVE_PROPERTY( SmoothMouseY );
  LINK_NATIVE_PROPERTY( KbdAccel );
  LINK_NATIVE_PROPERTY( bMouseSmoothing );
  LINK_NATIVE_PROPERTY( MouseSmoothThreshold );
  LINK_NATIVE_PROPERTY( BorrowedMouseX );
  LINK_NATIVE_PROPERTY( BorrowedMouseY );
  LINK_NATIVE_PROPERTY( MouseZeroTime );
  LINK_NATIVE_PROPERTY( bNeverAutoSwitch );
  LINK_NATIVE_PROPERTY( bIgnoreMusicChange );
  LINK_NATIVE_PROPERTY( CameraRegion );
  LINK_NATIVE_PROPERTY( CrouchHeightPct );
  LINK_NATIVE_PROPERTY( bIsReducedCrouch );
  LINK_NATIVE_PROPERTY( CrouchCheckTime );
  LINK_NATIVE_PROPERTY( SpecialCollisionHeight );
  LINK_NATIVE_PROPERTY( PrePivotZModifier );
  LINK_NATIVE_PROPERTY( bCanChangeBehindView );
  LINK_NATIVE_PROPERTY( bForwardUserCommands );
  LINK_NATIVE_PROPERTY( AccumulatedHTurn );
  LINK_NATIVE_PROPERTY( AccumulatedVTurn );
  LINK_NATIVE_PROPERTY( RealCrouchInfo );
  LINK_NATIVE_PROPERTY( bDistanceFogEnabled );
  LINK_NATIVE_PROPERTY( bBlendingDistanceFog );
  LINK_NATIVE_PROPERTY( DistanceFogColor );
  LINK_NATIVE_PROPERTY( DistanceFogBlend );
  LINK_NATIVE_ARRAY   ( DistanceFogDistance );
  LINK_NATIVE_ARRAY   ( DistanceFogStart );
  LINK_NATIVE_ARRAY   ( DistanceFogBlendTimer );
  LINK_NATIVE_PROPERTY( DistanceFogOld );
  LINK_NATIVE_PROPERTY( FogDensity );
  LINK_NATIVE_PROPERTY( FogMode );
  LINK_NATIVE_PROPERTY( DemoViewPitch );
  LINK_NATIVE_PROPERTY( DemoViewYaw );
  LINK_NATIVE_PROPERTY( aBaseX );
  LINK_NATIVE_PROPERTY( aBaseY );
  LINK_NATIVE_PROPERTY( aBaseZ );
  LINK_NATIVE_PROPERTY( aMouseX );
  LINK_NATIVE_PROPERTY( aMouseY );
  LINK_NATIVE_PROPERTY( aForward );
  LINK_NATIVE_PROPERTY( aTurn );
  LINK_NATIVE_PROPERTY( aStrafe );
  LINK_NATIVE_PROPERTY( aUp );
  LINK_NATIVE_PROPERTY( aLookUp );
  LINK_NATIVE_PROPERTY( aExtra4 );
  LINK_NATIVE_PROPERTY( aExtra3 );
  LINK_NATIVE_PROPERTY( aExtra2 );
  LINK_NATIVE_PROPERTY( aExtra1 );
  LINK_NATIVE_PROPERTY( aExtra0 );
  LINK_NATIVE_PROPERTY( SavedMoves );
  LINK_NATIVE_PROPERTY( FreeMoves );
  LINK_NATIVE_PROPERTY( PendingMove );
  LINK_NATIVE_PROPERTY( CurrentTimeStamp );
  LINK_NATIVE_PROPERTY( LastUpdateTime );
  LINK_NATIVE_PROPERTY( ServerTimeStamp );
  LINK_NATIVE_PROPERTY( TimeMargin );
  LINK_NATIVE_PROPERTY( MaxTimeMargin );
  LINK_NATIVE_PROPERTY( ClientUpdateTime );
  LINK_NATIVE_PROPERTY( LastPlaySound );
  LINK_NATIVE_PROPERTY( LastMessageWindow );
  LINK_NATIVE_ARRAY   ( ProgressMessage );
  LINK_NATIVE_ARRAY   ( ProgressColor );
  LINK_NATIVE_PROPERTY( ProgressTimeOut );
  LINK_NATIVE_PROPERTY( QuickSaveString );
  LINK_NATIVE_PROPERTY( NoPauseMessage );
  LINK_NATIVE_PROPERTY( ViewingFrom );
  LINK_NATIVE_PROPERTY( OwnCamera );
  LINK_NATIVE_PROPERTY( FailedView );
  LINK_NATIVE_PROPERTY( CantChangeNameMsg );
  LINK_NATIVE_PROPERTY( GameReplicationInfo );
  LINK_NATIVE_PROPERTY( ngWorldSecret );
  LINK_NATIVE_PROPERTY( ngSecretSet );
  LINK_NATIVE_PROPERTY( ReceivedSecretChecksum );
  LINK_NATIVE_PROPERTY( bRepTargetViewRotation );
  LINK_NATIVE_PROPERTY( TargetViewRotation );
  LINK_NATIVE_PROPERTY( TargetEyeHeight );
  LINK_NATIVE_PROPERTY( TargetWeaponViewOffset );
  LINK_NATIVE_PROPERTY( CalcCameraLocation );
  LINK_NATIVE_PROPERTY( CalcCameraRotation );
  LINK_NATIVE_PROPERTY( LastCameraUpdate );
  LINK_NATIVE_PROPERTY( CustomPlayerStateInfo );
  LINK_NATIVE_PROPERTY( RealCrouchController );
  LINK_NATIVE_PROPERTY( ActiveLadder );
  LINK_NATIVE_ARRAY   ( AdditionalViews );
  LINK_NATIVE_PROPERTY( TurnRateAdjuster );
  LINK_NATIVE_PROPERTY( myDebugHUD );
  LINK_NATIVE_PROPERTY( HudTranslucency );
  LINK_NATIVE_PROPERTY( AtrophyTimer );
  LINK_NATIVE_PROPERTY( bBloodLust );
  LINK_NATIVE_PROPERTY( PolyColorAdjust );
  LINK_NATIVE_PROPERTY( DesiredPolyColorAdjust );
  LINK_NATIVE_PROPERTY( bClientSideAlpha );
  LINK_NATIVE_PROPERTY( ClientSideAlphaScale );
  LINK_NATIVE_PROPERTY( OldStyle );
  LINK_NATIVE_PROPERTY( OldScale );
  LINK_NATIVE_PROPERTY( StowSpot );
  LINK_NATIVE_PROPERTY( bJustSpawned );
  LINK_NATIVE_PROPERTY( LevelFadeAlpha );
  LINK_NATIVE_PROPERTY( oldPhysics );
END_PROPERTY_LINK()

