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
 * UFire.cpp - Fire texture functionality
 * 
 * Inspiration taken from YRex's "NuPlayer" FireTexture code
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Engine/UEngine.h"
#include "Engine/UFire.h"

/*-----------------------------------------------------------------------------
 * UFractalTexture
-----------------------------------------------------------------------------*/
UFractalTexture::UFractalTexture()
  : UTexture()
{
}

UFractalTexture::~UFractalTexture()
{
}

void UFractalTexture::Tick( float DeltaTime )
{
}

/*-----------------------------------------------------------------------------
 * UFireTexture
-----------------------------------------------------------------------------*/
UFireTexture::UFireTexture()
  : UFractalTexture()
{
  Sparks = new TArray<Spark>();
  Particles = new TArray<Particle>();
}

UFireTexture::~UFireTexture()
{
  if ( Sparks )
    delete Sparks;

  if ( Particles )
    delete Particles;
}

void UFireTexture::Tick( float DeltaTime )
{
  Accumulator += DeltaTime;
  if ( Accumulator < (1.0f/40.0f) )
    return;

  Accumulator = 0.0f;
  bRealtimeChanged = true;

  // Enforce number of sparks based on array size
  NumSparks = Sparks->Size();

  u8* Buf = Buffer->DataArray.Data();

  #define BUF(x,y) (Buf[((y)<<UBits)+(x)])
  #define HEAT(h) (RenderTable[h&0x3FF])

  // TODO: SIMD acceleration?
  // Modify frame buffer
  for ( int y = 0; y < VSize; y++ )
  {
    for ( int x = 0; x < USize; x++ )
    {
      int Value;
      float HeatCoef;
      if ( bRising )
      {
        Value  = BUF(x,y);
        Value += BUF((x - 1) & UMask,(y + 1) & VMask);
        Value += BUF(x,(y + 1) & VMask);
        Value += BUF((x + 1) & UMask,(y + 1) & VMask);
        Value += 64;
        HeatCoef = 0.567;
      }
      else
      {
        Value  = BUF(x,(y + 1) & VMask); 
        Value += BUF(x,y);
        Value += BUF((x + 1) & UMask, y);
        Value += BUF((x - 1) & UMask, y);
        HeatCoef = 0.06 * (255-RenderHeat);
      }
      Value /= 4;
      Value += (int)ceil( (float)(RenderHeat - 250) * HeatCoef );
      if ( Value < 0 )
        Value = 0;

      BUF(x,y) = Value;
    }
  }


  // Iterate through all sparks and generate new particles for them
  for ( int i = 0; i < NumSparks; i++ )
  {
    if ( Particles->Size() < SparksLimit )
    {
      Spark& S = Sparks->At( i );
      u8 Heat;

      switch ( S.Type )
      {
      case SPARK_Burn:
        Heat = rand() % 224;
        BUF( S.X & UMask, S.Y & VMask ) = Heat;
        break;
      }
    }
  }

  // Go through each particle, adjust it's position and heat
  for ( int i = 0; i < Particles->Size(); i++ )
  {
    Particle& P = Particles->At(i);
    switch ( P.Type )
    {
    case SPARK_Burn:
      P.Heat = HEAT( P.Heat - 8 );
      P.Y -= 1;
      break;
    default:
      break;
    }

    if ( P.Heat == 0 )
    {
      Particles->Erase( i );
      i--;
      continue;
    }

    // Add it's contribution directly to the framebuffer
    if ( !bHardwareAccelerated )
    {
      // Modify the frame buffer
      switch ( P.Type )
      {
        case SPARK_Burn:
          BUF( P.X & UMask, P.Y & VMask ) = P.Heat;
          BUF( P.X & UMask, (P.Y - 1) & VMask ) = HEAT( P.Heat-12 );
          BUF( (P.X + 1) & UMask, P.Y & VMask ) = HEAT( P.Heat-24 );
          BUF( (P.X + 1) & UMask, (P.Y - 1) & VMask ) = HEAT( P.Heat-36 );
          break;
        default:
          BUF( P.X & UMask, (P.Y - 1) & VMask ) = P.Heat;
          BUF( (P.X - 1) & UMask, P.Y & VMask ) = HEAT( P.Heat );
          BUF( P.X & UMask, P.Y & VMask ) = HEAT( P.Heat );
          BUF( (P.X + 1) & UMask, P.Y & VMask ) = HEAT( P.Heat );
          break;
      }
    }
  }
}

void UFireTexture::Load()
{
  Super::Load();
  FPackageFileIn& In = *PkgFile;

  // Probably an index, but im unsure
  In >> CINDEX( NumSparks );
  Sparks->Reserve( NumSparks );

  for ( int i = 0; i < NumSparks; i++ )
  {
    Spark NewSpark;
    In >> NewSpark;
    Sparks->PushBack( NewSpark );
  }

  if ( GEngine->Render )
  {
    if ( !GEngine->Render->bAccelerateFractalTextures )
    {
      // Construct framebuffer for use in software
      bHardwareAccelerated = false;
      Buffer = &Mips[0];
      Buffer->DataArray.Resize( Buffer->USize * Buffer->VSize );
      UMask = Buffer->USize - 1;
      VMask = Buffer->VSize - 1;

      // Construct render table for fast color lookup without
      // necessarily caring about integer wraparound.
      // I have no clue if this was the original use for it,
      // but it seems like it can work here
      for ( int i = 0; i < 1028; i++ )
        RenderTable[i] = Clamp( (i / 8) * (RenderHeat / 8), 0, 255 );
    }
    else
    {
      // Renderer will handle drawing of fire textures
      bHardwareAccelerated = true;
    }
  }
}

bool UFireTexture::ExportToFile( const char* dir, const char* Type )
{
  return false;
}

/*-----------------------------------------------------------------------------
 * UWaterTexture
-----------------------------------------------------------------------------*/
UWaterTexture::UWaterTexture()
  : UFractalTexture()
{
}

UWaterTexture::~UWaterTexture()
{
}

/*-----------------------------------------------------------------------------
 * UWaveTexture
-----------------------------------------------------------------------------*/
UWaveTexture::UWaveTexture()
  : UWaterTexture()
{
}

UWaveTexture::~UWaveTexture()
{
}

/*-----------------------------------------------------------------------------
 * UWetTexture
-----------------------------------------------------------------------------*/
UWetTexture::UWetTexture()
  : UWaterTexture()
{
}

UWetTexture::~UWetTexture()
{
}

/*-----------------------------------------------------------------------------
 * UIceTexture
-----------------------------------------------------------------------------*/
UIceTexture::UIceTexture()
  : UFractalTexture()
{
}

UIceTexture::~UIceTexture()
{
}

IMPLEMENT_NATIVE_CLASS( UFractalTexture );
IMPLEMENT_NATIVE_CLASS( UFireTexture );
IMPLEMENT_NATIVE_CLASS( UWaterTexture );
IMPLEMENT_NATIVE_CLASS( UWaveTexture );
IMPLEMENT_NATIVE_CLASS( UWetTexture );
IMPLEMENT_NATIVE_CLASS( UIceTexture );

BEGIN_PROPERTY_LINK( UFractalTexture, 8 )
  LINK_NATIVE_PROPERTY( UMask );
  LINK_NATIVE_PROPERTY( VMask );
  LINK_NATIVE_PROPERTY( LightOutput );
  LINK_NATIVE_PROPERTY( SoundOutput );
  LINK_NATIVE_PROPERTY( GlobalPhase );
  LINK_NATIVE_PROPERTY( DrawPhase );
  LINK_NATIVE_PROPERTY( AuxPhase );
  LINK_NATIVE_PROPERTY( bHasInit );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UFireTexture, 20 )
  LINK_NATIVE_PROPERTY( SparkType );
  LINK_NATIVE_PROPERTY( RenderHeat );
  LINK_NATIVE_PROPERTY( bRising );
  LINK_NATIVE_PROPERTY( FX_Heat );
  LINK_NATIVE_PROPERTY( FX_Size );
  LINK_NATIVE_PROPERTY( FX_AuxSize );
  LINK_NATIVE_PROPERTY( FX_Area );
  LINK_NATIVE_PROPERTY( FX_Frequency );
  LINK_NATIVE_PROPERTY( FX_Phase );
  LINK_NATIVE_PROPERTY( FX_HorizSpeed );
  LINK_NATIVE_PROPERTY( FX_VertSpeed );
  LINK_NATIVE_PROPERTY( DrawMode );
  LINK_NATIVE_PROPERTY( SparksLimit );
  LINK_NATIVE_PROPERTY( NumSparks );
  LINK_NATIVE_PROPERTY( Sparks );
  LINK_NATIVE_PROPERTY( OldRenderHeat );
  LINK_NATIVE_ARRAY   ( RenderTable );
  LINK_NATIVE_PROPERTY( StarStatus );
  LINK_NATIVE_PROPERTY( PenDownX );
  LINK_NATIVE_PROPERTY( PenDownY );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UWaterTexture, 17 )
  LINK_NATIVE_PROPERTY( DropType );
  LINK_NATIVE_PROPERTY( WaveAmp );
  LINK_NATIVE_PROPERTY( FX_Frequency );
  LINK_NATIVE_PROPERTY( FX_Phase );
  LINK_NATIVE_PROPERTY( FX_Amplitude );
  LINK_NATIVE_PROPERTY( FX_Speed );
  LINK_NATIVE_PROPERTY( FX_Radius );
  LINK_NATIVE_PROPERTY( FX_Size );
  LINK_NATIVE_PROPERTY( FX_Depth );
  LINK_NATIVE_PROPERTY( FX_Time );
  LINK_NATIVE_PROPERTY( NumDrops );
  LINK_NATIVE_ARRAY   ( Drops );
  LINK_NATIVE_PROPERTY( SourceFields );
  LINK_NATIVE_ARRAY   ( RenderTable );
  LINK_NATIVE_ARRAY   ( WaterTable );
  LINK_NATIVE_PROPERTY( WaterParity );
  LINK_NATIVE_PROPERTY( OldWaveAmp );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UWaveTexture, 4 )
  LINK_NATIVE_PROPERTY( BumpMapLight );
  LINK_NATIVE_PROPERTY( BumpMapAngle );
  LINK_NATIVE_PROPERTY( PhongRange );
  LINK_NATIVE_PROPERTY( PhongSize );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UWetTexture, 3 )
  LINK_NATIVE_PROPERTY( SourceTexture );
  LINK_NATIVE_PROPERTY( OldSourceTex );
  LINK_NATIVE_PROPERTY( LocalSourceBitmap );
END_PROPERTY_LINK()

BEGIN_PROPERTY_LINK( UIceTexture, 21 )
  LINK_NATIVE_PROPERTY( GlassTexture );
  LINK_NATIVE_PROPERTY( SourceTexture );
  LINK_NATIVE_PROPERTY( PanningStyle );
  LINK_NATIVE_PROPERTY( TimeMethod );
  LINK_NATIVE_PROPERTY( HorizPanSpeed );
  LINK_NATIVE_PROPERTY( VertPanSpeed );
  LINK_NATIVE_PROPERTY( Frequency );
  LINK_NATIVE_PROPERTY( Amplitude );
  LINK_NATIVE_PROPERTY( MoveIce );
  LINK_NATIVE_PROPERTY( MasterCount );
  LINK_NATIVE_PROPERTY( UDisplace );
  LINK_NATIVE_PROPERTY( VDisplace );
  LINK_NATIVE_PROPERTY( UPosition );
  LINK_NATIVE_PROPERTY( VPosition );
  LINK_NATIVE_PROPERTY( TickAccu );
  LINK_NATIVE_PROPERTY( OldUDisplace );
  LINK_NATIVE_PROPERTY( OldVDisplace );
  LINK_NATIVE_PROPERTY( OldGlassTex );
  LINK_NATIVE_PROPERTY( OldSourceTex );
  LINK_NATIVE_PROPERTY( LocalSource );
  LINK_NATIVE_PROPERTY( ForceRefresh );
END_PROPERTY_LINK()

