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
 * UEdApp.h - Application class for the editor.
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include <wx/wx.h>
#include "FConfig.h"

class UEdApp : public wxApp
{
public:
  virtual bool OnInit();

  // Accessors
  u32 GetUiScale();
  FConfig* GetConfig();

protected:
  static const char* GetEditorIniPath();
  static const char* GetDefaultEditorIniPath();

  FConfig* Config;
  u32 UiScale;
};

extern UEdApp* Editor;

