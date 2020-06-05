/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU General Public License as published by     *|
|*  the Free Software Foundation, either version 3 of the License, or        *|
|*  (at your option) any later version.                                      *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU General Public License for more details.                             *|
|*                                                                           *|
|*  You should have received a copy of the GNU General Public License        *|
|*  along with this program. If not, see <https://www.gnu.org/licenses/>.    *|
\*===========================================================================*/

/*========================================================================
 * UEngine.cpp - Base Engine Functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Engine/UEngine.h"

/*-----------------------------------------------------------------------------
 * UClient
-----------------------------------------------------------------------------*/

UClient::UClient()
  : UObject()
{
}

UClient::~UClient()
{
}

bool UClient::Init()
{
  GLogf( LOG_CRIT, "Tried to initialize null client type" );
  return false;
}

bool UClient::Exit()
{
  return false;
}

void UClient::HandleInput( int Key, bool bDown )
{
  if ( Key < 0xFF && Key >= 0x00 )
    if ( InputFuncs[Key] != NULL )
      InputFuncs[Key]( (EInputKey)Key, Engine->CurrentDeltaTime, bDown );
}

void UClient::HandleMouseInput( int XPos, int YPos )
{
  if ( MouseFunc != NULL )
  {
    // Get delta movement from center of screen
    int DeltaX = (CurrentViewport->Width / 2) - XPos;
    int DeltaY = (CurrentViewport->Height / 2) - YPos;
    MouseFunc( Engine->CurrentDeltaTime, DeltaX, DeltaY );
  }
}

void UClient::BindKeyInput( EInputKey Key, InputFunc Func )
{
  if ( Key < 0xFF && Key >= 0x00 )
    InputFuncs[Key] = Func;
}

void UClient::BindMouseInput( AxisInputFunc Func )
{
  MouseFunc = Func;
}

/*-----------------------------------------------------------------------------
 * UEngine
-----------------------------------------------------------------------------*/

UEngine* GEngine = NULL;

UEngine::UEngine()
  : USubsystem()
{
  GameRenderDevice = NULL;
  AudioDevice = NULL;
  Console = NULL;
  NetworkDevice = NULL;
  Language = NULL;
}

UEngine::~UEngine()
{

}

#if defined LIBUNR_WIN32
  #define DEFAULT_CLIENT "WindowsClient"
#elif defined LIBUNR_POSIX
  #define DEFAULT_CLIENT "X11Client"
#else
  #define DEFAULT_CLIENT "Client"
#endif

bool UEngine::Init()
{
  Pkg = UPackage::StaticLoadPackage( "Engine" );

  // Load default & white texture
  DefaultTexture = (UTexture*)StaticLoadObject( Pkg, "DefaultTexture", UTexture::StaticClass(), NULL );
  WhiteTexture = (UTexture*)StaticLoadObject( Pkg, "WhiteTexture", UTexture::StaticClass(), NULL );

  // Initialize our client
  FString ClientClassStr = GLibunrConfig->ReadString( "Engine.Engine", "Client", 0, DEFAULT_CLIENT );
  
  ClientClass = UObject::FindClass( FName( ClientClassStr.Data() ) );
  if ( ClientClass == NULL )
  {
    GLogf( LOG_CRIT, "Invalid client type '%s' specified", ClientClassStr.Data() );
    return false;
  }

  Client = (UClient*)ClientClass->CreateObject( FName( "LocalClient" ) );
  if ( !Client->Init() )
  {
    GLogf( LOG_CRIT, "Failed to initialize local client" );
    return false;
  }
  Client->Engine = this;
  
  // Init audio device
  FString Device( GSystem->AudioDevice );
  
  if( Device == FString("None") || Device.IsEmpty() )
  {
    GLogf( LOG_WARN, "Null AudioDevice selected." );
  }
  else
  {
  
    size_t Dot = Device.Find( '.' );
    if ( Dot == MAX_SIZE )
    {
      GLogf( LOG_ERR, "Incorrectly formatted AudioDevice setting in libunr.ini" );
      return false;
    }

    FString FileName = Device.Substr( 0, Dot );
    FString ClassName = Device.Substr( Dot+1 );

    AudioModule = new UDynamicNativeModule();
    if ( !AudioModule->Load( FileName.Data() ) )
    {
      GLogf( LOG_ERR, "Failed to load audio device '%s', cannot load native module", Device.Data() );
      return false;
    }

    AudioDevice = AudioModule->GetNativeClass( ClassName.Data() );
    if ( AudioDevice == NULL )
    {
      GLogf( LOG_ERR, "Failed to load audio device '%s', class does not exist", Device.Data() );
      return false;
    }

    Audio = (UAudioSubsystem*)AudioDevice->CreateObject( FName( "Audio", RF_Native ) );
    if ( !Audio->Init() )
    {
      GLogf( LOG_ERR, "Audio device initialization failed" );
      return false;
    }
  }
  
  // Init Render Device
  Device = FString( GSystem->RenderDevice );
  if ( Device == "None" || Device.IsEmpty() )
  {
    GLogf( LOG_WARN, "Null RenderDevice selected." );
  }
  else
  {
    size_t Dot = Device.Find( '.' );
    if ( Dot == MAX_SIZE )
    {
      GLogf( LOG_ERR, "Incorrectly formatted RenderDevice setting in libunr.ini" );
      return false;
    }

    FString FileName = Device.Substr( 0, Dot );
    FString ClassName = Device.Substr( Dot + 1 );

    RenderModule = new UDynamicNativeModule();
    if ( !RenderModule->Load( FileName.Data() ) )
    {
      GLogf( LOG_ERR, "Failed to load render device '%s', cannot load native module", Device.Data() );
      return false;
    }

    GameRenderDevice = RenderModule->GetNativeClass( ClassName.Data() );
    if ( GameRenderDevice == NULL )
    {
      GLogf( LOG_ERR, "Failed to load render device '%s', class does not exist", Device.Data() );
      return false;
    }

    Render = (URenderDevice*)GameRenderDevice->CreateObject( FName( "Render", RF_Native ) );
    if ( !Render->Init() )
    {
      GLogf( LOG_ERR, "Render device initialization failed" );
      return false;
    }

  }

  bInitialized = true;
  return true;
}

bool UEngine::Exit()
{
  Client->Exit();
  Audio->Exit();
  Render->Exit();
  return true;
}

void UEngine::Tick( float DeltaTime )
{
  if ( DeltaTime <= FLT_MIN )
    DeltaTime = FLT_MIN;

  CurrentDeltaTime = DeltaTime;
  TimeSeconds += DeltaTime;

  // Process audio
  if ( Audio )
    Audio->Tick( DeltaTime );

  // Process rendering
  if ( Render )
    Render->Tick( DeltaTime );

  // Process client events
  Client->Tick( DeltaTime );

  TickCycles++;
}

#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( UClient );
IMPLEMENT_NATIVE_CLASS( UEngine );

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
