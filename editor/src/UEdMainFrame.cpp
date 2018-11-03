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
 * UEdMainFrame.cpp - Main editor frame code
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "libunr.h"
#include "UEdMainFrame.h"

UEdMainFrame::UEdMainFrame( const wxString& Title, const wxPoint& Pos, const wxSize& Size )
  : UEdFrame( Title, Pos, Size )
{
  DrawFrame();
}

void UEdMainFrame::DrawFrame()
{
  // Do generic setup
  UEdFrame::DrawFrame();

  // TODO: Draw side button bar
  // TODO: Draw viewports
}

void UEdMainFrame::DrawMenuBar()
{
  UEdFrame::DrawMenuBar();

  // File menu
  wxMenu* FileMenu = new wxMenu;
  
  // Edit menu
  wxMenu* EditMenu = new wxMenu;

  // View menu
  wxMenu* ViewMenu = new wxMenu;

  // Brush menu
  wxMenu* BrushMenu = new wxMenu;

  // Build menu
  wxMenu* BuildMenu = new wxMenu;
  
  // Tools menu
  wxMenu* ToolsMenu = new wxMenu;

  // Help menu
  wxMenu* HelpMenu = new wxMenu;

  // Append all menus
  MenuBar->Append( FileMenu, "&File" );
  MenuBar->Append( EditMenu, "&Edit" );
  MenuBar->Append( ViewMenu, "&View" );
  MenuBar->Append( BrushMenu, "&Brush" );
  MenuBar->Append( BuildMenu, "&Build" );
  MenuBar->Append( ToolsMenu, "&Tools" );
  MenuBar->Append( HelpMenu, "&Help" );
}

void UEdMainFrame::DrawButtonBar()
{
  UEdFrame::DrawButtonBar();

  // TODO: Get button images
  
  // New/Open/Save Map
  AddButtonToBar( wxID_ANY, wxEmptyString );
  AddButtonToBar( wxID_ANY, wxEmptyString );
  AddButtonToBar( wxID_ANY, wxEmptyString );
  AddSpacerToBar();

  // Undo/Redo
  AddButtonToBar( wxID_ANY, wxEmptyString );
  AddButtonToBar( wxID_ANY, wxEmptyString );
  AddSpacerToBar();

  // Search Actors
  AddButtonToBar( wxID_ANY, wxEmptyString );
  AddSpacerToBar();

  // Content Browsers
  AddButtonToBar( wxID_ANY, "P" );
  AddButtonToBar( wxID_ANY, wxEmptyString );
  AddButtonToBar( wxID_ANY, wxEmptyString );
  AddButtonToBar( wxID_ANY, wxEmptyString );
  AddButtonToBar( wxID_ANY, wxEmptyString );
  AddButtonToBar( wxID_ANY, wxEmptyString );
  AddButtonToBar( wxID_ANY, wxEmptyString );
  AddSpacerToBar();
}

