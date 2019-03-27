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

ARuneActor::ARuneActor()
  : AActor()
{
}

ARuneActor::~ARuneActor()
{
}

#include "Core/UClass.h"
IMPLEMENT_NATIVE_CLASS( AAnimationProxy );
IMPLEMENT_ALIASED_CLASS( ARuneActor );

BEGIN_PROPERTY_LINK( AAnimationProxy, 4 )
  LINK_NATIVE_PROPERTY( curWeapon );
  LINK_NATIVE_PROPERTY( NewWeapon );
  LINK_NATIVE_PROPERTY( curShield );
  LINK_NATIVE_PROPERTY( newShield );
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
