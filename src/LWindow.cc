#include <SDL2/SDL.h>
#include <string>
#include <sstream>
#include <iostream>

#include "LWindow.hh"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)

LWindow::LWindow() {
	mWindow = NULL;
	mWidth = 0;
	mHeight = 0;
	mMouseFocus = false;
	mKeyboardFocus = false;
	mFullScreen = false;
	mMinimized = false;
}

// Create the window and set the appropriate variables
bool LWindow::init() {
	mWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
														 SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
														 SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (mWindow == NULL) {
		std::cout << "Window could not be created: " << SDL_GetError() << '\n';
		return false;
	}
	mRenderer = createRenderer();
	if (mRenderer == NULL) {
		std::cout << "Renderer could not be created: " << SDL_GetError() << '\n';
		SDL_DestroyWindow(mWindow);
		return false;
	}

	mWidth = SCREEN_WIDTH;
	mHeight = SCREEN_HEIGHT;
	mMouseFocus = true;
	mKeyboardFocus = true;

	SDL_SetRenderDrawColor(mRenderer, 0xff, 0xff, 0xff, 0xff);
	mWindowID = SDL_GetWindowID(mWindow); // Grab window ID from SDL
	mWindowDisplayID = SDL_GetWindowDisplayIndex(mWindow);
	mShown = true;

	return true;
}

// Create and return a renderer that can be used to operate on the window
// Can be used when rendering is done outside of the class
SDL_Renderer* LWindow::createRenderer() {
	return SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
}

// Free Memory
void LWindow::free() {
	if (mWindow != NULL) {
		SDL_DestroyWindow(mWindow);
	}
	if (mRenderer != NULL) {
		SDL_DestroyRenderer(mRenderer);
	}

	mMouseFocus = false;
	mKeyboardFocus = false;
	mWidth = 0;
	mHeight = 0;
}

void LWindow::handleEvent(SDL_Event& e, SDL_Renderer* renderer,
													SDL_Rect* displayBounds, Uint32 totalDisplays) {
	bool updateCaption = false; // Check if we need to update caption
	// Check that it's a window event and that it's referring to this window
	if (e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowID) {
		// Caption will keep track of mouse and keyboard focus
		switch (e.window.event) {
			// Window possibly moved to a different display
			case SDL_WINDOWEVENT_MOVED:
				mWindowDisplayID = SDL_GetWindowDisplayIndex(mWindow);
				updateCaption = true;
				break;
			// Window shown/hidden
			case SDL_WINDOWEVENT_SHOWN:
				mShown = true;
				break;
			case SDL_WINDOWEVENT_HIDDEN:
				mShown = false;
				break;
			// Dimensions changes
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				mWidth = e.window.data1;
				mHeight = e.window.data2;
				if (renderer == NULL) {
					SDL_RenderPresent(mRenderer);
				} else {
					SDL_RenderPresent(renderer);
				}
				break;
			// Repaint when window is exposed (was obscured, now it isn't)
			case SDL_WINDOWEVENT_EXPOSED:
				if (renderer == NULL) {
					SDL_RenderPresent(mRenderer);
				} else {
					SDL_RenderPresent(renderer);
				}
				break;
			// Mouse moves in/out of window
			case SDL_WINDOWEVENT_ENTER:
				mMouseFocus = true;
				updateCaption = true;
				break;
			case SDL_WINDOWEVENT_LEAVE:
				mMouseFocus = false;
				updateCaption = true;
				break;
			// Click in/out of the window
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				mKeyboardFocus = true;
				updateCaption = true;
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				mKeyboardFocus = false;
				updateCaption = true;
			case SDL_WINDOWEVENT_MINIMIZED:
				mMinimized = true;
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
				mMinimized = false;
				break;
			case SDL_WINDOWEVENT_RESTORED:
				mMinimized = false;
				break;
			// Hide window on close since we may not quit
			case SDL_WINDOWEVENT_CLOSE:
				SDL_HideWindow(mWindow);
				break;
		}
		if (updateCaption) { // Update caption if needed
			std::stringstream caption;
			caption << "SDL Tutorial - ID: " << mWindowID << " MouseFocus: " <<
								 (mMouseFocus ? "On" : "Off") << " KeyboardFocus: " <<
								 (mKeyboardFocus ? "On" : "Off");
			SDL_SetWindowTitle(mWindow, caption.str().c_str());
		}
	}
	// Full screen is done w/ return key
	else if (e.type == SDL_KEYDOWN) {
		bool switchDisplay = false;
		switch (e.key.keysym.sym) {
			case SDLK_RETURN:
				if (mFullScreen) {
					SDL_SetWindowFullscreen(mWindow, SDL_FALSE);
					mFullScreen = false;
				} else {
					SDL_SetWindowFullscreen(mWindow, SDL_TRUE);
					mFullScreen = true;
					mMinimized = false;
				}
				break;
			case SDLK_UP:
				if (totalDisplays > 0) {
					mWindowDisplayID++;
					switchDisplay = true;
				}
				break;
			case SDLK_DOWN:
				if (totalDisplays > 0) {
					if (mWindowDisplayID == 0) {
						mWindowDisplayID = totalDisplays - 1;
					} else {
						mWindowDisplayID--;
					}
					switchDisplay = true;
				}
				break;
		}

		if (switchDisplay) {
			// Bounding index
			if (mWindowDisplayID >= totalDisplays) {
				mWindowDisplayID = 0;
			}

			// Move window to center of the new display
			SDL_SetWindowPosition(mWindow,
													  displayBounds[mWindowDisplayID].x + (displayBounds[mWindowDisplayID].w - mWidth) / 2,
														displayBounds[mWindowDisplayID].y + (displayBounds[mWindowDisplayID].h - mHeight) / 2);
		}
	}
	if (updateCaption) { // Update caption if needed
		std::stringstream caption;
		caption << "SDL Tutorial - ID: " << mWindowID << " Display: " << mWindowDisplayID <<
							 " MouseFocus: " <<(mMouseFocus ? "On" : "Off") << " KeyboardFocus: " <<
							 (mKeyboardFocus ? "On" : "Off");
		SDL_SetWindowTitle(mWindow, caption.str().c_str());
	}

}

// Bring a window back into focus
void LWindow::focus() {
	if (!mShown) {
		SDL_ShowWindow(mWindow);
	}
	// Move window forward
	SDL_RaiseWindow(mWindow);
}

void LWindow::render() {
	if (!mMinimized) {
		SDL_SetRenderDrawColor(mRenderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderClear(mRenderer);
		SDL_RenderPresent(mRenderer);
	}
}

// Getters
int LWindow::getWidth() {
	return mWidth;
}

int LWindow::getHeight() {
	return mHeight;
}

bool LWindow::hasMouseFocus() {
	return mMouseFocus;
}

bool LWindow::hasKeyboardFocus() {
	return mKeyboardFocus;
}

bool LWindow::isMinimized() {
	return mMinimized;
}

bool LWindow::isShown() {
	return mShown;
}
