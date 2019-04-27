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
 * AParticleFX.cpp - Harry Potter particle fx stub
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "HPSS/AParticleFX.h"

AParticleFX::AParticleFX()
  : AActor()
{
}

AParticleFX::~AParticleFX()
{
}

#include "Core/UClass.h"
IMPLEMENT_NATIVE_CLASS( AParticleFX );


BEGIN_PROPERTY_LINK( AParticleFX, 60 )
  LINK_NATIVE_PROPERTY( ParticlesPerSec );
  LINK_NATIVE_PROPERTY( SourceWidth );
  LINK_NATIVE_PROPERTY( SourceHeight );
  LINK_NATIVE_PROPERTY( SourceDepth );
  LINK_NATIVE_PROPERTY( Period );
  LINK_NATIVE_PROPERTY( Decay );
  LINK_NATIVE_PROPERTY( AngularSpreadWidth );
  LINK_NATIVE_PROPERTY( AngularSpreadHeight );
  LINK_NATIVE_PROPERTY( bSteadyState );
  LINK_NATIVE_PROPERTY( bPrime );
  LINK_NATIVE_PROPERTY( Speed );
  LINK_NATIVE_PROPERTY( Lifetime );
  LINK_NATIVE_PROPERTY( ColorStart );
  LINK_NATIVE_PROPERTY( ColorEnd );
  LINK_NATIVE_PROPERTY( AlphaStart );
  LINK_NATIVE_PROPERTY( AlphaEnd );
  LINK_NATIVE_PROPERTY( SizeWidth );
  LINK_NATIVE_PROPERTY( SizeLength );
  LINK_NATIVE_PROPERTY( SizeEndScale );
  LINK_NATIVE_PROPERTY( SpinRate );
  LINK_NATIVE_PROPERTY( DripTime );
  LINK_NATIVE_PROPERTY( bUpdate );
  LINK_NATIVE_PROPERTY( bVelocityRelative );
  LINK_NATIVE_PROPERTY( bSystemRelative );
  LINK_NATIVE_PROPERTY( ParentBlend );
  LINK_NATIVE_PROPERTY( LOD );
  LINK_NATIVE_PROPERTY( AlphaDelay );
  LINK_NATIVE_PROPERTY( ColorDelay );
  LINK_NATIVE_PROPERTY( SizeDelay );
  LINK_NATIVE_PROPERTY( AlphaGrowPeriod );
  LINK_NATIVE_PROPERTY( SizeGrowPeriod );
  LINK_NATIVE_PROPERTY( Chaos );
  LINK_NATIVE_PROPERTY( ChaosDelay );
  LINK_NATIVE_PROPERTY( Elasticity );
  LINK_NATIVE_PROPERTY( Attraction );
  LINK_NATIVE_PROPERTY( Damping );
  LINK_NATIVE_PROPERTY( Distribution );
  LINK_NATIVE_PROPERTY( Pattern );
  LINK_NATIVE_PROPERTY( WindModifier );
  LINK_NATIVE_PROPERTY( bWindPerParticle );
  LINK_NATIVE_PROPERTY( GravityModifier );
  LINK_NATIVE_PROPERTY( Gravity );
  LINK_NATIVE_PROPERTY( ParticlesAlive );
  LINK_NATIVE_PROPERTY( ParticlesMax );
  LINK_NATIVE_PROPERTY( Textures );
  LINK_NATIVE_PROPERTY( ColorPalette );
  LINK_NATIVE_PROPERTY( RenderPrimitive );
  LINK_NATIVE_PROPERTY( ParticleList );
  LINK_NATIVE_PROPERTY( EmitDelay );
  LINK_NATIVE_PROPERTY( LastUpdateLocation );
  LINK_NATIVE_PROPERTY( LastEmitLocation );
  LINK_NATIVE_PROPERTY( LastUpdateRotation );
  LINK_NATIVE_PROPERTY( EmissionResidue );
  LINK_NATIVE_PROPERTY( Age );
  LINK_NATIVE_PROPERTY( ElapsedTime );
  LINK_NATIVE_PROPERTY( ParticlesEmitted );
  LINK_NATIVE_PROPERTY( LightColor );
  LINK_NATIVE_PROPERTY( CurrentPriorityTag );
  LINK_NATIVE_PROPERTY( bShellOnly );
  LINK_NATIVE_PROPERTY( bEmit );
END_PROPERTY_LINK()

