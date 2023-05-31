#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <string>
#include <iostream>

#include "LTexture.hh"
#include "LWindow.hh"

#define TOTAL_WINDOWS (3)

bool init(LWindow*);
bool loadMedia(LTexture*);
void closeSDL(LWindow*, SDL_Renderer**, LTexture*, int);

bool init(LWindow* window_ptr) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Init Error: " << SDL_GetError() << '\n';
		return false;
	}
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		std::cout << "Warning: Linear texture filtering not enabled\n";
	}

	if (!window_ptr->init()) {
		std::cout << "Window 0 creation error: " << SDL_GetError() << '\n';
		return false;
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cout << "Image init error: " << IMG_GetError() << '\n';
		return false;
	}
	return true;
}

void closeSDL(LWindow* window_ptr, LTexture* textures,
							int numTextures) {
	for (int i = 0; i < numTextures; i++) {
		textures[i].free();
	}
	
	for (int i = 0; i < TOTAL_WINDOWS; i++) {
		window_ptr[i].free();
	}

	SDL_Quit();
	IMG_Quit();
}
	
int main(int argc, char** argv) {
	LTexture texture;

  LWindow windows[3];
	if (!init(windows)) {
		return -1;
	}

	for (int i = 1; i < TOTAL_WINDOWS; i++) {
		windows[i].init();
	}

	bool quit = false;
	SDL_Event e;
	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			for (int i = 0; i < TOTAL_WINDOWS; i++) {
				windows[i].handleEvent(e);
			}

			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
					case SDLK_1:
						windows[0].focus();
						break;
					case SDLK_2:
						windows[1].focus();
						break;
					case SDLK_3:
						windows[2].focus();
						break;
				}
			}
		}

		// Render windows, also check if they are all closed
		bool allClosed = true;
		for (int i = 0; i < TOTAL_WINDOWS; i++) {
			windows[i].render();
			if (windows[i].isShown()) {
				allClosed = false;
			}
		}
		if (allClosed) {
			quit = true;
		}
	}
	closeSDL(windows, &texture, 1);
	return 0;
}
