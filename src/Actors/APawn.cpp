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

BEGIN_PROPERTY_LINK( APawn, 289 )
  LINK_NATIVE_PROPERTY( bBehindView );
  LINK_NATIVE_PROPERTY( bIsPlayer );
  LINK_NATIVE_PROPERTY( bJustLanded );
  LINK_NATIVE_PROPERTY( bUpAndOut );
  LINK_NATIVE_PROPERTY( bIsWalking );
  LINK_NATIVE_PROPERTY( bHitSlopedWall );
  LINK_NATIVE_PROPERTY( bNeverSwitchOnPickup );
  LINK_NATIVE_PROPERTY( bWarping );
  LINK_NATIVE_PROPERTY( bUpdatingDisplay );
  LINK_NATIVE_PROPERTY( bClientSimFall );
  LINK_NATIVE_PROPERTY( bClientTick );
  LINK_NATIVE_PROPERTY( bUseNoWalkInAir );
  LINK_NATIVE_PROPERTY( bNoStopAtLedge );
  LINK_NATIVE_PROPERTY( bPostRender2D );
  LINK_NATIVE_PROPERTY( bCanStrafe );
  LINK_NATIVE_PROPERTY( bFixedStart );
  LINK_NATIVE_PROPERTY( bReducedSpeed );
  LINK_NATIVE_PROPERTY( bCanJump );
  LINK_NATIVE_PROPERTY( bCanWalk );
  LINK_NATIVE_PROPERTY( bCanSwim );
  LINK_NATIVE_PROPERTY( bCanFly );
  LINK_NATIVE_PROPERTY( bCanOpenDoors );
  LINK_NATIVE_PROPERTY( bCanDoSpecial );
  LINK_NATIVE_PROPERTY( bDrowning );
  LINK_NATIVE_PROPERTY( bLOSFlag );
  LINK_NATIVE_PROPERTY( bFromWall );
  LINK_NATIVE_PROPERTY( bHunting );
  LINK_NATIVE_PROPERTY( bAvoidLedges );
  LINK_NATIVE_PROPERTY( bStopAtLedges );
  LINK_NATIVE_PROPERTY( bJumpOffPawn );
  LINK_NATIVE_PROPERTY( bShootSpecial );
  LINK_NATIVE_PROPERTY( bAutoActivate );
  LINK_NATIVE_PROPERTY( bIsHuman );
  LINK_NATIVE_PROPERTY( bIsFemale );
  LINK_NATIVE_PROPERTY( bIsMultiSkinned );
  LINK_NATIVE_PROPERTY( bCountJumps );
  LINK_NATIVE_PROPERTY( bAdvancedTactics );
  LINK_NATIVE_PROPERTY( bViewTarget );
  LINK_NATIVE_PROPERTY( SelectionMesh );
  LINK_NATIVE_PROPERTY( SpecialMesh );
  LINK_NATIVE_PROPERTY( bEnhancedSightCheck );
  LINK_NATIVE_PROPERTY( bRepHealth );
  LINK_NATIVE_PROPERTY( bDoAutoSerpentine );
  LINK_NATIVE_PROPERTY( bIsCrawler );
  LINK_NATIVE_PROPERTY( bIsBleeding );
  LINK_NATIVE_PROPERTY( bIsAmbientCreature );
  LINK_NATIVE_PROPERTY( LastAnchor );
  LINK_NATIVE_PROPERTY( SightCounter );
  LINK_NATIVE_PROPERTY( PainTime );
  LINK_NATIVE_PROPERTY( SpeechTime );
  LINK_NATIVE_PROPERTY( AvgPhysicsTime );
  LINK_NATIVE_PROPERTY( FootRegion );
  LINK_NATIVE_PROPERTY( HeadRegion );
  LINK_NATIVE_PROPERTY( MoveTimer );
  LINK_NATIVE_PROPERTY( MoveTarget );
  LINK_NATIVE_PROPERTY( FaceTarget );
  LINK_NATIVE_PROPERTY( Destination );
  LINK_NATIVE_PROPERTY( Focus );
  LINK_NATIVE_PROPERTY( DesiredSpeed );
  LINK_NATIVE_PROPERTY( MaxDesiredSpeed );
  LINK_NATIVE_PROPERTY( MeleeRange );
  LINK_NATIVE_PROPERTY( SerpentineDist );
  LINK_NATIVE_PROPERTY( SerpentineTime );
  LINK_NATIVE_PROPERTY( MovementStart );
  LINK_NATIVE_PROPERTY( GroundSpeed );
  LINK_NATIVE_PROPERTY( WaterSpeed );
  LINK_NATIVE_PROPERTY( AirSpeed );
  LINK_NATIVE_PROPERTY( AccelRate );
  LINK_NATIVE_PROPERTY( JumpZ );
  LINK_NATIVE_PROPERTY( MaxStepHeight );
  LINK_NATIVE_PROPERTY( AirControl );
  LINK_NATIVE_PROPERTY( MinHitWall );
  LINK_NATIVE_PROPERTY( Visibility );
  LINK_NATIVE_PROPERTY( Alertness );
  LINK_NATIVE_PROPERTY( Stimulus );
  LINK_NATIVE_PROPERTY( SightRadius );
  LINK_NATIVE_PROPERTY( PeripheralVision );
  LINK_NATIVE_PROPERTY( HearingThreshold );
  LINK_NATIVE_PROPERTY( HuntOffDistance );
  LINK_NATIVE_PROPERTY( LastSeenPos );
  LINK_NATIVE_PROPERTY( LastSeeingPos );
  LINK_NATIVE_PROPERTY( LastSeenTime );
  LINK_NATIVE_PROPERTY( Enemy );
  LINK_NATIVE_PROPERTY( Weapon );
  LINK_NATIVE_PROPERTY( PendingWeapon );
  LINK_NATIVE_PROPERTY( SelectedItem );
  LINK_NATIVE_PROPERTY( LastDamageInstigator );
  LINK_NATIVE_PROPERTY( LastDamageHitLocation );
  LINK_NATIVE_PROPERTY( LastDamageMomentum );
  LINK_NATIVE_PROPERTY( LastDamageType );
  LINK_NATIVE_PROPERTY( LastDamageTime );
  LINK_NATIVE_PROPERTY( bLastDamageSpawnedBlood );
  LINK_NATIVE_PROPERTY( ViewRotation );
  LINK_NATIVE_PROPERTY( WalkBob );
  LINK_NATIVE_PROPERTY( BaseEyeHeight );
  LINK_NATIVE_PROPERTY( EyeHeight );
  LINK_NATIVE_PROPERTY( Floor );
  LINK_NATIVE_PROPERTY( SplashTime );
  LINK_NATIVE_PROPERTY( OrthoZoom );
  LINK_NATIVE_PROPERTY( FovAngle );
  LINK_NATIVE_PROPERTY( DieCount );
  LINK_NATIVE_PROPERTY( ItemCount );
  LINK_NATIVE_PROPERTY( KillCount );
  LINK_NATIVE_PROPERTY( SecretCount );
  LINK_NATIVE_PROPERTY( Spree );
  LINK_NATIVE_PROPERTY( Health );
  LINK_NATIVE_PROPERTY( ReducedDamageType );
  LINK_NATIVE_PROPERTY( ReducedDamagePct );
  LINK_NATIVE_PROPERTY( DropWhenKilled );
  LINK_NATIVE_PROPERTY( UnderWaterTime );
  LINK_NATIVE_PROPERTY( AttitudeToPlayer );
  LINK_NATIVE_PROPERTY( Intelligence );
  LINK_NATIVE_PROPERTY( Skill );
  LINK_NATIVE_PROPERTY( SpecialGoal );
  LINK_NATIVE_PROPERTY( SpecialPause );
  LINK_NATIVE_PROPERTY( Noise1Spot );
  LINK_NATIVE_PROPERTY( Noise1Time );
  LINK_NATIVE_PROPERTY( Noise1Other );
  LINK_NATIVE_PROPERTY( Noise1Loudness );
  LINK_NATIVE_PROPERTY( Noise2Spot );
  LINK_NATIVE_PROPERTY( Noise2Time );
  LINK_NATIVE_PROPERTY( Noise2Other );
  LINK_NATIVE_PROPERTY( Noise2Loudness );
  LINK_NATIVE_PROPERTY( LastPainSound );
  LINK_NATIVE_PROPERTY( NextPawn );
  LINK_NATIVE_PROPERTY( HitSound1 );
  LINK_NATIVE_PROPERTY( HitSound2 );
  LINK_NATIVE_PROPERTY( Land );
  LINK_NATIVE_PROPERTY( Die );
  LINK_NATIVE_PROPERTY( WaterStep );
  LINK_NATIVE_PROPERTY( bZoom );
  LINK_NATIVE_PROPERTY( bRun );
  LINK_NATIVE_PROPERTY( bLook );
  LINK_NATIVE_PROPERTY( bDuck );
  LINK_NATIVE_PROPERTY( bSnapLevel );
  LINK_NATIVE_PROPERTY( bStrafe );
  LINK_NATIVE_PROPERTY( bFire );
  LINK_NATIVE_PROPERTY( bAltFire );
  LINK_NATIVE_PROPERTY( bFreeLook );
  LINK_NATIVE_PROPERTY( bExtra0 );
  LINK_NATIVE_PROPERTY( bExtra1 );
  LINK_NATIVE_PROPERTY( bExtra2 );
  LINK_NATIVE_PROPERTY( bExtra3 );
  LINK_NATIVE_PROPERTY( CombatStyle );
  LINK_NATIVE_PROPERTY( Home );
  LINK_NATIVE_PROPERTY( NextState );
  LINK_NATIVE_PROPERTY( NextLabel );
  LINK_NATIVE_PROPERTY( SoundDampening );
  LINK_NATIVE_PROPERTY( DamageScaling );
  LINK_NATIVE_PROPERTY( AlarmTag );
  LINK_NATIVE_PROPERTY( SharedAlarmTag );
  LINK_NATIVE_PROPERTY( CarriedDecoration );
  LINK_NATIVE_PROPERTY( PlayerRestartState );
  LINK_NATIVE_PROPERTY( MenuName );
  LINK_NATIVE_PROPERTY( MenuNameDative );
  LINK_NATIVE_PROPERTY( NameArticle );
  LINK_NATIVE_PROPERTY( VoicePitch );
  LINK_NATIVE_PROPERTY( VoiceType );
  LINK_NATIVE_PROPERTY( OldMessageTime );
  LINK_NATIVE_ARRAY   ( RouteCache );
  LINK_NATIVE_PROPERTY( PlayerReplicationInfoClass );
  LINK_NATIVE_PROPERTY( PlayerReplicationInfo );
  LINK_NATIVE_PROPERTY( BleedingActor );
  LINK_NATIVE_PROPERTY( SightDistanceMulti );
  LINK_NATIVE_PROPERTY( Shadow );
  LINK_NATIVE_PROPERTY( Pawn_BloodsprayClass );
  LINK_NATIVE_PROPERTY( Pawn_BleedingClass );
  LINK_NATIVE_PROPERTY( bIsSpeaking );
  LINK_NATIVE_PROPERTY( bWasSpeaking );
  LINK_NATIVE_PROPERTY( LastPhoneme );
  LINK_NATIVE_PROPERTY( NextPhoneme );
  LINK_NATIVE_PROPERTY( SightCheckType );
  LINK_NATIVE_PROPERTY( BeaconOffset );
  LINK_NATIVE_PROPERTY( MaxFrobDistance );
  LINK_NATIVE_PROPERTY( FrobTarget );
  LINK_NATIVE_PROPERTY( bCanGlide );
  LINK_NATIVE_PROPERTY( HealthHead );
  LINK_NATIVE_PROPERTY( HealthTorso );
  LINK_NATIVE_PROPERTY( HealthLegLeft );
  LINK_NATIVE_PROPERTY( HealthLegRight );
  LINK_NATIVE_PROPERTY( HealthArmLeft );
  LINK_NATIVE_PROPERTY( HealthArmRight );
  LINK_NATIVE_ARRAY   ( AnimTimer );
  LINK_NATIVE_PROPERTY( bOnFire );
  LINK_NATIVE_PROPERTY( BurnTimer );
  LINK_NATIVE_PROPERTY( AIHorizontalFov );
  LINK_NATIVE_PROPERTY( AspectRatio );
  LINK_NATIVE_PROPERTY( AngularResolution );
  LINK_NATIVE_PROPERTY( MinAngularSize );
  LINK_NATIVE_PROPERTY( VisibilityThreshold );
  LINK_NATIVE_PROPERTY( SmellThreshold );
  LINK_NATIVE_PROPERTY( Alliance );
  LINK_NATIVE_PROPERTY( AIAddViewRotation );
  LINK_NATIVE_PROPERTY( CarcassType );
  LINK_NATIVE_PROPERTY( bCanGrabEdges );
  LINK_NATIVE_PROPERTY( bAlignToFloor );
  LINK_NATIVE_PROPERTY( AmbientSoundTime );
  LINK_NATIVE_PROPERTY( CombatRange );
  LINK_NATIVE_PROPERTY( bStopMoveIfCombatRange );
  LINK_NATIVE_PROPERTY( WalkingSpeed );
  LINK_NATIVE_PROPERTY( MovementSpeed );
  LINK_NATIVE_PROPERTY( bHurrying );
  LINK_NATIVE_PROPERTY( SpeedScale );
  LINK_NATIVE_PROPERTY( ClassID );
  LINK_NATIVE_PROPERTY( Shield );
  LINK_NATIVE_PROPERTY( StartEvent );
  LINK_NATIVE_PROPERTY( CurrentSkin );
  LINK_NATIVE_PROPERTY( ViewLocation );
  LINK_NATIVE_PROPERTY( GrabLocationUp );
  LINK_NATIVE_PROPERTY( GrabLocationIn );
  LINK_NATIVE_PROPERTY( Strength );
  LINK_NATIVE_PROPERTY( RunePower );
  LINK_NATIVE_PROPERTY( MaxHealth );
  LINK_NATIVE_PROPERTY( MaxStrength );
  LINK_NATIVE_PROPERTY( MaxPower );
  LINK_NATIVE_PROPERTY( BodyPartHealth );
  LINK_NATIVE_PROPERTY( GibCount );
  LINK_NATIVE_PROPERTY( GibClass );
  LINK_NATIVE_PROPERTY( PainDelay );
  LINK_NATIVE_PROPERTY( bGibbable );
  LINK_NATIVE_PROPERTY( bInvisible );
  LINK_NATIVE_PROPERTY( HitSound3 );
  LINK_NATIVE_PROPERTY( Die2 );
  LINK_NATIVE_PROPERTY( Die3 );
  LINK_NATIVE_PROPERTY( GibSound );
  LINK_NATIVE_PROPERTY( FootstepVolume );
  LINK_NATIVE_PROPERTY( LandGrunt );
  LINK_NATIVE_PROPERTY( FootStepWood );
  LINK_NATIVE_PROPERTY( FootStepMetal );
  LINK_NATIVE_PROPERTY( FootStepStone );
  LINK_NATIVE_PROPERTY( FootStepFlesh );
  LINK_NATIVE_PROPERTY( FootStepIce );
  LINK_NATIVE_PROPERTY( FootStepEarth );
  LINK_NATIVE_PROPERTY( FootStepSnow );
  LINK_NATIVE_PROPERTY( FootStepBreakableWood );
  LINK_NATIVE_PROPERTY( FootStepBreakableStone );
  LINK_NATIVE_PROPERTY( FootStepWater );
  LINK_NATIVE_PROPERTY( FootStepMud );
  LINK_NATIVE_PROPERTY( FootStepLava );
  LINK_NATIVE_PROPERTY( LandSoundWood );
  LINK_NATIVE_PROPERTY( LandSoundMetal );
  LINK_NATIVE_PROPERTY( LandSoundStone );
  LINK_NATIVE_PROPERTY( LandSoundFlesh );
  LINK_NATIVE_PROPERTY( LandSoundIce );
  LINK_NATIVE_PROPERTY( LandSoundSnow );
  LINK_NATIVE_PROPERTY( LandSoundEarth );
  LINK_NATIVE_PROPERTY( LandSoundBreakableWood );
  LINK_NATIVE_PROPERTY( LandSoundBreakableStone );
  LINK_NATIVE_PROPERTY( LandSoundWater );
  LINK_NATIVE_PROPERTY( LandSoundMud );
  LINK_NATIVE_PROPERTY( LandSoundLava );
  LINK_NATIVE_PROPERTY( NextStateAfterPain );
  LINK_NATIVE_PROPERTY( WeaponJoint );
  LINK_NATIVE_PROPERTY( ShieldJoint );
  LINK_NATIVE_PROPERTY( StabJoint );
  LINK_NATIVE_PROPERTY( bCanLook );
  LINK_NATIVE_PROPERTY( MaxBodyAngle );
  LINK_NATIVE_PROPERTY( MaxHeadAngle );
  LINK_NATIVE_PROPERTY( bRotateHead );
  LINK_NATIVE_PROPERTY( bRotateTorso );
  LINK_NATIVE_PROPERTY( bHeadLookUpDouble );
  LINK_NATIVE_PROPERTY( LookDegPerSec );
  LINK_NATIVE_PROPERTY( LookTarget );
  LINK_NATIVE_PROPERTY( LookSpot );
  LINK_NATIVE_PROPERTY( LookAngle );
  LINK_NATIVE_PROPERTY( targetangle );
  LINK_NATIVE_PROPERTY( bOverrideLookTarget );
  LINK_NATIVE_PROPERTY( MouthRot );
  LINK_NATIVE_PROPERTY( DesiredMouthRot );
  LINK_NATIVE_PROPERTY( MouthRotRate );
  LINK_NATIVE_PROPERTY( MaxMouthRot );
  LINK_NATIVE_PROPERTY( MaxMouthRotRate );
  LINK_NATIVE_PROPERTY( FootprintClass );
  LINK_NATIVE_PROPERTY( WetFootprintClass );
  LINK_NATIVE_PROPERTY( BloodyFootprintClass );
  LINK_NATIVE_PROPERTY( LFootJoint );
  LINK_NATIVE_PROPERTY( RFootJoint );
  LINK_NATIVE_PROPERTY( bFootsteps );
  LINK_NATIVE_PROPERTY( WaterSteps );
  LINK_NATIVE_PROPERTY( BloodSteps );
  LINK_NATIVE_PROPERTY( bSwingingHigh );
  LINK_NATIVE_PROPERTY( bSwingingLow );
  LINK_NATIVE_PROPERTY( DeathRadius );
  LINK_NATIVE_PROPERTY( DeathHeight );
  LINK_NATIVE_PROPERTY( bAllowStandOn );
  LINK_NATIVE_PROPERTY( bLeadEnemy );
  LINK_NATIVE_PROPERTY( UninterruptedAnim );
  LINK_NATIVE_PROPERTY( UseActor );
  LINK_NATIVE_PROPERTY( SkinDefaultText );
END_PROPERTY_LINK()

