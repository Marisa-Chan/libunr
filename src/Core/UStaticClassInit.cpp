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

#include "Core/UObject.h"
#include "Core/UClass.h"
#include "Core/UAudio.h"
#include "Core/UCanvas.h"
#include "Core/UConsole.h"
#include "Core/UEngine.h"
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
    Result &= UMesh::StaticClassInit();
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
    Result &= AKeypoint::StaticClassInit();
      Result &= AInterpolationPoint::StaticClassInit();
      Result &= AGameInfo::StaticClassInit();
      Result &= AReplicationInfo::StaticClassInit();
        Result &= AGameReplicationInfo::StaticClassInit();
        Result &= APlayerReplicationInfo::StaticClassInit();
      Result &= AStatLog::StaticClassInit();
      Result &= AZoneInfo::StaticClassInit();
        Result &= ADynamicZoneInfo::StaticClassInit();
        Result &= ALevelInfo::StaticClassInit();
    Result &= AInventory::StaticClassInit();
      Result &= AWeapon::StaticClassInit();
    Result &= AInventoryAttachment::StaticClassInit();
      Result &= AWeaponAttachment::StaticClassInit();
      Result &= AWeaponMuzzleFlash::StaticClassInit();
    Result &= AMenu::StaticClassInit();
    Result &= ANavigationPoint::StaticClassInit();
      Result &= AInventorySpot::StaticClassInit();
      Result &= APlayerStart::StaticClassInit();
      Result &= ATeleporter::StaticClassInit();
    Result &= APawn::StaticClassInit();
      Result &= APlayerPawn::StaticClassInit();
    Result &= AProjectile::StaticClassInit();
    Result &= ASpawnNotify::StaticClassInit();
    Result &= ATriggers::StaticClassInit();
      Result &= ATrigger::StaticClassInit();

  return Result;
}

  IMPLEMENT_NATIVE_CLASS( UAnimation );
  IMPLEMENT_NATIVE_CLASS( UAnimationNotify );
  IMPLEMENT_NATIVE_CLASS( UCanvas );
  IMPLEMENT_NATIVE_CLASS( UClient );
    IMPLEMENT_NATIVE_CLASS( UConsole );
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
  IMPLEMENT_NATIVE_CLASS( USkeletalMesh );
  IMPLEMENT_NATIVE_CLASS( USkeletalMeshInstance );
  IMPLEMENT_NATIVE_CLASS( USubsystem );
    IMPLEMENT_NATIVE_CLASS( UAudioSubsystem );
    IMPLEMENT_NATIVE_CLASS( UEngine );
    IMPLEMENT_NATIVE_CLASS( UNetDriver );
        IMPLEMENT_NATIVE_CLASS( USystem );
  IMPLEMENT_NATIVE_CLASS( UTextBuffer );
 
  IMPLEMENT_NATIVE_CLASS( AHUD );
  IMPLEMENT_NATIVE_CLASS( AHUDOverlay );
  IMPLEMENT_NATIVE_CLASS( ADynamicZoneInfo );

BEGIN_PROPERTY_LINK( UAnimationNotify, 5 )
  LINK_NATIVE_ARRAY   ( UAnimationNotify, AnimationNotify );
  LINK_NATIVE_PROPERTY( UAnimationNotify, NumNotifies );
  LINK_NATIVE_PROPERTY( UAnimationNotify, Owner );
  LINK_NATIVE_PROPERTY( UAnimationNotify, bInitialized );
  LINK_NATIVE_PROPERTY( UAnimationNotify, bErrorOccured );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UConsole, 40 )
  LINK_NATIVE_PROPERTY( UConsole, vtblOut );
  LINK_NATIVE_PROPERTY( UConsole, Viewport );
  LINK_NATIVE_PROPERTY( UConsole, HistoryTop );
  LINK_NATIVE_PROPERTY( UConsole, HistoryBot );
  LINK_NATIVE_PROPERTY( UConsole, HistoryCur );
  LINK_NATIVE_PROPERTY( UConsole, TypedStr );
  LINK_NATIVE_ARRAY   ( UConsole, History );
  LINK_NATIVE_PROPERTY( UConsole, Scrollback );
  LINK_NATIVE_PROPERTY( UConsole, NumLines );
  LINK_NATIVE_PROPERTY( UConsole, TopLine );
  LINK_NATIVE_PROPERTY( UConsole, TextLines );
  LINK_NATIVE_PROPERTY( UConsole, MsgTime );
  LINK_NATIVE_ARRAY   ( UConsole, MsgText );
  LINK_NATIVE_ARRAY   ( UConsole, MsgPRINames );
  LINK_NATIVE_ARRAY   ( UConsole, MsgPlayer );
  LINK_NATIVE_ARRAY   ( UConsole, MsgTick );
  LINK_NATIVE_PROPERTY( UConsole, BorderSize );
  LINK_NATIVE_PROPERTY( UConsole, ConsoleLines );
  LINK_NATIVE_PROPERTY( UConsole, BorderLines );
  LINK_NATIVE_PROPERTY( UConsole, BorderPixels );
  LINK_NATIVE_PROPERTY( UConsole, ConsolePos );
  LINK_NATIVE_PROPERTY( UConsole, ConsoleDest );
  LINK_NATIVE_PROPERTY( UConsole, FrameX );
  LINK_NATIVE_PROPERTY( UConsole, FrameY );
  LINK_NATIVE_PROPERTY( UConsole, ConBackground );
  LINK_NATIVE_PROPERTY( UConsole, Border );
  LINK_NATIVE_PROPERTY( UConsole, bNoStuff );
  LINK_NATIVE_PROPERTY( UConsole, bTyping );
  LINK_NATIVE_PROPERTY( UConsole, bTimeDemo );
  LINK_NATIVE_PROPERTY( UConsole, TimeDemo );
  LINK_NATIVE_PROPERTY( UConsole, bNoDrawWorld );
  LINK_NATIVE_PROPERTY( UConsole, TypingOffset );
  LINK_NATIVE_PROPERTY( UConsole, GlobalConsoleKey );
  LINK_NATIVE_PROPERTY( UConsole, GlobalWindowKey );
  LINK_NATIVE_PROPERTY( UConsole, LoadingMessage );
  LINK_NATIVE_PROPERTY( UConsole, SavingMessage );
  LINK_NATIVE_PROPERTY( UConsole, ConnectingMessage );
  LINK_NATIVE_PROPERTY( UConsole, PausedMessage );
  LINK_NATIVE_PROPERTY( UConsole, PrecachingMessage );
  LINK_NATIVE_PROPERTY( UConsole, bValidKeyEvent );
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

