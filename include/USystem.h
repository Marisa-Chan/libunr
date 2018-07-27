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
 * USystem.h - System class, manages runtime OS variables
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "UObject.h"

class USubsystem : public UObject
{
  DECLARE_ABSTRACT_CLASS( USubsystem, UObject, 0, Core )
  USubsystem();
};

class USystem : public USubsystem
{
  DECLARE_CLASS( USystem, USubsystem, 0, Core )
 
  const char* ResolvePath( const char* PkgName );

  // Unitech specific
  String* GamePath;
  String* GameName;

  // Per game
  int PurgeCacheDays;
  String* SavePath;
  String* CachePath;
  String* CacheExt;
  Array<String*> Paths;
};

extern USystem* GSystem;