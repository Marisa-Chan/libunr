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
 * UMusic.h - Native music object type
 * See the 'Class Music' in UT-Package-File-Format.pdf
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/UObject.h"
#include "Core/USystem.h"

enum EMusicTransition
{
	MTRAN_None,
	MTRAN_Instant,
	MTRAN_Segue,
	MTRAN_Fade,
	MTRAN_FastFade,
	MTRAN_SlowFade,
};

#define ERR_LIBXMP_LOAD_FAILED -1
#define ERR_LIBXMP_START_PLAY_FAILED -2
#define ERR_LIBXMP_PLAY_BUF_FAILED -3
#define ERR_NO_AUDIO_DEVICE -4

/*-----------------------------------------------------------------------------
 * UMusic
 * Contains data about a music file
-----------------------------------------------------------------------------*/

class LIBUNR_API UMusic : public UObject
{
  DECLARE_NATIVE_CLASS( UMusic, UObject, CLASS_NoExport | CLASS_SafeReplace, Core )
  UMusic();

  virtual void Load();
  virtual bool ExportToFile( const char* Dir, const char* Type );
  
  u16   ChunkCount; //?
  u32   _unknown0; // PackageVerison > 61
  idx   ChunkSize;
  u8*   ChunkData;
  FName MusicType;
};

/*-----------------------------------------------------------------------------
 * FMusicPlayer
 * Handles rendering of music on a separate thread
 * Rendered music goes to the audio device
-----------------------------------------------------------------------------*/
class LIBUNR_API FMusicPlayer
{
public:

  FMusicPlayer();
  ~FMusicPlayer();

  int GetStatus();
  void ClearStatus();

  void Play( UMusic* Music, int SongSection, EMusicTransition Transition );
  void Stop( EMusicTransition Transition );
  float GetCurrentVolume();

protected:

  static ThreadReturnType StaticThreadFunc( void* Args );

  ThreadReturnType PlayerThread();
  void RegisterMusic( UMusic* Music );
  void UnregisterMusic( UMusic* Music );
  void RenderMusic( float* Buf, size_t Size );

  void* MusicThread;
  void* MusicHandle;
  UMusic* CurrentTrack;
  UMusic* QueuedTrack;
  int CurrentSection;
  int QueuedSection;
  float CurrentVolume;
  float TargetVolume;
  EMusicTransition CurrentTransition;
  bool bTransitioning;
  int RequestExit;
  int ThreadErr;
};
