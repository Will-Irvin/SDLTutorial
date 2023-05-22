#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "LTexture.hh"

#include <string>
#include <cstdio>
#include <iostream>

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

/**
 * Multiplies a color value onto the the values that are currently displayed
 * For example, if you give 128 (half of 255) if will half the current r, g,
 * or b value for that color
 */
void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

/**
 * Set rectangle space and render
 * If clip is specified use those dimensions instead of the default
 */
void LTexture::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL) {
	SDL_Rect rect = {x, y, mWidth, mHeight};
	if (clip != NULL) {
		rect.w = clip->w;
		rect.h = clip->h;
	}
	// Use clip rectangle as source rectangle to get particular portion of the image
	SDL_RenderCopy(renderer, mTexture, clip, &rect);
}

// Dimension getters
int LTexture::getWidth() {
	return mWidth;
}

int LTexture::getHeight() {
	return mHeight;
}
