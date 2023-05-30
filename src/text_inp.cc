#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <cstdio>
#include <cmath>
#include <iostream>

#include "LTexture.hh"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

bool init(SDL_Window**, SDL_Renderer**);
bool loadMedia(LTexture*, SDL_Renderer*, TTF_Font**);
void closeSDL(SDL_Window**, SDL_Renderer**, TTF_Font**, LTexture*, int);

// Initialize SDL, Window, Renderer, Image, and TTF
bool init(SDL_Window** window, SDL_Renderer** renderer) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Init Error: " << SDL_GetError() << '\n';
		return false;
	}
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		std::cout << "Warning: Linear texture filtering not enabled\n";
	}
	*window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
														 SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, 
														 SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (*window == NULL) {
		std::cout << "Window creation error: " << SDL_GetError() << '\n';
		return false;
	}

	// PRESENT_VSYNC flag has SDL uptate at the same time as the monitor during vertical refresh
	// Default is 60 fps
	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
	if (*renderer == NULL) {
		std::cout << "Renderer creation error: " << SDL_GetError() << '\n';
		return false;
	}

	SDL_SetRenderDrawColor(*renderer, 0xff, 0xff, 0xff, 0xff);

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cout << "SDL_Image init error: " << IMG_GetError() << '\n';
		return false;
	}

	if (TTF_Init() == -1) {
		std::cout << "TTF Init error: " << TTF_GetError() << '\n';
		return false;
	}
	return true;
}

bool loadMedia(LTexture* texture, SDL_Renderer* renderer, TTF_Font** font_ptr) {
	// First arg is font file, second is point size to render at
	*font_ptr = TTF_OpenFont("fonts/lazy.ttf", 28);
	if (*font_ptr == NULL) {
		std::cout << "Failed to load lazy font: " << TTF_GetError() << '\n';
		return false;
	}

	SDL_Color textColor = {0, 0, 0};

	*texture = LTexture();
	// Note make as few of these calls as possible
	if (!texture->loadFromRenderedText("Enter text", textColor, renderer, 
																		 *font_ptr))	{
		return false;
	}
	return true;
}

void closeSDL(SDL_Window** window, SDL_Renderer** renderer, TTF_Font** font_ptr,
							LTexture* textures, int numTextures) {
	for (int i = 0; i < numTextures; i++) {
		textures[i].free();
	}

	TTF_CloseFont(*font_ptr);
	*font_ptr = NULL;

	SDL_DestroyRenderer(*renderer);
	*renderer = NULL;
	SDL_DestroyWindow(*window);
	*window = NULL;

	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
}

int main(int argc, char** argv) {
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	TTF_Font* font = NULL;

	LTexture textures[2];

	if (!init(&window, &renderer)) {
		return -1;
	}
	if (!loadMedia(textures, renderer, &font)) {
		return -1;
	}

	SDL_Event e;
	bool quit = false;

	// Variables to manage the text
	SDL_Color textColor = {0, 0, 0, 0xff};

	std::string currText = "Some Text";
	textures[1].loadFromRenderedText(currText.c_str(), textColor, renderer, font);

	SDL_StartTextInput(); // Start listening for text input

	while (!quit) {
		bool renderText = false;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			} else if (e.type == SDL_KEYDOWN) {
				// Special cases
				// Note use binary & to check ctrl keys
				if (e.key.keysym.sym == SDLK_BACKSPACE && currText.length() > 0) {
					currText.pop_back();
					renderText = true;
				} else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() &
									 KMOD_CTRL) { // Check ctrl-c (copy)
					SDL_SetClipboardText(currText.c_str());
					renderText = true;
				} else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() &
									 KMOD_CTRL) { // Check ctrl-v (paste)
					currText = SDL_GetClipboardText();
					renderText = true;
				}
			} else if (e.type == SDL_TEXTINPUT) { 
				// These events simplify capitalization inputs, handle that work

				// Check that no special keys have been pressed
				if (!(SDL_GetModState() & KMOD_CTRL && 
						 (e.text.text[0] == 'c' || e.text.text[0] == 'C' ||
						  e.text.text[0] == 'v' || e.text.text[0] == 'V'))) {
					currText += e.text.text;
					renderText = true;
				}
			}	
		}

		if (renderText) {
			if (currText.length() > 0) {
				textures[1].loadFromRenderedText(currText.c_str(), textColor,
																					 renderer, font);
			} else { // Empty text
				textures[1].loadFromRenderedText(" ", textColor, renderer, font);
			}
		}

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);

		textures[0].render(renderer, (SCREEN_WIDTH - textures[0].getWidth()) / 2, 0);
		textures[1].render(renderer, (SCREEN_WIDTH - textures[1].getWidth()) / 2,
											 textures[0].getHeight());

		SDL_RenderPresent(renderer);
	}
	SDL_StopTextInput();

	closeSDL(&window, &renderer, &font, textures, 2);
	return 0;
}
