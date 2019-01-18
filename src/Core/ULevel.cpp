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
 * ULevel.cpp - Level functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/ULevel.h"

FURL::FURL()
{
}

DLL_EXPORT FPackageFileIn& operator>>( FPackageFileIn& Ar, FURL& URL )
{
  Ar >> Protocol;
  Ar >> Host;
  Ar >> Map;
  Ar >> Portal;

  u8 _Unknown0;
  Ar >> _Unknown0;

  Ar >> Port;

  int Valid;
  Ar >> Valid;
  bValid = (Valid == 1);

  return Ar;
}

ULevelBase::ULevelBase()
  : UObject()
{
  NetDriver = NULL;
  Engine = NULL;
  DemoRecDriver = NULL;
}

ULevelBase::~ULevelBase()
{
}

ULevel::ULevel()
  : ULevelBase()
{
  Model = NULL;
}

ULevel::~ULevel()
{
}

void ULevelBase::Load()
{
  Super::Load();

  int NumActors = 0;
  int _Unknown0 = 0;

  *PkgFile >> NumActors;
  *PkgFile >> _Unknown0;

  Actors.Reserve( NumActors );
  for ( int i = 0; i < NumActors; i++ )
  {
    idx ActorObjRef = 0;
    *PkgFile >> CINDEX( ActorObjRef );

    Actors.PushBack( (UActor*)LoadObject( ActorObjRef, NULL, NULL ) );
  }

  *PkgFile >> URL;
}

void ULevel::Load()
{
  Super::Load();

  idx ObjRef;
  *PkgFile >> CINDEX( ObjRef );
  Model = (UModel*)LoadObject( ObjRef, UModel::StaticClass(), this );

  idx ReachSpecsLength;
  *PkgFile >> CINDEX( ReachSpecsLength );
  ReachSpecs.Resize( ReachSpecsLength );

  for ( int i = 0; i < ReachSpecsLength; i++ )
    *PkgFile >> ReachSpecs[i];

  *PkgFile >> TimeSeconds;

  // TODO: What is this?
  u8 _Unknown[3];
  PkgFile->Read( &Unknown, 3 );

  // Unused for now...
  idx TextBuffer0;
  *PkgFile >> CINDEX( TextBuffer0 );

  // TODO: ???
  //PkgFile->Read( &Unknown, 10 );
}

