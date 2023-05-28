#include <SDL2/SDL.h>

#include "LTimer.hh"

// Constructor
LTimer::LTimer() {
	mStartTicks = 0;
	mPauseTicks = 0;

	mPaused = false;
	mStarted = false;
}

// Start the timer (unpaused, from scratch)
void LTimer::start() {
	mStarted = true;
	mPaused = false;

	mStartTicks = SDL_GetTicks();
	mPauseTicks = 0;
}

// Stop timer, reset variables
void LTimer::stop() {
	mStarted = false;
	mPaused = false;

	mStartTicks = 0;
	mPauseTicks = 0;
}

// Pause timer
void LTimer::pause() {
	if (mStarted && !mPaused) {
		mPaused = true;

		mPauseTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
	}
}

// Unpause a paused timer
void LTimer::unpause() {
	if (mStarted && mPaused) {
		mPaused = false;

		mStartTicks = SDL_GetTicks() - mPauseTicks;
		mPauseTicks = 0;
	}
}

// Get current elapsed ticks
Uint32 LTimer::getTicks() {
	if (mStarted) {
		if (mPaused) {
			return mPauseTicks;
		} else {
			return SDL_GetTicks() - mStartTicks;
		}
	}
	return 0;
}

// Getters
bool LTimer::isStarted() {
	return mStarted;
}

bool LTimer::isPaused() {
	return mPaused && mStarted;
}
