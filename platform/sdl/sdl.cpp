/*
Portable ZX-Spectrum emulator.
Copyright (C) 2001-2013 SMT, Dexus, Alone Coder, deathsoft, djdron, scor

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

#include <SDL.h>
#include "../../tools/options.h"
#include "../../options_common.h"
#include "../../tools/tick.h"
// libmmenu
#include <dlfcn.h>
#include <mmenu.h>

namespace xPlatform
{

static bool sdl_inited = false;
static SDL_Surface* screen = NULL;
static SDL_Surface* offscreen = NULL;
void* mmenu;
char* rom_path;
char SAVE_DIRECTORY[] = "/mnt/SDCARD/Roms/ZXSpectrum/.saves/";

void* getMmenu() {
	return mmenu;
}

void* setMmenu(void* toSet) {
	mmenu = toSet;
}
void setRomPath(char* name);

char* getRomPath();

bool InitVideo();
bool InitAudio();
void DoneVideo();
void DoneAudio();
void UpdateAudio();
void UpdateScreen();
void ProcessKey(SDL_Event& e);

#ifdef SDL_USE_JOYSTICK
void ProcessJoy(SDL_Event& e);
static SDL_Joystick* joystick = NULL;
#endif//SDL_USE_JOYSTICK

static bool Init()
{
#ifndef SDL_DEFAULT_FOLDER
#define SDL_DEFAULT_FOLDER "/"
#endif//SDL_DEFAULT_FOLDER
	OpLastFile(SDL_DEFAULT_FOLDER);
	Handler()->OnInit();

	Uint32 init_flags = SDL_INIT_VIDEO|SDL_INIT_AUDIO;
#ifdef SDL_USE_JOYSTICK
	init_flags |= SDL_INIT_JOYSTICK;
#endif//SDL_USE_JOYSTICK
	if(SDL_Init(init_flags) < 0)
        return false;

#ifdef SDL_USE_JOYSTICK
	SDL_JoystickEventState(SDL_ENABLE);
	joystick = SDL_JoystickOpen(0);
#endif//SDL_USE_JOYSTICK

    sdl_inited = true;
	SDL_ShowCursor(SDL_DISABLE);
	SDL_WM_SetCaption(Handler()->WindowCaption(), NULL);

	if(!InitVideo())
		return false;
	if(!InitAudio())
		return false;
	return true;
}

static void Done()
{
#ifdef SDL_USE_JOYSTICK
	if(joystick)
	{
		SDL_JoystickClose(joystick);
		joystick = NULL;
	}
#endif//SDL_USE_JOYSTICK
	DoneAudio();
	DoneVideo();
	if(sdl_inited)
		SDL_Quit();
	Handler()->OnDone();
}

static void Loop()
{
	eTick last_tick;
	last_tick.SetCurrent();
	bool quit = false;
	while(!quit)
	{
		SDL_Event e;
		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				ProcessKey(e);
				break;
#ifdef SDL_USE_JOYSTICK
			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP:
			case SDL_JOYAXISMOTION:
// #ifndef GCWZERO
// 				ProcessJoy(e);
// #endif//GCWZERO
#endif//SDL_USE_JOYSTICK
			default:
// #ifdef GCWZERO //invoke processjoy to stop A-stick continuing to report movemet when centred
// 				ProcessJoy(e);
// #endif//GCWZERO
				break;

			}
		}
		Handler()->OnLoop();
		if(!OpQuit()) {
			UpdateScreen();
			UpdateAudio();
			while(last_tick.Passed().Ms() < 15)
			{
				SDL_Delay(3);
			}
			last_tick.SetCurrent();
		}
		if(OpQuit())
			quit = true;
	}
}

}
//namespace xPlatform

int main(int argc, char* argv[])
{
	if(!xPlatform::Init())
	{
		xPlatform::Done();
		return -1;
	}
	if(argc > 1) {
		xPlatform::setRomPath(argv[1]);
		xPlatform::Handler()->OnOpenFile(argv[1]);
	}

	if(xPlatform::getMmenu() == NULL) {
		xPlatform::setMmenu(dlopen("libmmenu.so", RTLD_LAZY));

			    int resume_slot = -1;
			    ResumeSlot_t ResumeSlot = (ResumeSlot_t)dlsym(xPlatform::getMmenu(), "ResumeSlot");
			    if (ResumeSlot) resume_slot = ResumeSlot();

			    if (resume_slot!=-1) {
			    	using namespace xOptions;
			    	eOptionB* o = eOptionB::Find("load state");
			    	SAFE_CALL(o)->Change(resume_slot);
			    }
	}

	xPlatform::Loop();
	xPlatform::Done();
	return 0;
}

#endif//USE_SDL
