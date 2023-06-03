#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sstream>
#include <iostream>

#include "LTexture.hh"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

class DataStream {
	public:
		DataStream();
		bool loadMedia();
		void free();
		void* getBuffer();
	
	private:
		SDL_Surface* mImages[4];
		int mCurrentImage;
		int mDelayFrames;
};

DataStream::DataStream() {
	for (int i = 0; i < 4; i++) {
		mImages[i] = NULL;
	}

	mCurrentImage = 0;
	mDelayFrames = 4;
}

bool DataStream::loadMedia() {
	for (int i = 0; i < 4; i++) {
		std::stringstream path;
		path << "images/foo_walk_" << i << ".png";

		SDL_Surface* loadedSurface = IMG_Load(path.str().c_str());
		if (loadedSurface == NULL) {
			std::cout << "Unable to load image (" << path.str() << "): " << IMG_GetError() << '\n';
			return false;
		}
		mImages[i] = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_RGBA8888, 0);
		SDL_FreeSurface(loadedSurface);
	}
	return true;
}

void DataStream::free() {
	for (int i = 0; i < 4; i++) {
		SDL_FreeSurface(mImages[i]);
		mImages[i] = NULL;
	}
}

void* DataStream::getBuffer() {
	--mDelayFrames;
	if (mDelayFrames == 0) {
		++mCurrentImage;
		mDelayFrames = 4;
	}
	if (mCurrentImage == 4) {
		mCurrentImage = 0;
	}
	return mImages[mCurrentImage]->pixels;
}

bool init(SDL_Window** window, SDL_Renderer** renderer) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Could not initialize SDL " << SDL_GetError() << '\n';
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
	return true;
}

bool loadMedia(LTexture* texture, DataStream* dataStream, SDL_Renderer* renderer) {
	if (!texture->createBlank(64, 205, renderer)) {
		return false;
	}
	if (!dataStream->loadMedia()) {
		return false;
	}
	return true;
}

void closeSDL(SDL_Window** window_ptr, SDL_Renderer** renderer, DataStream* dataStream,
							LTexture* texture) {
	dataStream->free();
	texture->free();
	SDL_DestroyWindow(*window_ptr);
	*window_ptr = NULL;
	SDL_DestroyRenderer(*renderer);
	*renderer = NULL;

	SDL_Quit();
	IMG_Quit();
}
	
int main(int argc, char** argv) {
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	LTexture texture;
	DataStream dataStream;

	if (!init(&window, &renderer)) {
		return false;
	}

	if (!loadMedia(&texture, &dataStream, renderer)) {
		return false;
	}

	bool quit = false;
	SDL_Event e;
	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}
		
		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);

		// Copy frame in buffer
		texture.lockTexture();
		texture.copyRawPixels32(dataStream.getBuffer());
		texture.unlockTexture();

		texture.render(renderer, (SCREEN_WIDTH - texture.getWidth()) / 2,
									 (SCREEN_HEIGHT - texture.getHeight()) / 2);
		
		SDL_RenderPresent(renderer);
	}

	closeSDL(&window, &renderer, &dataStream, &texture);
	return 0;
}
