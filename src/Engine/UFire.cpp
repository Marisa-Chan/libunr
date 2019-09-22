/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU General Public License as published by     *|
|*  the Free Software Foundation, either version 3 of the License, or        *|
|*  (at your option) any later version.                                      *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU General Public License for more details.                             *|
|*                                                                           *|
|*  You should have received a copy of the GNU General Public License        *|
|*  along with this program. If not, see <https://www.gnu.org/licenses/>.    *|
\*===========================================================================*/

/*========================================================================
 * UFire.cpp - Fire texture functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Engine/UFire.h"

UFractalTexture::UFractalTexture()
  : UTexture()
{
}

UFractalTexture::~UFractalTexture()
{
}

UFireTexture::UFireTexture()
  : UFractalTexture()
{
}

UFireTexture::~UFireTexture()
{
}

UWaterTexture::UWaterTexture()
  : UFractalTexture()
{
}

UWaterTexture::~UWaterTexture()
{
}

UWaveTexture::UWaveTexture()
  : UWaterTexture()
{
}

UWaveTexture::~UWaveTexture()
{
}

UWetTexture::UWetTexture()
  : UWaterTexture()
{
}

UWetTexture::~UWetTexture()
{
}

UIceTexture::UIceTexture()
  : UFractalTexture()
{
}

UIceTexture::~UIceTexture()
{
}

IMPLEMENT_NATIVE_CLASS( UFractalTexture );
IMPLEMENT_NATIVE_CLASS( UFireTexture );
IMPLEMENT_NATIVE_CLASS( UWaterTexture );
IMPLEMENT_NATIVE_CLASS( UWaveTexture );
IMPLEMENT_NATIVE_CLASS( UWetTexture );
IMPLEMENT_NATIVE_CLASS( UIceTexture );

BEGIN_PROPERTY_LINK( UFractalTexture, 8 )
  LINK_NATIVE_PROPERTY( UMask );
  LINK_NATIVE_PROPERTY( VMask );
  LINK_NATIVE_PROPERTY( LightOutput );
  LINK_NATIVE_PROPERTY( SoundOutput );
  LINK_NATIVE_PROPERTY( GlobalPhase );
  LINK_NATIVE_PROPERTY( DrawPhase );
  LINK_NATIVE_PROPERTY( AuxPhase );
  LINK_NATIVE_PROPERTY( bHasInit );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UFireTexture, 20 )
  LINK_NATIVE_PROPERTY( SparkType );
  LINK_NATIVE_PROPERTY( RenderHeat );
  LINK_NATIVE_PROPERTY( bRising );
  LINK_NATIVE_PROPERTY( FX_Heat );
  LINK_NATIVE_PROPERTY( FX_Size );
  LINK_NATIVE_PROPERTY( FX_AuxSize );
  LINK_NATIVE_PROPERTY( FX_Area );
  LINK_NATIVE_PROPERTY( FX_Frequency );
  LINK_NATIVE_PROPERTY( FX_Phase );
  LINK_NATIVE_PROPERTY( FX_HorizSpeed );
  LINK_NATIVE_PROPERTY( FX_VertSpeed );
  LINK_NATIVE_PROPERTY( DrawMode );
  LINK_NATIVE_PROPERTY( SparksLimit );
  LINK_NATIVE_PROPERTY( NumSparks );
  LINK_NATIVE_PROPERTY( Sparks );
  LINK_NATIVE_PROPERTY( OldRenderHeat );
  LINK_NATIVE_ARRAY   ( RenderTable );
  LINK_NATIVE_PROPERTY( StarStatus );
  LINK_NATIVE_PROPERTY( PenDownX );
  LINK_NATIVE_PROPERTY( PenDownY );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UWaterTexture, 17 )
  LINK_NATIVE_PROPERTY( DropType );
  LINK_NATIVE_PROPERTY( WaveAmp );
  LINK_NATIVE_PROPERTY( FX_Frequency );
  LINK_NATIVE_PROPERTY( FX_Phase );
  LINK_NATIVE_PROPERTY( FX_Amplitude );
  LINK_NATIVE_PROPERTY( FX_Speed );
  LINK_NATIVE_PROPERTY( FX_Radius );
  LINK_NATIVE_PROPERTY( FX_Size );
  LINK_NATIVE_PROPERTY( FX_Depth );
  LINK_NATIVE_PROPERTY( FX_Time );
  LINK_NATIVE_PROPERTY( NumDrops );
  LINK_NATIVE_ARRAY   ( Drops );
  LINK_NATIVE_PROPERTY( SourceFields );
  LINK_NATIVE_ARRAY   ( RenderTable );
  LINK_NATIVE_ARRAY   ( WaterTable );
  LINK_NATIVE_PROPERTY( WaterParity );
  LINK_NATIVE_PROPERTY( OldWaveAmp );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UWaveTexture, 4 )
  LINK_NATIVE_PROPERTY( BumpMapLight );
  LINK_NATIVE_PROPERTY( BumpMapAngle );
  LINK_NATIVE_PROPERTY( PhongRange );
  LINK_NATIVE_PROPERTY( PhongSize );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UWetTexture, 3 )
  LINK_NATIVE_PROPERTY( SourceTexture );
  LINK_NATIVE_PROPERTY( OldSourceTex );
  LINK_NATIVE_PROPERTY( LocalSourceBitmap );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UIceTexture, 21 )
  LINK_NATIVE_PROPERTY( GlassTexture );
  LINK_NATIVE_PROPERTY( SourceTexture );
  LINK_NATIVE_PROPERTY( PanningStyle );
  LINK_NATIVE_PROPERTY( TimeMethod );
  LINK_NATIVE_PROPERTY( HorizPanSpeed );
  LINK_NATIVE_PROPERTY( VertPanSpeed );
  LINK_NATIVE_PROPERTY( Frequency );
  LINK_NATIVE_PROPERTY( Amplitude );
  LINK_NATIVE_PROPERTY( MoveIce );
  LINK_NATIVE_PROPERTY( MasterCount );
  LINK_NATIVE_PROPERTY( UDisplace );
  LINK_NATIVE_PROPERTY( VDisplace );
  LINK_NATIVE_PROPERTY( UPosition );
  LINK_NATIVE_PROPERTY( VPosition );
  LINK_NATIVE_PROPERTY( TickAccu );
  LINK_NATIVE_PROPERTY( OldUDisplace );
  LINK_NATIVE_PROPERTY( OldVDisplace );
  LINK_NATIVE_PROPERTY( OldGlassTex );
  LINK_NATIVE_PROPERTY( OldSourceTex );
  LINK_NATIVE_PROPERTY( LocalSource );
  LINK_NATIVE_PROPERTY( ForceRefresh );
END_PROPERTY_LINK()

