/*========================================================================*\
|*  This file is part of libunr.                                          *|
|*                                                                        *|
|*  libunr is free software: you can redistribute it and/or modify        *|
|*  it under the terms of the GNU General Public License as published by  *|
|*  the Free Software Foundation, either version 3 of the License, or     *|
|*  (at your option) any later version.                                   *|
|*                                                                        *|
|*  libunr is distributed in the hope that it will be useful,             *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of        *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *|
|*  GNU General Public License for more details.                          *|
|*                                                                        *|
|*  You should have received a copy of the GNU General Public License     *|
|*  along with libunr.  If not, see <http://www.gnu.org/licenses/>.       *|
|*                                                                        *|
\*========================================================================*/

/*========================================================================
 * UStaticClassInit.cpp - Initializes the object class for all native objects
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "UObject.h"
#include "UClass.h"
#include "ULevel.h"
#include "UMesh.h"
#include "UMusic.h"
#include "UProperty.h"
#include "URender.h"
#include "USound.h"
#include "USystem.h"
#include "UTexture.h"

#include "AActor.h"
#include "AZoneInfo.h"
#include "ADynamicZoneInfo.h"

bool UObject::StaticInit()
{
  bool Result = true;

  ObjectPool = new Array<UObject*>();
  ClassPool  = new Array<UClass*> ();
  NativePropertyLists = new Array<FNativePropertyList*>();
  NativeFunctions = new Array<UFunction*>();

  if ( ObjectPool == NULL || ClassPool == NULL || NativePropertyLists == NULL ) 
    return false;

  ObjectPool->Reserve( 64 );
  ClassPool->Reserve( 64 );
  NativePropertyLists->Reserve( 64 );
  NativeFunctions->Reserve( 4096 );

  // Create the "class" class first, everything else depends on it
  if ( !UClass::StaticCreateClass() )
    return false;

  // Create UObject class so child classes have a parent
  Result &= UObject::StaticCreateClass();

  // Register low level classes for Core.u
  Result &= UPackage::StaticClassInit(); 
  Result &= UTextBuffer::StaticClassInit();
    Result &= UField::StaticClassInit();
      Result &= UConst::StaticClassInit();
      Result &= UEnum::StaticClassInit();
      Result &= UProperty::StaticClassInit();
        Result &= UByteProperty::StaticClassInit();
        Result &= UIntProperty::StaticClassInit();
        Result &= UBoolProperty::StaticClassInit();
        Result &= UFloatProperty::StaticClassInit();
        Result &= UObjectProperty::StaticClassInit();
          Result &= UClassProperty::StaticClassInit();
        Result &= UNameProperty::StaticClassInit();
        Result &= UStringProperty::StaticClassInit();
        Result &= UArrayProperty::StaticClassInit();
        Result &= UStructProperty::StaticClassInit();
        Result &= UStrProperty::StaticClassInit();
        Result &= UMapProperty::StaticClassInit();
        Result &= UFixedArrayProperty::StaticClassInit();
      Result &= UStruct::StaticClassInit();
        Result &= UFunction::StaticClassInit();
        Result &= UState::StaticClassInit();
          Result &= UClass::StaticClassInit();

  // Now initialize all of UObject
  Result &= UObject::StaticClassInit();
  Result &= USubsystem::StaticClassInit();
  Result &= UCommandlet::StaticClassInit();

  // Register System, which also does not have a class
  Result &= USystem::StaticClassInit();

  // Register Engine.u classes
  Result &= UAnimation::StaticClassInit();
  Result &= ULevelBase::StaticClassInit();
  Result &= ULevel::StaticClassInit();
  Result &= USkeletalMesh::StaticClassInit();
  Result &= USound::StaticClassInit();
  Result &= UMusic::StaticClassInit();
  Result &= UPrimitive::StaticClassInit();

  Result &= UPalette::StaticClassInit();
  Result &= UBitmap::StaticClassInit();
    Result &= UTexture::StaticClassInit();
//  Result &= UAnimationNotify::StaticClassInit();
//  Result &= USkeletalMeshInstance::StaticClassInit();
//  Result &= URenderIterator::StaticClassInit();

  // Circular dependencies in Engine.u require that we initialize property lists first
  Result &= AActor::StaticLinkNativeProperties();
  Result &= AInfo::StaticLinkNativeProperties();
  Result &= AZoneInfo::StaticLinkNativeProperties();
  Result &= ADynamicZoneInfo::StaticLinkNativeProperties();

  // Now init classes
  Result &= AActor::StaticClassInit();
    Result &= AInfo::StaticClassInit();
      Result &= AZoneInfo::StaticClassInit();
        Result &= ADynamicZoneInfo::StaticClassInit();

  return Result;
}

IMPLEMENT_NATIVE_CLASS( UObject );
  IMPLEMENT_NATIVE_CLASS( UAnimation );
  IMPLEMENT_NATIVE_CLASS( UAnimationNotify );
  IMPLEMENT_NATIVE_CLASS( UCommandlet );
  IMPLEMENT_NATIVE_CLASS( ULevelBase );
    IMPLEMENT_NATIVE_CLASS( ULevel );
  IMPLEMENT_NATIVE_CLASS( UTextBuffer );
  IMPLEMENT_NATIVE_CLASS( UField );
    IMPLEMENT_NATIVE_CLASS( UConst );
    IMPLEMENT_NATIVE_CLASS( UEnum );
    IMPLEMENT_NATIVE_CLASS( UProperty );
      IMPLEMENT_NATIVE_CLASS( UByteProperty );
      IMPLEMENT_NATIVE_CLASS( UIntProperty );
      IMPLEMENT_NATIVE_CLASS( UBoolProperty );
      IMPLEMENT_NATIVE_CLASS( UFloatProperty );
      IMPLEMENT_NATIVE_CLASS( UObjectProperty );
        IMPLEMENT_NATIVE_CLASS( UClassProperty );
      IMPLEMENT_NATIVE_CLASS( UNameProperty );
      IMPLEMENT_NATIVE_CLASS( UStringProperty );
      IMPLEMENT_NATIVE_CLASS( UArrayProperty );
      IMPLEMENT_NATIVE_CLASS( UStructProperty );
      IMPLEMENT_NATIVE_CLASS( UStrProperty );
      IMPLEMENT_NATIVE_CLASS( UMapProperty );
      IMPLEMENT_NATIVE_CLASS( UFixedArrayProperty );
    IMPLEMENT_NATIVE_CLASS( UStruct );
      IMPLEMENT_NATIVE_CLASS( UFunction );
      IMPLEMENT_NATIVE_CLASS( UState );
        IMPLEMENT_NATIVE_CLASS( UClass );
  IMPLEMENT_NATIVE_CLASS( UMusic );
  IMPLEMENT_NATIVE_CLASS( USound );
  IMPLEMENT_NATIVE_CLASS( UPalette );
  IMPLEMENT_NATIVE_CLASS( UPackage );
  IMPLEMENT_NATIVE_CLASS( UPrimitive );
  IMPLEMENT_NATIVE_CLASS( USkeletalMesh );
  IMPLEMENT_NATIVE_CLASS( USkeletalMeshInstance );
  IMPLEMENT_NATIVE_CLASS( USubsystem );
    IMPLEMENT_NATIVE_CLASS( USystem );
  IMPLEMENT_NATIVE_CLASS( UBitmap );
    IMPLEMENT_NATIVE_CLASS( UTexture );

IMPLEMENT_NATIVE_CLASS( AActor );
  IMPLEMENT_NATIVE_CLASS( AInfo );
    IMPLEMENT_NATIVE_CLASS( AZoneInfo );
      IMPLEMENT_NATIVE_CLASS( ADynamicZoneInfo );

bool UObject::StaticLinkNativeProperties()
{
  if ( StaticInitNativePropList( 5 ) )
  {
    LINK_NATIVE_ARRAY( UObject, ObjectInternal );
    LINK_NATIVE_PROPERTY( UObject, Outer );
    LINK_NATIVE_PROPERTY( UObject, Flags );
    LINK_NATIVE_PROPERTY( UObject, Name );
    LINK_NATIVE_PROPERTY( UObject, Class );
    return true;
  }
  return false;
}

BEGIN_PROPERTY_LINK( UAnimationNotify, 5 )
  LINK_NATIVE_ARRAY   ( UAnimationNotify, AnimationNotify );
  LINK_NATIVE_PROPERTY( UAnimationNotify, NumNotifies );
  LINK_NATIVE_PROPERTY( UAnimationNotify, Owner );
  LINK_NATIVE_PROPERTY( UAnimationNotify, bInitialized );
  LINK_NATIVE_PROPERTY( UAnimationNotify, bErrorOccured );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UCommandlet, 14 )
  LINK_NATIVE_PROPERTY( UCommandlet, HelpCmd );
  LINK_NATIVE_PROPERTY( UCommandlet, HelpOneLiner );
  LINK_NATIVE_PROPERTY( UCommandlet, HelpUsage );
  LINK_NATIVE_PROPERTY( UCommandlet, HelpWebLink );
  LINK_NATIVE_ARRAY   ( UCommandlet, HelpParm );
  LINK_NATIVE_ARRAY   ( UCommandlet, HelpDesc );
  LINK_NATIVE_PROPERTY( UCommandlet, LogToStdout );
  LINK_NATIVE_PROPERTY( UCommandlet, IsServer );
  LINK_NATIVE_PROPERTY( UCommandlet, IsClient );
  LINK_NATIVE_PROPERTY( UCommandlet, IsEditor );
  LINK_NATIVE_PROPERTY( UCommandlet, LazyLoad );
  LINK_NATIVE_PROPERTY( UCommandlet, ShowErrorCount );
  LINK_NATIVE_PROPERTY( UCommandlet, ShowBanner );
  LINK_NATIVE_PROPERTY( UCommandlet, ForceInt );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UPalette, 1 )
  LINK_NATIVE_ARRAY( UPalette, Colors ); 
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UBitmap, 11 )
  LINK_NATIVE_PROPERTY( UBitmap, Format );
  LINK_NATIVE_PROPERTY( UBitmap, Palette );
  LINK_NATIVE_PROPERTY( UBitmap, UBits );
  LINK_NATIVE_PROPERTY( UBitmap, VBits );
  LINK_NATIVE_PROPERTY( UBitmap, USize );
  LINK_NATIVE_PROPERTY( UBitmap, VSize );
  LINK_NATIVE_PROPERTY( UBitmap, UClamp );
  LINK_NATIVE_PROPERTY( UBitmap, VClamp );
  LINK_NATIVE_PROPERTY( UBitmap, MipZero );
  LINK_NATIVE_PROPERTY( UBitmap, MaxColor );
  LINK_NATIVE_ARRAY( UBitmap, InternalTime );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UTexture, 32 )
  LINK_NATIVE_PROPERTY( UTexture, BumpMap );
  LINK_NATIVE_PROPERTY( UTexture, DetailTexture );
  LINK_NATIVE_PROPERTY( UTexture, MacroTexture );
  LINK_NATIVE_PROPERTY( UTexture, Diffuse );
  LINK_NATIVE_PROPERTY( UTexture, Specular );
  LINK_NATIVE_PROPERTY( UTexture, Alpha );
  LINK_NATIVE_PROPERTY( UTexture, DrawScale );
  LINK_NATIVE_PROPERTY( UTexture, Friction );
  LINK_NATIVE_PROPERTY( UTexture, MipMult );
  LINK_NATIVE_ARRAY   ( UTexture, FootstepSound );
  LINK_NATIVE_PROPERTY( UTexture, HitSound );
  LINK_NATIVE_PROPERTY( UTexture, bHighColorQuality );
  LINK_NATIVE_PROPERTY( UTexture, bHighTextureQuality );
  LINK_NATIVE_PROPERTY( UTexture, bRealtime );
  LINK_NATIVE_PROPERTY( UTexture, bParametric );
  LINK_NATIVE_PROPERTY( UTexture, bRealtimeChanged );
  LINK_NATIVE_PROPERTY( UTexture, bHasComp );
  LINK_NATIVE_PROPERTY( UTexture, bFractical );
  LINK_NATIVE_PROPERTY( UTexture, LODSet );
  LINK_NATIVE_PROPERTY( UTexture, AnimNext );
  LINK_NATIVE_PROPERTY( UTexture, AnimCurrent );
  LINK_NATIVE_PROPERTY( UTexture, PrimeCount );
  LINK_NATIVE_PROPERTY( UTexture, PrimeCurrent );
  LINK_NATIVE_PROPERTY( UTexture, MinFrameRate );
  LINK_NATIVE_PROPERTY( UTexture, MaxFrameRate );
  LINK_NATIVE_PROPERTY( UTexture, Accumulator );
  LINK_NATIVE_PROPERTY( UTexture, Mips );
  LINK_NATIVE_PROPERTY( UTexture, CompMips );
  LINK_NATIVE_PROPERTY( UTexture, CompFormat );
  LINK_NATIVE_PROPERTY( UTexture, SurfaceType );
  LINK_NATIVE_PROPERTY( UTexture, UClampMode );
  LINK_NATIVE_PROPERTY( UTexture, VClampMode );
  LINK_NATIVE_PROPERTY( UTexture, PaletteTransform );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( USkeletalMeshInstance, 20 )
  LINK_NATIVE_PROPERTY( USkeletalMeshInstance, SpaceBases );
  LINK_NATIVE_PROPERTY( USkeletalMeshInstance, CachedLinks );
  LINK_NATIVE_PROPERTY( USkeletalMeshInstance, bHasUpdated );
  LINK_NATIVE_PROPERTY( USkeletalMeshInstance, LastDrawnMesh );
  LINK_NATIVE_PROPERTY( USkeletalMeshInstance, CachedAnim );
  LINK_NATIVE_PROPERTY( USkeletalMeshInstance, CachedOrientations );
  LINK_NATIVE_PROPERTY( USkeletalMeshInstance, CachedPositions );
  LINK_NATIVE_PROPERTY( USkeletalMeshInstance, TweenStartFrame );
  LINK_NATIVE_PROPERTY( USkeletalMeshInstance, Base );
  LINK_NATIVE_PROPERTY( USkeletalMeshInstance, bHasCachedFrame );
  LINK_NATIVE_PROPERTY( USkeletalMeshInstance, bWasTweening );
  LINK_NATIVE_PROPERTY( USkeletalMeshInstance, CachedTweenSeq );
  LINK_NATIVE_PROPERTY( USkeletalMeshInstance, Modifiers );
  LINK_NATIVE_PROPERTY( USkeletalMeshInstance, Channels );
  LINK_NATIVE_PROPERTY( USkeletalMeshInstance, TChannelPtr );
  LINK_NATIVE_PROPERTY( USkeletalMeshInstance, AttachedActors );
  LINK_NATIVE_PROPERTY( USkeletalMeshInstance, AttachedBoneIndex );
  LINK_NATIVE_PROPERTY( USkeletalMeshInstance, AttachedBoneName );
  LINK_NATIVE_PROPERTY( USkeletalMeshInstance, MyAttachment );
  LINK_NATIVE_PROPERTY( USkeletalMeshInstance, HardAttachFlags );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AInfo, 0 )
END_PROPERTY_LINK()

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

BEGIN_PROPERTY_LINK( ADynamicZoneInfo, 10 )
  LINK_NATIVE_PROPERTY( ADynamicZoneInfo, ZoneAreaType );
  LINK_NATIVE_PROPERTY( ADynamicZoneInfo, BoxMin );
  LINK_NATIVE_PROPERTY( ADynamicZoneInfo, BoxMax );
  LINK_NATIVE_PROPERTY( ADynamicZoneInfo, CylinderSize );
  LINK_NATIVE_PROPERTY( ADynamicZoneInfo, SphereSize );
  LINK_NATIVE_PROPERTY( ADynamicZoneInfo, MatchOnlyZone );
  LINK_NATIVE_PROPERTY( ADynamicZoneInfo, bUseRelativeToRotation );
  LINK_NATIVE_PROPERTY( ADynamicZoneInfo, bMovesForceTouchUpdate );
  LINK_NATIVE_PROPERTY( ADynamicZoneInfo, bUpdateTouchers );
  LINK_NATIVE_PROPERTY( ADynamicZoneInfo, OldPose );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AActor, 221 )
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
  LINK_NATIVE_PROPERTY( AActor, CollisionOverride );
  LINK_NATIVE_PROPERTY( AActor, MeshInstance );
  LINK_NATIVE_PROPERTY( AActor, RelativeLocation );
  LINK_NATIVE_PROPERTY( AActor, RelativeRotation );
  LINK_NATIVE_PROPERTY( AActor, LightDataPtr );
  LINK_NATIVE_PROPERTY( AActor, MeshDataPtr );
  LINK_NATIVE_PROPERTY( AActor, DrawScale3D );
  LINK_NATIVE_PROPERTY( AActor, ProjectorList );
  LINK_NATIVE_PROPERTY( AActor, InitialNetProperties );
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

