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
 * USound.cpp - Native sound object type
 * See the 'Class Sound' in UT-Package-File-Format.pdf
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "USound.h"
#include "UPackage.h"

USound::USound()
{
  SoundFormat = 0;
  SoundSize = 0;
  SoundData = NULL;
}

USound::~USound()
{
  if (SoundData != NULL)
    delete SoundData;
}

void USound::LoadFromPackage( FPackageFileIn* In )
{
  ReadDefaultProperties( In );

  *In >> CINDEX( SoundFormat );
  if (In->Ver >= PKG_VER_UN_220)
    *In >> OffsetNext;
  
  *In >> CINDEX( SoundSize );
  
  SoundData = new u8[SoundSize];
  In->Read( SoundData, SoundSize );
}

// TODO: Support exporting to a number of different formats
bool USound::ExportToFile( const char* Dir, const char* Type )
{
  // Set up filename
  String* Filename = new String( Dir );
#if defined LIBUNR_WIN32
  Filename->ReplaceChars( '\\', '/' );
#endif
  if ( Filename->Back() != '/' )
    Filename->Append( "/" );

  Filename->Append( Pkg->ResolveNameFromIdx( NameIdx ) );
  Filename->Append( "." );
  Filename->Append( Pkg->ResolveNameFromIdx( SoundFormat ) );
  
  // Open file
  FileStreamOut* Out = new FileStreamOut();
  if ( Out->Open( *Filename ) != 0 )
  {
    Logf( LOG_WARN, "Failed to export sound to wav file '%s'", Filename->Data() );
    return false;
  }

  // Write
  Out->Write( SoundData, SoundSize );
  
  // Close
  Out->Close();
  delete Out;
  delete Filename;  
  return true;
}
