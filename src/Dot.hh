#include <SDL2/SDL.h>

#include "LTexture.hh"

class Dot {
	public:
		// Dimensions
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		// Velocity increment of dot
		static const int DOT_VEL = 10;

		Dot();
		void handleEvent(SDL_Event&);
		void move();
		void move(SDL_Rect&, bool checkCollision(SDL_Rect, SDL_Rect));
		void render(SDL_Renderer*, LTexture*);

	private:
		// Current position and velocity
		int mPosX, mPosY;
		int mVelX, mVelY;
		SDL_Rect mCollider; // Collision box
};
