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
 * UEdFrame.cpp - Base editor frame code. Handles UI scaling
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "libunr.h"
#include "UEdApp.h"
#include "UEdFrame.h"

UEdFrame::UEdFrame( const wxString& Title, const wxPoint& Pos, const wxSize& Size )
  : wxFrame( NULL, wxID_ANY, Title, Pos, Size )
{
}

void UEdFrame::DrawFrame()
{
  DrawMenuBar();

  // Prepare main sizer
  MainSizer = new wxBoxSizer( wxVERTICAL );
  SetSizerAndFit(MainSizer);

  DrawButtonBar();
}

void UEdFrame::DrawMenuBar()
{
  MenuBar = new wxMenuBar();
  SetMenuBar( MenuBar );
}

void UEdFrame::DrawButtonBar()
{
  u32 ButtonDim = MAX( 24, 24 * Editor->GetUiScale() );
  ButtonSize.Set( ButtonDim, ButtonDim );

  ButtonSizer = new wxGridSizer( 0, 0, 0 );
  MainSizer->Add( ButtonSizer );
}

void UEdFrame::AddButtonToBar( wxStandardID Id, const wxString& Label )
{
  ButtonSizer->Add( new wxButton( this, Id, Label, wxDefaultPosition, ButtonSize ),
      0, wxLEFT, 1 );
}

void UEdFrame::AddSpacerToBar()
{
  ButtonSizer->AddSpacer( ButtonSize.GetWidth() / 4 );
}

