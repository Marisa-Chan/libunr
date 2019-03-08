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
  Tag = Class->Export->ObjectName;
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

ATriggers::ATriggers()
  : AActor()
{
}

ATriggers::~ATriggers()
{
}

ATrigger::ATrigger()
  : ATriggers()
{
}

ATrigger::~ATrigger()
{
}

IMPLEMENT_NATIVE_CLASS( AActor );
IMPLEMENT_NATIVE_CLASS( ACarcass );
IMPLEMENT_NATIVE_CLASS( ADecoration );
IMPLEMENT_NATIVE_CLASS( AInfo );
IMPLEMENT_NATIVE_CLASS( AInterpolationPoint );
IMPLEMENT_NATIVE_CLASS( AKeypoint );
IMPLEMENT_NATIVE_CLASS( ALight );
IMPLEMENT_NATIVE_CLASS( AMenu );
IMPLEMENT_NATIVE_CLASS( AProjectile );
IMPLEMENT_NATIVE_CLASS( ASavedMove );
IMPLEMENT_NATIVE_CLASS( ASpawnNotify );
IMPLEMENT_NATIVE_CLASS( ATriggers );
IMPLEMENT_NATIVE_CLASS( ATrigger );

BEGIN_PROPERTY_LINK( ADecoration, 14 )
  LINK_NATIVE_PROPERTY( ADecoration, bPushable ); 
  LINK_NATIVE_PROPERTY( ADecoration, bOnlyTriggerable );
  LINK_NATIVE_PROPERTY( ADecoration, bSplash );
  LINK_NATIVE_PROPERTY( ADecoration, bBobbing );
  LINK_NATIVE_PROPERTY( ADecoration, bWasCarried );
  LINK_NATIVE_PROPERTY( ADecoration, bPushSoundPlaying );
  LINK_NATIVE_PROPERTY( ADecoration, EffectWhenDestroyed );
  LINK_NATIVE_PROPERTY( ADecoration, PushSound );
  LINK_NATIVE_PROPERTY( ADecoration, NumLandings );
  LINK_NATIVE_PROPERTY( ADecoration, Contents );
  LINK_NATIVE_PROPERTY( ADecoration, Content2 );
  LINK_NATIVE_PROPERTY( ADecoration, Content3 );
  LINK_NATIVE_PROPERTY( ADecoration, EndPushSound );
  LINK_NATIVE_PROPERTY( ADecoration, Shadow );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ACarcass, 9 )
  LINK_NATIVE_PROPERTY( ACarcass, bPlayerCarcass );
  LINK_NATIVE_PROPERTY( ACarcass, Flies );
  LINK_NATIVE_PROPERTY( ACarcass, Rats );
  LINK_NATIVE_PROPERTY( ACarcass, bReducedHeight );
  LINK_NATIVE_PROPERTY( ACarcass, bDecorative );
  LINK_NATIVE_PROPERTY( ACarcass, bSlidingCarcass );
  LINK_NATIVE_PROPERTY( ACarcass, CumulativeDamage );
  LINK_NATIVE_PROPERTY( ACarcass, PlayerOwner );
  LINK_NATIVE_PROPERTY( ACarcass, Bugs );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AProjectile, 11 )
  LINK_NATIVE_PROPERTY( AProjectile, Speed );
  LINK_NATIVE_PROPERTY( AProjectile, MaxSpeed );
  LINK_NATIVE_PROPERTY( AProjectile, Damage );
  LINK_NATIVE_PROPERTY( AProjectile, MomentumTransfer );
  LINK_NATIVE_PROPERTY( AProjectile, MyDamageType );
  LINK_NATIVE_PROPERTY( AProjectile, SpawnSound );
  LINK_NATIVE_PROPERTY( AProjectile, ImpactSound );
  LINK_NATIVE_PROPERTY( AProjectile, MiscSound );
  LINK_NATIVE_PROPERTY( AProjectile, ExploWallOut );
  LINK_NATIVE_PROPERTY( AProjectile, ExplosionDecal );
  LINK_NATIVE_PROPERTY( AProjectile, LastHitActor );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ASpawnNotify, 2 )
  LINK_NATIVE_PROPERTY( ASpawnNotify, ActorClass );
  LINK_NATIVE_PROPERTY( ASpawnNotify, Next );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AInfo, 0 )
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ASavedMove, 9 )
  LINK_NATIVE_PROPERTY( ASavedMove, NextMove );
  LINK_NATIVE_PROPERTY( ASavedMove, TimeStamp );
  LINK_NATIVE_PROPERTY( ASavedMove, Delta );
  LINK_NATIVE_PROPERTY( ASavedMove, DodgeMove );
  LINK_NATIVE_PROPERTY( ASavedMove, bRun );
  LINK_NATIVE_PROPERTY( ASavedMove, bDuck );
  LINK_NATIVE_PROPERTY( ASavedMove, bPressedJump );
  LINK_NATIVE_PROPERTY( ASavedMove, bSent );
  LINK_NATIVE_PROPERTY( ASavedMove, bIsReducedCrouch );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AKeypoint, 0 )
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ALight, 2 )
  LINK_NATIVE_PROPERTY( ALight, ShadowMoverTags );
  LINK_NATIVE_PROPERTY( ALight, MoverShadowSurfs );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AInterpolationPoint, 10 )
  LINK_NATIVE_PROPERTY( AInterpolationPoint, Position );
  LINK_NATIVE_PROPERTY( AInterpolationPoint, RateModifier );
  LINK_NATIVE_PROPERTY( AInterpolationPoint, GameSpeedModifier );
  LINK_NATIVE_PROPERTY( AInterpolationPoint, FovModifier );
  LINK_NATIVE_PROPERTY( AInterpolationPoint, bEndOfPath );
  LINK_NATIVE_PROPERTY( AInterpolationPoint, bSkipNextPath );
  LINK_NATIVE_PROPERTY( AInterpolationPoint, ScreenFlashScale );
  LINK_NATIVE_PROPERTY( AInterpolationPoint, ScreenFlashFog );
  LINK_NATIVE_PROPERTY( AInterpolationPoint, Prev );
  LINK_NATIVE_PROPERTY( AInterpolationPoint, Next );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AMenu, 16 )
  LINK_NATIVE_PROPERTY( AMenu, ParentMenu );
  LINK_NATIVE_PROPERTY( AMenu, Selection );
  LINK_NATIVE_PROPERTY( AMenu, MenuLength );
  LINK_NATIVE_PROPERTY( AMenu, bConfigChanged );
  LINK_NATIVE_PROPERTY( AMenu, bExitAllMenus );
  LINK_NATIVE_PROPERTY( AMenu, PlayerOwner );
  LINK_NATIVE_ARRAY   ( AMenu, HelpMessage );
  LINK_NATIVE_ARRAY   ( AMenu, MenuList );
  LINK_NATIVE_PROPERTY( AMenu, LeftString );
  LINK_NATIVE_PROPERTY( AMenu, RightString );
  LINK_NATIVE_PROPERTY( AMenu, CenterString );
  LINK_NATIVE_PROPERTY( AMenu, EnabledString );
  LINK_NATIVE_PROPERTY( AMenu, DisabledString );
  LINK_NATIVE_PROPERTY( AMenu, MenuTitle );
  LINK_NATIVE_PROPERTY( AMenu, YesString );
  LINK_NATIVE_PROPERTY( AMenu, NoString );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ATriggers, 0 )
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ATrigger, 12 )
  LINK_NATIVE_PROPERTY( ATrigger, TriggerType );
  LINK_NATIVE_PROPERTY( ATrigger, Message );
  LINK_NATIVE_PROPERTY( ATrigger, bTriggerOnceOnly );
  LINK_NATIVE_PROPERTY( ATrigger, bInitiallyActive );
  LINK_NATIVE_PROPERTY( ATrigger, BACKUP_InitActive );
  LINK_NATIVE_PROPERTY( ATrigger, ClassProximityType );
  LINK_NATIVE_PROPERTY( ATrigger, RepeatTriggerTime );
  LINK_NATIVE_PROPERTY( ATrigger, ReTriggerDelay );
  LINK_NATIVE_PROPERTY( ATrigger, TriggerTime );
  LINK_NATIVE_PROPERTY( ATrigger, DamageThreshold );
  LINK_NATIVE_PROPERTY( ATrigger, TriggerActor );
  LINK_NATIVE_PROPERTY( ATrigger, TriggerActor2 );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AActor, 222 )
  LINK_NATIVE_PROPERTY( AActor, bStatic );
  LINK_NATIVE_PROPERTY( AActor, bHidden );
  LINK_NATIVE_PROPERTY( AActor, bNoDelete );
  LINK_NATIVE_PROPERTY( AActor, bAnimFinished );
  LINK_NATIVE_PROPERTY( AActor, bAnimLoop );
  LINK_NATIVE_PROPERTY( AActor, bAnimNotify );
  LINK_NATIVE_PROPERTY( AActor, bAnimByOwner );
  LINK_NATIVE_PROPERTY( AActor, bDeleteMe );
  LINK_NATIVE_PROPERTY( AActor, bAssimilated );
  LINK_NATIVE_PROPERTY( AActor, bTicked );
  LINK_NATIVE_PROPERTY( AActor, bLightChanged );
  LINK_NATIVE_PROPERTY( AActor, bDynamicLight );
  LINK_NATIVE_PROPERTY( AActor, bTimerLoop );
  LINK_NATIVE_PROPERTY( AActor, bNetNotify );
  LINK_NATIVE_PROPERTY( AActor, bHandleOwnCorona );
  LINK_NATIVE_PROPERTY( AActor, bRenderMultiEnviroMaps );
  LINK_NATIVE_PROPERTY( AActor, bWorldGeometry );
  LINK_NATIVE_PROPERTY( AActor, bUseMeshCollision );
  LINK_NATIVE_PROPERTY( AActor, bEditorSelectRender );
  LINK_NATIVE_PROPERTY( AActor, bNoDynamicShadowCast );
  LINK_NATIVE_PROPERTY( AActor, bIsInOctree );
  LINK_NATIVE_PROPERTY( AActor, bProjectorDecal );
  LINK_NATIVE_PROPERTY( AActor, bUseLitSprite );
  LINK_NATIVE_PROPERTY( AActor, bHiddenEd );
  LINK_NATIVE_PROPERTY( AActor, bDirectional );
  LINK_NATIVE_PROPERTY( AActor, bSelected );
  LINK_NATIVE_PROPERTY( AActor, bMemorized );
  LINK_NATIVE_PROPERTY( AActor, bHighlighted );
  LINK_NATIVE_PROPERTY( AActor, bEdLocked );
  LINK_NATIVE_PROPERTY( AActor, bEdShouldSnap );
  LINK_NATIVE_PROPERTY( AActor, bEdSnap );
  LINK_NATIVE_PROPERTY( AActor, bTempEditor );
  LINK_NATIVE_PROPERTY( AActor, bDifficulty0 );
  LINK_NATIVE_PROPERTY( AActor, bDifficulty1 );
  LINK_NATIVE_PROPERTY( AActor, bDifficulty2 );
  LINK_NATIVE_PROPERTY( AActor, bDifficulty3 );
  LINK_NATIVE_PROPERTY( AActor, bSinglePlayer );
  LINK_NATIVE_PROPERTY( AActor, bNet );
  LINK_NATIVE_PROPERTY( AActor, bNetSpecial );
  LINK_NATIVE_PROPERTY( AActor, bScriptInitialized );
  LINK_NATIVE_PROPERTY( AActor, bCanTeleport );
  LINK_NATIVE_PROPERTY( AActor, bIsSecretGoal );
  LINK_NATIVE_PROPERTY( AActor, bIsKillGoal );
  LINK_NATIVE_PROPERTY( AActor, bIsItemGoal );
  LINK_NATIVE_PROPERTY( AActor, bCollideWhenPlacing );
  LINK_NATIVE_PROPERTY( AActor, bTravel );
  LINK_NATIVE_PROPERTY( AActor, bMovable );
  LINK_NATIVE_PROPERTY( AActor, bHighDetail );
  LINK_NATIVE_PROPERTY( AActor, bStasis );
  LINK_NATIVE_PROPERTY( AActor, bForceStasis );
  LINK_NATIVE_PROPERTY( AActor, bIsPawn );
  LINK_NATIVE_PROPERTY( AActor, bNetTemporary );
  LINK_NATIVE_PROPERTY( AActor, bNetOptional );
  LINK_NATIVE_PROPERTY( AActor, bReplicateInstigator );
  LINK_NATIVE_PROPERTY( AActor, bTrailerSameRotation );
  LINK_NATIVE_PROPERTY( AActor, bUnlit );
  LINK_NATIVE_PROPERTY( AActor, bNoSmooth );
  LINK_NATIVE_PROPERTY( AActor, bParticles );
  LINK_NATIVE_PROPERTY( AActor, bRandomFrame );
  LINK_NATIVE_PROPERTY( AActor, bMeshEnviroMap );
  LINK_NATIVE_PROPERTY( AActor, bFilterByVolume );
  LINK_NATIVE_PROPERTY( AActor, bMeshCurvy );
  LINK_NATIVE_PROPERTY( AActor, bShadowCast );
  LINK_NATIVE_PROPERTY( AActor, bOwnerNoSee );
  LINK_NATIVE_PROPERTY( AActor, bOnlyOwnerSee );
  LINK_NATIVE_PROPERTY( AActor, bIsMover );
  LINK_NATIVE_PROPERTY( AActor, bAlwaysRelevant );
  LINK_NATIVE_PROPERTY( AActor, bAlwaysTick );
  LINK_NATIVE_PROPERTY( AActor, bHurtEntry );
  LINK_NATIVE_PROPERTY( AActor, bGameRelevant );
  LINK_NATIVE_PROPERTY( AActor, bCarriedItem );
  LINK_NATIVE_PROPERTY( AActor, bForcePhysicsUpdate );
  LINK_NATIVE_PROPERTY( AActor, bSkipActorReplication );
  LINK_NATIVE_PROPERTY( AActor, bRepAnimations );
  LINK_NATIVE_PROPERTY( AActor, bRepAmbientSound );
  LINK_NATIVE_PROPERTY( AActor, bSimulatedPawnRep );
  LINK_NATIVE_PROPERTY( AActor, bRepMesh );
  LINK_NATIVE_PROPERTY( AActor, bCollideActors );
  LINK_NATIVE_PROPERTY( AActor, bCollideWorld );
  LINK_NATIVE_PROPERTY( AActor, bBlockActors );
  LINK_NATIVE_PROPERTY( AActor, bBlockPlayers );
  LINK_NATIVE_PROPERTY( AActor, bProjTarget );
  LINK_NATIVE_PROPERTY( AActor, bPathCollision );
  LINK_NATIVE_PROPERTY( AActor, bSpecialLit );
  LINK_NATIVE_PROPERTY( AActor, bActorShadows );
  LINK_NATIVE_PROPERTY( AActor, bCorona );
  LINK_NATIVE_PROPERTY( AActor, bLensFlare );
  LINK_NATIVE_PROPERTY( AActor, bDarkLight );
  LINK_NATIVE_PROPERTY( AActor, bNetInitial );
  LINK_NATIVE_PROPERTY( AActor, bNetOwner );
  LINK_NATIVE_PROPERTY( AActor, bNetRelevant );
  LINK_NATIVE_PROPERTY( AActor, bNetSee );
  LINK_NATIVE_PROPERTY( AActor, bNetHear );
  LINK_NATIVE_PROPERTY( AActor, bNetFeel );
  LINK_NATIVE_PROPERTY( AActor, bSimulatedPawn );
  LINK_NATIVE_PROPERTY( AActor, bDemoRecording );
  LINK_NATIVE_PROPERTY( AActor, bBounce );
  LINK_NATIVE_PROPERTY( AActor, bFixedRotationDir );
  LINK_NATIVE_PROPERTY( AActor, bRotateToDesired );
  LINK_NATIVE_PROPERTY( AActor, bInterpolating );
  LINK_NATIVE_PROPERTY( AActor, bJustTeleported );
  LINK_NATIVE_PROPERTY( AActor, bIsFrobable );
  LINK_NATIVE_PROPERTY( AActor, RandomDelayTime );
  LINK_NATIVE_PROPERTY( AActor, RandomValue );
  LINK_NATIVE_PROPERTY( AActor, LastRandomTime );
  LINK_NATIVE_PROPERTY( AActor, LastRenderedTime );
  LINK_NATIVE_PROPERTY( AActor, ActorRenderColor );
  LINK_NATIVE_PROPERTY( AActor, ActorGUnlitColor );
  LINK_NATIVE_PROPERTY( AActor, CollisionOverride );
  LINK_NATIVE_PROPERTY( AActor, MeshInstance );
  LINK_NATIVE_PROPERTY( AActor, RelativeLocation );
  LINK_NATIVE_PROPERTY( AActor, RelativeRotation );
  LINK_NATIVE_PROPERTY( AActor, LightDataPtr );
  LINK_NATIVE_PROPERTY( AActor, MeshDataPtr );
  LINK_NATIVE_PROPERTY( AActor, DrawScale3D );
  LINK_NATIVE_PROPERTY( AActor, ProjectorList );
  LINK_NATIVE_PROPERTY( AActor, NetInitialProperties );
  LINK_NATIVE_PROPERTY( AActor, RealTouching );
  LINK_NATIVE_PROPERTY( AActor, MultiTimers );
  LINK_NATIVE_PROPERTY( AActor, Physics );
  LINK_NATIVE_PROPERTY( AActor, Role );
  LINK_NATIVE_PROPERTY( AActor, RemoteRole );
  LINK_NATIVE_PROPERTY( AActor, Owner );
  LINK_NATIVE_PROPERTY( AActor, InitialState );
  LINK_NATIVE_PROPERTY( AActor, Group );
  LINK_NATIVE_PROPERTY( AActor, TimerRate );
  LINK_NATIVE_PROPERTY( AActor, TimerCounter );
  LINK_NATIVE_PROPERTY( AActor, LifeSpan );
  LINK_NATIVE_PROPERTY( AActor, AnimSequence );
  LINK_NATIVE_PROPERTY( AActor, AnimFrame );
  LINK_NATIVE_PROPERTY( AActor, AnimRate );
  LINK_NATIVE_PROPERTY( AActor, TweenRate );
  LINK_NATIVE_PROPERTY( AActor, SkelAnim );
  LINK_NATIVE_PROPERTY( AActor, LODBias );
  LINK_NATIVE_PROPERTY( AActor, DefaultAnimationNotify );
  LINK_NATIVE_PROPERTY( AActor, AnimationNotify );
  LINK_NATIVE_PROPERTY( AActor, Level );
  LINK_NATIVE_PROPERTY( AActor, XLevel );
  LINK_NATIVE_PROPERTY( AActor, Tag );
  LINK_NATIVE_PROPERTY( AActor, Event );
  LINK_NATIVE_PROPERTY( AActor, Target );
  LINK_NATIVE_PROPERTY( AActor, Instigator );
  LINK_NATIVE_PROPERTY( AActor, Inventory );
  LINK_NATIVE_PROPERTY( AActor, Base );
  LINK_NATIVE_PROPERTY( AActor, Region );
  LINK_NATIVE_PROPERTY( AActor, AttachTag );
  LINK_NATIVE_PROPERTY( AActor, StandingCount );
  LINK_NATIVE_PROPERTY( AActor, MiscNumber );
  LINK_NATIVE_PROPERTY( AActor, LatentByte );
  LINK_NATIVE_PROPERTY( AActor, LatentInt );
  LINK_NATIVE_PROPERTY( AActor, LatentFloat );
  LINK_NATIVE_PROPERTY( AActor, LatentActor );
  LINK_NATIVE_ARRAY   ( AActor, Touching );
  LINK_NATIVE_PROPERTY( AActor, Deleted );
  LINK_NATIVE_PROPERTY( AActor, CollisionTag );
  LINK_NATIVE_PROPERTY( AActor, LightingTag );
  LINK_NATIVE_PROPERTY( AActor, NetTag );
  LINK_NATIVE_PROPERTY( AActor, OtherTag );
  LINK_NATIVE_PROPERTY( AActor, ExtraTag );
  LINK_NATIVE_PROPERTY( AActor, SpecialTag );
  LINK_NATIVE_PROPERTY( AActor, Location );
  LINK_NATIVE_PROPERTY( AActor, Rotation );
  LINK_NATIVE_PROPERTY( AActor, OldLocation );
  LINK_NATIVE_PROPERTY( AActor, ColLocation );
  LINK_NATIVE_PROPERTY( AActor, Velocity );
  LINK_NATIVE_PROPERTY( AActor, Acceleration );
  LINK_NATIVE_PROPERTY( AActor, OddsOfAppearing );
  LINK_NATIVE_PROPERTY( AActor, HitActor );
  LINK_NATIVE_PROPERTY( AActor, DrawType );
  LINK_NATIVE_PROPERTY( AActor, Style );
  LINK_NATIVE_PROPERTY( AActor, Sprite );
  LINK_NATIVE_PROPERTY( AActor, Texture );
  LINK_NATIVE_PROPERTY( AActor, Skin );
  LINK_NATIVE_PROPERTY( AActor, Mesh );
  LINK_NATIVE_PROPERTY( AActor, ShadowMesh );
  LINK_NATIVE_PROPERTY( AActor, Brush );
  LINK_NATIVE_PROPERTY( AActor, DrawScale );
  LINK_NATIVE_PROPERTY( AActor, PrePivot );
  LINK_NATIVE_PROPERTY( AActor, ScaleGlow );
  LINK_NATIVE_PROPERTY( AActor, VisibilityRadius );
  LINK_NATIVE_PROPERTY( AActor, VisibilityHeight );
  LINK_NATIVE_PROPERTY( AActor, AmbientGlow );
  LINK_NATIVE_PROPERTY( AActor, Fatness );
  LINK_NATIVE_PROPERTY( AActor, SpriteProjForward );
  LINK_NATIVE_ARRAY   ( AActor, MultiSkins );
  LINK_NATIVE_PROPERTY( AActor, RenderIteratorClass );
  LINK_NATIVE_PROPERTY( AActor, RenderInterface );
  LINK_NATIVE_PROPERTY( AActor, SoundRadius );
  LINK_NATIVE_PROPERTY( AActor, SoundVolume );
  LINK_NATIVE_PROPERTY( AActor, SoundPitch );
  LINK_NATIVE_PROPERTY( AActor, AmbientSound );
  LINK_NATIVE_PROPERTY( AActor, TransientSoundVolume );
  LINK_NATIVE_PROPERTY( AActor, TransientSoundRadius );
  LINK_NATIVE_PROPERTY( AActor, CollisionRadius );
  LINK_NATIVE_PROPERTY( AActor, CollisionHeight );
  LINK_NATIVE_PROPERTY( AActor, LightType );
  LINK_NATIVE_PROPERTY( AActor, LightEffect );
  LINK_NATIVE_PROPERTY( AActor, LightBrightness );
  LINK_NATIVE_PROPERTY( AActor, LightHue );
  LINK_NATIVE_PROPERTY( AActor, LightSaturation );
  LINK_NATIVE_PROPERTY( AActor, LightRadius );
  LINK_NATIVE_PROPERTY( AActor, LightPeriod );
  LINK_NATIVE_PROPERTY( AActor, LightPhase );
  LINK_NATIVE_PROPERTY( AActor, LightCone );
  LINK_NATIVE_PROPERTY( AActor, VolumeBrightness );
  LINK_NATIVE_PROPERTY( AActor, VolumeRadius );
  LINK_NATIVE_PROPERTY( AActor, VolumeFog );
  LINK_NATIVE_PROPERTY( AActor, CoronaAlpha );
  LINK_NATIVE_PROPERTY( AActor, DodgeDir );
  LINK_NATIVE_PROPERTY( AActor, Mass );
  LINK_NATIVE_PROPERTY( AActor, Buoyancy );
  LINK_NATIVE_PROPERTY( AActor, RotationRate );
  LINK_NATIVE_PROPERTY( AActor, DesiredRotation );
  LINK_NATIVE_PROPERTY( AActor, PhysAlpha );
  LINK_NATIVE_PROPERTY( AActor, PhysRate );
  LINK_NATIVE_PROPERTY( AActor, PendingTouch );
  LINK_NATIVE_PROPERTY( AActor, AnimLast );
  LINK_NATIVE_PROPERTY( AActor, AnimMinRate );
  LINK_NATIVE_PROPERTY( AActor, OldAnimRate );
  LINK_NATIVE_PROPERTY( AActor, SimAnim );
  LINK_NATIVE_PROPERTY( AActor, NetPriority );
  LINK_NATIVE_PROPERTY( AActor, NetUpdateFrequency );
END_PROPERTY_LINK()

