#ifndef LTEXTURE

#define LTEXTURE

#include <SDL2/SDL.h>
#include <string>

// Texture Wrapper Class
class LTexture {
	public:
		LTexture(); // Constructor

		~LTexture(); // Destructor

		bool loadFromFile(std::string, SDL_Renderer*); // Load image into texture

		void free(); // Deallocate texture

		void setColor(Uint8 red, Uint8 green, Uint8 blue); // Set color modulation

		void render(SDL_Renderer*, int, int, SDL_Rect* = NULL); // Render at given point

		// Dimension getters
		int getWidth();
		int getHeight();

	private:
		SDL_Texture* mTexture;
		int mWidth;
		int mHeight;
};

#endif
