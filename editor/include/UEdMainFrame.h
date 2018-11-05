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
 * UEdMainFrame.h - Main editor frame class
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include "UEdFrame.h"
#include "UEdEvents.h"

class UEdMainFrame : public UEdFrame
{
public:
  UEdMainFrame( const wxString& Title, const wxPoint& Pos, const wxSize& Size );

protected:
  // UEdMainFrame functions
  virtual void DrawFrame();
  virtual void DrawMenuBar();
  virtual void DrawButtonBar();

  // Event handlers
  virtual void OnPkgBrowser( wxCommandEvent& Event );
  virtual void OnExit( wxCommandEvent& Event );

  wxDECLARE_EVENT_TABLE();
};

