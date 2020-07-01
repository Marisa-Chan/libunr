/*===========================================================================*\
|*  libunr - An open source Unreal Engine 1 implementation in library form   *|
|*  Copyright (C) 2018-2020  Adam W.E. Smith                                 *|
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
 * USdlViewport.h - A viewport implemented in SDL
 *========================================================================
*/

#pragma once

#include <SDL.h>
#include "Engine/UEngine.h"

class LIBUNR_API USdlViewport : public UViewport
{
	DECLARE_NATIVE_CLASS( USdlViewport, UViewport, CLASS_NoExport, SdlDrv )

	USdlViewport();

	bool Init( int InWidth = 0, int InHeight = 0 ) override;
	bool Exit() override;
	void Show() override;
	void Hide() override;
	bool Resize( int NewWidth, int NewHeight ) override;
	uint32_t GetId() const { return SDL_GetWindowID(window); }

	SDL_Window * getWindow() { return window; }

protected:
	uint32_t flags;
	SDL_Window * window;
};
