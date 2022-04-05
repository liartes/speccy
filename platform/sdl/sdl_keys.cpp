/*
Portable ZX-Spectrum emulator.
Copyright (C) 2001-2010 SMT, Dexus, Alone Coder, deathsoft, djdron, scor

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../platform.h"

#ifdef USE_SDL
#ifdef SDL_KEYS_COMMON

#include <SDL.h>
#include "../../tools/options.h"
#include "../../options_common.h"

#ifdef SDL_POCKETGO_KEYS
#define DINGOO_BUTTON_R             SDLK_BACKSPACE
#define DINGOO_BUTTON_L             SDLK_TAB
#endif	

namespace xPlatform
{

	static bool l_shift = false, r_shift = false, b_select = false, b_start = false;


static bool ProcessFuncKey(SDL_Event& e)
{
	if(e.key.keysym.mod)
		return false;
	switch(e.key.keysym.sym)
	{
	case SDLK_F2:
		{
			using namespace xOptions;
			eOptionB* o = eOptionB::Find("save state");
			SAFE_CALL(o)->Change();
		}
		return true;
	case SDLK_F3:
		{
			using namespace xOptions;
			eOptionB* o = eOptionB::Find("load state");
			SAFE_CALL(o)->Change();
		}
		return true;
	case SDLK_F5:
		Handler()->OnAction(A_TAPE_TOGGLE);
		return true;
	case SDLK_F7:
		{
			using namespace xOptions;
			eOptionB* o = eOptionB::Find("pause");
			SAFE_CALL(o)->Change();
		}
		return true;
	case SDLK_F12:
		Handler()->OnAction(A_RESET);
		return true;
	default:
		return false;
	}
}

static byte TranslateKey(SDLKey _key, dword& _flags)
{
	bool ui_focused = Handler()->VideoDataUI();
	switch(_key)
	{
	case SDLK_RSHIFT:	return 'c';
	case SDLK_RALT:		return 's';

#ifdef PLATFORM_TRIMUI

	case SDLK_RCTRL: // DINGOO_BUTTON_SELECT:
		b_select = _flags&KF_DOWN;
		if(b_select && b_start)
			OpQuit(true);
		return 'm';
	case SDLK_RETURN: // DINGOO_BUTTON_START:
		b_start = _flags&KF_DOWN;
		if(b_select && b_start)
			OpQuit(true);
		return 'k';
#else
#ifdef SDL_POCKETGO_KEYS

	case SDLK_ESCAPE: // DINGOO_BUTTON_SELECT:
		b_select = _flags&KF_DOWN;
		if(b_select && b_start)
			OpQuit(true);
		return 'm';
	case SDLK_RETURN: // DINGOO_BUTTON_START:
		b_start = _flags&KF_DOWN;
		if(b_select && b_start)
			OpQuit(true);
		return 'k';
#else		
	case SDLK_LSHIFT:	return 'c';
	case SDLK_LALT:		return 's';
	case SDLK_RETURN:	return 'e';
#endif	
#endif	
	case SDLK_BACKQUOTE: return 'p';
	// case SDLK_BACKSPACE:
	// 	_flags |= KF_SHIFT;
	// 	return '0';


	case SDLK_QUOTE:
		_flags |= KF_ALT;
		if(_flags&KF_SHIFT)
		{
			_flags &= ~KF_SHIFT;
			return 'P';
		}
		else
			return '7';
	case SDLK_COMMA:
		_flags |= KF_ALT;
		if(_flags&KF_SHIFT)
		{
			_flags &= ~KF_SHIFT;
			return 'R';
		}
		else
			return 'N';
	case SDLK_PERIOD:
		_flags |= KF_ALT;
		if(_flags&KF_SHIFT)
		{
			_flags &= ~KF_SHIFT;
			return 'T';
		}
		else
			return 'M';
	case SDLK_SEMICOLON:
		_flags |= KF_ALT;
		if(_flags&KF_SHIFT)
		{
			_flags &= ~KF_SHIFT;
			return 'Z';
		}
		else
			return 'O';
	case SDLK_SLASH:
		_flags |= KF_ALT;
		if(_flags&KF_SHIFT)
		{
			_flags &= ~KF_SHIFT;
			return 'C';
		}
		else
			return 'V';
	case SDLK_MINUS:
		_flags |= KF_ALT;
		if(_flags&KF_SHIFT)
		{
			_flags &= ~KF_SHIFT;
			return '0';
		}
		else
			return 'J';
	case SDLK_EQUALS:
		_flags |= KF_ALT;
		if(_flags&KF_SHIFT)
		{
			_flags &= ~KF_SHIFT;
			return 'K';
		}
		else
			return 'L';
	// case SDLK_TAB:
	// 	_flags |= KF_ALT;
	// 	_flags |= KF_SHIFT;
	// 	return 0;
	case SDLK_LEFT:		return 'l';
	case SDLK_RIGHT:	return 'r';
#ifdef PLATFORM_TRIMUI
	case SDLK_LALT:     return '1'; /*Y BUTTON*/
	case SDLK_LCTRL:	return 'f'; /*B BUTTON */
	case SDLK_LSHIFT:	return '2'; /*X BUTTON */
	case SDLK_SPACE:	return 'u'; /*A BUITTON*/

	//case SDLK_BACKSPACE:
	case DINGOO_BUTTON_R:
		//redefine R as save state
		l_shift = _flags&KF_DOWN;
		if(!ui_focused)
		{
            using namespace xOptions;
            eOptionB* o = eOptionB::Find("save state");
            SAFE_CALL(o)->Change();
        }
		break;

	//case SDLK_TAB:
	case DINGOO_BUTTON_L:
		//redefine L as load state
		r_shift = _flags&KF_DOWN;
		if(!ui_focused)
        {
            using namespace xOptions;
            eOptionB* o = eOptionB::Find("load state");
            SAFE_CALL(o)->Change();
        }
		break;

#else 
#ifdef SDL_POCKETGO_KEYS
	case SDLK_SPACE:    return '1'; /*Y BUTTON*/
	case SDLK_LCTRL:	return 'f'; /*B BUTTON */
	case SDLK_LSHIFT:	return '2'; /*X BUTTON */
	case SDLK_LALT:		return 'u'; /*A BUITTON*/

	//case SDLK_BACKSPACE:
	case DINGOO_BUTTON_R:
		//redefine R as save state
		l_shift = _flags&KF_DOWN;
		if(!ui_focused)
		{
            using namespace xOptions;
            eOptionB* o = eOptionB::Find("save state");
            SAFE_CALL(o)->Change();
        }
		break;

	//case SDLK_TAB:
	case DINGOO_BUTTON_L:
		//redefine L as load state
		r_shift = _flags&KF_DOWN;
		if(!ui_focused)
        {
            using namespace xOptions;
            eOptionB* o = eOptionB::Find("load state");
            SAFE_CALL(o)->Change();
        }
		break;

#else 
	case SDLK_LCTRL:	return 'f';
	case SDLK_BACKSPACE:
	 	_flags |= KF_SHIFT;
	 	return '0';

	case SDLK_TAB:
	 	_flags |= KF_ALT;
	 	_flags |= KF_SHIFT;
	 	return 0;
#endif
#endif
	case SDLK_UP:		return 'u';
	case SDLK_DOWN:		return 'd';
	case SDLK_INSERT:
#ifdef PLATFORM_TRIMUI
case SDLK_ESCAPE: return 'm'; /* Reset button*/ //OpQuit(true);
#else
#ifdef SDL_POCKETGO_KEYS
case SDLK_RCTRL: return 'm'; /* Reset button*/ //OpQuit(true);
#else
	case SDLK_RCTRL:
#endif
#endif
/*
	case SDLK_UP:		return 'u';
	case SDLK_DOWN:		return 'd';
	case SDLK_INSERT:
	case SDLK_RCTRL:	return 'm';
	case SDLK_LCTRL:	return 'f';
	*/
	default:
		break;
	}
	if(_key >= SDLK_0 && _key <= SDLK_9)
		return _key;
	if(_key >= SDLK_a && _key <= SDLK_z)
		return toupper(_key);
	if(_key == SDLK_SPACE)
		return _key;
	return 0;
}

void ProcessKey(SDL_Event& e)
{
	switch(e.type)
	{
	case SDL_KEYDOWN:
		{
			dword flags = KF_DOWN|OpJoyKeyFlags();
			if(e.key.keysym.mod&KMOD_ALT)
				flags |= KF_ALT;
			if(e.key.keysym.mod&KMOD_SHIFT)
				flags |= KF_SHIFT;
			byte key = TranslateKey(e.key.keysym.sym, flags);
			Handler()->OnKey(key, flags);
		}
		break;
	case SDL_KEYUP:
		if(!ProcessFuncKey(e))
		{
			dword flags = 0;
			if(e.key.keysym.mod&KMOD_ALT)
				flags |= KF_ALT;
			if(e.key.keysym.mod&KMOD_SHIFT)
				flags |= KF_SHIFT;
			byte key = TranslateKey(e.key.keysym.sym, flags);
			Handler()->OnKey(key, OpJoyKeyFlags());
		}
		break;
	default:
		break;
	}
}

}
//namespace xPlatform

#endif//SDL_KEYS_COMMON
#endif//USE_SDL
