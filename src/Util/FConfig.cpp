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
 * FConfig.h - Class for loading and saving INI files
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#if defined _WIN32
  #include <direct.h>
#endif

#include "Util/FConfig.h"
#include "Util/FFileArchive.h"
#include "Util/FLogFile.h"

LIBUNR_API FConfigManager* GConfigManager = NULL;
LIBUNR_API FConfig* GLibunrConfig = NULL;
LIBUNR_API FConfig* GGameConfig = NULL;
LIBUNR_API FConfig* GUserConfig = NULL;

// FIXME: this is NOT thread safe!
static int NumLines;

static inline bool IsAcceptedChar( char C, const char* Accepted )
{
  size_t Len = strlen( Accepted );
  for ( size_t i = 0; i < Len; i++ )
  {
    if ( C == Accepted[i] )
      return true;
  }

  return false;
}

// Why does windows have '\r\n' instead of just '\n' -____-

static inline void ReadNewLine( FFileArchiveIn& In, const char* Filename )
{
  if ( In.Eof() )
    return;

  char C = '\0';
  In.Read( &C, 1 );
  if ( C == '\r' )
  {
    In.Read( &C, 1 );
    if ( UNLIKELY ( C != '\n' ) )
    {
      GLogf( LOG_WARN, "Unpaired carriage return in config file '%s' (Pos = %llu, C = %c)", 
          Filename, In.Tell(), C );
    }
  }
  else if ( C != '\n' )
  {
    GLogf( LOG_WARN, "Expected newline in config file '%s' (Pos = %llu, C = %c)",
        Filename, In.Tell(), C );
  }
  else
    NumLines++;
}

static inline bool IsNextCharSeqNewline( FFileArchiveIn& In, const char* Filename )
{
  char C = In.Peek();
  if ( C == '\r' )
  {
    In.Read( &C, 1 );
    C = In.Peek();
    if ( UNLIKELY ( C != '\n' ) )
    {
      GLogf( LOG_WARN, "Unpaired carriage return in config file '%s' (Pos = %llu, C = %c)", 
          Filename, In.Tell(), C );
      In.Close();
      return false;
    }
    In.Seek( -1, Cur );
    return true;
  }
  else if ( C == '\n' )
  {
    return true;
  }

  return false;
}

FConfig::FConfig()
{
}

FConfig::FConfig( const char* Filename )
{
  Load( Filename );
}

FConfig::~FConfig()
{
  for ( size_t i = 0; i < Categories.size(); i++ )
    delete Categories[i];

  Categories.clear();
}

int FConfig::Load( const char* Filename )
{
  char CategoryBuf[128];
  char VariableBuf[128];
  char ValueBuf[512];
  bool bIndexed = false;
  u32  Index = 0;
  FHash PreviousHash = {0, 0};
  NumLines = 0;

  FFileArchiveIn IniFile;
  int Status = IniFile.Open( Filename );
  if ( Status != 0 )
  {
    GLogf( LOG_WARN, "Can't open ini file '%s' (errno = %s)", Filename, strerror( Status ) );
    return ERR_FILE_NOT_EXIST;
  }


  // Read until the end of the file
  FConfigCategory* Category = NULL;
  FConfigEntry* Entry = NULL;
  memset( CategoryBuf, 0, sizeof( CategoryBuf ) );
  memset( VariableBuf, 0, sizeof( VariableBuf ) );
  memset( ValueBuf, 0, sizeof( ValueBuf ) );

  char Probe = '\0';
  while ( !IniFile.Eof() ) // why doesn't Eof() work??
  {
    Status = IniFile.Read( &Probe, 1 );
    if ( Status == MAX_UINT32 )
    {
      GLogf( LOG_WARN, "Failed to read from INI file '%s'", Filename );
      IniFile.Close();
      return ERR_BAD_DATA;
    }
    else if ( Status == 0 )
    {
      break;
    }

    if ( Probe == ';' || Probe == '\'' )
    {
      // Read characters till we hit a newline
      while ( 1 )
      {
        if ( IniFile.Read( &Probe, 1 ) == 0 )
          break;

        if ( Probe == '\n' )
          break;

        if ( Probe == '\r' )
          if ( IniFile.Read( &Probe, 1 ) != 0 && Probe == '\n' )
            break;
      }   
    }
    else if ( Probe == '[' )
    {
      char* CategoryPtr = CategoryBuf;
      memset( CategoryBuf, 0, sizeof( CategoryBuf ) );
      while ( PtrDistance( CategoryPtr, CategoryBuf ) < sizeof( CategoryBuf ) )
      {
        IniFile.Read( &Probe, 1 );
        if ( Probe == ']' )
          break;

        if ( !isalnum( Probe ) && !IsAcceptedChar( Probe, "_./* " ) )
        {
          memset( CategoryBuf, 0, sizeof( CategoryBuf ) );
          break;
        }

        *CategoryPtr++ = Probe;
      }

      if ( CategoryBuf[0] == '\0' )
      {
        GLogf( LOG_WARN, 
            "Malformed or corrupted category in file '%s' (Line = %llu)", 
            Filename, NumLines );
        IniFile.Close();
        return ERR_BAD_DATA;
      }

      Category = new FConfigCategory();
      Category->Name = strdup( CategoryBuf );
      Category->Hash = FnvHashString( Category->Name );
      Categories.PushBack( Category );

      ReadNewLine( IniFile, Filename );
    }
    // We're in a category, read a full line
    else if ( Probe != '\n' && Probe != '\r' )
    {
      if ( Category == NULL )
      {
        GLogf( LOG_WARN, 
            "Got non-category character, but no category in config file '%s' (Line = %llu)",
            Filename, NumLines );
        IniFile.Close();
        return ERR_BAD_DATA;
      }
      // Read the variable name
      char* VariablePtr = VariableBuf;
      memset( VariableBuf, 0, sizeof( VariableBuf ) );
      while ( PtrDistance( VariablePtr, VariableBuf ) < sizeof( VariableBuf ) )
      {
        // Put the character we just read first
        *VariablePtr++ = Probe;

        // Then get a new one
        IniFile.Read( &Probe, 1 );
        if ( Probe == '=' )
          break;

        else if ( Probe == '[' )
        {
          bIndexed = true;
          break;
        }

        if ( !isalnum( Probe ) && !IsAcceptedChar( Probe, "./_ " ) )
        {
          memset( VariableBuf, 0, sizeof( VariableBuf ) );
          break;
        }        
      }

      if ( VariableBuf[0] == '\0' )
      {
        GLogf( LOG_WARN, 
            "Malformed or corrupted variable in file '%s' (Line = %llu)", 
            Filename, NumLines );
        IniFile.Close();
        return ERR_BAD_DATA;
      }

      FHash VarHash = FnvHashString( VariableBuf );
      if ( LIKELY( VarHash != PreviousHash ) )
      {
        Entry = new FConfigEntry();
        Category->Entries->PushBack( Entry );
        Entry->Name = strdup( VariableBuf );
        Entry->Hash = VarHash;
      }

      if ( bIndexed )
      {
        char IndexBuf[8];
        char* IndexPtr = IndexBuf;
        while( 1 )
        {
          IniFile.Read( &Probe, 1 );
          if ( Probe == ']' )
            break;

           *IndexPtr++ = Probe;
        }

        IniFile.Read( &Probe, 1 );
        if ( Probe != '=' )
        {
          GLogf( LOG_WARN, "Missing equals sign after indexed variable in file '%s' (Line = %llu)",
              Filename, NumLines );
          IniFile.Close();
          return ERR_BAD_DATA;
        }

        if ( IndexBuf[0] == '\0' )
        {
          GLogf( LOG_WARN, 
              "std::vector indexed variable, but no index found in file '%s' (Line = %llu, C = '%c')", 
              Filename, NumLines, Probe );
          IniFile.Close();
          return ERR_BAD_DATA;
        }

        Index = strtol( IndexBuf, NULL, 10 );
        Entry->Values->resize( Index+1 );
        memset( IndexBuf, 0, sizeof( IndexBuf ) );

        Entry->bWriteIndices = true;
      }
      
      // Read the value
      char* ValuePtr = ValueBuf;
      memset( ValueBuf, 0, sizeof( ValueBuf ) );
      while ( PtrDistance( ValuePtr, ValueBuf ) < sizeof( ValueBuf ) )
      {
        if ( IsNextCharSeqNewline( IniFile, Filename ) || IniFile.Eof() )
          break;

        IniFile.Read( &Probe, 1 );
        *ValuePtr++ = Probe;
      }

      if ( !bIndexed )
        Entry->Values->PushBack( strdup( ValueBuf ) );
      else
        (*Entry->Values)[Index] = strdup( ValueBuf );

      ReadNewLine( IniFile, Filename );
      bIndexed = false;
      PreviousHash = VarHash;
    }
  }
  GLogf( LOG_INFO, "Successfully read config file '%s'", Filename );

  Name = strdup( Filename );
  char* Dot = strrchr( Name, '.' );
  if ( Dot )
    *Dot = '\0';

  Path = strdup( Filename );
  char* Slash = strrchr( Name, DIRECTORY_SEPARATOR );
  if ( Slash )
  {
    *Slash = '\0';
  }
  else
  {
    free( Path );
    Path = new char[296];
    getcwd( Path, 296 );
  }

  IniFile.Close();
  return 0;
}

int FConfig::Save()
{
  FFileArchiveOut IniFile;
  FString Filename( Path );

  Filename += DIRECTORY_SEPARATOR;
  Filename += Name;
  Filename += ".ini";

  int Status = IniFile.Open( Filename );
  if ( Status != 0 )
  {
    GLogf( LOG_WARN, "Failed to open ini file '%s' for saving (errno = %s)", Name, strerror( Status ) );
    return ERR_FILE_CREATE;
  }

  int WriteLen = 0;
  char WriteBuf[512];
  char NewLine[] = "\r\n";
  for( size_t i = 0; i < Categories.size(); i++ )
  {
    memset( WriteBuf, 0, sizeof( WriteBuf ) );
    FConfigCategory* Category = Categories[i];
    WriteLen = snprintf( WriteBuf, sizeof( WriteBuf ), "[%s]\r\n", Category->Name );
    IniFile.Write( WriteBuf, MIN( WriteLen, sizeof( WriteBuf ) ) );

    for( size_t j = 0; j < Category->Entries->size(); j++ )
    {
      FConfigEntry* Entry = (*Category->Entries)[j];
      for ( size_t k = 0; k < Entry->Values->size(); k++ )
      {
        char* Value = (*Entry->Values)[k];
        memset( WriteBuf, 0, sizeof( WriteBuf ) );
        if ( UNLIKELY( Entry->bWriteIndices ) )
        {
          WriteLen = snprintf( WriteBuf, sizeof( WriteBuf ), "%s[%lu]=%s\r\n", 
              Entry->Name, k, Value );
        }
        else
        {
          WriteLen = snprintf( WriteBuf, sizeof( WriteBuf ), "%s=%s\r\n", 
              Entry->Name, Value );
        }

        IniFile.Write( WriteBuf, MIN( WriteLen, sizeof( WriteBuf ) ) );
      }
    }

    IniFile.Write( NewLine, 2 );
  }

  IniFile.Close();
  return 0;
}

char* FConfig::ReadString( const char* Category, const char* Variable, size_t Index, const char* Default )
{
  FConfigCategory* CatIter = NULL;
  FConfigEntry* Entry = NULL;
  FHash CatHash = FnvHashString( Category ); // meow
  FHash VarHash = FnvHashString( Variable );

  for ( size_t i = 0; i < Categories.size(); i++ )
  {
    // Get Variable
    CatIter = Categories[i];
    if ( CatIter->Hash == CatHash )
    {
      // Found Category, get Entry
      for ( size_t j = 0; j < CatIter->Entries->size(); j++ )
      {
        Entry = (*CatIter->Entries)[j];
        if ( Entry->Hash == VarHash )
        {
          if ( UNLIKELY( Index >= Entry->Values->size() ) )
            return NULL;

          char* Value = (*Entry->Values)[Index];
          return strdup( Value );
        }
      }
      Entry = NULL;
      goto MakeEntry;
    }
  }

  // Did not find entry, make one
  CatIter = new FConfigCategory();
  CatIter->Name = strdup( Category );
  CatIter->Hash = CatHash;
  Categories.PushBack( CatIter );

MakeEntry:
  Entry = new FConfigEntry();
  Entry->Name = strdup( Variable );
  Entry->Hash = VarHash;
  Entry->Values->PushBack( strdup( Default ) );

  CatIter->Entries->PushBack( Entry );

  return (char*)Default;
}

bool FConfig::ReadBool( const char* Category, const char* Variable, size_t Index, bool Default )
{
  bool Value = false;
  char* StrVar = ReadString( Category, Variable, Index, (Default) ? "true" : "false" );
  if ( LIKELY( StrVar ) )
  {
    if ( strncmp( StrVar, "true", 4 ) == 0 )
      Value = true;

    free( StrVar );
  }
  return Value;
}

static inline u64 ReadUInt( FConfig* Config, const char* Category, const char* Variable, size_t Index, u64 Default )
{
  u64 Value = 0;
  char StrDef[24];
  sprintf( StrDef, "%lu", Default );

  char* StrVar = Config->ReadString( Category, Variable, Index, StrDef );
  if ( LIKELY( StrVar ) )
  {
    Value = strtoull( StrVar, NULL, 10 );
    if ( Value == MAX_UINT64 )
      Value = 0;

    free( StrVar );
  }
  return Value;
}

u64 FConfig::ReadUInt64( const char* Category, const char* Variable, size_t Index, u64 Default )
{
  return ReadUInt( this, Category, Variable, Index, Default );
}

u32 FConfig::ReadUInt32( const char* Category, const char* Variable, size_t Index, u32 Default )
{
  return ReadUInt( this, Category, Variable, Index, Default ) & MAX_UINT32;
}

u16 FConfig::ReadUInt16( const char* Category, const char* Variable, size_t Index, u16 Default )
{
  return ReadUInt( this, Category, Variable, Index, Default ) & MAX_UINT16;
}

u8 FConfig::ReadUInt8( const char* Category, const char* Variable, size_t Index, u8 Default )
{
  return ReadUInt( this, Category, Variable, Index, Default ) & MAX_UINT8;
}

static inline i64 ReadInt( FConfig* Config, const char* Category, const char* Variable, size_t Index, i64 Default )
{
  i64 Value = 0;
  char StrDef[24];
  sprintf( StrDef, "%ld", Default );

  char* StrVar = Config->ReadString( Category, Variable, Index, StrDef );
  if ( LIKELY( StrVar ) )
  {
    Value = strtoll( StrVar, NULL, 10 );
    if ( Value == MAX_INT64 || Value == MIN_INT64 )
      Value = 0;

    free( StrVar );
  }
  return Value;
}

i64 FConfig::ReadInt64( const char* Category, const char* Variable, size_t Index, i64 Default )
{
  return ReadInt( this, Category, Variable, Index, Default );
}

i32 FConfig::ReadInt32( const char* Category, const char* Variable, size_t Index, i32 Default )
{
  return ReadInt( this, Category, Variable, Index, Default ) & MAX_UINT32;
}

i16 FConfig::ReadInt16( const char* Category, const char* Variable, size_t Index, i16 Default )
{
  return ReadInt( this, Category, Variable, Index, Default ) & MAX_UINT16;
}

i8 FConfig::ReadInt8( const char* Category, const char* Variable, size_t Index, i8 Default )
{
  return ReadInt( this, Category, Variable, Index, Default ) & MAX_UINT8;
}

float FConfig::ReadFloat( const char* Category, const char* Variable, size_t Index, float Default )
{
  float Value = 0.f;
  char StrDef[16];
  sprintf( StrDef, "%f", Default );

  char* StrVar = ReadString( Category, Variable, Index, StrDef );
  if ( LIKELY( StrVar ) )
  {
    Value = strtof( StrVar, NULL );
    free( StrVar );
  }
  return Value;
}

double FConfig::ReadDouble( const char* Category, const char* Variable, size_t Index, double Default )
{
  double Value = 0.0;
  char StrDef[32];
  sprintf( StrDef, "%lf", Default );

  char* StrVar = ReadString( Category, Variable, Index, StrDef );
  if ( LIKELY( StrVar ) )
  {
    Value = strtod( StrVar, NULL );
    free( StrVar );
  }
  return Value;
}

void FConfig::ReadStruct( const char* Category, const char* Variable, UStruct* Struct, void* StructLoc, size_t Index )
{
}

void FConfig::ReadObject( const char* Category, const char* Variable, UObject* Obj, size_t Index )
{
}

void FConfig::WriteString( const char* Category, const char* Variable, const char* Value, size_t Index )
{
  FConfigCategory* Cat = NULL; // meow
  FConfigEntry* Entry = NULL;
  FHash CatHash = FnvHashString( Category );
  for ( size_t i = 0; i < Categories.size(); i++ )
  {
    FConfigCategory* CatIter = Categories[i];
    if ( CatIter->Hash == CatHash )
    {
      Cat = CatIter;
      break;
    }
  }

  if ( Cat == NULL )
  {
    Cat = new FConfigCategory();
    Cat->Name = strdup( Category );
    Cat->Hash = CatHash;
  }

  FHash VarHash = FnvHashString( Variable );
  for ( size_t j = 0; j < Cat->Entries->size(); j++ )
  {
    FConfigEntry* EntryIter = (*Cat->Entries)[j];
    if ( EntryIter->Hash == VarHash )
    {
      Entry = EntryIter;
      break;
    }
  }

  if ( Entry == NULL )
  {
    Entry = new FConfigEntry();
    Entry->Name = strdup( Variable );
    Entry->Hash = VarHash;
    Cat->Entries->push_back( Entry );
  }

  if ( Index >= Entry->Values->size() )
  {
    const char* Empty = "";
    Entry->Values->resize( Index+1, (char*)Empty );
  }
     
  char** Val = &Entry->Values->data()[ Index ];
  *Val = strdup( Value );
}

TArray<char*>* FConfig::CreateEntry( const char* Category, const char* Variable )
{
  FConfigCategory* CatIter;
  FConfigEntry* Entry;
  FHash CatHash = FnvHashString( Category ); // meow
  for ( size_t i = 0; i < Categories.size(); i++ )
  {
    CatIter = Categories[i];
    if ( CatIter->Hash == CatHash )
    {
      FHash VarHash = FnvHashString( Variable );
      for ( size_t j = 0; j < CatIter->Entries->size(); j++ )
      {
        Entry = (*CatIter->Entries)[j];
        if ( Entry->Hash == VarHash )
          return Entry->Values; // Don't make a new one if one already exists
      }
      goto makeEntry;
    }
  }

  CatIter = new FConfigCategory();
  CatIter->Name = strdup( Category );
  CatIter->Hash = FnvHashString( CatIter->Name );
  Categories.PushBack( CatIter );

makeEntry:
  Entry = new FConfigEntry();
  Entry->Name = strdup( Variable );
  Entry->Hash = FnvHashString( Entry->Name );
  CatIter->Entries->PushBack( Entry );
  return Entry->Values;
}

const char* FConfig::GetName()
{
  return Name;
}

FConfig::FConfigEntry::FConfigEntry()
{
  Name = NULL;
  Hash = ZERO_HASH;
  Values = new TArray<char*>();
  StructVars = NULL;
  bWriteIndices = false; // TODO: Make libunr.ini option
}

FConfig::FConfigEntry::~FConfigEntry()
{
  if ( Name != NULL )
    delete Name;

  if ( Values != NULL )
    for ( int i = 0; i < Values->size(); i++ )
      delete (*Values)[i];
  
  if ( StructVars != NULL )
  {
    // TODO:
  }
}

FConfig::FConfigCategory::FConfigCategory()
{
  Name = NULL;
  Hash = ZERO_HASH;
  Entries = new TArray<FConfigEntry*>();
  //Entries->Reserve( 4 );
}

FConfig::FConfigCategory::~FConfigCategory()
{
  if ( Name != NULL )
    delete Name;

  if ( Entries != NULL )
    for ( int i = 0; i < Entries->Size(); i++ )
      delete (*Entries)[i];
}

FConfigManager::FConfigManager()
{
}

FConfigManager::~FConfigManager()
{
}

void FConfigManager::AddConfig( FConfig* Cfg )
{
  Configs.PushBack( Cfg );
}

FConfig* FConfigManager::GetConfig( const char* Name )
{
  size_t NameLen = strlen( Name );
  for ( int i = 0; i < Configs.size(); i++ )
  {
    if ( strnicmp( Name, Configs[i]->GetName(), NameLen ) == 0 )
      return Configs[i];
  }

  return NULL;
}

void FConfigManager::DelConfig( FConfig* Cfg )
{
  for ( int i = 0; i < Configs.size(); i++ )
  {
    if ( Configs[i] == Cfg )
    {
      delete Cfg;
      Configs.data()[i] = NULL;
    }
  }
}

void FConfigManager::SaveAndCloseConfigs()
{
  for ( int i = 0; i < Configs.size(); i++ )
  {
    if ( !Configs[i]->Save() )
      GLogf( LOG_WARN, "Failed to save config file '%s'", Configs[i]->GetName() );
    
    delete Configs[i];
  }

  Configs.clear();
  Configs.shrink_to_fit();
}

void FConfigManager::CloseConfigs()
{
  for ( int i = 0; i < Configs.size(); i++ )
    delete Configs[i];

  Configs.clear();
  Configs.shrink_to_fit();
}

