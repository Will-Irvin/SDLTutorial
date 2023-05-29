#include <SDL2/SDL.h>
#include <vector>

#include "LTexture.hh"

typedef struct Circle {
	int x;
	int y;
	int r;
} Circle;

class Dot {
	public:
		// Dimensions
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		// Velocity increment of dot
		static const int DOT_VEL = 10;

		Dot();
		Dot(int x, int y);
		void handleEvent(SDL_Event&);
		// Move with no collision
		void move();

		// Move with rectangles of collision
		void move(SDL_Rect&, bool checkCollision(SDL_Rect, SDL_Rect));

		// Move with pixel perfect collision
		void move(std::vector<SDL_Rect>&,
							bool checkCollision(std::vector<SDL_Rect>, std::vector<SDL_Rect>));
		
		// Move with circular collision
		void move(SDL_Rect&, Circle&);

		void render(SDL_Renderer*, LTexture*);

		std::vector<SDL_Rect>& getColliders();

		Circle& getCircularCollider();

	private:
		// Current position and velocity
		int mPosX, mPosY;
		int mVelX, mVelY;
		std::vector<SDL_Rect> mColliders; // Collision box
		Circle mCircleCollider;

		void shiftColliders();
};
