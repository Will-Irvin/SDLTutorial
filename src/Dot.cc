#include <SDL2/SDL.h>
#include <vector>

#include "Dot.hh"
#include "LTexture.hh"

#define LEVEL_WIDTH (1280)
#define LEVEL_HEIGHT (960)
#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

// Collision Check functions for circles
bool checkCollision(Circle&, Circle&);
bool checkCollision(Circle&, SDL_Rect&);
		
// Helper function to calculate distance between points
double distanceSquared(int, int, int, int);

// Constructor
Dot::Dot() {
	mPosX = 0;
	mPosY = 0;
	mVelX = 0;
	mVelY = 0;

	mColliders.resize(1);
	mColliders[0].w = DOT_WIDTH;
	mColliders[0].h = DOT_HEIGHT;
	mCircleCollider = {0, 0, 0};
}

Dot::Dot(int x, int y) {
	// For a circle these indicate the center rather than the top left
	mPosX = x;
	mPosY = y;

	mVelX = 0;
	mVelY = 0;

	mColliders.resize(11);

	// Initialize collision boxes width/height
	// Note: this level of detail is usually excessive
	// Does not need to be pixel perfect, could instead be rectangles that are
	// close enough
	mColliders[0].w = 6;
	mColliders[0].h = 1;

	mColliders[1].w = 10;
	mColliders[1].h = 1;

	mColliders[2].w = 14;
	mColliders[2].h = 1;

	mColliders[3].w = 16;
	mColliders[3].h = 2;

	mColliders[4].w = 18;
	mColliders[4].h = 2;

	mColliders[5].w = 20;
	mColliders[5].h = 6;

	mColliders[6].w = 18;
	mColliders[6].h = 2;

	mColliders[7].w = 16;
	mColliders[7].h = 2;

	mColliders[8].w = 14;
	mColliders[8].h = 1;


	mColliders[9].w = 10;
	mColliders[9].h = 1;

	mColliders[10].w = 6;
	mColliders[10].h = 1;

	mCircleCollider.r = DOT_WIDTH / 2;

	shiftColliders();
}

// Getters

int Dot::getPosX() {
	return mPosX;
}

int Dot::getPosY() {
	return mPosY;
}

std::vector<SDL_Rect>& Dot::getColliders() {
	return mColliders;
}

Circle& Dot::getCircularCollider() {
	return mCircleCollider;
}

void Dot::handleEvent(SDL_Event& e) {
	// Start moving dot
	// e.key.repeat makes sure we only look at the first key press
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
			case SDLK_UP: 
				mVelY -= DOT_VEL;
				break;
			case SDLK_DOWN: 
				mVelY += DOT_VEL;
				break;
			case SDLK_LEFT:
				mVelX -= DOT_VEL;
				break;
			case SDLK_RIGHT:
				mVelX += DOT_VEL;
				break;
		}
	} 
	// Stop moving dot after key is released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
			case SDLK_UP:
				mVelY += DOT_VEL;
				break;
			case SDLK_DOWN:
				mVelY -= DOT_VEL;
				break;
			case SDLK_LEFT:
				mVelX += DOT_VEL;
				break;
			case SDLK_RIGHT:
				mVelX -= DOT_VEL;
				break;
		}
	}
}

// Move freely
void Dot::move(bool level) {
	if (level) { // Stay in bounds of level
		mPosX += mVelX;

		if (mPosX < 0 || mPosX + DOT_WIDTH > LEVEL_WIDTH) {
			mPosX -= mVelX;
		}

		mPosY += mVelY;
		if (mPosY < 0 || mPosY + DOT_HEIGHT > LEVEL_HEIGHT) {
			mPosY -= mVelY;
		}
	} else { // Stay in bounds of screen
		mPosX += mVelX;
		// Too far, move back
		if (mPosX < 0 || mPosX + DOT_WIDTH > SCREEN_WIDTH) {
			mPosX -= mVelX;
		}

		mPosY += mVelY;
		if (mPosY < 0 || mPosY + DOT_HEIGHT > SCREEN_HEIGHT) {
			mPosY -= mVelY;
		}

		mColliders[0].x = mPosX;
		mColliders[0].y = mPosY;
	}
}

// Move while checking that there is no collision with the given rectangle
void Dot::move(SDL_Rect& wall, bool checkCollision(SDL_Rect, SDL_Rect)) {
	mPosX += mVelX;
	mColliders[0].x = mPosX;
	// Too far, move back
	if (mPosX < 0 || mPosX + DOT_WIDTH > SCREEN_WIDTH || checkCollision(mColliders[0], wall)) {
		mPosX -= mVelX;
		mColliders[0].x = mPosX;
	}

	mPosY += mVelY;
	mColliders[0].y = mPosY;
	if (mPosY < 0 || mPosY + DOT_HEIGHT > SCREEN_HEIGHT || checkCollision(mColliders[0], wall)) {
		mPosY -= mVelY;
		mColliders[0].y = mPosY;
	}
}

void Dot::move(std::vector<SDL_Rect>& otherColliders,
							 bool checkCollision(std::vector<SDL_Rect>, std::vector<SDL_Rect>)) {
	mPosX += mVelX;
	shiftColliders();
	if (mPosX < 0 || mPosX + DOT_WIDTH > SCREEN_WIDTH ||
			checkCollision(mColliders, otherColliders)) {
		mPosX -= mVelX;
	}

	mPosY += mVelY;
	shiftColliders();

	if (mPosY < 0 || mPosY + DOT_HEIGHT > SCREEN_HEIGHT ||
			checkCollision(mColliders, otherColliders)) {
		mPosY -= mVelY;
		shiftColliders();
	}
}

void Dot::move(SDL_Rect& square, Circle& circle) {
	mPosX += mVelX;
	shiftColliders();

	if (mPosX - mCircleCollider.r < 0 || mPosX + mCircleCollider.r > SCREEN_WIDTH ||
			checkCollision(mCircleCollider, square) || checkCollision(mCircleCollider, circle)) {
		mPosX -= mVelX;
	}

	mPosY += mVelY;
	shiftColliders();
	if (mPosY - mCircleCollider.r < 0 || mPosY + mCircleCollider.r > SCREEN_HEIGHT ||
			checkCollision(mCircleCollider, square) || checkCollision(mCircleCollider, circle)) {
		mPosY-= mVelY;
		shiftColliders();
	}
}

void Dot::render(SDL_Renderer* renderer, LTexture* texture_ptr) {
	texture_ptr->render(renderer, mPosX - mCircleCollider.r, mPosY - mCircleCollider.r);
}

void Dot::render(SDL_Renderer* renderer, LTexture* texture_ptr, int camX, int camY) {
	texture_ptr->render(renderer, mPosX - camX, mPosY - camY);
}

// Reset the position of each collider for the dot's new position
void Dot::shiftColliders() {
	int r = 0;

	for (size_t i = 0; i < mColliders.size(); i++) {
		// Set coordinates of each collider
		mColliders[i].x = mPosX + (DOT_WIDTH - mColliders[i].w) / 2;
		mColliders[i].y = mPosY + r;

		// Update current "height" of the collider
		r += mColliders[i].h;
	}

	mCircleCollider.x = mPosX;
	mCircleCollider.y = mPosY;
}

bool checkCollision(Circle& a, Circle& b) {
	int rSquared = a.r + b.r;
	rSquared *= rSquared;

	// Check if circles have collided
	if (distanceSquared(a.x, a.y, b.x, b.y) < rSquared) {
		return true;
	}

	return false;
}

bool checkCollision(Circle& a, SDL_Rect& b) {
	// Find closed point on the box to the circle
	int cX;
	int cY;

	// Figure out which side of the box the circle is closest to
	if (a.x < b.x) {
		cX = b.x;
	} else if (a.x > b.x + b.w) {
		cX = b.x + b.w;
	} else {
		cX = a.x;
	}

	if (a.y < b.y) {
		cY = b.y;
	} else if (a.y > b.y + b.h) {
		cY = b.y + b.h;
	} else {
		cY = a.y;
	}

	// Check if they collide
	if (distanceSquared(a.x, a.y, cX, cY) < a.r * a.r) {
		return true;
	}

	return false;
}

double distanceSquared(int x1, int y1, int x2, int y2) {
	int dX = x2 - x1;
	int dY = y2 - y1;
	return dX * dX + dY * dY;
}
