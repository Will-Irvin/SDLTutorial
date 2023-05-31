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
#define TOTAL_DATA (10)

bool init(SDL_Window**, SDL_Renderer**);
bool loadMedia(LTexture*, SDL_Renderer*, TTF_Font**, Sint32*);
void closeSDL(SDL_Window**, SDL_Renderer**, TTF_Font**, Sint32*, LTexture*, int);

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

bool loadMedia(LTexture* textures, SDL_Renderer* renderer, TTF_Font** font_ptr,
							 Sint32* data) {
	*font_ptr = TTF_OpenFont("fonts/lazy.ttf", 28);
	if (*font_ptr == NULL) {
		std::cout << "Failed to load lazy font: " << TTF_GetError() << '\n';
		return false;
	}

	SDL_Color textColor = {0, 0, 0, 0xff};

	*textures = LTexture();
	if (!textures->loadFromRenderedText("Enter Data:", textColor, renderer, *font_ptr))	{
		return false;
	}

	// Open file for reading binary
	SDL_RWops* file = SDL_RWFromFile("files/nums.bin", "r+b");
	if (file == NULL) { // File couldn't be open
		std::cout << "Error Opening File: " << SDL_GetError() << '\n';

		// Create file to write to it
		file = SDL_RWFromFile("files/nums.bin", "w+b");
		if (file != NULL) {
			std::cout << "New file created\n";
			for (int i = 0; i < TOTAL_DATA; i++) {
				data[i] = 0;
				// Read write similar to regular binary writing but for SDL Files
				SDL_RWwrite(file, &data[i], sizeof(Sint32), 1);
			}
			SDL_RWclose(file);
		} else {
			std::cout << "Unable to create file " << SDL_GetError() << '\n';
			return false;
		}
	} else { // File initialized successfully
		for (int i = 0; i < TOTAL_DATA; i++) {
			SDL_RWread(file, &data[i], sizeof(Sint32), 1);
		}
		SDL_RWclose(file);
	}

	// Initialize data textures
	SDL_Color highlightColor = {0xff, 0, 0, 0xff};

	// First one should be highlighted, rest are normal
	textures[1].loadFromRenderedText(std::to_string(data[0]), highlightColor,
																	 renderer, *font_ptr);
	for (int i = 2; i < TOTAL_DATA + 1; i++) {
		textures[i].loadFromRenderedText(std::to_string(data[i - 1]), textColor,
																		 renderer, *font_ptr);
	}
	return true;
}

void closeSDL(SDL_Window** window, SDL_Renderer** renderer, TTF_Font** font_ptr,
							Sint32* data, LTexture* textures, int numTextures) {
	// Write new data to file
	SDL_RWops* file = SDL_RWFromFile("files/nums.bin", "w+b");
	if (file != NULL) {
		for (int i = 0; i < TOTAL_DATA; i++) {
			SDL_RWwrite(file, &data[i], sizeof(Sint32), 1);
		}
		SDL_RWclose(file);
	} else {
		std::cout << "Unable to save to file: " << SDL_GetError() << '\n';
	}

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
	Sint32 data[TOTAL_DATA];

	LTexture textures[TOTAL_DATA + 1];

	if (!init(&window, &renderer)) {
		return -1;
	}
	if (!loadMedia(textures, renderer, &font, data)) {
		return -1;
	}

	SDL_Event e;
	bool quit = false;

	SDL_Color highlightColor = {0xff, 0, 0, 0xff};
	SDL_Color textColor = {0, 0, 0, 0xff};

	// Current data texture index
	int currentData = 1;

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			} else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
					case SDLK_UP:
						// Move back up data list
						textures[currentData].loadFromRenderedText(std::to_string(data[currentData - 1]),
																											 textColor, renderer, font);
						currentData--;
						if (currentData < 1) {
							currentData = TOTAL_DATA;
						}

						textures[currentData].loadFromRenderedText(std::to_string(data[currentData - 1]),
																											 highlightColor, renderer, font);
						break;
					case SDLK_DOWN:
						// Move down the data list
						textures[currentData].loadFromRenderedText(std::to_string(data[currentData - 1]),
																											 textColor, renderer, font);
						currentData++;
						if (currentData > TOTAL_DATA) {
							currentData = 1;
						}
						textures[currentData].loadFromRenderedText(std::to_string(data[currentData - 1]),
																											 highlightColor, renderer, font);
						break;
					case SDLK_LEFT:
						// Decrement current value
						data[currentData - 1]--;
						textures[currentData].loadFromRenderedText(std::to_string(data[currentData - 1]),
																											 highlightColor, renderer, font);
						break;
					case SDLK_RIGHT:
						// Increment current value
						data[currentData - 1]++;
						textures[currentData].loadFromRenderedText(std::to_string(data[currentData - 1]),
																											 highlightColor, renderer, font);
						break;
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);

		// Render text
		textures[0].render(renderer, (SCREEN_WIDTH - textures[0].getWidth()) / 2, 0);
		for (int i = 1; i < TOTAL_DATA + 1; i++) {
			textures[i].render(renderer, (SCREEN_WIDTH - textures[i].getWidth()) / 2,
												 textures[0].getHeight() + i * textures[1].getHeight());
		}

		SDL_RenderPresent(renderer);
	}

	closeSDL(&window, &renderer, &font, data, textures, TOTAL_DATA + 1);
	return 0;
}
