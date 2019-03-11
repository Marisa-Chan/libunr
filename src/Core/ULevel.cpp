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
      Actors.PushBack( (AActor*)LoadObject( ActorObjRef, NULL, NULL, true ) );
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
  PkgFile->Read( &_Unknown, 3 );

  // Unused for now...
  idx TextBuffer0;
  *PkgFile >> CINDEX( TextBuffer0 );

  // TODO: ???
  //PkgFile->Read( &_Unknown, 10 );
}

bool ULevel::ExportToFile( const char* Dir, const char* Type )
{
  String* Filename = new String( Dir );
#if defined LIBUNR_WIN32
  Filename->ReplaceChars( '\\', '/' );
#endif
  if ( Filename->Back() != '/' )
    Filename->Append( "/" );

  Filename->Append( Pkg->Name );
  Filename->Append( ".t3d" ); // Levels won't get exported to any other type
 
  FileStreamOut* Out = new FileStreamOut();
  if ( Out->Open( *Filename ) != 0 )
  {
    Logf( LOG_WARN, "Failed to export level to file '%s'", Filename->Data() );
    return false;
  }

  // Temporarily change package name to MyLevel
  const char* PkgName = Pkg->Name;
  Pkg->Name = "MyLevel";

  // Write beginning map
  Out->Write( (char*)"Begin Map\r\n", 11 );

  // Loop through all actors
  String ActorBuf;
  String ValueBuf;
  for ( size_t i = 0; i < Actors.Size() && i != MAX_SIZE; i++ )
  {
    AActor* Actor = Actors[i];

    // Write begin actor
    ActorBuf += "Begin Actor Class=";
    ActorBuf += Actor->Class->Name;
    ActorBuf += " Name=";
    ActorBuf += Actor->Name;
    ActorBuf += "\r\n";
    Out->Write( ActorBuf.Data(), ActorBuf.Length() );

    // Write actor properties
    ActorBuf.Erase();
    for ( UField* Iter = Actor->Field; Iter != NULL; Iter = Iter->Next )
    {
      UProperty* Prop = SafeCast<UProperty>( Iter );
      if ( Prop != NULL )
      {
        if ( UNLIKELY( Prop->Offset & 0x80000000 ) )
        {
          Logf( LOG_WARN, "Bad offset for property '%s' (Offset = %x)",
              Prop->Name, Prop->Offset );
          continue;
        }

        if ( (Prop->ObjectFlags & RF_TagExp) && 
           (!(Prop->PropertyFlags & CPF_Const) || (Prop->PropertyFlags & (CPF_Edit|CPF_ExportObject)) ) )
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
              else if ( Actor->Class == ABrush::StaticClass() && Prop->Hash == FnvHashString("Brush") )
              {
                ABrush* Brush = (ABrush*)Actor;
                Brush->ExportToLevelText( Out );
              }
              else if ( Prop->Class == UObjectProperty::StaticClass() &&
                        ((UObjectProperty*)Prop)->ObjectType->IsA( ANavigationPoint::StaticClass() ) )
              {
                continue;
              }

              if ( Prop->ArrayDim > 1 )
                Out->Printf( "\t%s(%i)=%s\r\n", Prop->Name, j, ValueBuf.Data() );
              else
                Out->Printf( "\t%s=%s\r\n", Prop->Name, ValueBuf.Data() );
              
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

  // Restore package name
}

IMPLEMENT_NATIVE_CLASS( ULevelBase );
IMPLEMENT_NATIVE_CLASS( ULevel );

