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

#include "FUtil.h"
#include "Array.h"
#include "FileStream.h"

using namespace xstl;
class UObject;

class FConfig
{
public:
	FConfig();
	FConfig( const char* Filename );
	~FConfig();

	bool Load( const char* Filename );
	bool Save();

	// Read functions
	// These functions will try to read the requested variable
	// If the Variable does not exist, a default value will be created and added to the config
	// If the SubVariable does not exist, a default value will be returned but not added to config
	char* ReadString( const char* Category, const char* Variable );
	bool  ReadBool  ( const char* Category, const char* Variable );
  u64 ReadUInt64( const char* Category, const char* Variable ); 
	u32 ReadUInt32( const char* Category, const char* Variable );
	u16 ReadUInt16( const char* Category, const char* Variable );
	u8  ReadUInt8 ( const char* Category, const char* Variable );

	i64 ReadInt64( const char* Category, const char* Variable );
  i32 ReadInt32( const char* Category, const char* Variable );
  i16 ReadInt16( const char* Category, const char* Variable );
	i8  ReadInt8 ( const char* Category, const char* Variable );

	float  ReadFloat ( const char* Category, const char* Variable );
  double ReadDouble( const char* Category, const char* Variable );

	void ReadObject( const char* Category, const char* Variable, UObject* Obj );

	// Write functions
	// These functions will write a variable to the config, updating it if it exists, creating
	// a new one otherwise
	void WriteString( const char* Category, const char* Variable, const char* Value );
	void WriteBool  ( const char* Category, const char* Variable, bool Value );
  void WriteUInt64( const char* Category, const char* Variable, u64 Value );
	void WriteUInt32( const char* Category, const char* Variable, u32 Value );
	void WriteUInt16( const char* Category, const char* Variable, u16 Value );
	void WriteUInt8 ( const char* Category, const char* Variable, u8 Value );
   
  void WriteInt64( const char* Category, const char* Variable, i64 Value );
	void WriteInt32( const char* Category, const char* Variable, i32 Value );
	void WriteInt16( const char* Category, const char* Variable, i16 Value );
	void WriteInt8 ( const char* Category, const char* Variable, i8 Value );

	void WriteFloat ( const char* Category, const char* Variable, float Value );
	void WriteDouble( const char* Category, const char* Variable, double Value );

	void WriteObject( const char* Category, const char* Variable, UObject* Obj );

private:
	struct FConfigEntry
	{
		FConfigEntry();
		~FConfigEntry();

		char*  Name;
		size_t Hash;
		Array<char*>* Values;
		bool bWriteIndices;
	};

	struct FConfigCategory
	{
		FConfigCategory();
		~FConfigCategory();

		char*  Name;
		size_t Hash;
		Array<FConfigEntry*>* Entries;
	};

	Array<FConfigCategory*> Categories;
	char* Name;
};


