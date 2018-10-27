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
  DrawButtonBar();
}

void UEdFrame::DrawMenuBar()
{
  MenuBar = new wxMenuBar();
  SetMenuBar( MenuBar );
}

void UEdFrame::DrawButtonBar()
{
  u32 ButtonDim = MIN( 64, 64 * Editor->GetUiScale() );
  ButtonSize.Set( ButtonDim, ButtonDim );

  ButtonSizer = new wxBoxSizer( wxHORIZONTAL );
  ButtonSizer->SetMinSize( ButtonSize );
  ButtonSizer->SetContainingWindow( this );
}
