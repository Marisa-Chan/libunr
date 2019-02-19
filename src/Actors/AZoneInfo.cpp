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

IMPLEMENT_NATIVE_CLASS( AZoneInfo );
IMPLEMENT_NATIVE_CLASS( ULevelSummary );
IMPLEMENT_NATIVE_CLASS( ALevelInfo );

BEGIN_PROPERTY_LINK( AZoneInfo, 70 )
  LINK_NATIVE_PROPERTY( AZoneInfo, ZoneTag );
  LINK_NATIVE_PROPERTY( AZoneInfo, ZoneGravity );
  LINK_NATIVE_PROPERTY( AZoneInfo, ZoneVelocity );
  LINK_NATIVE_PROPERTY( AZoneInfo, ZoneGroundFriction );
  LINK_NATIVE_PROPERTY( AZoneInfo, ZoneFluidFriction );
  LINK_NATIVE_PROPERTY( AZoneInfo, ZoneTerminalVelocity );
  LINK_NATIVE_PROPERTY( AZoneInfo, ZonePlayerEvent );
  LINK_NATIVE_PROPERTY( AZoneInfo, ZonePlayerCount );
  LINK_NATIVE_PROPERTY( AZoneInfo, ZoneTimeDilation );
  LINK_NATIVE_PROPERTY( AZoneInfo, NumCarcasses );
  LINK_NATIVE_PROPERTY( AZoneInfo, DamagePerSec );
  LINK_NATIVE_PROPERTY( AZoneInfo, DamageType );
  LINK_NATIVE_PROPERTY( AZoneInfo, DamageString );
  LINK_NATIVE_PROPERTY( AZoneInfo, ZoneName );
  LINK_NATIVE_ARRAY   ( AZoneInfo, LocationStrings );
  LINK_NATIVE_PROPERTY( AZoneInfo, LocationID );
  LINK_NATIVE_PROPERTY( AZoneInfo, MaxCarcasses );
  LINK_NATIVE_PROPERTY( AZoneInfo, EntrySound );
  LINK_NATIVE_PROPERTY( AZoneInfo, ExitSound );
  LINK_NATIVE_PROPERTY( AZoneInfo, EntryActor );
  LINK_NATIVE_PROPERTY( AZoneInfo, ExitActor );
  LINK_NATIVE_PROPERTY( AZoneInfo, SkyZone );
  LINK_NATIVE_PROPERTY( AZoneInfo, bWaterZone );
  LINK_NATIVE_PROPERTY( AZoneInfo, bFogZone );
  LINK_NATIVE_PROPERTY( AZoneInfo, bKillZone );
  LINK_NATIVE_PROPERTY( AZoneInfo, bNeutralZone );
  LINK_NATIVE_PROPERTY( AZoneInfo, bGravityZone );
  LINK_NATIVE_PROPERTY( AZoneInfo, bPainZone );
  LINK_NATIVE_PROPERTY( AZoneInfo, bDestructive );
  LINK_NATIVE_PROPERTY( AZoneInfo, bNoInventory );
  LINK_NATIVE_PROPERTY( AZoneInfo, bMoveProjectiles );
  LINK_NATIVE_PROPERTY( AZoneInfo, bReverbZone );
  LINK_NATIVE_PROPERTY( AZoneInfo, bRaytraceReverb );
  LINK_NATIVE_PROPERTY( AZoneInfo, bRepZoneProperties );
  LINK_NATIVE_PROPERTY( AZoneInfo, bDistanceFogClips );
  LINK_NATIVE_PROPERTY( AZoneInfo, bDistanceFog );
  LINK_NATIVE_PROPERTY( AZoneInfo, bZoneBasedFog );
  LINK_NATIVE_PROPERTY( AZoneInfo, FogDistanceStart );
  LINK_NATIVE_PROPERTY( AZoneInfo, FogColor );
  LINK_NATIVE_PROPERTY( AZoneInfo, FogDistance );
  LINK_NATIVE_PROPERTY( AZoneInfo, FadeTime );
  LINK_NATIVE_PROPERTY( AZoneInfo, FogDensity );
  LINK_NATIVE_PROPERTY( AZoneInfo, FogMode );
  LINK_NATIVE_PROPERTY( AZoneInfo, AmbientBrightness );
  LINK_NATIVE_PROPERTY( AZoneInfo, AmbientHue );
  LINK_NATIVE_PROPERTY( AZoneInfo, AmbientSaturation );
  LINK_NATIVE_PROPERTY( AZoneInfo, EnvironmentMap );
  LINK_NATIVE_PROPERTY( AZoneInfo, EnvironmentUScale );
  LINK_NATIVE_PROPERTY( AZoneInfo, EnvironmentVScale );
  LINK_NATIVE_PROPERTY( AZoneInfo, EnvironmentColor );
  LINK_NATIVE_PROPERTY( AZoneInfo, TexUPanSpeed );
  LINK_NATIVE_PROPERTY( AZoneInfo, TexVPanSpeed );
  LINK_NATIVE_PROPERTY( AZoneInfo, ViewFlash );
  LINK_NATIVE_PROPERTY( AZoneInfo, ViewFog );
  LINK_NATIVE_PROPERTY( AZoneInfo, DirtyShadowLevel );
  LINK_NATIVE_ARRAY   ( AZoneInfo, LightMapDetailLevels );
  LINK_NATIVE_PROPERTY( AZoneInfo, SpeedOfSound );
  LINK_NATIVE_PROPERTY( AZoneInfo, MasterGain );
  LINK_NATIVE_PROPERTY( AZoneInfo, CutoffHz );
  LINK_NATIVE_ARRAY   ( AZoneInfo, Delay );
  LINK_NATIVE_ARRAY   ( AZoneInfo, Gain );
  LINK_NATIVE_PROPERTY( AZoneInfo, EFXAmbients );
  LINK_NATIVE_ARRAY   ( AZoneInfo, LensFlare );
  LINK_NATIVE_ARRAY   ( AZoneInfo, LensFlareOffset );
  LINK_NATIVE_ARRAY   ( AZoneInfo, LensFlareScale );
  LINK_NATIVE_PROPERTY( AZoneInfo, MinLightCount );
  LINK_NATIVE_PROPERTY( AZoneInfo, MaxLightCount );
  LINK_NATIVE_PROPERTY( AZoneInfo, MinLightingPolyCount );
  LINK_NATIVE_PROPERTY( AZoneInfo, MaxLightingPolyCount );
  LINK_NATIVE_PROPERTY( AZoneInfo, VisNotify );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ULevelSummary, 6 )
  LINK_NATIVE_PROPERTY( ULevelSummary, Title );
  LINK_NATIVE_PROPERTY( ULevelSummary, Author );
  LINK_NATIVE_PROPERTY( ULevelSummary, IdealPlayerCount );
  LINK_NATIVE_PROPERTY( ULevelSummary, RecommendedEnemies );
  LINK_NATIVE_PROPERTY( ULevelSummary, RecommendedTeammates );
  LINK_NATIVE_PROPERTY( ULevelSummary, LevelEnterText );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( ALevelInfo, 74 )
  LINK_NATIVE_PROPERTY( ALevelInfo, TimeDilation );
  LINK_NATIVE_PROPERTY( ALevelInfo, TimeSeconds );
  LINK_NATIVE_PROPERTY( ALevelInfo, Year );
  LINK_NATIVE_PROPERTY( ALevelInfo, Month );
  LINK_NATIVE_PROPERTY( ALevelInfo, Day );
  LINK_NATIVE_PROPERTY( ALevelInfo, DayOfWeek );
  LINK_NATIVE_PROPERTY( ALevelInfo, Hour ); 
  LINK_NATIVE_PROPERTY( ALevelInfo, Minute );
  LINK_NATIVE_PROPERTY( ALevelInfo, Second );
  LINK_NATIVE_PROPERTY( ALevelInfo, Millisecond );
  LINK_NATIVE_PROPERTY( ALevelInfo, Title );
  LINK_NATIVE_PROPERTY( ALevelInfo, Author );
  LINK_NATIVE_PROPERTY( ALevelInfo, IdealPlayerCount );
  LINK_NATIVE_PROPERTY( ALevelInfo, RecommendedEnemies );
  LINK_NATIVE_PROPERTY( ALevelInfo, RecommendedTeammates );
  LINK_NATIVE_PROPERTY( ALevelInfo, LevelEnterText );
  LINK_NATIVE_PROPERTY( ALevelInfo, LocalizedPkg );
  LINK_NATIVE_PROPERTY( ALevelInfo, Pauser );
  LINK_NATIVE_PROPERTY( ALevelInfo, Summary );
  LINK_NATIVE_PROPERTY( ALevelInfo, VisibleGroups );
  LINK_NATIVE_PROPERTY( ALevelInfo, bLonePlayer );
  LINK_NATIVE_PROPERTY( ALevelInfo, bBegunPlay );
  LINK_NATIVE_PROPERTY( ALevelInfo, bPlayersOnly );
  LINK_NATIVE_PROPERTY( ALevelInfo, bHighDetailMode );
  LINK_NATIVE_PROPERTY( ALevelInfo, bDropDetail );
  LINK_NATIVE_PROPERTY( ALevelInfo, bAggressiveLOD );
  LINK_NATIVE_PROPERTY( ALevelInfo, bStartup );
  LINK_NATIVE_PROPERTY( ALevelInfo, bHumansOnly );
  LINK_NATIVE_PROPERTY( ALevelInfo, bNoCheating );
  LINK_NATIVE_PROPERTY( ALevelInfo, bAllowFOV );
  LINK_NATIVE_PROPERTY( ALevelInfo, bSupportsRealCrouching );
  LINK_NATIVE_PROPERTY( ALevelInfo, EdBuildOpt );
  LINK_NATIVE_PROPERTY( ALevelInfo, bSpecularLight );
  LINK_NATIVE_PROPERTY( ALevelInfo, Song );
  LINK_NATIVE_PROPERTY( ALevelInfo, SongSection );
  LINK_NATIVE_PROPERTY( ALevelInfo, CdTrack );
  LINK_NATIVE_PROPERTY( ALevelInfo, PlayerDoppler );
  LINK_NATIVE_PROPERTY( ALevelInfo, backup_Song );
  LINK_NATIVE_PROPERTY( ALevelInfo, backup_SongSection );
  LINK_NATIVE_PROPERTY( ALevelInfo, LocalPlayer );
  LINK_NATIVE_PROPERTY( ALevelInfo, Brightness );
  LINK_NATIVE_PROPERTY( ALevelInfo, Screenshot );
  LINK_NATIVE_PROPERTY( ALevelInfo, DefaultTexture );
  LINK_NATIVE_PROPERTY( ALevelInfo, WhiteTexture );
  LINK_NATIVE_PROPERTY( ALevelInfo, TemplateLightTex );
  LINK_NATIVE_PROPERTY( ALevelInfo, HubStackLevel );
  LINK_NATIVE_PROPERTY( ALevelInfo, LevelAction );
  LINK_NATIVE_PROPERTY( ALevelInfo, NetMode );
  LINK_NATIVE_PROPERTY( ALevelInfo, ComputerName );
  LINK_NATIVE_PROPERTY( ALevelInfo, EngineVersion );
  LINK_NATIVE_PROPERTY( ALevelInfo, EngineSubVersion );
  LINK_NATIVE_PROPERTY( ALevelInfo, MinNetVersion );
  LINK_NATIVE_PROPERTY( ALevelInfo, DefaultGameType );
  LINK_NATIVE_PROPERTY( ALevelInfo, Game );
  LINK_NATIVE_PROPERTY( ALevelInfo, NavigationPointList );
  LINK_NATIVE_PROPERTY( ALevelInfo, PawnList );
  LINK_NATIVE_PROPERTY( ALevelInfo, NextURL );
  LINK_NATIVE_PROPERTY( ALevelInfo, bNextItems );
  LINK_NATIVE_PROPERTY( ALevelInfo, NextSwitchCountdown );
  LINK_NATIVE_ARRAY   ( ALevelInfo, AIProfile );
  LINK_NATIVE_PROPERTY( ALevelInfo, AvgAITime );
  LINK_NATIVE_PROPERTY( ALevelInfo, bCheckWalkSurfaces );
  LINK_NATIVE_PROPERTY( ALevelInfo, SpawnNotify );
  LINK_NATIVE_PROPERTY( ALevelInfo, FootprintManager );
  LINK_NATIVE_PROPERTY( ALevelInfo, ObjList );
  LINK_NATIVE_PROPERTY( ALevelInfo, DynamicZonesList );
  LINK_NATIVE_PROPERTY( ALevelInfo, ReplicationTarget );
  LINK_NATIVE_PROPERTY( ALevelInfo, MaxCommonRadius );
  LINK_NATIVE_PROPERTY( ALevelInfo, MaxCommonHeight );
  LINK_NATIVE_PROPERTY( ALevelInfo, MinCommonHeight );
  LINK_NATIVE_PROPERTY( ALevelInfo, MinCommonRadius );
  LINK_NATIVE_PROPERTY( ALevelInfo, CommonRadius );
  LINK_NATIVE_PROPERTY( ALevelInfo, HumanRadius );
  LINK_NATIVE_PROPERTY( ALevelInfo, HumanHeight );
END_PROPERTY_LINK()

