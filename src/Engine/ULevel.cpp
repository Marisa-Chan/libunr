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
 * ULevel.cpp - Level functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Core/UProperty.h"
#include "Engine/ULevel.h"
#include "Actors/AActor.h"
#include "Actors/ABrush.h"
#include "Actors/ANavigationPoint.h"
#include "Actors/APlayerPawn.h"

LIBUNR_API FPackageFileIn& operator>>( FPackageFileIn& In, FReachSpec& RS )
{
  In >> RS.Distance;
  
  idx ObjRef;
  In >> CINDEX( ObjRef );
  RS.Start = (AActor*)UObject::StaticLoadObject( In.Pkg, ObjRef, AActor::StaticClass(), NULL );

  In >> CINDEX( ObjRef );
  RS.End = (AActor*)UObject::StaticLoadObject( In.Pkg, ObjRef, AActor::StaticClass(), NULL );

  In >> RS.CollisionRadius;
  In >> RS.CollisionHeight;
  In >> RS.ReachFlags;
  In >> RS.bPruned;
  
  return In;
}

LIBUNR_API FPackageFileIn& operator>>( FPackageFileIn& In, FURL& URL )
{
  In >> URL.Protocol;
  In >> URL.Host;
  In >> URL.Map;
  In >> URL.Portal;

  u8 _Unknown0;
  In >> _Unknown0;

  In >> URL.Port;

  int Valid;
  In >> Valid;
  URL.bValid = (Valid == 1);

  return In;
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

  bDeletedBrushwork = true;
  int NumBrushes = 0;

  int NumActors = 0;
  int _Unknown0 = 0;

  *PkgFile >> NumActors;
  *PkgFile >> _Unknown0;

  Actors.Reserve( NumActors );
  for ( int i = 0; i < NumActors; i++ )
  {
    idx ActorObjRef = 0;
    *PkgFile >> CINDEX( ActorObjRef );

    // Don't load garbage zeroes
    if ( ActorObjRef != 0 )
    {
      AActor* Actor = (AActor*)LoadObject( ActorObjRef, NULL, NULL, LOAD_Immediate );
      if ( bDeletedBrushwork )
      {
        if ( Actor->Class == ABrush::StaticClass() )
          NumBrushes++;
        if ( NumBrushes > 2 )
          bDeletedBrushwork = false;
      }
      Actors.PushBack( Actor );
    }
  }

  // Unreal seems to preserve all Brush actors in the package export tree 
  // even when they're "deleted". The actor properties of each brush will
  // have bDeleteMe set to true. These brushes can (and should be) recovered
  // if this is the case.
  //
  // This is an over 20 year old game with a very dead community. Part of 
  // the reason why it is so dead (unlike Doom) is because people have this
  // silly idea that content should be kept proprietary and nobody should ever
  // be able to learn or be inspired from one another whatsoever. 
  //
  // Stop that shit.
  //
  // By keeping mods and maps proprietary, you actively destroy what's
  // left of the community.
  if ( bDeletedBrushwork )
  {
    TArray<FExport>& Exports = Pkg->GetExportTable();
    for ( int i = 0; i < Exports.Size(); i++ )
    {
      FExport* Export = &Exports[i];
      if ( stricmp( Pkg->ResolveNameFromObjRef( Export->Class ), "Brush" ) == 0 )
      {
        ABrush* Brush = (ABrush*)StaticLoadObject( Pkg, Export, ABrush::StaticClass(), NULL, LOAD_Immediate );
        Actors.PushBack( Brush );
      }
    }
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
  u8 _Unknown[16];
  PkgFile->Read( &_Unknown, 3 );

  // Unused for now...
  idx TextBuffer0;
  *PkgFile >> CINDEX( TextBuffer0 );

  // ???
  // On version 60, there seems to be 9 bytes, on 61, there's 11,
  // otherwise, I've only seen 10. They're always zeros, but what even
  // is this crap?
/*  int ReadNum;
  switch (PkgFile->Ver)
  {
    case 60:
      ReadNum = 9;
      break;
    case 61:
      ReadNum = 11;
      break;
    default:
      ReadNum = 10;
  }
  PkgFile->Read( &_Unknown, ReadNum );
*/
}

IMPLEMENT_NATIVE_CLASS( ULevelBase );
IMPLEMENT_NATIVE_CLASS( ULevel );

