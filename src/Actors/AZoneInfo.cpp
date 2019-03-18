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
 * AZoneInfo.cpp - ZoneInfo functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Actors/AZoneInfo.h"
#include "Core/UClass.h"

AZoneInfo::AZoneInfo()
  : AInfo()
{
}

AZoneInfo::~AZoneInfo()
{
}

ULevelSummary::ULevelSummary()
  : UObject()
{
  Title = new String();
  Author = new String();
  IdealPlayerCount = new String();
  RecommendedEnemies = 0;
  RecommendedTeammates = 0;
  LevelEnterText = new String();
}

ULevelSummary::~ULevelSummary()
{
  delete Title;
  delete Author;
  delete IdealPlayerCount;
  delete LevelEnterText;
}

ALevelInfo::ALevelInfo()
  : AZoneInfo()
{
}

ALevelInfo::~ALevelInfo()
{
}

AWarpZoneInfo::AWarpZoneInfo()
  : AZoneInfo()
{
}

AWarpZoneInfo::~AWarpZoneInfo()
{
}

IMPLEMENT_NATIVE_CLASS( AZoneInfo );
IMPLEMENT_NATIVE_CLASS( ULevelSummary );
IMPLEMENT_NATIVE_CLASS( ALevelInfo );
IMPLEMENT_NATIVE_CLASS( AWarpZoneInfo );

BEGIN_PROPERTY_LINK( AZoneInfo, 70 )
  LINK_NATIVE_PROPERTY( ZoneTag );
  LINK_NATIVE_PROPERTY( ZoneGravity );
  LINK_NATIVE_PROPERTY( ZoneVelocity );
  LINK_NATIVE_PROPERTY( ZoneGroundFriction );
  LINK_NATIVE_PROPERTY( ZoneFluidFriction );
  LINK_NATIVE_PROPERTY( ZoneTerminalVelocity );
  LINK_NATIVE_PROPERTY( ZonePlayerEvent );
  LINK_NATIVE_PROPERTY( ZonePlayerCount );
  LINK_NATIVE_PROPERTY( ZoneTimeDilation );
  LINK_NATIVE_PROPERTY( NumCarcasses );
  LINK_NATIVE_PROPERTY( DamagePerSec );
  LINK_NATIVE_PROPERTY( DamageType );
  LINK_NATIVE_PROPERTY( DamageString );
  LINK_NATIVE_PROPERTY( ZoneName );
  LINK_NATIVE_ARRAY   ( LocationStrings );
  LINK_NATIVE_PROPERTY( LocationID );
  LINK_NATIVE_PROPERTY( MaxCarcasses );
  LINK_NATIVE_PROPERTY( EntrySound );
  LINK_NATIVE_PROPERTY( ExitSound );
  LINK_NATIVE_PROPERTY( EntryActor );
  LINK_NATIVE_PROPERTY( ExitActor );
  LINK_NATIVE_PROPERTY( SkyZone );
  LINK_NATIVE_PROPERTY( bWaterZone );
  LINK_NATIVE_PROPERTY( bFogZone );
  LINK_NATIVE_PROPERTY( bKillZone );
  LINK_NATIVE_PROPERTY( bNeutralZone );
  LINK_NATIVE_PROPERTY( bGravityZone );
  LINK_NATIVE_PROPERTY( bPainZone );
  LINK_NATIVE_PROPERTY( bDestructive );
  LINK_NATIVE_PROPERTY( bNoInventory );
  LINK_NATIVE_PROPERTY( bMoveProjectiles );
  LINK_NATIVE_PROPERTY( bReverbZone );
  LINK_NATIVE_PROPERTY( bRaytraceReverb );
  LINK_NATIVE_PROPERTY( bRepZoneProperties );
  LINK_NATIVE_PROPERTY( bDistanceFogClips );
  LINK_NATIVE_PROPERTY( bDistanceFog );
  LINK_NATIVE_PROPERTY( bZoneBasedFog );
  LINK_NATIVE_PROPERTY( bBounceVelocity );
  LINK_NATIVE_PROPERTY( FogDistanceStart );
  LINK_NATIVE_PROPERTY( FogColor );
  LINK_NATIVE_PROPERTY( FogDistance );
  LINK_NATIVE_PROPERTY( FadeTime );
  LINK_NATIVE_PROPERTY( FogDensity );
  LINK_NATIVE_PROPERTY( FogMode );
  LINK_NATIVE_PROPERTY( AmbientBrightness );
  LINK_NATIVE_PROPERTY( AmbientHue );
  LINK_NATIVE_PROPERTY( AmbientSaturation );
  LINK_NATIVE_PROPERTY( EnvironmentMap );
  LINK_NATIVE_PROPERTY( EnvironmentUScale );
  LINK_NATIVE_PROPERTY( EnvironmentVScale );
  LINK_NATIVE_PROPERTY( EnvironmentColor );
  LINK_NATIVE_PROPERTY( TexUPanSpeed );
  LINK_NATIVE_PROPERTY( TexVPanSpeed );
  LINK_NATIVE_PROPERTY( ViewFlash );
  LINK_NATIVE_PROPERTY( ViewFog );
  LINK_NATIVE_PROPERTY( DirtyShadowLevel );
  LINK_NATIVE_ARRAY   ( LightMapDetailLevels );
  LINK_NATIVE_PROPERTY( SpeedOfSound );
  LINK_NATIVE_PROPERTY( MasterGain );
  LINK_NATIVE_PROPERTY( CutoffHz );
  LINK_NATIVE_ARRAY   ( Delay );
  LINK_NATIVE_ARRAY   ( Gain );
  LINK_NATIVE_PROPERTY( EFXAmbients );
  LINK_NATIVE_ARRAY   ( LensFlare );
  LINK_NATIVE_ARRAY   ( LensFlareOffset );
  LINK_NATIVE_ARRAY   ( LensFlareScale );
  LINK_NATIVE_PROPERTY( MinLightCount );
  LINK_NATIVE_PROPERTY( MaxLightCount );
  LINK_NATIVE_PROPERTY( MinLightingPolyCount );
  LINK_NATIVE_PROPERTY( MaxLightingPolyCount );
  LINK_NATIVE_PROPERTY( VisNotify );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ULevelSummary, 6 )
  LINK_NATIVE_PROPERTY( Title );
  LINK_NATIVE_PROPERTY( Author );
  LINK_NATIVE_PROPERTY( IdealPlayerCount );
  LINK_NATIVE_PROPERTY( RecommendedEnemies );
  LINK_NATIVE_PROPERTY( RecommendedTeammates );
  LINK_NATIVE_PROPERTY( LevelEnterText );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ALevelInfo, 76 )
  LINK_NATIVE_PROPERTY( TimeDilation );
  LINK_NATIVE_PROPERTY( TimeSeconds );
  LINK_NATIVE_PROPERTY( Year );
  LINK_NATIVE_PROPERTY( Month );
  LINK_NATIVE_PROPERTY( Day );
  LINK_NATIVE_PROPERTY( DayOfWeek );
  LINK_NATIVE_PROPERTY( Hour ); 
  LINK_NATIVE_PROPERTY( Minute );
  LINK_NATIVE_PROPERTY( Second );
  LINK_NATIVE_PROPERTY( Millisecond );
  LINK_NATIVE_PROPERTY( Title );
  LINK_NATIVE_PROPERTY( Author );
  LINK_NATIVE_PROPERTY( IdealPlayerCount );
  LINK_NATIVE_PROPERTY( RecommendedEnemies );
  LINK_NATIVE_PROPERTY( RecommendedTeammates );
  LINK_NATIVE_PROPERTY( LevelEnterText );
  LINK_NATIVE_PROPERTY( LocalizedPkg );
  LINK_NATIVE_PROPERTY( Pauser );
  LINK_NATIVE_PROPERTY( Summary );
  LINK_NATIVE_PROPERTY( VisibleGroups );
  LINK_NATIVE_PROPERTY( bLowRes );
  LINK_NATIVE_PROPERTY( bNeverPrecache );
  LINK_NATIVE_PROPERTY( bLonePlayer );
  LINK_NATIVE_PROPERTY( bBegunPlay );
  LINK_NATIVE_PROPERTY( bPlayersOnly );
  LINK_NATIVE_PROPERTY( bHighDetailMode );
  LINK_NATIVE_PROPERTY( bDropDetail );
  LINK_NATIVE_PROPERTY( bAggressiveLOD );
  LINK_NATIVE_PROPERTY( bStartup );
  LINK_NATIVE_PROPERTY( bHumansOnly );
  LINK_NATIVE_PROPERTY( bNoCheating );
  LINK_NATIVE_PROPERTY( bAllowFOV );
  LINK_NATIVE_PROPERTY( bSupportsRealCrouching );
  LINK_NATIVE_PROPERTY( EdBuildOpt );
  LINK_NATIVE_PROPERTY( bSpecularLight );
  LINK_NATIVE_PROPERTY( Song );
  LINK_NATIVE_PROPERTY( SongSection );
  LINK_NATIVE_PROPERTY( CdTrack );
  LINK_NATIVE_PROPERTY( PlayerDoppler );
  LINK_NATIVE_PROPERTY( backup_Song );
  LINK_NATIVE_PROPERTY( backup_SongSection );
  LINK_NATIVE_PROPERTY( LocalPlayer );
  LINK_NATIVE_PROPERTY( Brightness );
  LINK_NATIVE_PROPERTY( Screenshot );
  LINK_NATIVE_PROPERTY( DefaultTexture );
  LINK_NATIVE_PROPERTY( WhiteTexture );
  LINK_NATIVE_PROPERTY( TemplateLightTex );
  LINK_NATIVE_PROPERTY( HubStackLevel );
  LINK_NATIVE_PROPERTY( LevelAction );
  LINK_NATIVE_PROPERTY( NetMode );
  LINK_NATIVE_PROPERTY( ComputerName );
  LINK_NATIVE_PROPERTY( EngineVersion );
  LINK_NATIVE_PROPERTY( EngineSubVersion );
  LINK_NATIVE_PROPERTY( MinNetVersion );
  LINK_NATIVE_PROPERTY( DefaultGameType );
  LINK_NATIVE_PROPERTY( Game );
  LINK_NATIVE_PROPERTY( NavigationPointList );
  LINK_NATIVE_PROPERTY( PawnList );
  LINK_NATIVE_PROPERTY( NextURL );
  LINK_NATIVE_PROPERTY( bNextItems );
  LINK_NATIVE_PROPERTY( NextSwitchCountdown );
  LINK_NATIVE_ARRAY   ( AIProfile );
  LINK_NATIVE_PROPERTY( AvgAITime );
  LINK_NATIVE_PROPERTY( bCheckWalkSurfaces );
  LINK_NATIVE_PROPERTY( SpawnNotify );
  LINK_NATIVE_PROPERTY( FootprintManager );
  LINK_NATIVE_PROPERTY( ObjList );
  LINK_NATIVE_PROPERTY( DynamicZonesList );
  LINK_NATIVE_PROPERTY( ReplicationTarget );
  LINK_NATIVE_PROPERTY( MaxCommonRadius );
  LINK_NATIVE_PROPERTY( MaxCommonHeight );
  LINK_NATIVE_PROPERTY( MinCommonHeight );
  LINK_NATIVE_PROPERTY( MinCommonRadius );
  LINK_NATIVE_PROPERTY( CommonRadius );
  LINK_NATIVE_PROPERTY( HumanRadius );
  LINK_NATIVE_PROPERTY( HumanHeight );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AWarpZoneInfo, 9 )
  LINK_NATIVE_PROPERTY( OtherSideURL );
  LINK_NATIVE_PROPERTY( ThisTag );
  LINK_NATIVE_PROPERTY( bNoTeleFrag );
  LINK_NATIVE_PROPERTY( iWarpZone );
  LINK_NATIVE_PROPERTY( WarpCoords );
  LINK_NATIVE_PROPERTY( OtherSideActor );
  LINK_NATIVE_PROPERTY( OtherSideLevel );
  LINK_NATIVE_ARRAY   ( Destinations );
  LINK_NATIVE_PROPERTY( NumDestinations );
END_PROPERTY_LINK()

