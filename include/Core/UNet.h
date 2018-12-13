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
 * UNet.h - Networking classes
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "Core/UPlayer.h"
#include "Core/USystem.h"

class UNetConnection : public UPlayer
{
  DECLARE_NATIVE_CLASS( UNetConnection, UPlayer, 
      CLASS_NoExport | CLASS_Transient | CLASS_Config, Engine )

 
  UNetConnection();
};

class UNetDriver : public USubsystem
{
  DECLARE_NATIVE_CLASS( UNetDriver, USubsystem,
      CLASS_NoExport | CLASS_Transient, Engine )

  UNetDriver();

  // TODO:
};

