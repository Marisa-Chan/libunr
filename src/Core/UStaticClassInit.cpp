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
 * UStaticClassInit.cpp - Initializes the object class for all native objects
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UObject.h"
#include "Core/UClass.h"
#include "Core/UAudio.h"
#include "Core/UCanvas.h"
#include "Core/UConsole.h"
#include "Core/UEngine.h"
#include "Core/UFire.h"
#include "Core/ULocale.h"
#include "Core/ULevel.h"
#include "Core/UMesh.h"
#include "Core/UMusic.h"
#include "Core/UNet.h"
#include "Core/UPlayer.h"
#include "Core/UProperty.h"
#include "Core/URender.h"
#include "Core/USound.h"
#include "Core/USystem.h"
#include "Core/UTexture.h"
#include "Core/UViewport.h"

#include "Actors/AActor.h"
#include "Actors/ABrush.h"
#include "Actors/ADecal.h"
#include "Actors/ADynamicZoneInfo.h"
#include "Actors/AGameInfo.h"
#include "Actors/AHUD.h"
#include "Actors/AInventory.h"
#include "Actors/ANavigationPoint.h"
#include "Actors/APawn.h"
#include "Actors/APlayerPawn.h"
#include "Actors/AProjector.h"
#include "Actors/AStatLog.h"
#include "Actors/ATimeDemo.h"
#include "Actors/AWeapon.h"
#include "Actors/AZoneInfo.h"

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
  UClass::BootstrapStage1();

  // Create UObject class so child classes have a parent
  Result &= UObject::StaticCreateClass();

  // Register low level classes for Core.u
  Result &= ULanguage::StaticClassInit();
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
 
  UClass::BootstrapStage2();

  // Now initialize all of UObject
  Result &= UObject::StaticClassInit();
  UObject::StaticClass()->PreLoad();
  UObject::StaticClass()->Load();
  UObject::StaticClass()->PostLoad();

  // Mark the object system as bootstrapped
  bStaticBootstrapped = true;

  // (The only reason we have to do this is because Unreal does it, 
  // lets not do this if we add custom stuff in the future, it's not very elegant)
  // Expose the description property
  EXPOSE_PROPERTY( UProperty, Description, UStrProperty, sizeof(String*) );

  // All previously created classes need their properties linked with Object
  ULanguage::StaticClass()->LinkSuperClassChildren();
  UPackage::StaticClass()->LinkSuperClassChildren();
  UTextBuffer::StaticClass()->LinkSuperClassChildren();
  UField::StaticClass()->LinkSuperClassChildren();
  UConst::StaticClass()->LinkSuperClassChildren();
  UEnum::StaticClass()->LinkSuperClassChildren();
  UProperty::StaticClass()->LinkSuperClassChildren();
  UByteProperty::StaticClass()->LinkSuperClassChildren();
  UIntProperty::StaticClass()->LinkSuperClassChildren();
  UBoolProperty::StaticClass()->LinkSuperClassChildren();
  UFloatProperty::StaticClass()->LinkSuperClassChildren();
  UObjectProperty::StaticClass()->LinkSuperClassChildren();
  UClassProperty::StaticClass()->LinkSuperClassChildren();
  UNameProperty::StaticClass()->LinkSuperClassChildren();
  UStringProperty::StaticClass()->LinkSuperClassChildren();
  UArrayProperty::StaticClass()->LinkSuperClassChildren();
  UStructProperty::StaticClass()->LinkSuperClassChildren();
  UStrProperty::StaticClass()->LinkSuperClassChildren();
  UMapProperty::StaticClass()->LinkSuperClassChildren();
  UFixedArrayProperty::StaticClass()->LinkSuperClassChildren();
  UStruct::StaticClass()->LinkSuperClassChildren();
  UFunction::StaticClass()->LinkSuperClassChildren();
  UState::StaticClass()->LinkSuperClassChildren();
  UClass::StaticClass()->LinkSuperClassChildren();

  Result &= USubsystem::StaticClassInit();
  Result &= UCommandlet::StaticClassInit();
  Result &= USystem::StaticClassInit();

  // Register Engine.u classes
  Result &= UAnimation::StaticClassInit();
  Result &= UAnimationNotify::StaticClassInit();
  Result &= UAudioSubsystem::StaticClassInit();
  Result &= UBitmap::StaticClassInit();
  Result &= UBspNodes::StaticClassInit();
  Result &= UBspSurfs::StaticClassInit();
    Result &= UTexture::StaticClassInit();
      Result &= UFractalTexture::StaticClassInit();
        Result &= UFireTexture::StaticClassInit();
        Result &= UWaterTexture::StaticClassInit();
          Result &= UWaveTexture::StaticClassInit();
          Result &= UWetTexture::StaticClassInit();
        Result &= UIceTexture::StaticClassInit();
      Result &= UScriptedTexture::StaticClassInit();
  Result &= UCanvas::StaticClassInit();
  Result &= UClient::StaticClassInit();
  Result &= UConsole::StaticClassInit();
  Result &= UEngine::StaticClassInit();
  Result &= UFont::StaticClassInit();
  Result &= ULevelBase::StaticClassInit();
  Result &= ULevel::StaticClassInit();
  Result &= ULevelSummary::StaticClassInit();
  Result &= UNetConnection::StaticClassInit();
  Result &= UNetDriver::StaticClassInit();
  Result &= USkeletalMesh::StaticClassInit();
  Result &= USound::StaticClassInit();
  Result &= UStaticLightData::StaticClassInit();
  Result &= UMusic::StaticClassInit();
  Result &= UPalette::StaticClassInit();
  Result &= UPolys::StaticClassInit();
  Result &= UPrimitive::StaticClassInit();

    // Once again, please don't do this if we add new variables for
    // a drop in Core.u or Engine.u replacement
    Result &= UMesh::StaticClassInit();
    EXPOSE_PROPERTY( UMesh, bCurvyMesh, UBoolProperty, 1 );

      Result &= ULodMesh::StaticClassInit();
        Result &= USkeletalMesh::StaticClassInit();
    Result &= UModel::StaticClassInit();
  Result &= UPlayer::StaticClassInit();
    Result &= UViewport::StaticClassInit();
  Result &= URenderBase::StaticClassInit();
  Result &= URenderDevice::StaticClassInit();
  Result &= URenderIterator::StaticClassInit();
  Result &= USkeletalMeshInstance::StaticClassInit();
  Result &= UVectors::StaticClassInit();
  Result &= UVerts::StaticClassInit();
      
  // Init actor classes
  Result &= AActor::StaticClassInit();
    Result &= ABrush::StaticClassInit();
      Result &= AMover::StaticClassInit();
    Result &= ADecal::StaticClassInit();
    Result &= ADecoration::StaticClassInit();
      Result &= ACarcass::StaticClassInit();
    Result &= AHUD::StaticClassInit();
    Result &= AHUDOverlay::StaticClassInit();
    Result &= AInfo::StaticClassInit();
      Result &= AGameInfo::StaticClassInit();
      Result &= AReplicationInfo::StaticClassInit();
        Result &= AGameReplicationInfo::StaticClassInit();
        Result &= APlayerReplicationInfo::StaticClassInit();
      Result &= ASavedMove::StaticClassInit();
      Result &= AStatLog::StaticClassInit();
      Result &= ATimeDemo::StaticClassInit();
      Result &= AZoneInfo::StaticClassInit();
        Result &= ADynamicZoneInfo::StaticClassInit();
        Result &= ALevelInfo::StaticClassInit();
    Result &= AKeypoint::StaticClassInit();
      Result &= AInterpolationPoint::StaticClassInit();
    Result &= ALight::StaticClassInit();
    Result &= AInventory::StaticClassInit();
      Result &= AWeapon::StaticClassInit();
    Result &= AInventoryAttachment::StaticClassInit();
      Result &= AWeaponAttachment::StaticClassInit();
      Result &= AWeaponMuzzleFlash::StaticClassInit();
    Result &= AMenu::StaticClassInit();
    Result &= ANavigationPoint::StaticClassInit();
      Result &= AInventorySpot::StaticClassInit();
      Result &= ALiftExit::StaticClassInit();
      Result &= APlayerStart::StaticClassInit();
      Result &= ATeleporter::StaticClassInit();
    Result &= APawn::StaticClassInit();
      Result &= APlayerPawn::StaticClassInit();
        Result &= ACamera::StaticClassInit();
    Result &= AProjectile::StaticClassInit();
    Result &= AProjector::StaticClassInit();
    Result &= ASpawnNotify::StaticClassInit();
    Result &= ATriggers::StaticClassInit();
      Result &= ATrigger::StaticClassInit();

  // Load base actor class
  AActor::StaticClass()->PreLoad();
  AActor::StaticClass()->Load();
  AActor::StaticClass()->PostLoad();

  return Result;
}

  IMPLEMENT_NATIVE_CLASS( UAnimationNotify );
  IMPLEMENT_NATIVE_CLASS( UCanvas );
  IMPLEMENT_NATIVE_CLASS( UClient );
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
  IMPLEMENT_NATIVE_CLASS( ULanguage );
  IMPLEMENT_NATIVE_CLASS( UMusic );
  IMPLEMENT_NATIVE_CLASS( UNetConnection );
  IMPLEMENT_NATIVE_CLASS( UPackage );
  IMPLEMENT_NATIVE_CLASS( UPlayer );
    IMPLEMENT_NATIVE_CLASS( UViewport );
  IMPLEMENT_NATIVE_CLASS( UPrimitive );
    IMPLEMENT_NATIVE_CLASS( UMesh );
    IMPLEMENT_NATIVE_CLASS( USound );
    IMPLEMENT_NATIVE_CLASS( USubsystem );
    IMPLEMENT_NATIVE_CLASS( UAudioSubsystem );
    IMPLEMENT_NATIVE_CLASS( UEngine );
    IMPLEMENT_NATIVE_CLASS( UNetDriver );
        IMPLEMENT_NATIVE_CLASS( USystem );
  IMPLEMENT_NATIVE_CLASS( UTextBuffer );
 
BEGIN_PROPERTY_LINK( UAnimationNotify, 5 )
  LINK_NATIVE_ARRAY   ( UAnimationNotify, AnimationNotify );
  LINK_NATIVE_PROPERTY( UAnimationNotify, NumNotifies );
  LINK_NATIVE_PROPERTY( UAnimationNotify, Owner );
  LINK_NATIVE_PROPERTY( UAnimationNotify, bInitialized );
  LINK_NATIVE_PROPERTY( UAnimationNotify, bErrorOccured );
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

BEGIN_PROPERTY_LINK( UPlayer, 9 )
  LINK_NATIVE_PROPERTY( UPlayer, vfOut );
  LINK_NATIVE_PROPERTY( UPlayer, vfExec );
  LINK_NATIVE_PROPERTY( UPlayer, Actor );
  LINK_NATIVE_PROPERTY( UPlayer, Console );
  LINK_NATIVE_PROPERTY( UPlayer, bWindowsMouseAvailable );
  LINK_NATIVE_PROPERTY( UPlayer, bShowWindowsMouse );
  LINK_NATIVE_PROPERTY( UPlayer, WindowsMouseX );
  LINK_NATIVE_PROPERTY( UPlayer, WindowsMouseY );
  LINK_NATIVE_PROPERTY( UPlayer, SelectedCursor );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UEngine, 15 )
  LINK_NATIVE_PROPERTY( UEngine, GameRenderDevice );
  LINK_NATIVE_PROPERTY( UEngine, AudioDevice );
  LINK_NATIVE_PROPERTY( UEngine, Console );
  LINK_NATIVE_PROPERTY( UEngine, NetworkDevice );
  LINK_NATIVE_PROPERTY( UEngine, Language );
  LINK_NATIVE_PROPERTY( UEngine, Cylinder );
  LINK_NATIVE_PROPERTY( UEngine, Client );
  LINK_NATIVE_PROPERTY( UEngine, Render );
  LINK_NATIVE_PROPERTY( UEngine, Audio );
  LINK_NATIVE_PROPERTY( UEngine, TickCycles );
  LINK_NATIVE_PROPERTY( UEngine, GameCycles );
  LINK_NATIVE_PROPERTY( UEngine, ClientCycles );
  LINK_NATIVE_PROPERTY( UEngine, CacheSizeMegs );
  LINK_NATIVE_PROPERTY( UEngine, UseSound );
  LINK_NATIVE_PROPERTY( UEngine, CurrentTickRate );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AHUD, 6 )
  LINK_NATIVE_PROPERTY( AHUD, HudMode );
  LINK_NATIVE_PROPERTY( AHUD, Crosshair );
  LINK_NATIVE_PROPERTY( AHUD, MainMenuType );
  LINK_NATIVE_PROPERTY( AHUD, HUDConfigWindowType );
  LINK_NATIVE_PROPERTY( AHUD, MainMenu );
  LINK_NATIVE_PROPERTY( AHUD, Overlays );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AHUDOverlay, 3 )
  LINK_NATIVE_PROPERTY( AHUDOverlay, myHUD );
  LINK_NATIVE_PROPERTY( AHUDOverlay, bPostRender );
  LINK_NATIVE_PROPERTY( AHUDOverlay, bPreRender );
END_PROPERTY_LINK()


