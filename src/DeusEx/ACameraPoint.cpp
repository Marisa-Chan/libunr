/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2019  Adam W.E. Smith                                 *|
|*                                                                           *|
|*  This program is free software: you can redistribute it and/or modify     *|
|*  it under the terms of the GNU General Public License as published by     *|
|*  the Free Software Foundation, either version 3 of the License, or        *|
|*  (at your option) any later version.                                      *|
|*                                                                           *|
|*  This program is distributed in the hope that it will be useful,          *|
|*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *|
|*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *|
|*  GNU General Public License for more details.                             *|
|*                                                                           *|
|*  You should have received a copy of the GNU General Public License        *|
|*  along with this program. If not, see <https://www.gnu.org/licenses/>.    *|
\*===========================================================================*/

/*========================================================================
 * ACameraPoint.cpp - Deus Ex CameraPoint actor
 * 
 * written by Adam 'Xaleros' Smith
 *========================================================================
*/

#include "DeusEx/ACameraPoint.h"

ACameraPoint::ACameraPoint()
  : AKeypoint()
{
}

ACameraPoint::~ACameraPoint()
{
}

#include "Core/UClass.h"
IMPLEMENT_NATIVE_CLASS( ACameraPoint );

BEGIN_PROPERTY_LINK( ACameraPoint, 24 )
  LINK_NATIVE_PROPERTY( Cmd );
  LINK_NATIVE_PROPERTY( Value );
  LINK_NATIVE_PROPERTY( EventName );
  LINK_NATIVE_PROPERTY( TimeSmooth );
  LINK_NATIVE_PROPERTY( TimeWaitPost );
  LINK_NATIVE_PROPERTY( bParallel );
  LINK_NATIVE_PROPERTY( bRandom );
  LINK_NATIVE_PROPERTY( RandomCount );
  LINK_NATIVE_PROPERTY( PostRandomNum );
  LINK_NATIVE_PROPERTY( SequenceNum );
  LINK_NATIVE_PROPERTY( NextPoint );
  LINK_NATIVE_PROPERTY( PrevPoint );
  LINK_NATIVE_PROPERTY( CurTime );
  LINK_NATIVE_PROPERTY( StartLoc );
  LINK_NATIVE_PROPERTY( StartRot );
  LINK_NATIVE_PROPERTY( StartFov );
  LINK_NATIVE_PROPERTY( EndLoc );
  LINK_NATIVE_PROPERTY( EndRot );
  LINK_NATIVE_PROPERTY( EndFov );
  LINK_NATIVE_PROPERTY( Player );
  LINK_NATIVE_PROPERTY( bTickReady );
  LINK_NATIVE_PROPERTY( bFirstRandom );
  LINK_NATIVE_PROPERTY( RandomRemain );
  LINK_NATIVE_PROPERTY( ContinuePoint );
END_PROPERTY_LINK()

