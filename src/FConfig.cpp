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
 * FConfig.h - Class for loading and saving INI files
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "FConfig.h"
#include "Memory.h"

FConfigManager* GConfigManager = NULL;
FConfig* GLibunrConfig = NULL;
FConfig* GGameConfig = NULL;
FConfig* GUserConfig = NULL;

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

static inline void ReadNewLine( FileStreamIn& In, const char* Filename )
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
      Logf( LOG_WARN, "Unpaired carriage return in config file '%s' (Pos = %llu, C = %c)", 
          Filename, In.Tell(), C );
    }
  }
  else if ( C != '\n' )
  {
    Logf( LOG_WARN, "Expected newline in config file '%s' (Pos = %llu, C = %c)",
        Filename, In.Tell(), C );
  }
}

static inline bool IsNextCharSeqNewline( FileStreamIn& In, const char* Filename )
{
  char C = In.Peek();
  if ( C == '\r' )
  {
    In.Read( &C, 1 );
    C = In.Peek();
    if ( UNLIKELY ( C != '\n' ) )
    {
      Logf( LOG_WARN, "Unpaired carriage return in config file '%s' (Pos = %llu, C = %c)", 
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
  for ( size_t i = 0; i < Categories.Size() && i != MAX_SIZE; i++ )
    delete Categories[i];

  Categories.Clear();
}

int FConfig::Load( const char* Filename )
{
  char CategoryBuf[128];
  char VariableBuf[128];
  char ValueBuf[512];
  bool bIndexed = false;
  u32  Index = 0;
  FHash PreviousHash = {0, 0};

  FileStreamIn IniFile;
  int Status = IniFile.Open( Filename );
  if ( Status != 0 )
  {
    Logf( LOG_WARN, "Can't open ini file '%s' (errno = %s)", Filename, strerror( Status ) );
    return ERR_FILE_NOT_EXIST;
  }

  // Read until the end of the file
  FConfigCategory* Category = NULL;
  FConfigEntry* Entry = NULL;
  Set( CategoryBuf, 0, sizeof( CategoryBuf ) );
  Set( VariableBuf, 0, sizeof( VariableBuf ) );
  Set( ValueBuf, 0, sizeof( ValueBuf ) );

  char Probe = '\0';
  while ( !IniFile.Eof() ) // why doesn't Eof() work??
  {
    Status = IniFile.Read( &Probe, 1 );
    if ( Status == MAX_UINT64 )
    {
      Logf( LOG_WARN, "Failed to read from INI file '%s'", Filename );
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
    /*else if ( Probe == '\r' )
    {
      IniFile.Read( &Probe, 1 );
      if ( Probe == '\n' )
        Category = NULL; 
    }
    else if ( Probe == '\n' )
    {
      Category = NULL;
    }*/
    else if ( Probe == '[' )
    {
      char* CategoryPtr = CategoryBuf;
      xstl::Set( CategoryBuf, 0, sizeof( CategoryBuf ) );
      while ( PtrDistance( CategoryPtr, CategoryBuf ) < sizeof( CategoryBuf ) )
      {
        IniFile.Read( &Probe, 1 );
        if ( Probe == ']' )
          break;

        if ( !isalnum( Probe ) && !IsAcceptedChar( Probe, "_. " ) )
        {
          Set( CategoryBuf, 0, sizeof( CategoryBuf ) );
          break;
        }

        *CategoryPtr++ = Probe;
      }

      if ( CategoryBuf[0] == '\0' )
      {
        Logf( LOG_WARN, 
            "Malformed or corrupted category in file '%s' (Pos = %llu)", 
            Filename, IniFile.Tell() );
        IniFile.Close();
        return ERR_BAD_DATA;
      }

      Category = new FConfigCategory();
      Category->Name = StringDup( CategoryBuf );
      Category->Hash = FnvHashString( Category->Name );
      Categories.PushBack( Category );

      ReadNewLine( IniFile, Filename );
    }
    // We're in a category, read a full line
    else if ( Probe != '\n' && Probe != '\r' )
    {
      if ( Category == NULL )
      {
        Logf( LOG_WARN, 
            "Got non-category character, but no category in config file '%s' (Pos = %llu)",
            Filename, IniFile.Tell() );
        IniFile.Close();
        return ERR_BAD_DATA;
      }
      // Read the variable name
      char* VariablePtr = VariableBuf;
      xstl::Set( VariableBuf, 0, sizeof( VariableBuf ) );
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
          Set( VariableBuf, 0, sizeof( VariableBuf ) );
          break;
        }        
      }

      if ( VariableBuf[0] == '\0' )
      {
        Logf( LOG_WARN, 
            "Malformed or corrupted variable in file '%s' (Pos = %llu)", 
            Filename, IniFile.Tell() );
        IniFile.Close();
        return ERR_BAD_DATA;
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

        if ( IndexBuf[0] == '\0' )
        {
          Logf( LOG_WARN, 
              "Array indexed variable, but no index found in file '%s' (Pos = %llu, C = '%c')", 
              Filename, IniFile.Tell(), Probe );
          IniFile.Close();
          return ERR_BAD_DATA;
        }

        Index = strtol( IndexBuf, NULL, 10 );
        Entry->Values->Resize( Index );
        xstl::Set( IndexBuf, 0, sizeof( IndexBuf ) );
      }
      
      FHash VarHash = FnvHashString( VariableBuf );
      if ( LIKELY( VarHash != PreviousHash ) )
      {
        Entry = new FConfigEntry();
        Category->Entries->PushBack( Entry );
        Entry->Name = StringDup( VariableBuf );
        Entry->Hash = VarHash;
      }

      // Read the value
      char* ValuePtr = ValueBuf;
      xstl::Set( ValueBuf, 0, sizeof( ValueBuf ) );
      while ( PtrDistance( ValuePtr, ValueBuf ) < sizeof( ValueBuf ) )
      {
        if ( IsNextCharSeqNewline( IniFile, Filename ) || IniFile.Eof() )
          break;

        IniFile.Read( &Probe, 1 );
        *ValuePtr++ = Probe;
      }

      if ( LIKELY( !bIndexed || VarHash == PreviousHash ) )
        Entry->Values->PushBack( xstl::StringDup( ValueBuf ) );
      else
        Entry->Values->Assign( Index, xstl::StringDup( ValueBuf ) );

      ReadNewLine( IniFile, Filename );
      bIndexed = false;
      PreviousHash = VarHash;
    }
  }
  Logf( LOG_INFO, "Successfully read config file '%s'", Filename );
  Name = StringDup( Filename );
  IniFile.Close();
  return 0;
}

// TODO: Write failure cases
int FConfig::Save()
{
  FileStreamOut IniFile;
  int Status = IniFile.Open( Name );
  if ( Status != 0 )
  {
    Logf( LOG_WARN, "Failed to open ini file '%s' for saving (errno = %s)", Name, strerror( Status ) );
    return ERR_FILE_CREATE;
  }

  int WriteLen = 0;
  char WriteBuf[512];
  char NewLine[] = "\r\n";
  for( size_t i = 0; i < Categories.Size() && i != MAX_SIZE; i++ )
  {
    Set( WriteBuf, 0, sizeof( WriteBuf ) );
    FConfigCategory* Category = Categories[i];
    WriteLen = snprintf( WriteBuf, sizeof( WriteBuf ), "[%s]\r\n", Category->Name );
    IniFile.Write( WriteBuf, MIN( WriteLen, sizeof( WriteBuf ) ) );

    for( size_t j = 0; j < Category->Entries->Size() && j != MAX_SIZE; j++ )
    {
      FConfigEntry* Entry = (*Category->Entries)[j];
      for ( size_t k = 0; k < Entry->Values->Size() && k != MAX_SIZE; k++ )
      {
        char* Value = (*Entry->Values)[k];
        Set( WriteBuf, 0, sizeof( WriteBuf ) );
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

char* FConfig::ReadString( const char* Category, const char* Variable, size_t Index )
{
  FHash CatHash = FnvHashString( Category ); // meow
  for ( size_t i = 0; i < Categories.Size() && i != MAX_SIZE; i++ )
  {
    FConfigCategory* CatIter = Categories[i];
    if ( CatIter->Hash == CatHash )
    {
      FHash VarHash = FnvHashString( Variable );
      for ( size_t j = 0; j < CatIter->Entries->Size(); j++ )
      {
        FConfigEntry* Entry = (*CatIter->Entries)[j];
        if ( Entry->Hash == VarHash )
        {
          if ( UNLIKELY( Index >= Entry->Values->Size() ) )
            return NULL;

          char* Value = (*Entry->Values)[Index];
          return StringDup( Value );
        }
      }
    }
  }
  return NULL;
}

bool FConfig::ReadBool( const char* Category, const char* Variable, size_t Index )
{
  bool Value = false;
  char* StrVar = ReadString( Category, Variable, Index );
  if ( LIKELY( StrVar ) )
  {
    if ( strncmp( StrVar, "true", 4 ) == 0 )
      Value = true;

    Free( StrVar );
  }
  return Value;
}

static inline u64 ReadUInt( FConfig* Config, const char* Category, const char* Variable, size_t Index )
{
  u64 Value = 0;
  char* StrVar = Config->ReadString( Category, Variable, Index );
  if ( LIKELY( StrVar ) )
  {
    Value = strtoull( StrVar, NULL, 10 );
    if ( Value == MAX_UINT64 )
      Value = 0;

    Free( StrVar );
  }
  return Value;
}

u64 FConfig::ReadUInt64( const char* Category, const char* Variable, size_t Index )
{
  return ReadUInt( this, Category, Variable, Index );
}

u32 FConfig::ReadUInt32( const char* Category, const char* Variable, size_t Index )
{
  return ReadUInt( this, Category, Variable, Index ) & MAX_UINT32;
}

u16 FConfig::ReadUInt16( const char* Category, const char* Variable, size_t Index )
{
  return ReadUInt( this, Category, Variable, Index ) & MAX_UINT16;
}

u8 FConfig::ReadUInt8( const char* Category, const char* Variable, size_t Index )
{
  return ReadUInt( this, Category, Variable, Index ) & MAX_UINT8;
}

static inline i64 ReadInt( FConfig* Config, const char* Category, const char* Variable, size_t Index )
{
  i64 Value = 0;
  char* StrVar = Config->ReadString( Category, Variable, Index );
  if ( LIKELY( StrVar ) )
  {
    Value = strtoll( StrVar, NULL, 10 );
    if ( Value == MAX_INT64 || Value == MIN_INT64 )
      Value = 0;

    Free( StrVar );
  }
  return Value;
}

i64 FConfig::ReadInt64( const char* Category, const char* Variable, size_t Index )
{
  return ReadInt( this, Category, Variable, Index );
}

i32 FConfig::ReadInt32( const char* Category, const char* Variable, size_t Index )
{
  return ReadInt( this, Category, Variable, Index ) & MAX_UINT32;
}

i16 FConfig::ReadInt16( const char* Category, const char* Variable, size_t Index )
{
  return ReadInt( this, Category, Variable, Index ) & MAX_UINT16;
}

i8 FConfig::ReadInt8( const char* Category, const char* Variable, size_t Index )
{
  return ReadInt( this, Category, Variable, Index ) & MAX_UINT8;
}

float FConfig::ReadFloat( const char* Category, const char* Variable, size_t Index )
{
  float Value = 0.f;
  char* StrVar = ReadString( Category, Variable, Value );
  if ( LIKELY( StrVar ) )
  {
    Value = strtof( StrVar, NULL );
    Free( StrVar );
  }
  return Value;
}

double FConfig::ReadDouble( const char* Category, const char* Variable, size_t Index )
{
  double Value = 0.0;
  char* StrVar = ReadString( Category, Variable, Value );
  if ( LIKELY( StrVar ) )
  {
    Value = strtod( StrVar, NULL );
    Free( StrVar );
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
  for ( size_t i = 0; i < Categories.Size() && i != MAX_SIZE; i++ )
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
    Cat->Name = StringDup( Category );
    Cat->Hash = CatHash;
  }

  FHash VarHash = FnvHashString( Variable );
  for ( size_t j = 0; j < Cat->Entries->Size(); j++ )
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
    Entry->Name = StringDup( Variable );
    Entry->Hash = VarHash;
    Cat->Entries->PushBack( Entry );
  }

  if ( Index >= Entry->Values->Size() )
  {
    const char* Empty = "";
    Entry->Values->Resize( Index+1, (char*)Empty );
  }
     
  char** Val = &Entry->Values->Data()[ Index ];
  *Val = (char*)Value;
}

const char* FConfig::GetName()
{
  return Name;
}

FConfig::FConfigEntry::FConfigEntry()
{
  Name = NULL;
  Hash = ZERO_HASH;
  Values = new Array<char*>();
  StructVars = NULL;
  bWriteIndices = false; // TODO: Make libunr.ini option
}

FConfig::FConfigEntry::~FConfigEntry()
{
  if ( Name != NULL )
    delete Name;

  if ( Values != NULL )
    for ( int i = 0; i < Values->Size(); i++ )
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
  Entries = new Array<FConfigEntry*>();
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
  for ( int i = 0; i < Configs.Size(); i++ )
  {
    if ( strnicmp( Name, Configs[i]->GetName(), NameLen ) == 0 )
      return Configs[i];
  }

  return NULL;
}

void FConfigManager::DelConfig( FConfig* Cfg )
{
  for ( int i = 0; i < Configs.Size(); i++ )
  {
    if ( Configs[i] == Cfg )
    {
      delete Cfg;
      Configs.Data()[i] = NULL;

      // TODO: See ArrayBase.h and use here
    }
  }
}

void FConfigManager::SaveAndCloseConfigs()
{
  for ( int i = 0; i < Configs.Size(); i++ )
  {
    if ( !Configs[i]->Save() )
      Logf( LOG_WARN, "Failed to save config file '%s'", Configs[i]->GetName() );
    
    delete Configs[i];
  }

  Configs.Clear();
  Configs.Reclaim();
}

void FConfigManager::CloseConfigs()
{
  for ( int i = 0; i < Configs.Size(); i++ )
    delete Configs[i];

  Configs.Clear();
  Configs.Reclaim();
}

