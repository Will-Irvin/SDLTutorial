#include <SDL2/SDL.h>

class LWindow {
	public:
		LWindow();
		bool init(); // Create window
		SDL_Renderer* createRenderer(); // Create renderer for window
		void handleEvent(SDL_Event&, SDL_Renderer* = NULL); // Handle window events
		void focus(); // Focus on this window
		void render(); // Render using internal renderer
		void free(); // Deallocate internals
		int getWidth();
		int getHeight();
		// Window Focii
		bool hasMouseFocus();
		bool hasKeyboardFocus();
		bool isMinimized();
		bool isShown();

	private:
		SDL_Window* mWindow; // Actual window
		SDL_Renderer* mRenderer; // Renderer for window
		Uint32 mWindowID;

		// Dimensions
		int mWidth;
		int mHeight;

		// Focii
		bool mMouseFocus;
		bool mKeyboardFocus;
		bool mFullScreen;
		bool mMinimized;
		bool mShown;
};
