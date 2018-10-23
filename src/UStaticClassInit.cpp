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
#include "UMusic.h"
#include "UProperty.h"
#include "USound.h"
#include "USystem.h"
#include "UTexture.h"

#define INIT_CLASS(cls) cls::StaticCreateClass();

bool UObject::StaticInit()
{
  bool Result = true;

  ObjectPool = new Array<UObject*>();
  ClassPool  = new Array<UClass*> ();
  NativePropertyLists = new Array<FNativePropertyList*>();

  if ( ObjectPool == NULL || ClassPool == NULL || NativePropertyLists == NULL ) 
    return false;

  ObjectPool->Reserve( 64 );
  ClassPool->Reserve( 64 );
  NativePropertyLists->Reserve( 64 );

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

  // Register System, which also does not have a class
  Result &= USystem::StaticClassInit();

  // Register Engine.u classes
  Result &= USound::StaticClassInit();
  Result &= UMusic::StaticClassInit();
  Result &= UPalette::StaticClassInit();
  Result &= UBitmap::StaticClassInit();
    Result &= UTexture::StaticClassInit();
    
  return Result;
}

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

bool UPalette::StaticLinkNativeProperties()
{
  FNativePropertyList* SuperPropList = Super::StaticNativePropList;
  if ( SuperPropList == NULL )
    return false;

  if ( StaticInitNativePropList( 1 + SuperPropList->Num ) )
  {
    xstl::Copy( StaticNativePropList->Properties,
                sizeof( FNativePropertyLink ) * SuperPropList->Num,
                SuperPropList->Properties,
                sizeof( FNativePropertyLink ) * SuperPropList->Num );
    StaticNativePropList->Added = SuperPropList->Added;

    LINK_NATIVE_ARRAY( UPalette, Colors ); 
    return true;
  }
  return false;
}

bool UBitmap::StaticLinkNativeProperties()
{
  FNativePropertyList* SuperPropList = Super::StaticNativePropList;
  if ( SuperPropList == NULL )
    return false;

  if ( StaticInitNativePropList( 11 + SuperPropList->Num ) )
  {
    xstl::Copy( StaticNativePropList->Properties,
                sizeof( FNativePropertyLink ) * SuperPropList->Num,
                SuperPropList->Properties,
                sizeof( FNativePropertyLink ) * SuperPropList->Num );
    StaticNativePropList->Added = SuperPropList->Added;

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
    return true;
  }

  return false;
}

bool UTexture::StaticLinkNativeProperties()
{
  FNativePropertyList* SuperPropList = Super::StaticNativePropList;
  if ( SuperPropList == NULL )
    return false;

  // Double check this
  // this is gonna get REAL out of hand with Engine.Actor...
  if ( StaticInitNativePropList( 32 + SuperPropList->Num ) )
  {
    xstl::Copy( StaticNativePropList->Properties,
                sizeof( FNativePropertyLink ) * SuperPropList->Num,
                SuperPropList->Properties,
                sizeof( FNativePropertyLink ) * SuperPropList->Num );
    StaticNativePropList->Added = SuperPropList->Added;
         
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
    return true;
  }
  return false;
}

IMPLEMENT_NATIVE_CLASS( UObject );
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
  IMPLEMENT_NATIVE_CLASS( USubsystem );
    IMPLEMENT_NATIVE_CLASS( USystem );
  IMPLEMENT_NATIVE_CLASS( UBitmap );
    IMPLEMENT_NATIVE_CLASS( UTexture );

