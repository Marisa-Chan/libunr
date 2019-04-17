/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU Affero General Public License as           *|
|*  published by the Free Software Foundation, either version 3 of the       *|
|*  License, or (at your option) any later version.                          *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU Affero General Public License for more details.                      *|
|*                                                                           *|
|*  You should have received a copy of the GNU Affero General Public License *|
|*  along with this program.  If not, see <https://www.gnu.org/licenses/>.   *|
\*===========================================================================*/

/*========================================================================
 * ULevel.cpp - Level functionality
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "Core/UClass.h"
#include "Core/ULevel.h"
#include "Core/UProperty.h"
#include "Actors/AActor.h"
#include "Actors/ABrush.h"
#include "Actors/ANavigationPoint.h"
#include "Actors/APlayerPawn.h"

DLL_EXPORT FPackageFileIn& operator>>( FPackageFileIn& In, FReachSpec& RS )
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

DLL_EXPORT FPackageFileIn& operator>>( FPackageFileIn& In, FURL& URL )
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
      AActor* Actor = (AActor*)LoadObject( ActorObjRef, NULL, NULL, true );
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
    Array<FExport>* Exports = Pkg->GetExportTable();
    for ( int i = 0; i < Exports->Size(); i++ )
    {
      FExport* Export = &(*Exports)[i];
      if ( stricmp( Pkg->ResolveNameFromObjRef( Export->Class ), "Brush" ) == 0 )
      {
        ABrush* Brush = (ABrush*)StaticLoadObject( Pkg, Export, ABrush::StaticClass(), NULL, true );
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

bool ULevel::ExportToFile( const char* Dir, const char* Type )
{
  FString Filename( Dir );
#if defined LIBUNR_WIN32
  Filename.ReplaceChars( '\\', '/' );
#endif
  if ( Filename.Back() != '/' )
    Filename += "/";

  Filename += Pkg->Name;
  Filename += ".t3d"; // Levels won't get exported to any other type
 
  FileStreamOut* Out = new FileStreamOut();
  if ( Out->Open( Filename ) != 0 )
  {
    Logf( LOG_WARN, "Failed to export level to file '%s'", Filename.Data() );
    return false;
  }

  // Temporarily change package name to MyLevel
  FName PkgName = Pkg->Name;
  Pkg->Name = Pkg->GetGlobalName( Pkg->FindName( "MyLevel" ) );

  // Write beginning map
  Out->Printf( "Begin Map\r\n" );

  // Loop through all actors
  FString ValueBuf;
  for ( size_t i = 0; i < Actors.Size() && i != MAX_SIZE; i++ )
  {
    AActor* Actor = Actors[i];
      
    // Don't export unnecessary actors
    if ( Actor->Class == ACamera::StaticClass() || 
        (Actor->Class->ClassFlags & CLASS_NoUserCreate) )
      continue;

    // Write begin actor
    Out->Printf("Begin Actor Class=%s Name=%s\r\n", Actor->Class->Name.Data(), Actor->Name.Data() );

    // Write actor properties
    for ( UField* Iter = Actor->Field; Iter != NULL; Iter = Iter->Next )
    {
      UProperty* Prop = SafeCast<UProperty>( Iter );
      if ( Prop != NULL )
      {
        if ( UNLIKELY( Prop->Offset & 0x80000000 ) )
        {
          Logf( LOG_WARN, "Bad offset for property '%s' in Actor '%s' (Offset = %x)",
              Prop->Name.Data(), Actor->Name.Data(), Prop->Offset );
          continue;
        }

        if ( (Prop->ObjectFlags & RF_TagExp) && (
            !(Prop->PropertyFlags & (CPF_Const|CPF_EditConst)) || 
             (Prop->PropertyFlags & (CPF_Edit|CPF_ExportObject)) 
           ))
        {
          for ( int j = 0; j < Prop->ArrayDim; j++ )
          {
            // Get default property of this class
            Prop->GetText( ValueBuf, Actor, Actor->Class->Default, j );

            if ( ValueBuf.Length() > 0 )
            {
              if ( Prop->Class == UArrayProperty::StaticClass() )
              {
                Out->Write( ValueBuf.Data(), ValueBuf.Length() );
                ValueBuf.Erase();
                continue;
              }
              else if ( Prop->Name == ABrush::StaticClass()->Name )
              {
                ABrush* Brush = (ABrush*)Actor;
                Brush->ExportToLevelText( Out );
              }
              else if ( Prop->Class == UObjectProperty::StaticClass() &&
                        ((UObjectProperty*)Prop)->ObjectType->ClassIsA( ANavigationPoint::StaticClass() ) )
              {
                ValueBuf.Erase();
                break;
              }

              if ( Prop->ArrayDim > 1 )
                Out->Printf( "\t%s(%i)=%s\r\n", Prop->Name.Data(), j, ValueBuf.Data() );
              else
                Out->Printf( "\t%s=%s\r\n", Prop->Name.Data(), ValueBuf.Data() );
              
              ValueBuf.Erase();
            }
          }
        }
      }
    }

    // Write end actor
    Out->Write( (char*)"End Actor\r\n", 11 );
  }

  // Write end map
  Out->Write( (char*)"End Map\r\n", 9 );
  Out->Close();
  delete Out;

  // Restore package name
  Pkg->Name = PkgName;
  return true;
}

IMPLEMENT_NATIVE_CLASS( ULevelBase );
IMPLEMENT_NATIVE_CLASS( ULevel );

