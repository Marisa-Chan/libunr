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
 * UAudio.h - Base Audio Subsystem 
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "USystem.h"

class UAudioSubsystem : public USubsystem
{
  DECLARE_NATIVE_CLASS( UAudioSubsystem, USubsystem, CLASS_NoExport, Engine )
  EXPOSE_TO_USCRIPT()

  UAudioSubsystem();

  // TODO:
};

