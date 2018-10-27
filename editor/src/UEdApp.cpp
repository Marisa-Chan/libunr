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
 * UEdApp.cpp - Entry point for the editor
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "libunr.h"
#include "UEdApp.h"
#include "UEdMainFrame.h"

UEdApp* Editor = NULL;

bool UEdApp::OnInit()
{
  // TODO: Group all of this into one function
  // TODO: Write gui handler for first load
  // Initialize libunr
  if ( UNLIKELY( !USystem::StaticInit( NULL, NULL ) ) )
  {
    Logf( LOG_CRIT, "USystem::StaticInit() failed!" );
    return false;
  }

  if ( UNLIKELY( !UPackage::StaticInit() ) )
  {
    Logf( LOG_CRIT, "UPackage::StaticInit() failed!" );
    return false;
  }

  if ( UNLIKELY( !UObject::StaticInit() ) )
  {
    Logf( LOG_CRIT, "UObject::StaticInit() failed!" );
    return false;
  }

  // Load editor ini
  Config = new FConfig();
  const char* EditorIniPath = GetEditorIniPath();
  int IniStatus = Config->Load( EditorIniPath );
  if ( IniStatus == ERR_FILE_NOT_EXIST )
  {
    // Create an ini if it's missing
    Logf( LOG_WARN, "Editor ini file '%s' does not exist; creating one", EditorIniPath );
#if defined __unix__ || __linux__
    String* ConfigEditor = new String( USystem::GetHomeDir() );
    ConfigEditor->Append( "/.config/libunr/editor/" );
    DIR* ConfigEditorDir = opendir( ConfigEditor->Data() );
    if ( ConfigEditorDir == NULL )
      mkdir( ConfigEditor->Data(), S_IRWXU | S_IRGRP | S_IROTH );
#endif

    const char* DefaultEditorIniPath = GetDefaultEditorIniPath();
    if ( !USystem::CopyFile( DefaultEditorIniPath, EditorIniPath ) )
    {
      Logf( LOG_CRIT, "Default editor ini file '%s' does not exist; aborting",
          DefaultEditorIniPath );
      return false;
    }

    // If we can't make a new one, fail
    if ( Config->Load( EditorIniPath ) )
    {
      Logf( LOG_CRIT, "Error creating new editor ini file '%s'; aborting",
          EditorIniPath );
      return false;
    }
  }
  else if ( IniStatus != 0 )
  {
    Logf( LOG_CRIT, "Error parsing editor ini file '%s'; aborting",
        EditorIniPath );
    return false;
  }

  // Get editor info
  UiScale = Config->ReadInt32( "Editor", "UiScale" );

  // Set global editor instance
  Editor = this;

  // TODO: Initial size should be as big as the current display
  // Create main editor frame
  UEdMainFrame* MainFrame = new UEdMainFrame( "libunr Editor", 
      wxPoint( 50, 50 ),
      wxSize( 1024, 768 ) );

  MainFrame->Show();

  return true;
}

u32 UEdApp::GetUiScale()
{
  return UiScale;
}

const char* UEdApp::GetEditorIniPath()
{
#if defined __linux__ || __unix__
  static char LibUnrPath[1024] = { 0 };
  if ( LibUnrPath[0] == '\0' )
  {
    strcpy( LibUnrPath, USystem::GetHomeDir() );
    strcat( LibUnrPath, "/.config/libunr/editor/libunr-editor.ini" ); 
  }
  return LibUnrPath;
#elif defined _WIN32
  return "LibunrEditor.ini";
#else
  #error "Unknown operating system! Please add a section to USystem::GetEditorIniPath()"
  return NULL;
#endif
}

const char* UEdApp::GetDefaultEditorIniPath()
{
#if defined __linux__ || __unix__
  static char DefLibUnrPath[1024] = { 0 };
  if ( DefLibUnrPath[0] == '\0' )
  {
    strcpy( DefLibUnrPath, INSTALL_PREFIX );
    strcat( DefLibUnrPath, "/share/libunr/editor/DefaultLibunrEditor.ini" );
  }
  return DefLibUnrPath;
#elif defined _WIN32
  return "DefaultLibunrEditor.ini";
#else
  #error "Unknown operating system! Please add a section to UEdApp::GetDefaultEditorIniPath()"
  return NULL;
#endif
}

wxIMPLEMENT_APP(UEdApp);
