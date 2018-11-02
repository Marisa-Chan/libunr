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
 * UEdFrame.h - Base editor frame class
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once

#include <wx/wx.h>

class UEdFrame : public wxFrame
{
public:
  UEdFrame( const wxString& Title, const wxPoint& Pos, const wxSize& Size );

protected:
  // UI drawing functions
  virtual void DrawFrame();
  virtual void DrawMenuBar();
  virtual void DrawButtonBar();

  // TODO: Add icon argument
  virtual void AddButtonToBar( wxStandardID Id, const wxString& Label );
  virtual void AddSpacerToBar();

  u32 MenuFontSize;
  wxSize ButtonSize;

  wxMenuBar* MenuBar;
  wxBoxSizer* MainSizer;
  wxGridSizer* ButtonSizer;
};


