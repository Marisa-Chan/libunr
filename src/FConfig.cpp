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
  char C = '\0';
  In.Read( &C, 1 );
  if ( C == '\r' )
  {
    In.Read( &C, 1 );
    if ( UNLIKELY ( C != '\n' ) )
    {
      Logf( LOG_WARN, "Unpaired carriage return in config file '%s' (Pos = %llu, C = %c)", 
          Filename, In.Tell(), C );
      In.Close();
    }
  }
  else if ( C != '\n' )
  {
    Logf( LOG_WARN, "Expected newline in config file '%s' (Pos = %llu, C = %c)",
        Filename, In.Tell(), C );
    In.Close();
  }
}

static inline bool IsNextCharSeqNewline( FileStreamIn& In, const char* Filename )
{
  char C = '\0';
  In.Read( &C, 1 );
  if ( C == '\r' )
  {
    In.Read( &C, 1 );
    if ( UNLIKELY ( C != '\n' ) )
    {
      Logf( LOG_WARN, "Unpaired carriage return in config file '%s' (Pos = %llu, C = %c)", 
          Filename, In.Tell(), C );
      In.Close();
      return false;
    }
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

bool FConfig::Load( const char* Filename )
{
  char CategoryBuf[128];
  char VariableBuf[128];
  char ValueBuf[512];
  FHash PreviousHash = {0, 0};

  FileStreamIn IniFile;
  if ( !IniFile.Open( Filename ) )
  {
    Logf( LOG_WARN, "Can't open or create INI file '%s'", Filename );
    return false;
  }

  // Read until the end of the file
  FConfigCategory* Category = NULL;
  FConfigEntry* Entry = NULL;
  Set( CategoryBuf, 0, sizeof( CategoryBuf ) );
  Set( VariableBuf, 0, sizeof( VariableBuf ) );
  Set( ValueBuf, 0, sizeof( ValueBuf ) );
  while ( !IniFile.Eof() )
  {
    char Probe = '\0';
    IniFile.Read( &Probe, 1 );

    if ( Probe == ';' )
    {
      // Read characters till we hit a newline
      while ( !IsNextCharSeqNewline( IniFile, Filename ) );
    }
    else if ( Probe == '[' )
    {
      if ( Category != NULL )
      {
        Logf( LOG_WARN, "Got unexpected '[' in '%s' (Pos = %llu)", Filename, IniFile.Tell() );
        IniFile.Close();
        return false;
      }

      char* CategoryPtr = CategoryBuf;
      while ( PtrDistance( CategoryPtr, CategoryBuf ) < sizeof( CategoryBuf ) )
      {
        IniFile.Read( &Probe, 1 );
        if ( Probe == ']' )
          break;

        if ( !isalnum( Probe ) )
        {
          Set( CategoryBuf, 0, sizeof( CategoryBuf ) );
          break;
        }

        *CategoryPtr++ = Probe;
      }

      if ( CategoryBuf[0] == '\0' )
      {
        Logf( LOG_WARN, "Malformed or corrupted config file '%s' (Pos = %llu)", Filename, IniFile.Tell() );
        IniFile.Close();
        return false;
      }

      Category = new FConfigCategory();
      Category->Name = StringDup( CategoryBuf );
      Category->Hash = FnvHashString( Category->Name );

      ReadNewLine( IniFile, Filename );
    }
    // We're in a category, read a full line
    else
    {
      if ( Category == NULL )
      {
        Logf( LOG_WARN, "Got non-category character, but no category in config file '%s' (Pos = %llu)",
            Filename, IniFile.Tell() );
        IniFile.Close();
        return false;
      }
      // Read the variable name
      char* VariablePtr = VariableBuf;
      while ( PtrDistance( VariablePtr, VariableBuf ) < sizeof( VariableBuf ) )
      {
        IniFile.Read( &Probe, 1 );
        if ( Probe == '=' )
          break;

        if ( !isalnum( Probe ) && !IsAcceptedChar( Probe, "[]" ) )
        {
          Set( VariableBuf, 0, sizeof( VariableBuf ) );
          break;
        }

        *VariablePtr++ = Probe;
      }

      if ( VariableBuf[0] == '\0' )
      {
        Logf( LOG_WARN, "Malformed or corrupted config file '%s' (Pos = %llu)", Filename, IniFile.Tell() );
        IniFile.Close();
        return false;
      }
      
      // Check to see if we should treat this as an array entry
      char*  ValueStr = NULL;
      FHash VarHash = FnvHashString( VariableBuf );
      if ( LIKELY( VarHash != PreviousHash ) )
      {
        char* PosLeftBracket = strchr( VariableBuf, '[' );
        if ( LIKELY( PosLeftBracket == NULL ) )
        {
          Entry = new FConfigEntry();
        }
      
        // Handle explicitly indexed variables
        else
        {
          char IndexBuf[6];
          char* IndexPtr = IndexBuf;
          ValueStr = (char*)Malloc( sizeof( ValueBuf ) );
          while ( PtrDistance( IndexPtr, IndexBuf ) < sizeof( IndexBuf ) )
          {
            IniFile.Read( &Probe, 1 );
            if ( !isdigit( Probe ) )
            {
              Logf( LOG_WARN, "Non decimal index character in config file '%s' (Pos = %llu)", 
                  Filename, IniFile.Tell() );
              IniFile.Close();
              return false;
            }

            *IndexPtr++ = Probe;
          }

          int Index = strtoul( IndexBuf, 0, 10 );
          if ( IndexBuf[0] == '0' && Index != 0 )
          {
            Logf( LOG_WARN, "Invalid index conversion in config file '%s' (Pos = %llu)", 
                Filename, IniFile.Tell() );
            IniFile.Close();
            return false;
          }
          Entry->Values->Reserve( Index );
          Entry->Values->Assign( Index-1, ValueStr );
        }
      }
      Entry->Name = StringDup( VariableBuf );
      Entry->Hash = VarHash;

      // Read the value
      char* ValuePtr = ValueBuf;
      while ( PtrDistance( ValuePtr, ValueBuf ) < sizeof( ValueBuf ) )
      {
        IniFile.Read( &Probe, 1 );
        
        if ( IsNextCharSeqNewline( IniFile, Filename ) )
          break;

        // Check if its valid (or a struct value)
        if ( !isalnum( Probe ) )
        {
          if ( LIKELY( !IsAcceptedChar( Probe, "[](),.=" ) ) )
          {
            // Not valid
            Set( ValueBuf, 0, sizeof( ValueBuf ) );
            break;
          }
          else
          {
            // Struct value, 
          }
        }

        *ValuePtr++ = Probe;
      }

      if ( LIKELY( ValueStr == NULL ) )
        Entry->Values->PushBack( ValueBuf );
      else
        Copy( ValueStr, sizeof( ValueBuf ), ValueBuf, sizeof( ValueBuf ) );
    }
  }
  Logf( LOG_INFO, "Successfully read config file '%s'", Filename );
  Name = StringDup( Filename );
  IniFile.Close();
  return true;
}

// TODO: Write failure cases
bool FConfig::Save()
{
  FileStreamOut IniFile;
  IniFile.Open( Name );

  int WriteLen = 0;
  char WriteBuf[512];
  char NewLine[] = "\r\n";
  for( size_t i = 0; i < Categories.Size() && i != MAX_SIZE; i++ )
  {
    FConfigCategory* Category = Categories[i];
    WriteLen = snprintf( WriteBuf, sizeof( WriteBuf ), "[%s]\r\n", Category->Name );
    IniFile.Write( WriteBuf, MIN( WriteLen, sizeof( WriteBuf ) ) );

    for( size_t j = 0; j < Category->Entries->Size() && j != MAX_SIZE; j++ )
    {
      FConfigEntry* Entry = (*Category->Entries)[j];
      for ( size_t k = 0; k < Entry->Values->Size() && k != MAX_SIZE; k++ )
      {
        char* Value = (*Entry->Values)[k];
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

    IniFile.Write( NewLine, sizeof( NewLine ) );
  }

  IniFile.Close();
  return true;
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
  bWriteIndices = true; // TODO: Make libunr.ini option
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
  Entries = new Array<FConfigEntry*>( 4 );
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

