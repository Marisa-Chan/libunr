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
 * AActor.cpp - Actor class for tangible game objects
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Actors/AActor.h"

AActor::AActor()
  : UObject()
{
}

AActor::~AActor()
{
}

void AActor::PostDefaultLoad()
{
  Tag = Pkg->GetGlobalName( Class->Export->ObjectName );
}

ABlockingActor::ABlockingActor()
  : AActor()
{
}

ABlockingActor::~ABlockingActor()
{
}

ACollisionPlane::ACollisionPlane()
  : AKeypoint()
{
}

ACollisionPlane::~ACollisionPlane()
{
}

AMutator::AMutator()
  : AActor()
{
}

AMutator::~AMutator()
{
}

AMenu::AMenu()
  : AActor()
{
}

AMenu::~AMenu()
{
}

ADecoration::ADecoration()
  : AActor()
{
}

ADecoration::~ADecoration()
{
}

ACarcass::ACarcass()
  : ADecoration()
{
}

ACarcass::~ACarcass()
{
}

AInfo::AInfo()
  : AActor()
{
}

AInfo::~AInfo()
{
}

ASavedMove::ASavedMove()
  : AInfo()
{
}

ASavedMove::~ASavedMove()
{
}

AKeypoint::AKeypoint()
  : AActor()
{
}

AKeypoint::~AKeypoint()
{
}

ALocationID::ALocationID()
  : AKeypoint()
{
}

ALocationID::~ALocationID()
{
}

AInterpolationPoint::AInterpolationPoint()
  : AKeypoint()
{
}

AInterpolationPoint::~AInterpolationPoint()
{
}

ALight::ALight()
  : AActor()
{
}

ALight::~ALight()
{
}

ADynamicCorona::ADynamicCorona()
  : ALight()
{
}

ADynamicCorona::~ADynamicCorona()
{
}

ASunlightCorona::ASunlightCorona()
  : ALight()
{
}

ASunlightCorona::~ASunlightCorona()
{
}

AProjectile::AProjectile()
  : AActor()
{
}

AProjectile::~AProjectile()
{
}

ASpawnNotify::ASpawnNotify()
  : AActor()
{
}

ASpawnNotify::~ASpawnNotify()
{
}

AStaticMeshActor::AStaticMeshActor()
  : AActor()
{
}

AStaticMeshActor::~AStaticMeshActor()
{
}

ATriggers::ATriggers()
  : AActor()
{
}

ATriggers::~ATriggers()
{
}

ADispatcher::ADispatcher()
  : ATriggers()
{
}

ADispatcher::~ADispatcher()
{
}

ATrigger::ATrigger()
  : ATriggers()
{
}

ATrigger::~ATrigger()
{
}

AVisibilityNotify::AVisibilityNotify()
  : AInfo()
{
}

AVisibilityNotify::~AVisibilityNotify()
{
}

IMPLEMENT_NATIVE_CLASS( AActor );
IMPLEMENT_NATIVE_CLASS( ABlockingActor );
IMPLEMENT_NATIVE_CLASS( ACollisionPlane );
IMPLEMENT_NATIVE_CLASS( ACarcass );
IMPLEMENT_NATIVE_CLASS( ADecoration );
IMPLEMENT_NATIVE_CLASS( ADispatcher );
IMPLEMENT_NATIVE_CLASS( ADynamicCorona );
IMPLEMENT_NATIVE_CLASS( AInfo );
IMPLEMENT_NATIVE_CLASS( AInterpolationPoint );
IMPLEMENT_NATIVE_CLASS( AKeypoint );
IMPLEMENT_NATIVE_CLASS( ALocationID );
IMPLEMENT_NATIVE_CLASS( ALight );
IMPLEMENT_NATIVE_CLASS( AMenu );
IMPLEMENT_NATIVE_CLASS( AMutator );
IMPLEMENT_NATIVE_CLASS( AProjectile );
IMPLEMENT_NATIVE_CLASS( ASavedMove );
IMPLEMENT_NATIVE_CLASS( AStaticMeshActor );
IMPLEMENT_NATIVE_CLASS( ASpawnNotify );
IMPLEMENT_NATIVE_CLASS( ASunlightCorona );
IMPLEMENT_NATIVE_CLASS( ATriggers );
IMPLEMENT_NATIVE_CLASS( ATrigger );
IMPLEMENT_NATIVE_CLASS( AVisibilityNotify );

BEGIN_PROPERTY_LINK( ABlockingActor, 4 )
  LINK_NATIVE_PROPERTY( bBlockSubClasses );
  LINK_NATIVE_PROPERTY( BlockingClasses );
  LINK_NATIVE_PROPERTY( IgnoreSubClasses );
  LINK_NATIVE_PROPERTY( ScriptBlocking );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ACollisionPlane, 3 )
  LINK_NATIVE_ARRAY   ( CollisionBounds );
  LINK_NATIVE_PROPERTY( bBlockZeroExtent );
  LINK_NATIVE_PROPERTY( bBlockNonZeroExtent );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ADecoration, 15 )
  LINK_NATIVE_PROPERTY( bPushable ); 
  LINK_NATIVE_PROPERTY( bOnlyTriggerable );
  LINK_NATIVE_PROPERTY( bSplash );
  LINK_NATIVE_PROPERTY( bBobbing );
  LINK_NATIVE_PROPERTY( bWasCarried );
  LINK_NATIVE_PROPERTY( bPushSoundPlaying );
  LINK_NATIVE_PROPERTY( EffectWhenDestroyed );
  LINK_NATIVE_PROPERTY( PushSound );
  LINK_NATIVE_PROPERTY( NumLandings );
  LINK_NATIVE_PROPERTY( Contents );
  LINK_NATIVE_PROPERTY( Content2 );
  LINK_NATIVE_PROPERTY( Content3 );
  LINK_NATIVE_PROPERTY( EndPushSound );
  LINK_NATIVE_PROPERTY( Shadow );
  LINK_NATIVE_PROPERTY( BaseEyeHeight );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ACarcass, 9 )
  LINK_NATIVE_PROPERTY( bPlayerCarcass );
  LINK_NATIVE_PROPERTY( Flies );
  LINK_NATIVE_PROPERTY( Rats );
  LINK_NATIVE_PROPERTY( bReducedHeight );
  LINK_NATIVE_PROPERTY( bDecorative );
  LINK_NATIVE_PROPERTY( bSlidingCarcass );
  LINK_NATIVE_PROPERTY( CumulativeDamage );
  LINK_NATIVE_PROPERTY( PlayerOwner );
  LINK_NATIVE_PROPERTY( Bugs );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AProjectile, 11 )
  LINK_NATIVE_PROPERTY( Speed );
  LINK_NATIVE_PROPERTY( MaxSpeed );
  LINK_NATIVE_PROPERTY( Damage );
  LINK_NATIVE_PROPERTY( MomentumTransfer );
  LINK_NATIVE_PROPERTY( MyDamageType );
  LINK_NATIVE_PROPERTY( SpawnSound );
  LINK_NATIVE_PROPERTY( ImpactSound );
  LINK_NATIVE_PROPERTY( MiscSound );
  LINK_NATIVE_PROPERTY( ExploWallOut );
  LINK_NATIVE_PROPERTY( ExplosionDecal );
  LINK_NATIVE_PROPERTY( LastHitActor );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ASpawnNotify, 2 )
  LINK_NATIVE_PROPERTY( ActorClass );
  LINK_NATIVE_PROPERTY( Next );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AInfo, 0 )
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ASavedMove, 18 )
  LINK_NATIVE_PROPERTY( NextMove );
  LINK_NATIVE_PROPERTY( TimeStamp );
  LINK_NATIVE_PROPERTY( Delta );
  LINK_NATIVE_PROPERTY( DodgeMove );
  LINK_NATIVE_PROPERTY( bRun );
  LINK_NATIVE_PROPERTY( bDuck );
  LINK_NATIVE_PROPERTY( bPressedJump );
  LINK_NATIVE_PROPERTY( bSent );
  LINK_NATIVE_PROPERTY( bIsReducedCrouch );
  LINK_NATIVE_PROPERTY( bFire );
  LINK_NATIVE_PROPERTY( bAltFire );
  LINK_NATIVE_PROPERTY( bForceFire );
  LINK_NATIVE_PROPERTY( bForceAltFire );
  LINK_NATIVE_PROPERTY( CustomPlayerState );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AKeypoint, 0 )
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ALocationID, 3 )
  LINK_NATIVE_PROPERTY( LocationName );
  LINK_NATIVE_PROPERTY( Radius );
  LINK_NATIVE_PROPERTY( NextLocation );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ALight, 2 )
  LINK_NATIVE_PROPERTY( ShadowMoverTags );
  LINK_NATIVE_PROPERTY( MoverShadowSurfs );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ADynamicCorona, 9 )
  LINK_NATIVE_PROPERTY( bDirectionalCorona );
  LINK_NATIVE_PROPERTY( bBrightnessDependent );
  LINK_NATIVE_PROPERTY( MaxSize );
  LINK_NATIVE_PROPERTY( DisplayDistance );
  LINK_NATIVE_PROPERTY( CoronaSize );
  LINK_NATIVE_PROPERTY( FadeOutDistance );
  LINK_NATIVE_PROPERTY( CoronaColor );
  LINK_NATIVE_PROPERTY( CloseDistanceColor );
  LINK_NATIVE_PROPERTY( CoronaAttenuate );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ASunlightCorona, 11 )
  LINK_NATIVE_PROPERTY( bMustMatchZone );
  LINK_NATIVE_PROPERTY( bRenderLensFlares );
  LINK_NATIVE_PROPERTY( SunlightColor );
  LINK_NATIVE_PROPERTY( BlindingScale );
  LINK_NATIVE_PROPERTY( BlindingFOV );
  LINK_NATIVE_PROPERTY( MaxSkyDistance );
  LINK_NATIVE_PROPERTY( SunFadeTimeScale );
  LINK_NATIVE_PROPERTY( SunlightTexture );
  LINK_NATIVE_ARRAY   ( LensFlares );
  LINK_NATIVE_PROPERTY( FlaresDisplayFov );
  LINK_NATIVE_PROPERTY( bUScriptRenderHandler );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AInterpolationPoint, 12 )
  LINK_NATIVE_PROPERTY( Position );
  LINK_NATIVE_PROPERTY( RateModifier );
  LINK_NATIVE_PROPERTY( GameSpeedModifier );
  LINK_NATIVE_PROPERTY( FovModifier );
  LINK_NATIVE_PROPERTY( bEndOfPath );
  LINK_NATIVE_PROPERTY( bSkipNextPath );
  LINK_NATIVE_PROPERTY( ScreenFlashScale );
  LINK_NATIVE_PROPERTY( ScreenFlashFog );
  LINK_NATIVE_PROPERTY( Prev );
  LINK_NATIVE_PROPERTY( Next );
  LINK_NATIVE_PROPERTY( bInterpInstant );
  LINK_NATIVE_PROPERTY( bSplineThruPoints );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AMenu, 18 )
  LINK_NATIVE_PROPERTY( ParentMenu );
  LINK_NATIVE_PROPERTY( Selection );
  LINK_NATIVE_PROPERTY( MenuLength );
  LINK_NATIVE_PROPERTY( bConfigChanged );
  LINK_NATIVE_PROPERTY( bExitAllMenus );
  LINK_NATIVE_PROPERTY( PlayerOwner );
  LINK_NATIVE_ARRAY   ( HelpMessage );
  LINK_NATIVE_ARRAY   ( MenuList );
  LINK_NATIVE_PROPERTY( LeftString );
  LINK_NATIVE_PROPERTY( RightString );
  LINK_NATIVE_PROPERTY( CenterString );
  LINK_NATIVE_PROPERTY( EnabledString );
  LINK_NATIVE_PROPERTY( DisabledString );
  LINK_NATIVE_PROPERTY( MenuTitle );
  LINK_NATIVE_PROPERTY( YesString );
  LINK_NATIVE_PROPERTY( NoString );
  LINK_NATIVE_PROPERTY( OnString );
  LINK_NATIVE_PROPERTY( OffString );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AMutator, 7 )
  LINK_NATIVE_PROPERTY( NextMutator );
  LINK_NATIVE_PROPERTY( NextDamageMutator );
  LINK_NATIVE_PROPERTY( NextMessageMutator );
  LINK_NATIVE_PROPERTY( NextHUDMutator );
  LINK_NATIVE_PROPERTY( bHUDMutator );
  LINK_NATIVE_PROPERTY( DefaultWeapon );
  LINK_NATIVE_PROPERTY( DefaultShield );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AStaticMeshActor, 3 )
  LINK_NATIVE_PROPERTY( StaticLightD );
  LINK_NATIVE_PROPERTY( bBuildStaticLights );
  LINK_NATIVE_PROPERTY( bComputeUnlitColor );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ATriggers, 0 )
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ADispatcher, 4 )
  LINK_NATIVE_PROPERTY( OutEvents );
  LINK_NATIVE_PROPERTY( OutDelays );
  LINK_NATIVE_PROPERTY( bIsLooping );
  LINK_NATIVE_PROPERTY( i );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ATrigger, 14 )
  LINK_NATIVE_PROPERTY( TriggerType );
  LINK_NATIVE_PROPERTY( Message );
  LINK_NATIVE_PROPERTY( bTriggerOnceOnly );
  LINK_NATIVE_PROPERTY( bInitiallyActive );
  LINK_NATIVE_PROPERTY( BACKUP_InitActive );
  LINK_NATIVE_PROPERTY( ClassProximityType );
  LINK_NATIVE_PROPERTY( RepeatTriggerTime );
  LINK_NATIVE_PROPERTY( ReTriggerDelay );
  LINK_NATIVE_PROPERTY( TriggerTime );
  LINK_NATIVE_PROPERTY( DamageThreshold );
  LINK_NATIVE_PROPERTY( TriggerActor );
  LINK_NATIVE_PROPERTY( TriggerActor2 );
  LINK_NATIVE_PROPERTY( SightDistance );
  LINK_NATIVE_PROPERTY( SightAngle );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AVisibilityNotify, 1 )
  LINK_NATIVE_PROPERTY( NextNotify );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AActor, 317 )
  LINK_NATIVE_PROPERTY( bStatic );
  LINK_NATIVE_PROPERTY( bHidden );
  LINK_NATIVE_PROPERTY( bNoDelete );
  LINK_NATIVE_PROPERTY( bAnimFinished );
  LINK_NATIVE_PROPERTY( bAnimLoop );
  LINK_NATIVE_PROPERTY( bAnimNotify );
  LINK_NATIVE_PROPERTY( bAnimByOwner );
  LINK_NATIVE_PROPERTY( bDeleteMe );
  LINK_NATIVE_PROPERTY( bAssimilated );
  LINK_NATIVE_PROPERTY( bTicked );
  LINK_NATIVE_PROPERTY( bLightChanged );
  LINK_NATIVE_PROPERTY( bDynamicLight );
  LINK_NATIVE_PROPERTY( bTimerLoop );
  LINK_NATIVE_PROPERTY( bNetNotify );
  LINK_NATIVE_PROPERTY( bHandleOwnCorona );
  LINK_NATIVE_PROPERTY( bRenderMultiEnviroMaps );
  LINK_NATIVE_PROPERTY( bWorldGeometry );
  LINK_NATIVE_PROPERTY( bUseMeshCollision );
  LINK_NATIVE_PROPERTY( bEditorSelectRender );
  LINK_NATIVE_PROPERTY( bNoDynamicShadowCast );
  LINK_NATIVE_PROPERTY( bIsInOctree );
  LINK_NATIVE_PROPERTY( bProjectorDecal );
  LINK_NATIVE_PROPERTY( bUseLitSprite );
  LINK_NATIVE_PROPERTY( bTrailerPrePivot );
  LINK_NATIVE_PROPERTY( bClientAnim );
  LINK_NATIVE_PROPERTY( bSimFall );
  LINK_NATIVE_PROPERTY( bClientDemoRecording );
  LINK_NATIVE_PROPERTY( bClientDemoNetFunc );
  LINK_NATIVE_PROPERTY( bHiddenEd );
  LINK_NATIVE_PROPERTY( bDirectional );
  LINK_NATIVE_PROPERTY( bSelected );
  LINK_NATIVE_PROPERTY( bMemorized );
  LINK_NATIVE_PROPERTY( bHighlighted );
  LINK_NATIVE_PROPERTY( bEdLocked );
  LINK_NATIVE_PROPERTY( bEdShouldSnap );
  LINK_NATIVE_PROPERTY( bEdSnap );
  LINK_NATIVE_PROPERTY( bTempEditor );
  LINK_NATIVE_PROPERTY( bDifficulty0 );
  LINK_NATIVE_PROPERTY( bDifficulty1 );
  LINK_NATIVE_PROPERTY( bDifficulty2 );
  LINK_NATIVE_PROPERTY( bDifficulty3 );
  LINK_NATIVE_PROPERTY( bSinglePlayer );
  LINK_NATIVE_PROPERTY( bNet );
  LINK_NATIVE_PROPERTY( bNetSpecial );
  LINK_NATIVE_PROPERTY( bScriptInitialized );
  LINK_NATIVE_PROPERTY( bCanTeleport );
  LINK_NATIVE_PROPERTY( bIsSecretGoal );
  LINK_NATIVE_PROPERTY( bIsKillGoal );
  LINK_NATIVE_PROPERTY( bIsItemGoal );
  LINK_NATIVE_PROPERTY( bCollideWhenPlacing );
  LINK_NATIVE_PROPERTY( bTravel );
  LINK_NATIVE_PROPERTY( bMovable );
  LINK_NATIVE_PROPERTY( bHighDetail );
  LINK_NATIVE_PROPERTY( bStasis );
  LINK_NATIVE_PROPERTY( bForceStasis );
  LINK_NATIVE_PROPERTY( bIsPawn );
  LINK_NATIVE_PROPERTY( bNetTemporary );
  LINK_NATIVE_PROPERTY( bNetOptional );
  LINK_NATIVE_PROPERTY( bReplicateInstigator );
  LINK_NATIVE_PROPERTY( bTrailerSameRotation );
  LINK_NATIVE_PROPERTY( bUnlit );
  LINK_NATIVE_PROPERTY( bNoSmooth );
  LINK_NATIVE_PROPERTY( bParticles );
  LINK_NATIVE_PROPERTY( bRandomFrame );
  LINK_NATIVE_PROPERTY( bMeshEnviroMap );
  LINK_NATIVE_PROPERTY( bFilterByVolume );
  LINK_NATIVE_PROPERTY( bMeshCurvy );
  LINK_NATIVE_PROPERTY( bShadowCast );
  LINK_NATIVE_PROPERTY( bOwnerNoSee );
  LINK_NATIVE_PROPERTY( bOnlyOwnerSee );
  LINK_NATIVE_PROPERTY( bIsMover );
  LINK_NATIVE_PROPERTY( bAlwaysRelevant );
  LINK_NATIVE_PROPERTY( bAlwaysTick );
  LINK_NATIVE_PROPERTY( bHurtEntry );
  LINK_NATIVE_PROPERTY( bGameRelevant );
  LINK_NATIVE_PROPERTY( bCarriedItem );
  LINK_NATIVE_PROPERTY( bForcePhysicsUpdate );
  LINK_NATIVE_PROPERTY( bSkipActorReplication );
  LINK_NATIVE_PROPERTY( bRepAnimations );
  LINK_NATIVE_PROPERTY( bRepAmbientSound );
  LINK_NATIVE_PROPERTY( bSimulatedPawnRep );
  LINK_NATIVE_PROPERTY( bRepMesh );
  LINK_NATIVE_PROPERTY( bCollideActors );
  LINK_NATIVE_PROPERTY( bCollideWorld );
  LINK_NATIVE_PROPERTY( bBlockActors );
  LINK_NATIVE_PROPERTY( bBlockPlayers );
  LINK_NATIVE_PROPERTY( bProjTarget );
  LINK_NATIVE_PROPERTY( bPathCollision );
  LINK_NATIVE_PROPERTY( bSpecialLit );
  LINK_NATIVE_PROPERTY( bActorShadows );
  LINK_NATIVE_PROPERTY( bCorona );
  LINK_NATIVE_PROPERTY( bLensFlare );
  LINK_NATIVE_PROPERTY( bDarkLight );
  LINK_NATIVE_PROPERTY( bNetInitial );
  LINK_NATIVE_PROPERTY( bNetOwner );
  LINK_NATIVE_PROPERTY( bNetRelevant );
  LINK_NATIVE_PROPERTY( bNetSee );
  LINK_NATIVE_PROPERTY( bNetHear );
  LINK_NATIVE_PROPERTY( bNetFeel );
  LINK_NATIVE_PROPERTY( bSimulatedPawn );
  LINK_NATIVE_PROPERTY( bDemoRecording );
  LINK_NATIVE_PROPERTY( bBounce );
  LINK_NATIVE_PROPERTY( bFixedRotationDir );
  LINK_NATIVE_PROPERTY( bRotateToDesired );
  LINK_NATIVE_PROPERTY( bInterpolating );
  LINK_NATIVE_PROPERTY( bJustTeleported );
  LINK_NATIVE_PROPERTY( bAlwaysRender );
  LINK_NATIVE_PROPERTY( bIsPlayerPawn );
  LINK_NATIVE_PROPERTY( bIsProjectile );
  LINK_NATIVE_PROPERTY( bZoneNormalLight );
  LINK_NATIVE_PROPERTY( bIsFrobable );
  LINK_NATIVE_PROPERTY( RandomDelayTime );
  LINK_NATIVE_PROPERTY( RandomValue );
  LINK_NATIVE_PROPERTY( LastRandomTime );
  LINK_NATIVE_PROPERTY( LastRenderedTime );
  LINK_NATIVE_PROPERTY( ActorRenderColor );
  LINK_NATIVE_PROPERTY( ActorGUnlitColor );
  LINK_NATIVE_PROPERTY( AmbientGlowColor );
  LINK_NATIVE_PROPERTY( AmbientGlowPulseSpeed );
  LINK_NATIVE_PROPERTY( NormalLightRadius );
  LINK_NATIVE_PROPERTY( CollisionOverride );
  LINK_NATIVE_PROPERTY( MeshInstance );
  LINK_NATIVE_PROPERTY( RelativeLocation );
  LINK_NATIVE_PROPERTY( RelativeRotation );
  LINK_NATIVE_PROPERTY( LightDataPtr );
  LINK_NATIVE_PROPERTY( MeshDataPtr );
  LINK_NATIVE_PROPERTY( DrawScale3D );
  LINK_NATIVE_PROPERTY( ProjectorList );
  LINK_NATIVE_PROPERTY( NetInitialProperties );
  LINK_NATIVE_PROPERTY( RealTouching );
  LINK_NATIVE_PROPERTY( MultiTimers );
  LINK_NATIVE_PROPERTY( Physics );
  LINK_NATIVE_PROPERTY( Role );
  LINK_NATIVE_PROPERTY( RemoteRole );
  LINK_NATIVE_PROPERTY( Owner );
  LINK_NATIVE_PROPERTY( InitialState );
  LINK_NATIVE_PROPERTY( Group );
  LINK_NATIVE_PROPERTY( TimerRate );
  LINK_NATIVE_PROPERTY( TimerCounter );
  LINK_NATIVE_PROPERTY( LifeSpan );
  LINK_NATIVE_PROPERTY( AnimSequence );
  LINK_NATIVE_PROPERTY( AnimFrame );
  LINK_NATIVE_PROPERTY( AnimRate );
  LINK_NATIVE_PROPERTY( TweenRate );
  LINK_NATIVE_PROPERTY( SkelAnim );
  LINK_NATIVE_PROPERTY( LODBias );
  LINK_NATIVE_PROPERTY( DefaultAnimationNotify );
  LINK_NATIVE_PROPERTY( AnimationNotify );
  LINK_NATIVE_PROPERTY( Level );
  LINK_NATIVE_PROPERTY( XLevel );
  LINK_NATIVE_PROPERTY( Tag );
  LINK_NATIVE_PROPERTY( Event );
  LINK_NATIVE_PROPERTY( Target );
  LINK_NATIVE_PROPERTY( Instigator );
  LINK_NATIVE_PROPERTY( Inventory );
  LINK_NATIVE_PROPERTY( Base );
  LINK_NATIVE_PROPERTY( Region );
  LINK_NATIVE_PROPERTY( AttachTag );
  LINK_NATIVE_PROPERTY( StandingCount );
  LINK_NATIVE_PROPERTY( MiscNumber );
  LINK_NATIVE_PROPERTY( LatentByte );
  LINK_NATIVE_PROPERTY( LatentInt );
  LINK_NATIVE_PROPERTY( LatentFloat );
  LINK_NATIVE_PROPERTY( LatentActor );
  LINK_NATIVE_ARRAY   ( Touching );
  LINK_NATIVE_PROPERTY( Deleted );
  LINK_NATIVE_PROPERTY( CollisionTag );
  LINK_NATIVE_PROPERTY( LightingTag );
  LINK_NATIVE_PROPERTY( NetTag );
  LINK_NATIVE_PROPERTY( OtherTag );
  LINK_NATIVE_PROPERTY( ExtraTag );
  LINK_NATIVE_PROPERTY( SpecialTag );
  LINK_NATIVE_PROPERTY( Location );
  LINK_NATIVE_PROPERTY( Rotation );
  LINK_NATIVE_PROPERTY( OldLocation );
  LINK_NATIVE_PROPERTY( ColLocation );
  LINK_NATIVE_PROPERTY( Velocity );
  LINK_NATIVE_PROPERTY( Acceleration );
  LINK_NATIVE_PROPERTY( OddsOfAppearing );
  LINK_NATIVE_PROPERTY( HitActor );
  LINK_NATIVE_PROPERTY( DrawType );
  LINK_NATIVE_PROPERTY( Style );
  LINK_NATIVE_PROPERTY( Sprite );
  LINK_NATIVE_PROPERTY( Texture );
  LINK_NATIVE_PROPERTY( Skin );
  LINK_NATIVE_PROPERTY( Mesh );
  LINK_NATIVE_PROPERTY( ShadowMesh );
  LINK_NATIVE_PROPERTY( Brush );
  LINK_NATIVE_PROPERTY( DrawScale );
  LINK_NATIVE_PROPERTY( PrePivot );
  LINK_NATIVE_PROPERTY( ScaleGlow );
  LINK_NATIVE_PROPERTY( VisibilityRadius );
  LINK_NATIVE_PROPERTY( VisibilityHeight );
  LINK_NATIVE_PROPERTY( AmbientGlow );
  LINK_NATIVE_PROPERTY( Fatness );
  LINK_NATIVE_PROPERTY( SpriteProjForward );
  LINK_NATIVE_ARRAY   ( MultiSkins );
  LINK_NATIVE_PROPERTY( RenderIteratorClass );
  LINK_NATIVE_PROPERTY( RenderInterface );
  LINK_NATIVE_PROPERTY( SoundRadius );
  LINK_NATIVE_PROPERTY( SoundVolume );
  LINK_NATIVE_PROPERTY( SoundPitch );
  LINK_NATIVE_PROPERTY( AmbientSound );
  LINK_NATIVE_PROPERTY( TransientSoundVolume );
  LINK_NATIVE_PROPERTY( TransientSoundRadius );
  LINK_NATIVE_PROPERTY( CollisionRadius );
  LINK_NATIVE_PROPERTY( CollisionHeight );
  LINK_NATIVE_PROPERTY( CollisionWidth );
  LINK_NATIVE_PROPERTY( CollideType );
  LINK_NATIVE_PROPERTY( LightType );
  LINK_NATIVE_PROPERTY( LightEffect );
  LINK_NATIVE_PROPERTY( LightBrightness );
  LINK_NATIVE_PROPERTY( LightHue );
  LINK_NATIVE_PROPERTY( LightSaturation );
  LINK_NATIVE_PROPERTY( LightRadius );
  LINK_NATIVE_PROPERTY( LightPeriod );
  LINK_NATIVE_PROPERTY( LightPhase );
  LINK_NATIVE_PROPERTY( LightCone );
  LINK_NATIVE_PROPERTY( VolumeBrightness );
  LINK_NATIVE_PROPERTY( VolumeRadius );
  LINK_NATIVE_PROPERTY( VolumeFog );
  LINK_NATIVE_PROPERTY( CoronaAlpha );
  LINK_NATIVE_PROPERTY( DodgeDir );
  LINK_NATIVE_PROPERTY( Mass );
  LINK_NATIVE_PROPERTY( Buoyancy );
  LINK_NATIVE_PROPERTY( RotationRate );
  LINK_NATIVE_PROPERTY( DesiredRotation );
  LINK_NATIVE_PROPERTY( PhysAlpha );
  LINK_NATIVE_PROPERTY( PhysRate );
  LINK_NATIVE_PROPERTY( PendingTouch );
  LINK_NATIVE_PROPERTY( AnimLast );
  LINK_NATIVE_PROPERTY( AnimMinRate );
  LINK_NATIVE_PROPERTY( OldAnimRate );
  LINK_NATIVE_PROPERTY( SimAnim );
  LINK_NATIVE_PROPERTY( NetPriority );
  LINK_NATIVE_PROPERTY( NetUpdateFrequency );
  LINK_NATIVE_PROPERTY( bBlockSight );
  LINK_NATIVE_PROPERTY( bDetectable );
  LINK_NATIVE_PROPERTY( bTransient );
  LINK_NATIVE_PROPERTY( bIgnore );
  LINK_NATIVE_PROPERTY( LastRenderTime );
  LINK_NATIVE_PROPERTY( DistanceFromPlayer );
  LINK_NATIVE_ARRAY   ( BlendAnimSequence );
  LINK_NATIVE_ARRAY   ( BlendAnimFrame );
  LINK_NATIVE_ARRAY   ( BlendAnimRate );
  LINK_NATIVE_ARRAY   ( BlendTweenRate );
  LINK_NATIVE_ARRAY   ( BlendAnimLast );
  LINK_NATIVE_ARRAY   ( BlendAnimMinRate );
  LINK_NATIVE_ARRAY   ( OldBlendAnimRate );
  LINK_NATIVE_ARRAY   ( SimBlendAnim );
  LINK_NATIVE_PROPERTY( BindName );
  LINK_NATIVE_PROPERTY( BarkBindName );
  LINK_NATIVE_PROPERTY( FamiliarName );
  LINK_NATIVE_PROPERTY( UnfamiliarName );
  LINK_NATIVE_PROPERTY( ConListItems );
  LINK_NATIVE_PROPERTY( LastConEndTime );
  LINK_NATIVE_PROPERTY( ConStartInterval );
  LINK_NATIVE_PROPERTY( VisUpdateTime );
  LINK_NATIVE_PROPERTY( CurrentVisibility );
  LINK_NATIVE_PROPERTY( LastVisibility );
  LINK_NATIVE_PROPERTY( SmellClass );
  LINK_NATIVE_PROPERTY( LastSmellNode );
  LINK_NATIVE_PROPERTY( bOwned );
  LINK_NATIVE_PROPERTY( bVisionImportant );
  LINK_NATIVE_PROPERTY( RelevantRadius );
  LINK_NATIVE_PROPERTY( bReleaseLock );
  LINK_NATIVE_PROPERTY( bRenderedLastFrame );
  LINK_NATIVE_PROPERTY( bSpecialRender );
  LINK_NATIVE_PROPERTY( bFrameNotifies );
  LINK_NATIVE_PROPERTY( bLookFocusPlayer );
  LINK_NATIVE_PROPERTY( bLookFocusCreature );
  LINK_NATIVE_PROPERTY( bForceRender );
  LINK_NATIVE_PROPERTY( BaseJoint );
  LINK_NATIVE_PROPERTY( BaseMatterType );
  LINK_NATIVE_PROPERTY( BaseScrollDir );
  LINK_NATIVE_PROPERTY( bHasShadow );
  LINK_NATIVE_PROPERTY( ShadowTexture );
  LINK_NATIVE_PROPERTY( ShadowVector );
  LINK_NATIVE_PROPERTY( ColorAdjust );
  LINK_NATIVE_PROPERTY( DesiredColorAdjust );
  LINK_NATIVE_PROPERTY( DesiredFatness );
  LINK_NATIVE_PROPERTY( AlphaScale );
  LINK_NATIVE_PROPERTY( LODPolyCount );
  LINK_NATIVE_PROPERTY( LODDistMax );
  LINK_NATIVE_PROPERTY( LODDistMin );
  LINK_NATIVE_PROPERTY( LODPercentMin );
  LINK_NATIVE_PROPERTY( LODPercentMax );
  LINK_NATIVE_PROPERTY( LODCurve );
  LINK_NATIVE_PROPERTY( bPointLight );
  LINK_NATIVE_PROPERTY( bMirrored );
  LINK_NATIVE_PROPERTY( bPreLight );
  LINK_NATIVE_PROPERTY( bComplexOcclusion );
  LINK_NATIVE_PROPERTY( AttachParent );
  LINK_NATIVE_PROPERTY( AttachParentJoint );
  LINK_NATIVE_PROPERTY( IncrementStat );
  LINK_NATIVE_PROPERTY( bJointsBlock );
  LINK_NATIVE_PROPERTY( bJointsTouch );
  LINK_NATIVE_PROPERTY( bSweepable );
  LINK_NATIVE_PROPERTY( bSpecialLit2 );
  LINK_NATIVE_PROPERTY( bSpecialLit3 );
  LINK_NATIVE_PROPERTY( bNegativeLight );
  LINK_NATIVE_PROPERTY( bAffectWorld );
  LINK_NATIVE_PROPERTY( bAffectActors );
  LINK_NATIVE_PROPERTY( bNoSurfaceBob );
  LINK_NATIVE_PROPERTY( bDrawSkel );
  LINK_NATIVE_PROPERTY( bDrawJoints );
  LINK_NATIVE_PROPERTY( bDrawAxes );
  LINK_NATIVE_PROPERTY( bApplyLagToAccelerators );
  LINK_NATIVE_PROPERTY( SkelMesh );
  LINK_NATIVE_PROPERTY( Skeletal );
  LINK_NATIVE_PROPERTY( SubstituteMesh );
  LINK_NATIVE_PROPERTY( BlendAnimAlpha );
  LINK_NATIVE_PROPERTY( AnimProxy );
  LINK_NATIVE_PROPERTY( SkelGroupSkins );
  LINK_NATIVE_PROPERTY( SkelGroupFlags );
  LINK_NATIVE_PROPERTY( JointFlags );
  LINK_NATIVE_PROPERTY( JointChild );
END_PROPERTY_LINK()

