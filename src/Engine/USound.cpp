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
 * USound.cpp - Native sound object type
 * See the 'Class Sound' in UT-Package-File-Format.pdf
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UPackage.h"
#include "Engine/USound.h"

USound::USound()
{
  SoundFormat = 0;
  SoundSize = 0;
  SoundData = NULL;
  PcmData = NULL;
  Handle = 0;
  PcmSize = 0;
  SampleRate = 0;
  NumChannels = 0;
  BitsPerSample = 0;
}

USound::~USound()
{
  if (SoundData != NULL)
    delete SoundData;
}

void USound::Load()
{
  ReadDefaultProperties();

  *PkgFile >> CINDEX( SoundFormat );
  if ( PkgFile->Ver >= PKG_VER_UN_220 )
    *PkgFile >> OffsetNext;
  
  *PkgFile >> CINDEX( SoundSize );
  
  SoundData = new u8[SoundSize];
  PkgFile->Read( SoundData, SoundSize );
}

// TODO: Support exporting to a number of different formats
bool USound::ExportToFile( const char* Dir, const char* Type )
{
  const char* Ext = Pkg->ResolveNameFromIdx( SoundFormat );

  // Set up filename
  FString Filename( Dir );
#if defined LIBUNR_WIN32
  Filename.ReplaceChars( '\\', '/' );
#endif
  if ( Filename.Back() != '/' )
    Filename += '/';

  Filename += Name.Data();
  Filename += ".";
  Filename += Ext;

  GLogf( LOG_INFO, "Exporting %s.%s", Name.Data(), Ext );

  // Open file
  FFileArchiveOut* Out = new FFileArchiveOut();
  if ( Out->Open( Filename ) != 0 )
  {
    GLogf( LOG_WARN, "Failed to export sound to wav file '%s'", Filename.Data() );
    return false;
  }

  // Write
  Out->Write( SoundData, SoundSize );
  
  // Close
  Out->Close();
  delete Out;
  return true;
}

void* USound::GetRawPcm()
{
  if ( stricmp( SoundFormat.Data(), "wav" ) == 0 )
    return GetWavPcm();

  GLogf( LOG_WARN, "Unknown sound format '%s' in sound '%s.%s'", SoundFormat.Data(), Pkg->Name.Data(), Name.Data() );
  return NULL;
}

// "The Canonical WAVE file format" documentation here
// http://soundfile.sapp.org/doc/WaveFormat/
void* USound::GetWavPcm()
{
  u8* Data = SoundData;

  // Parse RIFF chunk
  if ( strncmp( (char*)Data, "RIFF", 4 ) != 0 )
  {
    GLogf( LOG_ERR, "USound::GetWavPcm() failed: bad RIFF chunk" );
    return NULL;
  }
  Data += 4;

  int ChunkSize = *(u32*)Data;
  Data += 4;
  
  // Expect WAVE format
  if ( strncmp ( (char*)Data, "WAVE", 4 ) != 0 )
  {
    GLogf( LOG_ERR, "USound::GetWavPcm() failed: expected WAVE format" );
    return NULL;
  }
  Data += 4;

  // Read format sub-chunk
  if ( strncmp( (char*)Data, "fmt ", 4 ) != 0 )
  {
    GLogf( LOG_ERR, "USound::GetWavPcm() failed: expected format subchunk" );
    return NULL;
  }
  Data += 4;

  // TODO: Verify chunk sizes for malformed wav files
  ChunkSize = *(u32*)Data;
  Data += 4;

  u16 AudioFormat = *(u16*)Data;
  Data += 2;

  if ( AudioFormat != 1 )
  {
    GLogf( LOG_ERR, "USound::GetWavPcm() needs handling for compressed PCM" );
    return NULL;
  }

  NumChannels = *(u16*)Data;
  Data += 2;

  SampleRate = *(u32*)Data;
  Data += 4;

  u32 ByteRate = *(u32*)Data;
  Data += 4;

  u16 BlockAlign = *(u16*)Data;
  Data += 2;

  BitsPerSample = *(u16*)Data;
  Data += 2;

  // Get actual data
  if ( strncmp( (char*)Data, "data", 4 ) != 0 )
  {
    GLogf( LOG_ERR, "USound::GetWavPcm() failed: expected data subchunk" );
    return NULL;
  }

  PcmSize = *(u32*)Data;
  Data += 4;

  PcmData = Data;
  return PcmData;
}

#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( USound );

