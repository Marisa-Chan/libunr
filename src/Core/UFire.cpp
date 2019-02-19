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
 * UFire.cpp - Fire texture functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Core/UFire.h"

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
  LINK_NATIVE_PROPERTY( UFractalTexture, UMask );
  LINK_NATIVE_PROPERTY( UFractalTexture, VMask );
  LINK_NATIVE_PROPERTY( UFractalTexture, LightOutput );
  LINK_NATIVE_PROPERTY( UFractalTexture, SoundOutput );
  LINK_NATIVE_PROPERTY( UFractalTexture, GlobalPhase );
  LINK_NATIVE_PROPERTY( UFractalTexture, DrawPhase );
  LINK_NATIVE_PROPERTY( UFractalTexture, AuxPhase );
  LINK_NATIVE_PROPERTY( UFractalTexture, bHasInit );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UFireTexture, 20 )
  LINK_NATIVE_PROPERTY( UFireTexture, SparkType );
  LINK_NATIVE_PROPERTY( UFireTexture, RenderHeat );
  LINK_NATIVE_PROPERTY( UFireTexture, bRising );
  LINK_NATIVE_PROPERTY( UFireTexture, FX_Heat );
  LINK_NATIVE_PROPERTY( UFireTexture, FX_Size );
  LINK_NATIVE_PROPERTY( UFireTexture, FX_AuxSize );
  LINK_NATIVE_PROPERTY( UFireTexture, FX_Area );
  LINK_NATIVE_PROPERTY( UFireTexture, FX_Frequency );
  LINK_NATIVE_PROPERTY( UFireTexture, FX_Phase );
  LINK_NATIVE_PROPERTY( UFireTexture, FX_HorizSpeed );
  LINK_NATIVE_PROPERTY( UFireTexture, FX_VertSpeed );
  LINK_NATIVE_PROPERTY( UFireTexture, DrawMode );
  LINK_NATIVE_PROPERTY( UFireTexture, SparksLimit );
  LINK_NATIVE_PROPERTY( UFireTexture, NumSparks );
  LINK_NATIVE_PROPERTY( UFireTexture, Sparks );
  LINK_NATIVE_PROPERTY( UFireTexture, OldRenderHeat );
  LINK_NATIVE_ARRAY   ( UFireTexture, RenderTable );
  LINK_NATIVE_PROPERTY( UFireTexture, StarStatus );
  LINK_NATIVE_PROPERTY( UFireTexture, PenDownX );
  LINK_NATIVE_PROPERTY( UFireTexture, PenDownY );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UWaterTexture, 17 )
  LINK_NATIVE_PROPERTY( UWaterTexture, DropType );
  LINK_NATIVE_PROPERTY( UWaterTexture, WaveAmp );
  LINK_NATIVE_PROPERTY( UWaterTexture, FX_Frequency );
  LINK_NATIVE_PROPERTY( UWaterTexture, FX_Phase );
  LINK_NATIVE_PROPERTY( UWaterTexture, FX_Amplitude );
  LINK_NATIVE_PROPERTY( UWaterTexture, FX_Speed );
  LINK_NATIVE_PROPERTY( UWaterTexture, FX_Radius );
  LINK_NATIVE_PROPERTY( UWaterTexture, FX_Size );
  LINK_NATIVE_PROPERTY( UWaterTexture, FX_Depth );
  LINK_NATIVE_PROPERTY( UWaterTexture, FX_Time );
  LINK_NATIVE_PROPERTY( UWaterTexture, NumDrops );
  LINK_NATIVE_ARRAY   ( UWaterTexture, Drops );
  LINK_NATIVE_PROPERTY( UWaterTexture, SourceFields );
  LINK_NATIVE_ARRAY   ( UWaterTexture, RenderTable );
  LINK_NATIVE_ARRAY   ( UWaterTexture, WaterTable );
  LINK_NATIVE_PROPERTY( UWaterTexture, WaterParity );
  LINK_NATIVE_PROPERTY( UWaterTexture, OldWaveAmp );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UWaveTexture, 4 )
  LINK_NATIVE_PROPERTY( UWaveTexture, BumpMapLight );
  LINK_NATIVE_PROPERTY( UWaveTexture, BumpMapAngle );
  LINK_NATIVE_PROPERTY( UWaveTexture, PhongRange );
  LINK_NATIVE_PROPERTY( UWaveTexture, PhongSize );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UWetTexture, 3 )
  LINK_NATIVE_PROPERTY( UWetTexture, SourceTexture );
  LINK_NATIVE_PROPERTY( UWetTexture, OldSourceTex );
  LINK_NATIVE_PROPERTY( UWetTexture, LocalSourceBitmap );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UIceTexture, 21 )
  LINK_NATIVE_PROPERTY( UIceTexture, GlassTexture );
  LINK_NATIVE_PROPERTY( UIceTexture, SourceTexture );
  LINK_NATIVE_PROPERTY( UIceTexture, PanningStyle );
  LINK_NATIVE_PROPERTY( UIceTexture, TimeMethod );
  LINK_NATIVE_PROPERTY( UIceTexture, HorizPanSpeed );
  LINK_NATIVE_PROPERTY( UIceTexture, VertPanSpeed );
  LINK_NATIVE_PROPERTY( UIceTexture, Frequency );
  LINK_NATIVE_PROPERTY( UIceTexture, Amplitude );
  LINK_NATIVE_PROPERTY( UIceTexture, MoveIce );
  LINK_NATIVE_PROPERTY( UIceTexture, MasterCount );
  LINK_NATIVE_PROPERTY( UIceTexture, UDisplace );
  LINK_NATIVE_PROPERTY( UIceTexture, VDisplace );
  LINK_NATIVE_PROPERTY( UIceTexture, UPosition );
  LINK_NATIVE_PROPERTY( UIceTexture, VPosition );
  LINK_NATIVE_PROPERTY( UIceTexture, TickAccu );
  LINK_NATIVE_PROPERTY( UIceTexture, OldUDisplace );
  LINK_NATIVE_PROPERTY( UIceTexture, OldVDisplace );
  LINK_NATIVE_PROPERTY( UIceTexture, OldGlassTex );
  LINK_NATIVE_PROPERTY( UIceTexture, OldSourceTex );
  LINK_NATIVE_PROPERTY( UIceTexture, LocalSource );
  LINK_NATIVE_PROPERTY( UIceTexture, ForceRefresh );
END_PROPERTY_LINK()

