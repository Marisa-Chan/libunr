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
#include "Core/ULocale.h"
#include "Core/USystem.h"
#include "Core/UProperty.h"

#include "Engine/UAudio.h"
#include "Engine/UCanvas.h"
#include "Engine/UConsole.h"
#include "Engine/UEngine.h"
#include "Engine/UFire.h"
#include "Engine/UGesture.h"
#include "Engine/ULevel.h"
#include "Engine/UMesh.h"
#include "Engine/UMusic.h"
#include "Engine/UNet.h"
#include "Engine/UPlayer.h"
#include "Engine/URender.h"
#include "Engine/USound.h"
#include "Engine/UStaticMesh.h"
#include "Engine/UTexture.h"
#include "Engine/UViewport.h"

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

// Deus Ex specific headers
#include "DeusEx/UEventManager.h"
#include "DeusEx/ACameraPoint.h"

// Rune specific headers
#include "Rune/ARuneActor.h"

// Harry Potter specific headers
#include "HPSS/UParticleList.h"
#include "HPSS/AParticleFX.h"

bool UObject::StaticInit()
{
  bool Result = true;

  ObjectPool = TArray<UObject*>();
  ClassPool  = TArray<UClass*> ();
  NativePropertyLists = TArray<FNativePropertyList*>();
  NativeFunctions = TArray<UFunction*>();
  NameTable = TArray<FNameEntry*>();

  ObjectPool.Reserve( 64 );
  ClassPool.Reserve( 64 );
  NativePropertyLists.Reserve( 64 );
  NativeFunctions.Reserve( 4096 );
  NameTable.Reserve( 16384 );

  // Index 0 must always have "None"
  FName::CreateName( "None", RF_TagExp | RF_LoadContextFlags | RF_Native );

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
  EXPOSE_PROPERTY( UProperty, Description, UStrProperty, sizeof(FString*), PROP_Ascii );

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
  Result &= UGesture::StaticClassInit();
  Result &= ULevelBase::StaticClassInit();
  Result &= ULevel::StaticClassInit();
  Result &= ULevelSummary::StaticClassInit();
  Result &= UNetConnection::StaticClassInit();
  Result &= UNetDriver::StaticClassInit();
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
    Result &= USkelModel::StaticClassInit();
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
  //Result &= ARuneActor::StaticClassInit(); // aliased class

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
    Result &= AInventory::StaticClassInit();
      Result &= APickup::StaticClassInit();
        Result &= AAmmo::StaticClassInit();
      Result &= AWeapon::StaticClassInit();
    Result &= AInventoryAttachment::StaticClassInit();
      Result &= AWeaponAttachment::StaticClassInit();
      Result &= AWeaponMuzzleFlash::StaticClassInit();
    Result &= AKeypoint::StaticClassInit();
      Result &= ACollisionPlane::StaticClassInit();
      Result &= ALocationID::StaticClassInit();
      Result &= AInterpolationPoint::StaticClassInit();
    Result &= ALight::StaticClassInit();
      Result &= ADynamicCorona::StaticClassInit();
      Result &= ASunlightCorona::StaticClassInit();
    Result &= AMenu::StaticClassInit();
    Result &= AMutator::StaticClassInit();
    Result &= ANavigationPoint::StaticClassInit();
      Result &= AInventorySpot::StaticClassInit();
      Result &= ALiftCenter::StaticClassInit();
      Result &= ALiftExit::StaticClassInit();
      Result &= APlayerStart::StaticClassInit();
      Result &= ATeleporter::StaticClassInit();
      Result &= AWarpZoneMarker::StaticClassInit();
    Result &= APawn::StaticClassInit();
     // Result &= ARunePawn::StaticClassInit(); //aliased
      Result &= APlayerPawn::StaticClassInit();
//        Result &= ARunePlayerPawn::StaticClassInit(); // aliased
        Result &= ACamera::StaticClassInit();
    Result &= AProjectile::StaticClassInit();
    Result &= AProjector::StaticClassInit();
    Result &= AStaticMeshActor::StaticClassInit();
    Result &= ASpawnNotify::StaticClassInit();
    Result &= ATriggers::StaticClassInit();
      Result &= ATrigger::StaticClassInit();

  // Init Deus Ex classes (if needed)
  if ( GSystem->GameFlags & GAME_DeusEx )
  {
    Result &= UEventManager::StaticClassInit();
    Result &= ACameraPoint::StaticClassInit();
  }

  if ( GSystem->GameFlags & GAME_Rune )
  {
    Result &= AAnimationProxy::StaticClassInit();
    Result &= AAutoLink::StaticClassInit();
    Result &= ADebugHUD::StaticClassInit();
    Result &= ADispatcher::StaticClassInit(); // only native in rune?
    Result &= AParticleSystem::StaticClassInit();
    Result &= APolyObj::StaticClassInit();
    Result &= AScriptDispatcher::StaticClassInit();
    Result &= AScriptAction::StaticClassInit();
    Result &= ASoundPlayer::StaticClassInit(); // useful for all games but why native?
  }

  if ( GSystem->GameFlags & GAME_HPSS )
  {
    Result &= UParticleList::StaticClassInit();
    Result &= AParticleFX::StaticClassInit();
  }

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

  // Load Deus Ex engine classes (if needed)
  if ( GSystem->GameFlags & GAME_DeusEx )
  {
    ACameraPoint::StaticClass()->PreLoad();
    ACameraPoint::StaticClass()->Load();
    ACameraPoint::StaticClass()->PostLoad();
  }

  return Result;
}

