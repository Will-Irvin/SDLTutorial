#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <sstream>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>

#include "LTexture.hh"
#include "Tile.hh"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

#define LEVEL_WIDTH (1280)
#define LEVEL_HEIGHT (960)

#define TILE_WIDTH (80)
#define TILE_HEIGHT (80)

#define TOTAL_TILES (192)
#define TOTAL_TILE_SPRITES (12)
#define TICKS_PER_FRAME (1000 / 60)

enum textureInd {
	DOTI,
	TILEI,
};

bool init(SDL_Window**, SDL_Renderer**);
bool loadMedia(LTexture*, SDL_Renderer*, Tile* tiles[]);
bool setTiles(Tile* tiles[]);
void closeSDL(SDL_Window**, SDL_Renderer**, LTexture*, int, Tile* tiles[]);

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

bool loadMedia(LTexture* textures, SDL_Rect* tileClips,
							 SDL_Renderer* renderer, Tile* tiles[]) {
	if (!textures[DOTI].loadFromFile("images/dot.bmp", renderer)) {
		return false;
	}

	if (!textures[TILEI].loadFromFile("images/tiles.png", renderer)) {
		return false;
	}

	if (!setTiles(tiles)) {
		std::cout << "Tile load error\n";
		return false;
	}

	// Initialize tile sprite clips
	tileClips[TILE_RED] = {0, 0, TILE_WIDTH, TILE_HEIGHT};
	tileClips[TILE_GREEN] = {0, 80, TILE_WIDTH, TILE_HEIGHT};
	tileClips[TILE_BLUE] = {0, 160, TILE_WIDTH, TILE_HEIGHT};
	tileClips[TILE_TOPLEFT] = {80, 0, TILE_WIDTH, TILE_HEIGHT};
	tileClips[TILE_LEFT] = {80, 80, TILE_WIDTH, TILE_HEIGHT};
	tileClips[TILE_BOTTOMLEFT] = {80, 160, TILE_WIDTH, TILE_HEIGHT};
	tileClips[TILE_TOP] = {160, 0, TILE_WIDTH, TILE_HEIGHT};
	tileClips[TILE_CENTER] = {160, 80, TILE_WIDTH, TILE_HEIGHT};
	tileClips[TILE_BOTTOM] = {160, 160, TILE_WIDTH, TILE_HEIGHT};
	tileClips[TILE_TOPRIGHT] = {240, 0, TILE_WIDTH, TILE_HEIGHT};
	tileClips[TILE_RIGHT] = {240, 80, TILE_WIDTH, TILE_HEIGHT};
	tileClips[TILE_BOTTOMRIGHT] = {240, 160, TILE_WIDTH, TILE_HEIGHT};
	return true;
}

bool setTiles(Tile* tiles[]) {
	int x = 0;
	int y = 0;

	std::ifstream map("files/lazy.map");
	if (map.fail()) {
		std::cout << "Unable to load file\n";
		return false;
	}
	for (int i = 0; i < TOTAL_TILES; i++) {
		int tileType = TILE_RED;
		map >> tileType;
		if (map.fail()) {
			std::cout << "Error reading from file\n";
			return false;
		}

		if (tileType >= TILE_RED && tileType < TOTAL_TILE_SPRITES) {
			tiles[i] = new Tile(x, y, (TileTypes) tileType);
		} else {
			std::cout << "Invalid Tile Type at " << i << '\n';
			return false;
		}

		// Adjust x and y to be at correct tile position
		x += TILE_WIDTH;
		if (x >= LEVEL_WIDTH) {
			x = 0;
			y += TILE_HEIGHT;
		}
	}
	map.close();
	return true;
}

void closeSDL(SDL_Window** window, SDL_Renderer** renderer,
							LTexture* textures, int numTextures, Tile* tiles[]) {
	for (int i = 0; i < numTextures; i++) {
		textures[i].free();
	}

	SDL_DestroyRenderer(*renderer);
	*renderer = NULL;
	SDL_DestroyWindow(*window);
	*window = NULL;

	for (int i = 0; i < TOTAL_TILES; i++) {
		delete tiles[i];
	}

	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
}

int main(int argc, char** argv) {
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	LTexture textures[2];
	Tile* tiles[TOTAL_TILES];
	SDL_Rect tileClips[TOTAL_TILE_SPRITES];

	if (!init(&window, &renderer)) {
		return -1;
	}
	if (!loadMedia(textures, tileClips, renderer, tiles)) {
		return -1;
	}

	SDL_Event e;
	bool quit = false;
	TileDot dot = TileDot();
	SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	while (!quit) {
		int startTime = SDL_GetTicks(); // Simple way to cap frame rate
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			dot.handleEvent(e);
		}
		dot.move(tiles);
		dot.setCamera(camera);

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);
	
		for (int i = 0; i < TOTAL_TILES; i++) {
			tiles[i]->render(renderer, camera, &textures[TILEI], tileClips);
		}

		dot.render(renderer, &textures[DOTI], camera);

		SDL_RenderPresent(renderer);

		// Simple frame cap
		int time = SDL_GetTicks() - startTime;
		if (time < 0) continue;

		int sleepTime = TICKS_PER_FRAME - time;
		if (sleepTime > 0) {
			SDL_Delay(sleepTime);
		}
	}

	closeSDL(&window, &renderer, textures, 2, tiles);
	return 0;
}
