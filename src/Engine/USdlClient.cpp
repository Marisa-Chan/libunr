#include "Engine/USdlClient.h"
#include "Engine/USdlViewport.h"

USdlClient::USdlClient():
	flags(SDL_INIT_VIDEO)
{}

USdlClient::~USdlClient()
{
}

bool USdlClient::Init()
{
	SDL_Init(flags);
	GLogf(LOG_DEV, "USdlClient::Init()");

	SetMouseCapture(true);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	return true;
}

bool USdlClient::Exit()
{
	SDL_QuitSubSystem(flags);

	bool result = true;

	for (int i = 0; i < Viewports.Size(); i++)
	{
		if (!Viewports[i]->Exit())
		{
			result = false;
		}
	}

	return true;
}

UViewport* USdlClient::OpenViewport(int width, int height)
{
	USdlViewport* viewport = new USdlViewport();

	if (!viewport->Init(width, height))
	{
		GLogf(LOG_ERR, "Failed to create SDL viewport");
		delete viewport;
		return nullptr;
	}

	Viewports.PushBack(viewport);
	CurrentViewport = viewport;

	uint32_t windowId = viewport->GetId();
	viewportsById[windowId] = viewport;

	GLogf(LOG_DEV, "Created SDL Viewport");

	return viewport;
}

bool USdlClient::CloseViewport(UViewport * viewport)
{
	auto sdlViewport = dynamic_cast<USdlViewport*>(viewport);
	if (sdlViewport == nullptr)
	{
		return false;
	}

	viewportsById.erase(sdlViewport->GetId());

    bool result = viewport->Exit();
	if (!RemoveViewport(viewport))
	{
		return false;
	}

	return result;
}

#define BEGIN_MAP_KEYS() if (false) {}

#define MAP_KEY(SdlkKey, IKKey) \
	else if (keycode == SdlkKey) \
	{ \
		return IKKey; \
	}

#define MAP_KEY_RANGE(SdlkMin, SdlkMax, IKBase) \
	else if (SdlkMin <= keycode && keycode <= SdlkMax) \
	{ \
		return static_cast<EInputKey>(IKBase + (keycode - SdlkMin)); \
	}

#define MAP_KEY_RANGE_LOOKUP(SdlkMin, SdlkMax, table) \
	else if (SdlkMin <= keycode && keycode <= SdlkMax) \
	{ \
		return table[keycode - SdlkMin]; \
	}

#define END_MAP_KEYS()

EInputKey keysQuoteToSlash[] = {
	IK_SingleQuote,
	IK_None, // (
	IK_None, // )
	IK_None, // *
	IK_None, // +
	IK_Minus,
	IK_Period,
	IK_Slash,
};

EInputKey keysBracketToBackquote[] = {
	IK_LeftBracket,
	IK_Backslash,
	IK_RightBracket,
	IK_None,
	IK_None,
	IK_Tilde,
};

EInputKey keysControls[] = {
	IK_LControl,
	IK_LShift,
	IK_Alt,
	IK_None,
	IK_RControl,
	IK_RShift,
	IK_Alt,
};

EInputKey keysSpecial1[] = {
	IK_PrintScrn,
	IK_ScrollLock,
	IK_Pause,
	IK_Insert,
	IK_Home,
	IK_PageUp,
	IK_Delete,
	IK_End,
	IK_PageDown,
	IK_Right,
	IK_Left,
	IK_Down,
	IK_Up
};

EInputKey USdlClient::mapKey(SDL_Keycode keycode)
{
	BEGIN_MAP_KEYS()
		MAP_KEY_RANGE(SDLK_a, SDLK_z, IK_A)
		MAP_KEY_RANGE(SDLK_0, SDLK_9, IK_0)
		MAP_KEY_RANGE(SDLK_KP_0, SDLK_KP_9, IK_NumPad0)
		MAP_KEY_RANGE_LOOKUP(SDLK_QUOTE, SDLK_SLASH, keysQuoteToSlash)
		MAP_KEY_RANGE_LOOKUP(SDLK_LEFTBRACKET, SDLK_BACKQUOTE, keysBracketToBackquote)
		MAP_KEY_RANGE_LOOKUP(SDLK_LCTRL, SDLK_RALT, keysControls)
		MAP_KEY_RANGE_LOOKUP(SDLK_PRINTSCREEN, SDLK_UP, keysSpecial1)
		MAP_KEY(SDLK_TAB, IK_Tab)
		MAP_KEY(SDLK_SPACE, IK_Space)
		MAP_KEY(SDLK_EQUALS, IK_Equals)
	END_MAP_KEYS()

	GLogf(LOG_ERR, "Could not map SDL keycode '%s' to an internal key", SDL_GetKeyName(keycode));
	return IK_None;
}

EInputKey USdlClient::mapMouseButton(int button)
{
	switch ( button )
	{
		case SDL_BUTTON_LEFT:
			return IK_LeftMouse;
		case SDL_BUTTON_RIGHT:
			return IK_RightMouse;
		case SDL_BUTTON_MIDDLE:
			return IK_MiddleMouse;
		default:
			break;
	}

	GLogf(LOG_WARN, "Unhandled mouse button %d", button);
	return IK_None;
}

void USdlClient::Tick(float dt)
{
	// XXX: Must be called in main thread
	SDL_Event event;
	int inputKey;
	EInputKey mouseButton;
	auto viewportIter = viewportsById.begin();
	USdlViewport* viewport = NULL;

	while (SDL_PollEvent(&event))
	{
		switch ( event.type )
		{
		case SDL_MOUSEMOTION:
			UClient::HandleMouseInput( -event.motion.xrel, -event.motion.yrel );
			break;

		case SDL_KEYDOWN:
			inputKey = mapKey( event.key.keysym.sym );
			if ( inputKey != IK_None )
				HandleInput( inputKey, true );
			break;

		case SDL_KEYUP:
			inputKey = mapKey( event.key.keysym.sym );
			if ( inputKey != IK_None )
				HandleInput( inputKey, false );
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouseButton = mapMouseButton( event.button.button );
			if ( mouseButton != IK_None )
				HandleInput( mouseButton, true );
			break;

		case SDL_MOUSEBUTTONUP:
			mouseButton = mapMouseButton( event.button.button );
			if ( mouseButton != IK_None )
				HandleInput( mouseButton, false );
			break;

		case SDL_WINDOWEVENT:
			viewportIter = viewportsById.find( event.window.windowID );
			if ( viewportIter == viewportsById.end() )
			{
				GLogf( LOG_ERR, "Unknown SDL Window ID %d", event.window.windowID );
				break;
			}
			viewport = viewportIter->second;
			if ( event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
			{
				viewport->Width = event.window.data1;
				viewport->Height = event.window.data2;
			}
			else if ( event.window.event == SDL_WINDOWEVENT_CLOSE )
			{
				CloseViewport( viewport );
				break;
			}
			break;
		case SDL_QUIT:
			GSystem->Exit( 0 );
			break;
		}
	}
}

void USdlClient::SetMouseCapture( bool capture )
{
	SDL_SetRelativeMouseMode(static_cast<SDL_bool>(capture));
}

#include "Core/UClass.h"
#include "Core/UPackage.h"
IMPLEMENT_NATIVE_CLASS( USdlClient );
