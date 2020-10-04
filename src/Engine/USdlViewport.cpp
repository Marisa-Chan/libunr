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
 * USdlViewport.cpp - A viewport implemented in WinAPI
 *========================================================================
*/

#include "Engine/USdlClient.h"
#include "Engine/USdlViewport.h"

USdlViewport::USdlViewport():
	flags(SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN),
	window(nullptr)
{}

USdlViewport::~USdlViewport()
{
}

bool USdlViewport::Init(int InWidth, int InHeight)
{
	Super::Init(InWidth, InHeight);

	if (!GEngine->Render)
	{
		GLogf(LOG_WARN, "Global renderer null. Skipping viewport init.");
		return true;
	}

	GLogf(LOG_DEV, "Window Size %dx%d", Width, Height);

	window = SDL_CreateWindow("USdlViewport",
	                          SDL_WINDOWPOS_UNDEFINED,
	                          SDL_WINDOWPOS_UNDEFINED,
	                          Width, Height,
	                          flags);

	if (window == nullptr)
	{
		GLogf(LOG_ERR, "Failed to create SDL Window");
		return false;
	}

	if (!GEngine->Render->InitViewport(this))
	{
		GLogf(LOG_ERR, "Failed to initialize SDL viewport with renderer");
		SDL_DestroyWindow(window);
		return false;
	}

	return true;
}

bool USdlViewport::Exit()
{
	if (window != nullptr)
	{
		SDL_DestroyWindow(window);
		window = nullptr;
		return true;
	}

	return false;
}

void USdlViewport::Show()
{
	if (window != nullptr)
	{
		SDL_ShowWindow(window);
	}
}

void USdlViewport::Hide()
{
	if (window != nullptr)
	{
		SDL_HideWindow(window);
	}
}

bool USdlViewport::Resize(int width, int height)
{
	if (window == nullptr)
	{
		return false;
	}

	if (width <= 0)
	{
		width = 640;
	}

	if (height <= 0)
	{
		height = 480;
	}

	SDL_SetWindowSize(window, width, height);

	return true;
}

#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( USdlViewport );
