#pragma once

#include "SDL.h"

class Surface {
private:
	SDL_Surface* surface = nullptr;

public:
	Surface() = default;

	Surface(SDL_Surface* sur)
	    : surface(sur) {}

	Surface(const Surface& other) = default;
	Surface& operator=(const Surface& other) = default;

	~Surface() {
		SDL_FreeSurface(surface);
	}

	SDL_Surface* get() {
		return surface;
	}
};