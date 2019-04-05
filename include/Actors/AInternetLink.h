/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU Affero General Public License as           *|
|*  published by the Free Software Foundation, either version 3 of the       *|
|*  License, or (at your option) any later version.                          *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU Affero General Public License for more details.                      *|
|*                                                                           *|
|*  You should have received a copy of the GNU Affero General Public License *|
|*  along with this program.  If not, see <https://www.gnu.org/licenses/>.   *|
\*===========================================================================*/

/*========================================================================
 * AInternetLink.h - Info classes for handling internet connection
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#pragma once
#include "Actors/AActor.h"

class DLL_EXPORT AInternetInfo : public AInfo
{
  DECLARE_NATIVE_CLASS( AInternetInfo, AInfo, CLASS_Transient, Engine )
  EXPOSE_TO_USCRIPT()

  AInternetInfo();
};

class DLL_EXPORT AInternetLink : public AInternetInfo
{
  DECLARE_NATIVE_CLASS( AInternetLink, AInternetInfo, CLASS_Transient, IpDrv )
  EXPOSE_TO_USCRIPT()

  AInternetLink();

  // Native variables
  struct IpAddr
  {
    int Addr;
    int Port;
  };

  enum ELinkMode
  {
    MODE_Text,
    MODE_Line,
    MODE_Binary
  } LinkMode;

  int Socket;
  int Port;
  int RemoteSocket;
  int PrivateResolveInfo; // ?
  int DataPending;

  enum EReceiveMode
  {
    RMODE_Manual,
    RMODE_Event
  } ReceiveMode;

  // Native functions
  bool IsDataPending();
  bool ParseURL( FString* URL, FString* Addr, int Port, FString* LevelName, FString* EntryName );
  void Resolve( FString* Domain );
  int GetLastError();
  FString* IpAddrToString( IpAddr Arg );
  FString* Validate( FString* ValidationString, FString* GameName );
};

class DLL_EXPORT ATcpLink : public AInternetLink
{
  DECLARE_NATIVE_CLASS( ATcpLink, AInternetLink, CLASS_Transient, IpDrv )
  EXPOSE_TO_USCRIPT()

  ATcpLink();

  // Native variables
  enum ELinkState
  {
    STATE_Initialized,         // Socket is initialized
    STATE_Ready,               // Port is bound, ready for activity
    STATE_Listening,           // Listening for connections
    STATE_Connecting,          // Attempting to connect
    STATE_Connected,           // Open and connected
    STATE_ListenClosePending,  // Socket in the process of closing
    STATE_ConnectClosePending, // ''
    STATE_ListenClosing,       // '' 
    STATE_ConnectClosing,      // ''
  } LinkState;

  IpAddr RemoteAddr;
  UClass* AcceptClass;
  Array<u8>* SendFIFO;

  // Native functions
  bool BindPort( int Port, bool bUseNextAvailable );
  bool Listen();
  bool Open( IpAddr Addr );
  bool Close();
  bool IsConnected();
  int SendText( FString* Str );
  int SendBinary( int Count, u8* B );
  int ReadText( FString* Str );
  int ReadBinary( int Count, u8* B );
};

class DLL_EXPORT AUdpLink : public AInternetLink
{
  DECLARE_NATIVE_CLASS( AUdpLink, AInternetLink, CLASS_Transient, IpDrv )
  EXPOSE_TO_USCRIPT()

  AUdpLink();

  // Native variables
  int BroadcastAddr;

  // Native functions
  bool BindPort( int Port, bool bUseNextAvailable );
  int SendText( FString* Str );
  int SendBinary( int Count, u8* B );
  int ReadText( FString* Str );
  int ReadBinary( int Count, u8* B );
};

