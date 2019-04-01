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
 * ARuneActor.cpp - Actor class replacement for Rune
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Rune/ARuneActor.h"

AAnimationProxy::AAnimationProxy()
  : AKeypoint()
{
}

AAnimationProxy::~AAnimationProxy()
{
}

AAutoLink::AAutoLink()
  : ANavigationPoint()
{
}

AAutoLink::~AAutoLink()
{
}

ADebugHUD::ADebugHUD()
  : AHUD()
{
}

ADebugHUD::~ADebugHUD()
{
}

AParticleSystem::AParticleSystem()
  : AActor()
{
}

AParticleSystem::~AParticleSystem()
{
}

ARuneActor::ARuneActor()
  : AActor()
{
}

ARuneActor::~ARuneActor()
{
}

ARuneMutator::ARuneMutator()
  : AMutator()
{
}

ARuneMutator::~ARuneMutator()
{
}

AScriptDispatcher::AScriptDispatcher()
  : AKeypoint()
{
}

AScriptDispatcher::~AScriptDispatcher()
{
}

AScriptAction::AScriptAction()
  : AKeypoint()
{
}

AScriptAction::~AScriptAction()
{
}

ASoundPlayer::ASoundPlayer()
  : AKeypoint()
{
}

ASoundPlayer::~ASoundPlayer()
{
}

#include "Core/UClass.h"
IMPLEMENT_NATIVE_CLASS( AAnimationProxy );
IMPLEMENT_NATIVE_CLASS( AAutoLink );
IMPLEMENT_NATIVE_CLASS( ADebugHUD );
IMPLEMENT_NATIVE_CLASS( AParticleSystem );
IMPLEMENT_NATIVE_CLASS( AScriptAction );
IMPLEMENT_NATIVE_CLASS( AScriptDispatcher );
IMPLEMENT_NATIVE_CLASS( ASoundPlayer );
IMPLEMENT_ALIASED_CLASS( ARuneActor, GAME_Rune );
IMPLEMENT_ALIASED_CLASS( ARuneMutator, GAME_Rune );

BEGIN_PROPERTY_LINK( AAnimationProxy, 4 )
  LINK_NATIVE_PROPERTY( curWeapon );
  LINK_NATIVE_PROPERTY( NewWeapon );
  LINK_NATIVE_PROPERTY( curShield );
  LINK_NATIVE_PROPERTY( newShield );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AAutoLink, 1 )
  LINK_NATIVE_PROPERTY( LinkTag );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ADebugHUD, 4 )
  LINK_NATIVE_PROPERTY( ModeTable );
  LINK_NATIVE_PROPERTY( Watch );
  LINK_NATIVE_PROPERTY( DebugMode );
  LINK_NATIVE_PROPERTY( DebugHudMode );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AParticleSystem, 66 )
  LINK_NATIVE_PROPERTY( IsLoaded );
  LINK_NATIVE_PROPERTY( bSpriteInEditor );
  LINK_NATIVE_PROPERTY( bSystemOneShot );
  LINK_NATIVE_PROPERTY( bSystemTicks );
  LINK_NATIVE_PROPERTY( bRelativeToSystem );
  LINK_NATIVE_PROPERTY( bEventDeath );
  LINK_NATIVE_PROPERTY( ParticleCount );
  LINK_NATIVE_PROPERTY( ParticleTexture );
  LINK_NATIVE_PROPERTY( bRandomTexture );
  LINK_NATIVE_PROPERTY( ParticleType );
  LINK_NATIVE_PROPERTY( ParticleSpriteType );
  LINK_NATIVE_PROPERTY( SpawnShape );
  LINK_NATIVE_PROPERTY( RandomDelay );
  LINK_NATIVE_PROPERTY( SystemLifeSpan );
  LINK_NATIVE_PROPERTY( CurrentDelay );
  LINK_NATIVE_PROPERTY( OldParticleCount );
  LINK_NATIVE_PROPERTY( SystemCoords );
  LINK_NATIVE_PROPERTY( HasValidCoords );
  LINK_NATIVE_PROPERTY( LastTime );
  LINK_NATIVE_PROPERTY( CurrentTime );
  LINK_NATIVE_PROPERTY( ParticleArray );
  LINK_NATIVE_PROPERTY( OriginOffset );
  LINK_NATIVE_PROPERTY( ShapeVector );
  LINK_NATIVE_PROPERTY( VelocityMin );
  LINK_NATIVE_PROPERTY( VelocityMax );
  LINK_NATIVE_PROPERTY( ScaleMin );
  LINK_NATIVE_PROPERTY( ScaleMax );
  LINK_NATIVE_PROPERTY( ScaleDeltaX );
  LINK_NATIVE_PROPERTY( ScaleDeltaY );
  LINK_NATIVE_PROPERTY( LifeSpanMin );
  LINK_NATIVE_PROPERTY( LifeSpanMax );
  LINK_NATIVE_PROPERTY( AlphaStart );
  LINK_NATIVE_PROPERTY( AlphaEnd );
  LINK_NATIVE_PROPERTY( PercentOffset );
  LINK_NATIVE_PROPERTY( bAlphaFade );
  LINK_NATIVE_PROPERTY( bApplyGravity );
  LINK_NATIVE_PROPERTY( GravityScale );
  LINK_NATIVE_PROPERTY( bApplyZoneVelocity );
  LINK_NATIVE_PROPERTY( ZoneVelocityScale );
  LINK_NATIVE_PROPERTY( bWaterOnly );
  LINK_NATIVE_PROPERTY( bOneShot );
  LINK_NATIVE_PROPERTY( bConvergeX );
  LINK_NATIVE_PROPERTY( bConvergeY );
  LINK_NATIVE_PROPERTY( bConvergeZ );
  LINK_NATIVE_PROPERTY( bConstrainToBounds );
  LINK_NATIVE_PROPERTY( SpawnDelay );
  LINK_NATIVE_PROPERTY( SpawnOverTime );
  LINK_NATIVE_PROPERTY( TextureChangeTime );
  LINK_NATIVE_PROPERTY( BaseJointIndex );
  LINK_NATIVE_PROPERTY( OffsetJointIndex );
  LINK_NATIVE_PROPERTY( OldBaseLocation );
  LINK_NATIVE_PROPERTY( OldOffsetLocation );
  LINK_NATIVE_PROPERTY( SwipeSpeed );
  LINK_NATIVE_PROPERTY( NumConPts );
  LINK_NATIVE_PROPERTY( BeamThickness );
  LINK_NATIVE_PROPERTY( BeamTextureScale );
  LINK_NATIVE_PROPERTY( TargetJointIndex );
  LINK_NATIVE_PROPERTY( ConnectionPoint );
  LINK_NATIVE_PROPERTY( ConnectionOffset );
  LINK_NATIVE_PROPERTY( bUseTargetLocation );
  LINK_NATIVE_PROPERTY( TargetLocation );
  LINK_NATIVE_PROPERTY( bEventSystemInit );
  LINK_NATIVE_PROPERTY( bEventSystemTick );
  LINK_NATIVE_PROPERTY( bEventParticleTick );
  LINK_NATIVE_PROPERTY( bTaperStartPoint );
  LINK_NATIVE_PROPERTY( bTaperEndPoint );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ARuneActor, 52 )
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

BEGIN_PROPERTY_LINK( ARuneMutator, 1 )
  LINK_NATIVE_PROPERTY( DefaultShield );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AScriptAction, 16 )
  LINK_NATIVE_PROPERTY( AnimToPlay );
  LINK_NATIVE_PROPERTY( PauseBeforeSound );
  LINK_NATIVE_PROPERTY( SoundToPlay );
  LINK_NATIVE_PROPERTY( AnimTimeToLoop );
  LINK_NATIVE_PROPERTY( bReleaseUponCompletion );
  LINK_NATIVE_PROPERTY( bFireEventImmediately );
  LINK_NATIVE_PROPERTY( bTurnToRotation );
  LINK_NATIVE_PROPERTY( ControlTorso );
  LINK_NATIVE_PROPERTY( NextOrder );
  LINK_NATIVE_PROPERTY( NextOrderTag );
  LINK_NATIVE_PROPERTY( bWaitToBeTriggered );
  LINK_NATIVE_PROPERTY( ControlMouth );
  LINK_NATIVE_PROPERTY( ControlHead );
  LINK_NATIVE_PROPERTY( ControlTimeGranularity );
  LINK_NATIVE_PROPERTY( LookTarget );
  LINK_NATIVE_PROPERTY( WaitingScripter );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AScriptDispatcher, 9 )
  LINK_NATIVE_PROPERTY( LookTarget );
  LINK_NATIVE_PROPERTY( ControlMouth );
  LINK_NATIVE_PROPERTY( ControlHead );
  LINK_NATIVE_PROPERTY( ControlTimeGranularity );
  LINK_NATIVE_PROPERTY( Actions );
  LINK_NATIVE_PROPERTY( NextOrder );
  LINK_NATIVE_PROPERTY( NextOrderTag );
  LINK_NATIVE_PROPERTY( bWaitToBeTriggered );
  LINK_NATIVE_PROPERTY( WaitingScripter );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ASoundPlayer, 26 )
  LINK_NATIVE_PROPERTY( bPlayerSound );
  LINK_NATIVE_PROPERTY( bAutoContinuous );
  LINK_NATIVE_PROPERTY( bRandomPosition );
  LINK_NATIVE_PROPERTY( RndPosHeight );
  LINK_NATIVE_PROPERTY( RndPosRadius );
  LINK_NATIVE_PROPERTY( RandomDelayMin );
  LINK_NATIVE_PROPERTY( RandomDelayMax );
  LINK_NATIVE_PROPERTY( RandomPercentPitch );
  LINK_NATIVE_PROPERTY( RandomPercentVolume );
  LINK_NATIVE_PROPERTY( TSound );
  LINK_NATIVE_PROPERTY( TSoundPitch );
  LINK_NATIVE_PROPERTY( TSoundProbability );
  LINK_NATIVE_PROPERTY( TSoundRadius );
  LINK_NATIVE_PROPERTY( TSoundVolume );
  LINK_NATIVE_PROPERTY( TriggerCountdown );
  LINK_NATIVE_PROPERTY( TriggerBehavior );
  LINK_NATIVE_PROPERTY( SelectMode );
  LINK_NATIVE_PROPERTY( SoundCount );
  LINK_NATIVE_PROPERTY( SoundXLat );
  LINK_NATIVE_PROPERTY( AdjustedProbability );
  LINK_NATIVE_PROPERTY( CSnd );
  LINK_NATIVE_PROPERTY( CSndDuration );
  LINK_NATIVE_PROPERTY( StartOffsetLocation );
  LINK_NATIVE_PROPERTY( RandomSize );
  LINK_NATIVE_PROPERTY( SPMaxSize );
  LINK_NATIVE_PROPERTY( CycleSound );
END_PROPERTY_LINK()

