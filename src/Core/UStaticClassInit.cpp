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
#include "Core/UStaticMesh.h"
#include "Core/USystem.h"
#include "Core/UTexture.h"
#include "Core/UViewport.h"

#include "Actors/AActor.h"
#include "Actors/ABrush.h"
#include "Actors/ADecal.h"
#include "Actors/ADynamicZoneInfo.h"
#include "Actors/AGameInfo.h"
#include "Actors/AHUD.h"
#include "Actors/AInternetLink.h"
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
  NameTable = new Array<FNameEntry*>();

  if ( ObjectPool == NULL || ClassPool == NULL || NativePropertyLists == NULL ) 
    return false;

  ObjectPool->Reserve( 64 );
  ClassPool->Reserve( 64 );
  NativePropertyLists->Reserve( 64 );
  NativeFunctions->Reserve( 4096 );
  NameTable->Reserve( 16384 );

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
  EXPOSE_PROPERTY( UProperty, Description, UStrProperty, sizeof(String*), PROP_Ascii );

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
    Result &= UTexture::StaticClassInit();
      Result &= UFractalTexture::StaticClassInit();
        Result &= UFireTexture::StaticClassInit();
        Result &= UWaterTexture::StaticClassInit();
          Result &= UWaveTexture::StaticClassInit();
          Result &= UWetTexture::StaticClassInit();
        Result &= UIceTexture::StaticClassInit();
      Result &= UScriptedTexture::StaticClassInit();
      Result &= UShadowBitMap::StaticClassInit();
  Result &= UBspNodes::StaticClassInit();
  Result &= UBspSurfs::StaticClassInit();
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
    EXPOSE_PROPERTY( UMesh, bCurvyMesh, UBoolProperty, 1, PROP_Bool );

      Result &= ULodMesh::StaticClassInit();
        Result &= USkeletalMesh::StaticClassInit();
      Result &= UStaticMesh::StaticClassInit();
    Result &= UModel::StaticClassInit();
  Result &= UPlayer::StaticClassInit();
    Result &= UViewport::StaticClassInit();
  Result &= URenderBase::StaticClassInit();
  Result &= URenderDevice::StaticClassInit();
  Result &= URenderIterator::StaticClassInit();
  Result &= USecurityData::StaticClassInit();
  Result &= USkeletalMeshInstance::StaticClassInit();
  Result &= UVectors::StaticClassInit();
  Result &= UVerts::StaticClassInit();
      
  // Init actor classes
  Result &= AActor::StaticClassInit();
    Result &= ABlockingActor::StaticClassInit();
    Result &= ABrush::StaticClassInit();
      Result &= AMover::StaticClassInit();
    Result &= ADecal::StaticClassInit();
    Result &= ADecoration::StaticClassInit();
      Result &= ACarcass::StaticClassInit();
    Result &= AHUD::StaticClassInit();
    Result &= AHUDOverlay::StaticClassInit();
    Result &= AInfo::StaticClassInit();
      Result &= AGameInfo::StaticClassInit();
      Result &= AInternetInfo::StaticClassInit();
        Result &= AInternetLink::StaticClassInit();
          Result &= ATcpLink::StaticClassInit();
          Result &= AUdpLink::StaticClassInit();
      Result &= AReplicationInfo::StaticClassInit();
        Result &= AGameReplicationInfo::StaticClassInit();
        Result &= APlayerReplicationInfo::StaticClassInit();
      Result &= ASavedMove::StaticClassInit();
      Result &= AStatLog::StaticClassInit();
        Result &= AStatLogFile::StaticClassInit();
      Result &= ATimeDemo::StaticClassInit();
      Result &= AVisibilityNotify::StaticClassInit();
      Result &= AZoneInfo::StaticClassInit();
        Result &= ADynamicZoneInfo::StaticClassInit();
        Result &= ALevelInfo::StaticClassInit();
        Result &= AWarpZoneInfo::StaticClassInit();
    Result &= AKeypoint::StaticClassInit();
      Result &= ACollisionPlane::StaticClassInit();
      Result &= AInterpolationPoint::StaticClassInit();
    Result &= ALight::StaticClassInit();
      Result &= ADynamicCorona::StaticClassInit();
      Result &= ASunlightCorona::StaticClassInit();
    Result &= AInventory::StaticClassInit();
      Result &= AWeapon::StaticClassInit();
    Result &= AInventoryAttachment::StaticClassInit();
      Result &= AWeaponAttachment::StaticClassInit();
      Result &= AWeaponMuzzleFlash::StaticClassInit();
    Result &= AMenu::StaticClassInit();
    Result &= ANavigationPoint::StaticClassInit();
      Result &= AInventorySpot::StaticClassInit();
      Result &= ALiftCenter::StaticClassInit();
      Result &= ALiftExit::StaticClassInit();
      Result &= APlayerStart::StaticClassInit();
      Result &= ATeleporter::StaticClassInit();
      Result &= AWarpZoneMarker::StaticClassInit();
    Result &= APawn::StaticClassInit();
      Result &= APlayerPawn::StaticClassInit();
        Result &= ACamera::StaticClassInit();
    Result &= AProjectile::StaticClassInit();
    Result &= AProjector::StaticClassInit();
    Result &= AStaticMeshActor::StaticClassInit();
    Result &= ASpawnNotify::StaticClassInit();
    Result &= ATriggers::StaticClassInit();
      Result &= ATrigger::StaticClassInit();

  // Load base actor class
  AActor::StaticClass()->PreLoad();
  AActor::StaticClass()->Load();
  AActor::StaticClass()->PostLoad();

  // Load IpDrv classes
  ATcpLink::StaticClass()->PreLoad();
  ATcpLink::StaticClass()->Load();
  ATcpLink::StaticClass()->PostLoad();

  AUdpLink::StaticClass()->PreLoad();
  AUdpLink::StaticClass()->Load();
  AUdpLink::StaticClass()->PostLoad();

  return Result;
}

  IMPLEMENT_NATIVE_CLASS( UAnimationNotify );
  IMPLEMENT_NATIVE_CLASS( UCanvas );
  IMPLEMENT_NATIVE_CLASS( UClient );
        IMPLEMENT_NATIVE_CLASS( ULanguage );
  IMPLEMENT_NATIVE_CLASS( UMusic );
  IMPLEMENT_NATIVE_CLASS( UPackage );
  IMPLEMENT_NATIVE_CLASS( UPlayer );
    IMPLEMENT_NATIVE_CLASS( UViewport );
  IMPLEMENT_NATIVE_CLASS( UPrimitive );
    IMPLEMENT_NATIVE_CLASS( UMesh );
    IMPLEMENT_NATIVE_CLASS( USound );
    IMPLEMENT_NATIVE_CLASS( USubsystem );
    IMPLEMENT_NATIVE_CLASS( UAudioSubsystem );
    IMPLEMENT_NATIVE_CLASS( UEngine );
    IMPLEMENT_NATIVE_CLASS( USystem );
  IMPLEMENT_NATIVE_CLASS( UTextBuffer );
 
BEGIN_PROPERTY_LINK( UAnimationNotify, 5 )
  LINK_NATIVE_ARRAY   ( AnimationNotify );
  LINK_NATIVE_PROPERTY( NumNotifies );
  LINK_NATIVE_PROPERTY( Owner );
  LINK_NATIVE_PROPERTY( bInitialized );
  LINK_NATIVE_PROPERTY( bErrorOccured );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( USkeletalMeshInstance, 20 )
  LINK_NATIVE_PROPERTY( SpaceBases );
  LINK_NATIVE_PROPERTY( CachedLinks );
  LINK_NATIVE_PROPERTY( bHasUpdated );
  LINK_NATIVE_PROPERTY( LastDrawnMesh );
  LINK_NATIVE_PROPERTY( CachedAnim );
  LINK_NATIVE_PROPERTY( CachedOrientations );
  LINK_NATIVE_PROPERTY( CachedPositions );
  LINK_NATIVE_PROPERTY( TweenStartFrame );
  LINK_NATIVE_PROPERTY( Base );
  LINK_NATIVE_PROPERTY( bHasCachedFrame );
  LINK_NATIVE_PROPERTY( bWasTweening );
  LINK_NATIVE_PROPERTY( CachedTweenSeq );
  LINK_NATIVE_PROPERTY( Modifiers );
  LINK_NATIVE_PROPERTY( Channels );
  LINK_NATIVE_PROPERTY( TChannelPtr );
  LINK_NATIVE_PROPERTY( AttachedActors );
  LINK_NATIVE_PROPERTY( AttachedBoneIndex );
  LINK_NATIVE_PROPERTY( AttachedBoneName );
  LINK_NATIVE_PROPERTY( MyAttachment );
  LINK_NATIVE_PROPERTY( HardAttachFlags );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UPlayer, 9 )
  LINK_NATIVE_PROPERTY( vfOut );
  LINK_NATIVE_PROPERTY( vfExec );
  LINK_NATIVE_PROPERTY( Actor );
  LINK_NATIVE_PROPERTY( Console );
  LINK_NATIVE_PROPERTY( bWindowsMouseAvailable );
  LINK_NATIVE_PROPERTY( bShowWindowsMouse );
  LINK_NATIVE_PROPERTY( WindowsMouseX );
  LINK_NATIVE_PROPERTY( WindowsMouseY );
  LINK_NATIVE_PROPERTY( SelectedCursor );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UEngine, 15 )
  LINK_NATIVE_PROPERTY( GameRenderDevice );
  LINK_NATIVE_PROPERTY( AudioDevice );
  LINK_NATIVE_PROPERTY( Console );
  LINK_NATIVE_PROPERTY( NetworkDevice );
  LINK_NATIVE_PROPERTY( Language );
  LINK_NATIVE_PROPERTY( Cylinder );
  LINK_NATIVE_PROPERTY( Client );
  LINK_NATIVE_PROPERTY( Render );
  LINK_NATIVE_PROPERTY( Audio );
  LINK_NATIVE_PROPERTY( TickCycles );
  LINK_NATIVE_PROPERTY( GameCycles );
  LINK_NATIVE_PROPERTY( ClientCycles );
  LINK_NATIVE_PROPERTY( CacheSizeMegs );
  LINK_NATIVE_PROPERTY( UseSound );
  LINK_NATIVE_PROPERTY( CurrentTickRate );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AHUD, 6 )
  LINK_NATIVE_PROPERTY( HudMode );
  LINK_NATIVE_PROPERTY( Crosshair );
  LINK_NATIVE_PROPERTY( MainMenuType );
  LINK_NATIVE_PROPERTY( HUDConfigWindowType );
  LINK_NATIVE_PROPERTY( MainMenu );
  LINK_NATIVE_PROPERTY( Overlays );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( AHUDOverlay, 3 )
  LINK_NATIVE_PROPERTY( myHUD );
  LINK_NATIVE_PROPERTY( bPostRender );
  LINK_NATIVE_PROPERTY( bPreRender );
END_PROPERTY_LINK()


