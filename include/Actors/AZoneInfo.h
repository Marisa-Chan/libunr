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
 * AZoneInfo.h - Infos that describe zones
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Actors/AActor.h"
#include "Core/UMusic.h"

class ASkyZoneInfo;

// TODO: Declare these properly
class ALocationID;
class AVisibilityNotify;

class AZoneInfo : public AInfo
{
  DECLARE_NATIVE_CLASS( AZoneInfo, AInfo, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AZoneInfo();

  // Zone Properties
  idx           ZoneTag;
  FVector       ZoneGravity;
  FVector       ZoneVelocity;
  float         ZoneGroundFriction;
  float         ZoneFluidFriction;
  float         ZoneTerminalVelocity;
  idx           ZonePlayerEvent;
  int           ZonePlayerCount;
  float         ZoneTimeDilation;
  int           NumCarcasses;
  int           DamagePerSec;
  int           DamageType;
  FString*       DamageString;
  FString*       ZoneName;
  FString*       LocationStrings[4];
  ALocationID*  LocationID;
  int           MaxCarcasses;
  USound*       EntrySound;
  USound*       ExitSound;
  UClass*       EntryActor;
  UClass*       ExitActor;
  ASkyZoneInfo* SkyZone;

  // Zone flags
  bool bWaterZone;
  bool bFogZone;
  bool bKillZone;
  bool bNeutralZone;
  bool bGravityZone;
  bool bPainZone;
  bool bDestructive;
  bool bNoInventory;
  bool bMoveProjectiles;
  bool bBounceVelocity;

  // Reverb
  bool bReverbZone;
  bool bRaytraceReverb;

  // Network
  bool bRepZoneProperties;
  
  // Distance Fog
  bool   bDistanceFogClips; 
  bool   bDistanceFog; 
  bool   bZoneBasedFog;
  float  FogDistanceStart; 
  FColor FogColor;
  float  FogDistance;
  float  FadeTime;   
  float  FogDensity; 
  int    FogMode;
  
  // Zone light
  u8        AmbientBrightness, AmbientHue, AmbientSaturation;
  UTexture* EnvironmentMap; 
  float     EnvironmentUScale,EnvironmentVScale; 
  FVector   EnvironmentColor; 
  float     TexUPanSpeed, TexVPanSpeed;
  FVector   ViewFlash, ViewFog;
  float     DirtyShadowLevel; 
  u8        LightMapDetailLevels[4];
  
  // Reverb Settings
  float SpeedOfSound;
  u8    MasterGain;
  int   CutoffHz;
  u8    Delay[6];
  u8    Gain[6];
  
  // 227 Ambients
  enum EAmbients
  {
    REVERB_PRESET_GENERIC,
    REVERB_PRESET_PADDEDCELL,
    REVERB_PRESET_ROOM,
    REVERB_PRESET_BATHROOM,
    REVERB_PRESET_LIVINGROOM,
    REVERB_PRESET_STONEROOM,
    REVERB_PRESET_AUDITORIUM,
    REVERB_PRESET_CONCERTHALL,
    REVERB_PRESET_CAVE,
    REVERB_PRESET_ARENA,
    REVERB_PRESET_HANGAR,
    REVERB_PRESET_CARPETTEDHALLWAY,
    REVERB_PRESET_HALLWAY,
    REVERB_PRESET_STONECORRIDOR,
    REVERB_PRESET_ALLEY,
    REVERB_PRESET_FOREST,
    REVERB_PRESET_CITY,
    REVERB_PRESET_MOUNTAINS,
    REVERB_PRESET_QUARRY,
    REVERB_PRESET_PLAIN,
    REVERB_PRESET_PARKINGLOT,
    REVERB_PRESET_SEWERPIPE,
    REVERB_PRESET_UNDERWATER,
    REVERB_PRESET_DRUGGED,
    REVERB_PRESET_DIZZY,
    REVERB_PRESET_PSYCHOTIC,
    REVERB_PRESET_CASTLE_SMALLROOM,
    REVERB_PRESET_CASTLE_SHORTPASSAGE,
    REVERB_PRESET_CASTLE_MEDIUMROOM,
    REVERB_PRESET_CASTLE_LONGPASSAGE,
    REVERB_PRESET_CASTLE_LARGEROOM,
    REVERB_PRESET_CASTLE_HALL,
    REVERB_PRESET_CASTLE_CUPBOARD,
    REVERB_PRESET_CASTLE_COURTYARD,
    REVERB_PRESET_CASTLE_ALCOVE,
    REVERB_PRESET_FACTORY_ALCOVE,
    REVERB_PRESET_FACTORY_SHORTPASSAGE,
    REVERB_PRESET_FACTORY_MEDIUMROOM,
    REVERB_PRESET_FACTORY_LONGPASSAGE,
    REVERB_PRESET_FACTORY_LARGEROOM,
    REVERB_PRESET_FACTORY_HALL,
    REVERB_PRESET_FACTORY_CUPBOARD,
    REVERB_PRESET_FACTORY_COURTYARD,
    REVERB_PRESET_FACTORY_SMALLROOM,
    REVERB_PRESET_ICEPALACE_ALCOVE,
    REVERB_PRESET_ICEPALACE_SHORTPASSAGE,
    REVERB_PRESET_ICEPALACE_MEDIUMROOM,
    REVERB_PRESET_ICEPALACE_LONGPASSAGE,
    REVERB_PRESET_ICEPALACE_LARGEROOM,
    REVERB_PRESET_ICEPALACE_HALL,
    REVERB_PRESET_ICEPALACE_CUPBOARD,
    REVERB_PRESET_ICEPALACE_COURTYARD,
    REVERB_PRESET_ICEPALACE_SMALLROOM,
    REVERB_PRESET_SPACESTATION_ALCOVE,
    REVERB_PRESET_SPACESTATION_MEDIUMROOM,
    REVERB_PRESET_SPACESTATION_SHORTPASSAGE,
    REVERB_PRESET_SPACESTATION_LONGPASSAGE,
    REVERB_PRESET_SPACESTATION_LARGEROOM,
    REVERB_PRESET_SPACESTATION_HALL,
    REVERB_PRESET_SPACESTATION_CUPBOARD,
    REVERB_PRESET_SPACESTATION_SMALLROOM,
    REVERB_PRESET_WOODEN_ALCOVE,
    REVERB_PRESET_WOODEN_SHORTPASSAGE,
    REVERB_PRESET_WOODEN_MEDIUMROOM,
    REVERB_PRESET_WOODEN_LONGPASSAGE,
    REVERB_PRESET_WOODEN_LARGEROOM,
    REVERB_PRESET_WOODEN_HALL,
    REVERB_PRESET_WOODEN_CUPBOARD,
    REVERB_PRESET_WOODEN_SMALLROOM,
    REVERB_PRESET_WOODEN_COURTYARD,
    REVERB_PRESET_SPORT_EMPTYSTADIUM,
    REVERB_PRESET_SPORT_SQUASHCOURT,
    REVERB_PRESET_SPORT_SMALLSWIMMINGPOOL,
    REVERB_PRESET_SPORT_LARGESWIMMINGPOOL,
    REVERB_PRESET_SPORT_GYMNASIUM,
    REVERB_PRESET_SPORT_FULLSTADIUM,
    REVERB_PRESET_SPORT_STADIUMTANNOY,
    REVERB_PRESET_PREFAB_WORKSHOP,
    REVERB_PRESET_PREFAB_SCHOOLROOM,
    REVERB_PRESET_PREFAB_PRACTISEROOM,
    REVERB_PRESET_PREFAB_OUTHOUSE,
    REVERB_PRESET_PREFAB_CARAVAN,
    REVERB_PRESET_DOME_TOMB,
    REVERB_PRESET_PIPE_SMALL,
    REVERB_PRESET_DOME_SAINTPAULS,
    REVERB_PRESET_PIPE_LONGTHIN,
    REVERB_PRESET_PIPE_LARGE,
    REVERB_PRESET_PIPE_RESONANT,
    REVERB_PRESET_OUTDOORS_BACKYARD,
    REVERB_PRESET_OUTDOORS_ROLLINGPLAINS,
    REVERB_PRESET_OUTDOORS_DEEPCANYON,
    REVERB_PRESET_OUTDOORS_CREEK,
    REVERB_PRESET_OUTDOORS_VALLEY,
    REVERB_PRESET_MOOD_HEAVEN,
    REVERB_PRESET_MOOD_HELL,
    REVERB_PRESET_MOOD_MEMORY,
    REVERB_PRESET_DRIVING_COMMENTATOR,
    REVERB_PRESET_DRIVING_PITGARAGE,
    REVERB_PRESET_DRIVING_INCAR_RACER,
    REVERB_PRESET_DRIVING_INCAR_SPORTS,
    REVERB_PRESET_DRIVING_INCAR_LUXURY,
    REVERB_PRESET_DRIVING_FULLGRANDSTAND,
    REVERB_PRESET_DRIVING_EMPTYGRANDSTAND,
    REVERB_PRESET_DRIVING_TUNNEL,
    REVERB_PRESET_CITY_STREETS,
    REVERB_PRESET_CITY_SUBWAY,
    REVERB_PRESET_CITY_MUSEUM,
    REVERB_PRESET_CITY_LIBRARY,
    REVERB_PRESET_CITY_UNDERPASS,
    REVERB_PRESET_CITY_ABANDONED,
    REVERB_PRESET_DUSTYROOM,
    REVERB_PRESET_CHAPEL,
    REVERB_PRESET_SMALLWATERROOM,
    REVERB_PRESET_UNDERSLIME,
    REVERB_PRESET_NONE,
  };
  
  EAmbients EFXAmbients;
  
  // Lens flare.
  UTexture* LensFlare[12];
  float     LensFlareOffset[12];
  float     LensFlareScale[12];
  
  // per-Zone mesh LOD lighting control
  u8  MinLightCount;
  u8  MaxLightCount;
  int MinLightingPolyCount;
  int MaxLightingPolyCount;
  
  // Network replication.
  AVisibilityNotify* VisNotify;
};

class ULevelSummary : public UObject
{
  DECLARE_NATIVE_CLASS( ULevelSummary, UObject, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ULevelSummary();

  FString* Title;
  FString* Author;
  FString* IdealPlayerCount;
  int     RecommendedEnemies;
  int     RecommendedTeammates;
  FString* LevelEnterText;
};

class ALevelInfo : public AZoneInfo
{
  DECLARE_NATIVE_CLASS( ALevelInfo, AZoneInfo, 0, Engine )
  EXPOSE_TO_USCRIPT()

  ALevelInfo();

  FString* GetLocalURL();
  FString* GetAddressURL();

  float TimeDilation;
  float TimeSeconds;
  int   Year;
  int   Month;
  int   Day;
  int   DayOfWeek;
  int   Hour;
  int   Minute;
  int   Second;
  int   Millisecond;

  FString* Title;
  FString* Author;
  FString* IdealPlayerCount;
  int     RecommendedEnemies;
  int     RecommendedTeammates;
  FString* LevelEnterText;
  FString* LocalizedPkg;
  FString* Pauser;
  ULevelSummary* Summary;
  FString* VisibleGroups;

  bool bLowRes;
  bool bNeverPrecache;
  bool bLonePlayer;
  bool bBegunPlay;
  bool bPlayersOnly;
  bool bHighDetailMode;
  bool bDropDetail;
  bool bAggressiveLOD;
  bool bStartup;
  bool bHumansOnly;
  bool bNoCheating;
  bool bAllowFOV;
  bool bSupportsRealCrouching;
  int  EdBuildOpt;
  bool bSpecularLight;

  UMusic* Song;
  u8      SongSection;
  u8      CdTrack;
  float   PlayerDoppler;
  UMusic* backup_Song;
  u8      backup_SongSection;

  APlayerPawn* LocalPlayer;

  float Brightness;
  UTexture* Screenshot;
  UTexture* DefaultTexture;
  UTexture* WhiteTexture;
  UTexture* TemplateLightTex;
  int       HubStackLevel;

  enum ELevelAction
  {
    LEVACT_None,
    LEVACT_Loading,
    LEVACT_Saving,
    LEVACT_Connecting,
    LEVACT_Precaching
  } LevelAction;

  enum ENetMode
  {
    NM_Standalone,
    NM_DedicatedServer,
    NM_ListenServer,
    NM_Client
  } NetMode;
  
  FString* ComputerName;
  FString* EngineVersion;
  FString* EngineSubVersion;
  FString* MinNetVersion;

  UClass* DefaultGameType;
  class AGameInfo* Game;

  ANavigationPoint* NavigationPointList;
  APawn* PawnList;

  FString* NextURL;
  bool bNextItems;
  float NextSwitchCountdown;

  int   AIProfile[8];
  float AvgAITime;

  bool bCheckWalkSurfaces;
  ASpawnNotify* SpawnNotify;
  UClass* FootprintManager;
  Array<UObject*>* ObjList;
  class ADynamicZoneInfo* DynamicZonesList;
  APlayerPawn* ReplicationTarget;

  int MaxCommonRadius;
  int MaxCommonHeight;
  int MinCommonHeight;
  int MinCommonRadius;
  int CommonRadius;
  int HumanRadius;
  int HumanHeight;

  // Deus Ex Variables
  class UEventManager* EventManager;
};

class AWarpZoneInfo : public AZoneInfo
{
  DECLARE_NATIVE_CLASS( AWarpZoneInfo, AZoneInfo, 0, Engine )
  EXPOSE_TO_USCRIPT()

  AWarpZoneInfo();

  FString* OtherSideURL;
  idx ThisTag;
  bool bNoTeleFrag;

  int            iWarpZone;
  FCoords        WarpCoords;
  AWarpZoneInfo* OtherSideActor;
  UObject*       OtherSideLevel;
  FString*       Destinations[8];
  int            NumDestinations;
};

