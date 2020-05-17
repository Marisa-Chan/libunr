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
 * Huge inspiration taken from YRex's "NuPlayer" FireTexture code
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Engine/UEngine.h"
#include "Engine/UFire.h"

// TODO: Super fast random number generation
// Calling rand() over and over again can't be very efficient...
inline int rand1()
{
  return (rand() % 2) ? 1 : -1;
}

inline int rand2()
{
  int r = rand() % 3;
  switch ( r )
  {
  case 0:
  case 1:
    return r;
  case 2:
    return -1;
  }
}

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
}

UFireTexture::~UFireTexture()
{
  if ( Sparks )
    delete Sparks;
}

void UFireTexture::Tick( float DeltaTime )
{
  // Tick once per frame
  if ( CurrentTick == GEngine->TickCycles )
    return;

  CurrentTick = GEngine->TickCycles;

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

  #define DRAW_BURN_POINT(x,y,h) \
  if ( bRising ) {BUF( (x-1) & UMask, y & VMask ) = h;} \
  {BUF( x & UMask, y & VMask ) = h; BUF( x & UMask, (y-1) & VMask ) = h; BUF( (x+1) & UMask, y & VMask ) = h;}

  #define DRAW_POINT(x, y, h) \
  Heat = h; \
  if ( bRising ) {BUF( (x+1) & UMask, y & VMask ) = Heat;} \
  {BUF( x & UMask, y & VMask ) = Heat; BUF( x & UMask, (y-1) & VMask ) = Heat; BUF( (x-1) & UMask, y & VMask ) = Heat;}

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

      BUF(x,y) = RenderTable[Value];
    }
  }


  // Iterate through all sparks and generate new particles for them
  for ( int i = 0; i < Sparks->Size(); i++ )
  {
    Spark& S = Sparks->At( i );
    Spark New;
    u8 Heat;
    u8 Extra;

    // General rules/mindset for figuring out how each spark type works
    //
    // 1) Effects seem to be very simple, don't over think it
    // 2) The "Byte" variables in each Spark must have a use if they aren't 0
    //
    switch ( S.Type )
    {
    case SPARK_Burn:
      BUF( S.X, S.Y ) = rand() % RenderHeat;
      break;
    case SPARK_Sparkle:
      Extra = ((rand() % 64) + 16);
      BUF( (S.X + Extra) & UMask, S.Y ) = (rand() % S.Heat) - 8;
      break;
    case SPARK_Pulse:
      S.ByteA += S.ByteD;
      BUF( S.X & UMask, S.Y & VMask ) = S.ByteA;
      break;
    case SPARK_Signal:
      DRAW_BURN_POINT( S.X, S.Y, 96 + (rand() % 127) );
      break;
    case SPARK_Blaze:
      if ( Sparks->Size() < SparksLimit && (rand() & 0xff) < 64 )
      {
        New.ByteA = rand() & 0xff;
        New.ByteB = rand() & 0xff;
        New.X = S.X;
        New.Y = S.Y;
        New.Heat = S.Heat;
        New.Type = SPARK_BlazeSpark;
        Sparks->PushBack( New );
      }
      break;
    case SPARK_BlazeSpark: // Spawned by SPARK_Blaze sparks
      S.Heat -= 5;
      if ( S.Heat < 5 )
        Sparks->Erase( i );
      else
      {
        if ( (rand() & 0x7f) < (S.ByteA & 0x7f) )
          S.X += (S.ByteA & 0x80) ? -1 : 1;
        if ( (rand() & 0x7f) < (S.ByteB & 0x7f) )
          S.Y += (S.ByteB & 0x80) ? -1 : 1;

        S.X &= UMask;
        S.Y &= VMask;

        BUF( S.X, S.Y ) = S.Heat;
      }
      break;
    case SPARK_OzHasSpoken:
      if ( Sparks->Size() < SparksLimit && (rand() & 0xff) < 128 )
      {
        New.ByteA = (rand() & 0x7f) - 63; // subtract by half max value to try and force negative values in a smaller range
        New.ByteB = 127;
        New.X = S.X;
        New.Y = S.Y;
        New.Heat = S.Heat;
        New.Type = SPARK_OzSpark;
        Sparks->PushBack( New );
      }
      break;
    case SPARK_OzSpark: // Spawned by SPARK_OzSpark sparks
      S.Heat -= 2;
      if ( S.Heat < 2 )
        Sparks->Erase( i );
      else
      {
        if ( (rand() & 0x7f) < (S.ByteA & 0x7f) )
          S.X += (S.ByteA & 0x80) ? 0 : 1;
        else
          S.X += (S.ByteA & 0x80) ? -1 : 0;

        if ( (rand() & 0x7f) < (S.ByteB & 0x7f) )
          S.Y--;

        S.X &= UMask;
        S.Y &= VMask;

        BUF( S.X, S.Y ) = S.Heat;
      }
      break;
    case SPARK_Cone:
      if ( Sparks->Size() < SparksLimit && (rand() & 0xff) > 127 )
      {
        New.ByteA = (rand() & 0x7f) - 63; // subtract by half max value to try and force negative values in a smaller range
        New.ByteC = 54; // seems to look right based on FractalTest.ConeTest, doesn't look like we trample anything either
        New.X = S.X;
        New.Y = S.Y;
        New.Heat = S.Heat;
        New.Type = SPARK_ConeSpark;
        Sparks->PushBack( New );
      }
      break;
    case SPARK_ConeSpark:
      S.ByteC -= 1;
      if ( S.ByteC == 1 )
        Sparks->Erase( i );
      else
      {
        if ( (rand() & 0x7f) < (S.ByteA & 0x7f) )
          S.X += (S.ByteA & 0x80) ? 0 : 1;
        else
          S.X += (S.ByteA & 0x80) ? -1 : 0;

        if ( (rand() & 0x1) )
          S.Y++;

        S.X &= UMask;
        S.Y &= VMask;

        BUF( S.X, S.Y ) = S.Heat;
      }
      break;
    case SPARK_BlazeLeft:
      if ( Sparks->Size() < SparksLimit && (rand() & 0xff) < 128 )
      {
        New.ByteA = (rand() & 0x7f) - 63;
        New.ByteC = 64;
        New.ByteD = 0;
        New.X = S.X;
        New.Y = S.Y;
        New.Heat = MIN( S.Heat, RenderHeat );
        New.Type = SPARK_BlazeLSpark;
        Sparks->PushBack( New );
      }
      break;
    case SPARK_BlazeLSpark:
      S.ByteC -= 1;
      if ( S.ByteC == 1 )
        Sparks->Erase( i );
      else
      {
        if ( (rand() & 0x7f) > (S.ByteA & 0x7f) )
        {
          S.X -= 1;
          S.ByteD++;
        }

        if ( (rand() & 0x3f) < S.ByteD )
          S.Y++;

        S.X &= UMask;
        S.Y &= VMask;

        BUF( S.X, S.Y ) = S.Heat;
      }
      break;
    case SPARK_BlazeRight:
      if ( Sparks->Size() < SparksLimit && (rand() & 0xff) < 128 )
      {
        New.ByteA = (rand() & 0x7f) - 63;
        New.ByteC = 100;
        New.ByteD = 0;
        New.X = S.X;
        New.Y = S.Y;
        New.Heat = S.Heat - S.ByteB;
        New.Type = SPARK_BlazeRSpark;
        Sparks->PushBack( New );
      }
      break;
    case SPARK_BlazeRSpark:
      S.ByteC -= 4;
      if ( S.ByteC < 4 )
        Sparks->Erase( i );
      else
      {
        if ( (rand() & 0x7f) > (S.ByteA & 0x7f) )
        {
          S.X += 1;
          S.ByteD++;
        }

        if ( S.ByteD > 8 && (rand() & 0x3f) < S.ByteD )
          S.Y++;

        S.X &= UMask;
        S.Y &= VMask;

        BUF( S.X, S.Y ) = S.Heat;
      }
      break;
    case SPARK_Cylinder:
      break;
    case SPARK_Cylinder3D:
      break;
    case SPARK_Lissajous:
      break;
    case SPARK_Jugglers:
      break;
    case SPARK_Emit:
      break;
    case SPARK_Fountain:
      break;
    case SPARK_Flocks:
      break;
    case SPARK_Eels:
      break;
    case SPARK_Organic:
      break;
    case SPARK_WanderOrganic:
      break;
    case SPARK_RandomCloud:
      break;
    case SPARK_CustomCloud:
      break;
    case SPARK_LocalCloud:
      break;
    case SPARK_Stars:
      break;
    case SPARK_LineLightning:
      break;
    case SPARK_RampLightning:
      break;
    case SPARK_SphereLightning:
      break;
    case SPARK_Wheel:
      break;
    case SPARK_Gametes:
      break;
    case SPARK_Sprinkler:
      break;
    }
  }
}

void UFireTexture::Load()
{
  Super::Load();
  FPackageFileIn& In = *PkgFile;

  // Probably an index, but im unsure
  In >> CINDEX( NumSparks );
  Sparks = new TArray<Spark>();
  Sparks->Reserve( NumSparks );

  for ( int i = 0; i < NumSparks; i++ )
  {
    Spark NewSpark;
    In >> NewSpark;
    switch ( NewSpark.Type )
    {
      case SPARK_Pulse:
        NewSpark.ByteA = NewSpark.Heat;
        break;
    }


    Sparks->PushBack( NewSpark );
  }

  if ( GEngine && GEngine->Render )
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
    float HeatIndex = i / 4.0;
    float RenderHeatContrib = -(255 - RenderHeat) / 16.0;
    float Adjust = 1.0;
    RenderTable[i] = (int)FClamp( HeatIndex + RenderHeatContrib + Adjust, 0.0, RenderHeat );
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

void UWaterTexture::Load()
{
  Super::Load();

  if ( GEngine && GEngine->Render )
  {
    bHardwareAccelerated = false;

    Mips.Resize( 1 );

    Buffer = &Mips[0];
    Buffer->DataArray.Resize( Buffer->USize * Buffer->VSize );

    UMask = Buffer->USize - 1;
    VMask = Buffer->VSize - 1;
    
    size_t SourceSize = Buffer->USize * Buffer->VSize * 2;
    SourceFields = new int[SourceSize];
    memset( SourceFields, 0, SourceSize * sizeof(int) );

    // Calculate color lookup render table
    CalculateRenderTable();
  }
}


void UWaterTexture::Tick( float DeltaTime )
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

void UWaveTexture::Tick( float DeltaTime )
{
  // Tick once per frame
  if ( CurrentTick == GEngine->TickCycles )
    return;
  CurrentTick = GEngine->TickCycles;

  // Tick at 40 fps
  Accumulator += DeltaTime;
  if ( Accumulator < (1.0f / 40.0f) )
    return;
  Accumulator = 0.0f;

  // Update wave state
  Super::Tick( DeltaTime );  
}

bool UWaveTexture::ExportToFile( const char* dir, const char* Type )
{
  return false;
}

void UWaveTexture::CalculateRenderTable()
{
  // Calculate the table so that it effectively follows a sine wave that gradually settles to a medium color (127)

  for ( int i = 0; i < 1024; i++ )
  {

  }
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

void UWetTexture::Tick( float DeltaTime )
{
  // Tick once per frame
  if ( CurrentTick == GEngine->TickCycles )
    return;
  CurrentTick = GEngine->TickCycles;

  // Tick at 40 fps
  Accumulator += DeltaTime;
  if ( Accumulator < (1.0f / 40.0f) )
    return;
  Accumulator = 0.0f;

  // Update pixel velocities
  Super::Tick( DeltaTime );


  int* Vel = SourceFields;
  int* Pos = &SourceFields[(VSize << UBits) + USize];
  #define VEL(x,y) (Vel[((y)<<UBits)+(x)])
  #define POS(x,y) (Pos[((y)<<UBits)+(x)])

  for ( int i = 0; i < NumDrops; i++ )
  {
    ADrop& Drop = Drops[i];
    int why = (int)(sin( GEngine->TimeSeconds * 7 + Drop.ByteC / 256 * 2 * PI ) * Drop.Depth * 256);
    int idx = ((Drop.Y << 1) << UBits) + (Drop.X<<1);
    Vel[idx] = why;
  }

  int PosOff  = 0;
  int OffMask = Mips[0].DataArray.Size()-1;
  for ( int x = 0; x < USize; x++ )
  {
    for ( int y = 0; y < VSize; y++ )
    {
      *Vel += ((Pos[(PosOff + y - VSize) & OffMask] + 
                    Pos[(PosOff + y + VSize) & OffMask] + 
                    Pos[(PosOff + y - 1) & OffMask] + 
                    Pos[(PosOff + y + 1) & OffMask] ) * 79) >> 8;
      *Vel -= (Pos[PosOff+y]*316) >> 8;
      *Vel = (*Vel * 485) >> 9;
      Vel++;
    }
    PosOff += VSize;
  }

  Vel = (int*)SourceFields;
  for ( int i = 0; i < Mips[0].DataArray.Size(); i++ )
    Pos[i] += Vel[i];
  
  u8* Buf = Buffer->DataArray.Data();
  u8* Src = SourceTexture->Mips[0].DataArray.Data();
  for ( int y = 0; y < VSize; y++ )
  {
    for ( int x = 0; x < USize; x++ )
    {
      //*dstptr = srclineptr[(x + ((position( x, y ) * WaveAmp) shr( 16 + 2 ))) and UMask];
      int SrcIdx = (x + ((POS( x, y ) * WaveAmp) >> (16 + 2)))& UMask;
      *Buf = Src[SrcIdx];
      Buf++;
    }
    Src += USize;
  }
}

bool UWetTexture::ExportToFile( const char* dir, const char* Type )
{
  return false;
}

void UWetTexture::CalculateRenderTable()
{
  // TODO
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
  LINK_NATIVE_ARRAY   ( WaveTable );
  LINK_NATIVE_PROPERTY( WaveParity );
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

