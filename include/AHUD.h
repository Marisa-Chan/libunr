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
 * AHUD.h - Heads-Up Display Class
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "AActor.h"

class AHUDOverlay;
class AMenu;

class DLL_EXPORT AHUD : public AActor
{
  DECLARE_NATIVE_CLASS( AHUD, AActor, CLASS_Abstract, Engine )
  EXPOSE_TO_USCRIPT()

  AHUD();

  int HudMode;
  int Crosshair;
  UClass* MainMenuType;
  String* HUDConfigWindowType;

  AMenu* MainMenu;
  Array<AHUDOverlay*>* Overlays;
};

class DLL_EXPORT AHUDOverlay : public AActor
{
  DECLARE_NATIVE_CLASS( AHUDOverlay, AActor, CLASS_Abstract, Engine )
  EXPOSE_TO_USCRIPT()

  AHUDOverlay();

  AHUD* myHUD;
  bool bPostRender;
  bool bPreRender;
};

