#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <cstdio>
#include <iostream>

#include "LTexture.hh"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

// Function declarations
bool init(SDL_Window**, SDL_Renderer**);
bool loadMedia(LTexture*, SDL_Renderer*, Mix_Music**, Mix_Chunk**);
void closeSDL(SDL_Window**, SDL_Renderer**, Mix_Music**, LTexture*, int, Mix_Chunk**, int);

enum sound_effects {
	SCRATCH,
	HIGH,
	MEDIUM,
	LOW,
	TOTAL
};

// Initialize SDL, Window, Renderer, Image, and Audio
bool init(SDL_Window** window, SDL_Renderer** renderer) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
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

	// Initialize audio
	// openAudio(frequency, sample format, # of channels (2 for stereo), sample size)
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		// Error occurred
		std::cout << "SDL_Mixer init error: " << Mix_GetError() << '\n';
		return false;
	}

	return true;
}

// Load image into texture object
bool loadMedia(LTexture* texture, SDL_Renderer* renderer, Mix_Music** music,
							 Mix_Chunk** effects) {
	*texture = LTexture();

	if (!texture->loadFromFile("images/prompt.png", renderer)) {
		return false;
	}

	// Load music (just takes filepath)
	*music = Mix_LoadMUS("sounds/beat.wav");
	if (*music == NULL) {
		std::cout << "Error loading music: beat.wav " << Mix_GetError() << '\n';
		return false;
	}

	// Load sound effects (also just takes filepath)
	effects[SCRATCH] = Mix_LoadWAV("sounds/scratch.wav");
	if (effects[SCRATCH] == NULL) {
		std::cout << "Error loading sound effect: scratch.wav " << Mix_GetError() << '\n';
		return false;
	}
	effects[HIGH] = Mix_LoadWAV("sounds/high.wav");
	if (effects[HIGH] == NULL) {
		std::cout << "Error loading sound effect: high.wav " << Mix_GetError() << '\n';
		return false;
	}

	effects[MEDIUM] = Mix_LoadWAV("sounds/medium.wav");
	if (effects[MEDIUM] == NULL) {
		std::cout << "Error loading sound effect: medium.wav " << Mix_GetError() << '\n';
		return false;
	}

	effects[LOW] = Mix_LoadWAV("sounds/low.wav");
	if (effects[LOW] == NULL) {
		std::cout << "Error loading sound effect: low.wav " << Mix_GetError() << '\n';
		return false;
	}

	return true;
}

// Free texture memory and quit SDL and imgs
void closeSDL(SDL_Window** window, SDL_Renderer** renderer, Mix_Music** music,
							LTexture* textures, int numTextures,
							Mix_Chunk** effects, int numEffects) {
	for (int i = 0; i < numTextures; i++) {
		textures[i].free();
	}

	SDL_DestroyRenderer(*renderer);
	*renderer = NULL;
	SDL_DestroyWindow(*window);
	*window = NULL;

	for (int i = 0; i < numEffects; i++) {
		Mix_FreeChunk(effects[i]);
		effects[i] = NULL;
	}

	Mix_FreeMusic(*music);
	*music = NULL;

	SDL_Quit();
	IMG_Quit();
	Mix_Quit(); // Quit SDL_Mixer
}

int main(int argc, char** argv) {
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	// Mixer variables/data structures

	// Music
	Mix_Music* music = NULL;

	// Sound effects
	Mix_Chunk* effects[TOTAL];

	LTexture texture;

	if (!init(&window, &renderer)) {
		return -1;
	}
	if (!loadMedia(&texture, renderer, &music, effects)) {
		return -1;
	}

	SDL_Event e;
	bool quit = false;

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			} else if (e.type == SDL_KEYDOWN) {
				// Mix_PlayChannel(channel (-1 for any), effect, # of repeats (0 for once)
				switch (e.key.keysym.sym) {
					// Sound effects
					case SDLK_1:
						Mix_PlayChannel(-1, effects[HIGH], 0);
						break;
					case SDLK_2:
						Mix_PlayChannel(-1, effects[MEDIUM], 0);
						break;
					case SDLK_3:
						Mix_PlayChannel(-1, effects[HIGH], 0);
						break;
					case SDLK_4:
						Mix_PlayChannel(-1, effects[SCRATCH], 0);
						break;
					// Music
					case SDLK_9:
						if (!Mix_PlayingMusic()) { // Check if music is currently playing
							Mix_PlayMusic(music, -1);
							// Function is music to play, then num repeats
							// Negative one means infinite repeat
						} else {
							if (Mix_PausedMusic()) { // Check if music paused
								Mix_ResumeMusic();
							} else {
								Mix_PauseMusic();
							}
						}
						break;
					case SDLK_0:
						Mix_HaltMusic(); // Stop music
						break;
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);

		texture.render(renderer, 0, 0);
		SDL_RenderPresent(renderer);
	}

	closeSDL(&window, &renderer, &music, &texture, 1, effects, TOTAL);
	return 0;
}

