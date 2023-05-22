#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cstdio>
#include <iostream>

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

// Texture Wrapper Class
class LTexture {
	public:
		LTexture(); // Constructor

		~LTexture(); // Destructor

		bool loadFromFile(std::string, SDL_Renderer*); // Load image into texture

		void free(); // Deallocate texture

		void render(int, int, SDL_Renderer*); // Render at given point

		// Dimension getters
		int getWidth();
		int getHeight();

	private:
		SDL_Texture* mTexture;
		int mWidth;
		int mHeight;
};

LTexture::LTexture() {
	mTexture = NULL;
	mWidth = -1;
	mHeight = -1;
}

LTexture::~LTexture() {
	free();
}

/**
 * Load image from specified path into this texture wrapper object
 * Set the appropriate attributes including color keying and height/width of
 * image
 */
bool LTexture::loadFromFile(std::string path, SDL_Renderer* renderer) {
	free(); // Remove previous texture (only one at a time)

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		std::cout << "Image load error (Path: " << path << "): " << SDL_GetError() << '\n';
		return false;
	}

	// Color key image
	// SetColorKey(surface, bool enable color key, pixel to color with)
	// This maps cyan with max blue and green
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xff, 0xff));

	newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	if (newTexture == NULL) {
		std::cout << "Texture conversion error: " << SDL_GetError() << '\n';
		return false;
	}
	// Get image dimensions from the surface
	mWidth = loadedSurface->w;
	mHeight = loadedSurface->h;

	SDL_FreeSurface(loadedSurface);
	mTexture = newTexture;
	return true;
}

// Free any memory associated with the texture if it exists
void LTexture::free() {
	if (mTexture != NULL) {
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = -1;
		mHeight = -1;
	}
}

// Set rectangle space and render
void LTexture::render(int x, int y, SDL_Renderer* renderer) {
	SDL_Rect rect = {x, y, mWidth, mHeight};
	SDL_RenderCopy(renderer, mTexture, NULL, &rect);
}

// Dimension getters
int LTexture::getWidth() {
	return mWidth;
}

int LTexture::getHeight() {
	return mHeight;
}

enum textures {
	FOO, BACKGROUND, TOTAL
};

// Function declarations
bool init(SDL_Window**, SDL_Renderer**);
bool loadMedia(LTexture*, SDL_Renderer*);
void closeSDL(SDL_Window**, SDL_Renderer**, LTexture*, int);

// Initialize SDL, Window, Renderer, and Image
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

// Load images into textures array
bool loadMedia(LTexture* textures, SDL_Renderer* renderer) {
	textures[FOO] = LTexture();
	textures[BACKGROUND] = LTexture();

	if (!textures[FOO].loadFromFile("images/foo.png", renderer)) {
		return false;
	}

	if (!textures[BACKGROUND].loadFromFile("images/background.png", renderer)) {
		return false;
	}
	return true;
}

// Free texture memory and quit SDL and imgs
void closeSDL(SDL_Window** window, SDL_Renderer** renderer, LTexture* textures, int numTextures) {
	for (int i = 0; i < numTextures; i++) {
		textures[i].free();
	}

	SDL_DestroyRenderer(*renderer);
	*renderer = NULL;
	SDL_DestroyWindow(*window);
	*window = NULL;

	SDL_Quit();
	IMG_Quit();
}

int main(int argc, char** argv) {
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	LTexture textures[TOTAL];
	if (!init(&window, &renderer)) {
		return -1;
	}
	if (!loadMedia(textures, renderer)) {
		return -1;
	}

	SDL_Event e;
	bool quit = false;
	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(renderer);

		// Render background texture
		textures[BACKGROUND].render(0, 0, renderer);

		// Render main image
		textures[FOO].render(240, 190, renderer);

		SDL_RenderPresent(renderer);
	}

	closeSDL(&window, &renderer, textures, TOTAL);
	return 0;
}

