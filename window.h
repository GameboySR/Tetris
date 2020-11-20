#pragma once
#include "SDL.h"
#include "cassert"
#include "surface.h"
#include <string>

class Window {
private:
	SDL_Window* window = nullptr;
	Surface surface;
	std::string name {};
	int width {};
	int height {};

public:
	Window()
	    : name("SDL2 Window")
	    , width(640)
	    , height(480) {
		assert(SDL_Init(SDL_INIT_VIDEO) >= 0);
		window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		assert(window);
		surface = SDL_GetWindowSurface(window);
	}

	Window(const Window& other) = default;

	Window& operator=(const Window& other) = default;

	~Window() {
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void setSurface(Surface& new_surface) {

		surface = Surface(new_surface);
		SDL_UpdateWindowSurface(window);
	}

	SDL_Window* getWindow() {
		return window;
	}

	SDL_Surface* getSurface() {
		return surface.get();
	}

	int getWidth() {
		return width;
	}

	int getHeight() {
		return height;
	}
};