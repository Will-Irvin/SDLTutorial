#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdio>
#include <string>
#include <iostream>

#include "LTexture.hh"
#include "LWindow.hh"

bool init(LWindow*, SDL_Renderer **);
bool loadMedia(LTexture*, SDL_Renderer*);
void closeSDL(LWindow*, SDL_Renderer**, LTexture*, int);

bool init(LWindow* window_ptr, SDL_Renderer** renderer_ptr) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Init Error: " << SDL_GetError() << '\n';
		return false;
	}
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		std::cout << "Warning: Linear texture filtering not enabled\n";
	}

	if (!window_ptr->init()) {
		std::cout << "Window creation error: " << SDL_GetError() << '\n';
		return false;
	}

	*renderer_ptr = window_ptr->createRenderer();
	if (*renderer_ptr == NULL) {
		std::cout << "Renderer creation error: " << SDL_GetError() << '\n';
	}

	SDL_SetRenderDrawColor(*renderer_ptr, 0xff, 0xff, 0xff, 0xff);
	
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cout << "Image init error: " << IMG_GetError() << '\n';
		return false;
	}
	return true;
}

bool loadMedia(LTexture* texture, SDL_Renderer* renderer) {
	if (!texture->loadFromFile("images/window.png", renderer)) {
		return false;
	}

	return true;
}

void closeSDL(LWindow* window_ptr, SDL_Renderer** renderer, LTexture* textures,
							int numTextures) {
	for (int i = 0; i < numTextures; i++) {
		textures[i].free();
	}
	
	window_ptr->free();
	SDL_DestroyRenderer(*renderer);
	*renderer = NULL;

	SDL_Quit();
	IMG_Quit();
}
	
int main(int argc, char** argv) {
	LTexture texture;

  LWindow window;
	SDL_Renderer* renderer = NULL;
	if (!init(&window, &renderer)) {
		return -1;
	}

	if (!loadMedia(&texture, renderer)) {
		return -1;
	}

	bool quit = false;
	SDL_Event e;
	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			window.handleEvent(e, renderer, NULL, -1);
		}

		if (!window.isMinimized()) {
			SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
			SDL_RenderClear(renderer);

			texture.render(renderer, (window.getWidth() - texture.getWidth()) / 2,
										 (window.getHeight() - texture.getHeight()) / 2);

			SDL_RenderPresent(renderer);
		}
	}

	closeSDL(&window, &renderer, &texture, 1);
	return 0;
}
