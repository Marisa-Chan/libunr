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
}

UFireTexture::~UFireTexture()
{
  if ( Sparks )
    delete Sparks;
}

void UFireTexture::Tick( float DeltaTime )
{
  Accumulator += DeltaTime;
  if ( Accumulator < (1.0f/40.0f) )
    return;

  Accumulator = 0.0f;
  bRealtimeChanged = true;

  // Make sure we catch any changes to variable state
  if ( RenderHeat != OldRenderHeat )
  {
    CalculateRenderTable();
    OldRenderHeat = RenderHeat;
  }

  // Enforce number of sparks based on array size
  NumSparks = Sparks->Size();

  u8* Buf = Buffer->DataArray.Data();

  #define BUF(x,y) (Buf[((y)<<UBits)+(x)])

  // TODO: SIMD acceleration?
  // Modify frame buffer
  for ( int y = 0; y < VSize; y++ )
  {
    for ( int x = 0; x < USize; x++ )
    {
      int Value;
      if ( bRising )
      {
        Value  = BUF( x, (y + 2) & VMask );
        Value += BUF( x, (y + 1) & VMask );
        Value += BUF( (x + 1) & UMask, (y + 1) & VMask );
        Value += BUF( (x - 1) & UMask, (y + 1) & VMask );
      }
      else
      {
        Value  = BUF(x,(y+1) & VMask); 
        Value += BUF(x,y);
        Value += BUF((x + 1) & UMask, y);
        Value += BUF((x - 1) & UMask, y);
      }

      // Setting render heat above 246 seems to make it go a bit crazy
      if ( RenderHeat > 246 && Value > 4 )
        Value += 2;
      else if ( RenderHeat > 220 && Value > 235 )
        Value += 1;

      BUF(x,y) = RenderTable[Value];
    }
  }


  // Iterate through all sparks and generate new particles for them
  for ( int i = 0; i < NumSparks; i++ )
  {
    if ( Sparks->Size() < SparksLimit )
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
      
      CalculateRenderTable();
      OldRenderHeat = RenderHeat;
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

void UFireTexture::CalculateRenderTable()
{

  // Construct render table for fast color lookup without
  // necessarily caring about integer wraparound.
  // I have no clue if this was the original use for it,
  // but it seems like it can work here
  for ( int i = 0; i < 1024; i++ )
  {
    // There's a couple of behaviors to observe here
    //
    // 1) On previous attempts, using RenderHeat on color values directly
    //    seemed to not be totally accurate. RenderHeat=249 did not give
    //    the desired result whereas lower RenderHeat values did
    //
    // 2) In the editor, any RenderHeat above 250 results in the image
    //    slowly going through each of the palette entries until the
    //    image is bright white
    //
    // Here, we try to set RenderTable colors based on RenderHeat and
    // span this color distribution out through the whole table.
    // Since the values we'll plug in here will be 0-255, and since
    // the render table is 1028 entries long (slightly higher than
    // 1024), we first try to divide i by 4 so that, as a baseline,
    // the render table is evenly spaced regardless of RenderHeat
    //
    // From there, we can use RenderHeat to adjust the color balance such
    // that colors lower in the table aren't just black/background color
    // 
    RenderTable[i] = Clamp( (i / 4) + ((RenderHeat - 250) / 16), 0, 255 );
  }
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

