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
 * APawn.cpp - Pawn functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Actors/APawn.h"
#include "Core/UClass.h"
#include "Core/UProperty.h"

APawn::APawn()
  : AActor()
{
}

APawn::~APawn()
{
}

void APawn::PostDefaultLoad()
{
  Super::PostDefaultLoad();
  if ( !GSystem->bEnhancedRuntime )
  {
    UProperty* PropFootRegion = FindProperty("FootRegion");
    UProperty* PropHeadRegion = FindProperty("HeadRegion");

    // These can never be set by the user in the editor, but official Engine.u
    // does not mark these as editconst
    PropFootRegion->PropertyFlags |= CPF_EditConst;
    PropHeadRegion->PropertyFlags |= CPF_EditConst;
  }
}

IMPLEMENT_NATIVE_CLASS( APawn );

BEGIN_PROPERTY_LINK( APawn, 175 )
  LINK_NATIVE_PROPERTY( APawn, bBehindView );
  LINK_NATIVE_PROPERTY( APawn, bIsPlayer );
  LINK_NATIVE_PROPERTY( APawn, bJustLanded );
  LINK_NATIVE_PROPERTY( APawn, bUpAndOut );
  LINK_NATIVE_PROPERTY( APawn, bIsWalking );
  LINK_NATIVE_PROPERTY( APawn, bHitSlopedWall );
  LINK_NATIVE_PROPERTY( APawn, bNeverSwitchOnPickup );
  LINK_NATIVE_PROPERTY( APawn, bWarping );
  LINK_NATIVE_PROPERTY( APawn, bUpdatingDisplay );
  LINK_NATIVE_PROPERTY( APawn, bClientSimFall );
  LINK_NATIVE_PROPERTY( APawn, bClientTick );
  LINK_NATIVE_PROPERTY( APawn, bUseNoWalkInAir );
  LINK_NATIVE_PROPERTY( APawn, bNoStopAtLedge );
  LINK_NATIVE_PROPERTY( APawn, bPostRender2D );
  LINK_NATIVE_PROPERTY( APawn, bCanStrafe );
  LINK_NATIVE_PROPERTY( APawn, bFixedStart );
  LINK_NATIVE_PROPERTY( APawn, bReducedSpeed );
  LINK_NATIVE_PROPERTY( APawn, bCanJump );
  LINK_NATIVE_PROPERTY( APawn, bCanWalk );
  LINK_NATIVE_PROPERTY( APawn, bCanSwim );
  LINK_NATIVE_PROPERTY( APawn, bCanFly );
  LINK_NATIVE_PROPERTY( APawn, bCanOpenDoors );
  LINK_NATIVE_PROPERTY( APawn, bCanDoSpecial );
  LINK_NATIVE_PROPERTY( APawn, bDrowning );
  LINK_NATIVE_PROPERTY( APawn, bLOSFlag );
  LINK_NATIVE_PROPERTY( APawn, bFromWall );
  LINK_NATIVE_PROPERTY( APawn, bHunting );
  LINK_NATIVE_PROPERTY( APawn, bAvoidLedges );
  LINK_NATIVE_PROPERTY( APawn, bStopAtLedges );
  LINK_NATIVE_PROPERTY( APawn, bJumpOffPawn );
  LINK_NATIVE_PROPERTY( APawn, bShootSpecial );
  LINK_NATIVE_PROPERTY( APawn, bAutoActivate );
  LINK_NATIVE_PROPERTY( APawn, bIsHuman );
  LINK_NATIVE_PROPERTY( APawn, bIsFemale );
  LINK_NATIVE_PROPERTY( APawn, bIsMultiSkinned );
  LINK_NATIVE_PROPERTY( APawn, bCountJumps );
  LINK_NATIVE_PROPERTY( APawn, bAdvancedTactics );
  LINK_NATIVE_PROPERTY( APawn, bViewTarget );
  LINK_NATIVE_PROPERTY( APawn, SelectionMesh );
  LINK_NATIVE_PROPERTY( APawn, SpecialMesh );
  LINK_NATIVE_PROPERTY( APawn, bEnhancedSightCheck );
  LINK_NATIVE_PROPERTY( APawn, bRepHealth );
  LINK_NATIVE_PROPERTY( APawn, bDoAutoSerpentine );
  LINK_NATIVE_PROPERTY( APawn, bIsCrawler );
  LINK_NATIVE_PROPERTY( APawn, bIsBleeding );
  LINK_NATIVE_PROPERTY( APawn, bIsAmbientCreature );
  LINK_NATIVE_PROPERTY( APawn, LastAnchor );
  LINK_NATIVE_PROPERTY( APawn, SightCounter );
  LINK_NATIVE_PROPERTY( APawn, PainTime );
  LINK_NATIVE_PROPERTY( APawn, SpeechTime );
  LINK_NATIVE_PROPERTY( APawn, AvgPhysicsTime );
  LINK_NATIVE_PROPERTY( APawn, FootRegion );
  LINK_NATIVE_PROPERTY( APawn, HeadRegion );
  LINK_NATIVE_PROPERTY( APawn, MoveTimer );
  LINK_NATIVE_PROPERTY( APawn, MoveTarget );
  LINK_NATIVE_PROPERTY( APawn, FaceTarget );
  LINK_NATIVE_PROPERTY( APawn, Destination );
  LINK_NATIVE_PROPERTY( APawn, Focus );
  LINK_NATIVE_PROPERTY( APawn, DesiredSpeed );
  LINK_NATIVE_PROPERTY( APawn, MaxDesiredSpeed );
  LINK_NATIVE_PROPERTY( APawn, MeleeRange );
  LINK_NATIVE_PROPERTY( APawn, SerpentineDist );
  LINK_NATIVE_PROPERTY( APawn, SerpentineTime );
  LINK_NATIVE_PROPERTY( APawn, MovementStart );
  LINK_NATIVE_PROPERTY( APawn, GroundSpeed );
  LINK_NATIVE_PROPERTY( APawn, WaterSpeed );
  LINK_NATIVE_PROPERTY( APawn, AirSpeed );
  LINK_NATIVE_PROPERTY( APawn, AccelRate );
  LINK_NATIVE_PROPERTY( APawn, JumpZ );
  LINK_NATIVE_PROPERTY( APawn, MaxStepHeight );
  LINK_NATIVE_PROPERTY( APawn, AirControl );
  LINK_NATIVE_PROPERTY( APawn, MinHitWall );
  LINK_NATIVE_PROPERTY( APawn, Visibility );
  LINK_NATIVE_PROPERTY( APawn, Alertness );
  LINK_NATIVE_PROPERTY( APawn, Stimulus );
  LINK_NATIVE_PROPERTY( APawn, SightRadius );
  LINK_NATIVE_PROPERTY( APawn, PeripheralVision );
  LINK_NATIVE_PROPERTY( APawn, HearingThreshold );
  LINK_NATIVE_PROPERTY( APawn, HuntOffDistance );
  LINK_NATIVE_PROPERTY( APawn, LastSeenPos );
  LINK_NATIVE_PROPERTY( APawn, LastSeeingPos );
  LINK_NATIVE_PROPERTY( APawn, LastSeenTime );
  LINK_NATIVE_PROPERTY( APawn, Enemy );
  LINK_NATIVE_PROPERTY( APawn, Weapon );
  LINK_NATIVE_PROPERTY( APawn, PendingWeapon );
  LINK_NATIVE_PROPERTY( APawn, SelectedItem );
  LINK_NATIVE_PROPERTY( APawn, LastDamageInstigator );
  LINK_NATIVE_PROPERTY( APawn, LastDamageHitLocation );
  LINK_NATIVE_PROPERTY( APawn, LastDamageMomentum );
  LINK_NATIVE_PROPERTY( APawn, LastDamageType );
  LINK_NATIVE_PROPERTY( APawn, LastDamageTime );
  LINK_NATIVE_PROPERTY( APawn, bLastDamageSpawnedBlood );
  LINK_NATIVE_PROPERTY( APawn, ViewRotation );
  LINK_NATIVE_PROPERTY( APawn, WalkBob );
  LINK_NATIVE_PROPERTY( APawn, BaseEyeHeight );
  LINK_NATIVE_PROPERTY( APawn, EyeHeight );
  LINK_NATIVE_PROPERTY( APawn, Floor );
  LINK_NATIVE_PROPERTY( APawn, SplashTime );
  LINK_NATIVE_PROPERTY( APawn, OrthoZoom );
  LINK_NATIVE_PROPERTY( APawn, FovAngle );
  LINK_NATIVE_PROPERTY( APawn, DieCount );
  LINK_NATIVE_PROPERTY( APawn, ItemCount );
  LINK_NATIVE_PROPERTY( APawn, KillCount );
  LINK_NATIVE_PROPERTY( APawn, SecretCount );
  LINK_NATIVE_PROPERTY( APawn, Spree );
  LINK_NATIVE_PROPERTY( APawn, Health );
  LINK_NATIVE_PROPERTY( APawn, ReducedDamageType );
  LINK_NATIVE_PROPERTY( APawn, ReducedDamagePct );
  LINK_NATIVE_PROPERTY( APawn, DropWhenKilled );
  LINK_NATIVE_PROPERTY( APawn, UnderWaterTime );
  LINK_NATIVE_PROPERTY( APawn, AttitudeToPlayer );
  LINK_NATIVE_PROPERTY( APawn, Intelligence );
  LINK_NATIVE_PROPERTY( APawn, Skill );
  LINK_NATIVE_PROPERTY( APawn, SpecialGoal );
  LINK_NATIVE_PROPERTY( APawn, SpecialPause );
  LINK_NATIVE_PROPERTY( APawn, Noise1Spot );
  LINK_NATIVE_PROPERTY( APawn, Noise1Time );
  LINK_NATIVE_PROPERTY( APawn, Noise1Other );
  LINK_NATIVE_PROPERTY( APawn, Noise1Loudness );
  LINK_NATIVE_PROPERTY( APawn, Noise2Spot );
  LINK_NATIVE_PROPERTY( APawn, Noise2Time );
  LINK_NATIVE_PROPERTY( APawn, Noise2Other );
  LINK_NATIVE_PROPERTY( APawn, Noise2Loudness );
  LINK_NATIVE_PROPERTY( APawn, LastPainSound );
  LINK_NATIVE_PROPERTY( APawn, NextPawn );
  LINK_NATIVE_PROPERTY( APawn, HitSound1 );
  LINK_NATIVE_PROPERTY( APawn, HitSound2 );
  LINK_NATIVE_PROPERTY( APawn, Land );
  LINK_NATIVE_PROPERTY( APawn, Die );
  LINK_NATIVE_PROPERTY( APawn, WaterStep );
  LINK_NATIVE_PROPERTY( APawn, bZoom );
  LINK_NATIVE_PROPERTY( APawn, bRun );
  LINK_NATIVE_PROPERTY( APawn, bLook );
  LINK_NATIVE_PROPERTY( APawn, bDuck );
  LINK_NATIVE_PROPERTY( APawn, bSnapLevel );
  LINK_NATIVE_PROPERTY( APawn, bStrafe );
  LINK_NATIVE_PROPERTY( APawn, bFire );
  LINK_NATIVE_PROPERTY( APawn, bAltFire );
  LINK_NATIVE_PROPERTY( APawn, bFreeLook );
  LINK_NATIVE_PROPERTY( APawn, bExtra0 );
  LINK_NATIVE_PROPERTY( APawn, bExtra1 );
  LINK_NATIVE_PROPERTY( APawn, bExtra2 );
  LINK_NATIVE_PROPERTY( APawn, bExtra3 );
  LINK_NATIVE_PROPERTY( APawn, CombatStyle );
  LINK_NATIVE_PROPERTY( APawn, Home );
  LINK_NATIVE_PROPERTY( APawn, NextState );
  LINK_NATIVE_PROPERTY( APawn, NextLabel );
  LINK_NATIVE_PROPERTY( APawn, SoundDampening );
  LINK_NATIVE_PROPERTY( APawn, DamageScaling );
  LINK_NATIVE_PROPERTY( APawn, AlarmTag );
  LINK_NATIVE_PROPERTY( APawn, SharedAlarmTag );
  LINK_NATIVE_PROPERTY( APawn, CarriedDecoration );
  LINK_NATIVE_PROPERTY( APawn, PlayerRestartState );
  LINK_NATIVE_PROPERTY( APawn, MenuName );
  LINK_NATIVE_PROPERTY( APawn, MenuNameDative );
  LINK_NATIVE_PROPERTY( APawn, NameArticle );
  LINK_NATIVE_PROPERTY( APawn, VoicePitch );
  LINK_NATIVE_PROPERTY( APawn, VoiceType );
  LINK_NATIVE_PROPERTY( APawn, OldMessageTime );
  LINK_NATIVE_ARRAY   ( APawn, RouteCache );
  LINK_NATIVE_PROPERTY( APawn, PlayerReplicationInfoClass );
  LINK_NATIVE_PROPERTY( APawn, PlayerReplicationInfo );
  LINK_NATIVE_PROPERTY( APawn, BleedingActor );
  LINK_NATIVE_PROPERTY( APawn, SightDistanceMulti );
  LINK_NATIVE_PROPERTY( APawn, Shadow );
  LINK_NATIVE_PROPERTY( APawn, Pawn_BloodsprayClass );
  LINK_NATIVE_PROPERTY( APawn, Pawn_BleedingClass );
  LINK_NATIVE_PROPERTY( APawn, bIsSpeaking );
  LINK_NATIVE_PROPERTY( APawn, bWasSpeaking );
  LINK_NATIVE_PROPERTY( APawn, LastPhoneme );
  LINK_NATIVE_PROPERTY( APawn, NextPhoneme );
  LINK_NATIVE_PROPERTY( APawn, SightCheckType );
  LINK_NATIVE_PROPERTY( APawn, BeaconOffset );
  LINK_NATIVE_PROPERTY( APawn, MaxFrobDistance );
  LINK_NATIVE_PROPERTY( APawn, FrobTarget );
END_PROPERTY_LINK()

