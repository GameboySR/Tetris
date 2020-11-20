#pragma once
#include "cstring"
#include "SDL.h"
#include "cassert"
#include <SDL_ttf.h>
#include <stdexcept>
#include <string>

//header for wrapper classes for multiple SDL constructs, pretty straight-forward

//class that initialized and quits TTF
class TTF {
public:
	TTF() {
		if (TTF_Init() < 0) {
			throw std::runtime_error("TTF not initialized.");
		}
	}

	TTF(const TTF&) = delete;
	TTF& operator=(const TTF&) = delete;
	~TTF() {
		TTF_Quit();
	}
};

class Font {
private:
	TTF_Font* font;

public:
	Font(const std::string& font_name)
	    : font(TTF_OpenFont(font_name.c_str(), 24)) {}

	Font(const Font&) = delete;
	Font& operator=(const Font&) = delete;
	~Font() {
		TTF_CloseFont(font);
	}

	TTF_Font* getFontPointer() const {
		return font;
	}
};

//class that initialized and quits SDL
class SDL {
public:
	SDL() {
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			throw std::runtime_error("SDL not initialized.");
		}
	}

	SDL(const SDL&) = delete;
	SDL& operator=(const SDL&) = delete;

	~SDL() {
		SDL_Quit();
	}
};

class Surface {
private:
	SDL_Surface* surface;

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

class Texture {
private:
	SDL_Texture* texture;

public:
	Texture(SDL_Renderer* renderer, Surface& surface)
	    : texture(SDL_CreateTextureFromSurface(renderer, surface.get())) {}

	Texture() = delete;

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	~Texture() {
		if (texture) {
			SDL_DestroyTexture(texture);
		}
	}

	SDL_Texture* getTexture() {
		return texture;
	}
};

class WindowRenderer {
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	int width {};
	int height {};

public:
	WindowRenderer()
	    : width(320)
	    , height(640) {

		SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_OPENGL, &window, &renderer);
		if (!window) {
			throw std::runtime_error("Window not initialized.");
		}
		SDL_SetWindowTitle(window, "SDL2 window");
		if (!renderer) {
			throw std::runtime_error("Renderer not initialized.");
		}
	}

	WindowRenderer(const std::string& name, int w, int h)
	    : width(w)
	    , height(h) {
		SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_OPENGL, &window, &renderer);
		if (!window) {
			throw std::runtime_error("Window not initialized.");
		}
		SDL_SetWindowTitle(window, name.c_str());
		if (!renderer) {
			throw std::runtime_error("Renderer not initialized.");
		}
	}

	WindowRenderer(const WindowRenderer& other) = delete;

	WindowRenderer& operator=(const WindowRenderer& other) = delete;

	~WindowRenderer() {
		if (renderer) {
			SDL_DestroyRenderer(renderer);
		}

		if (window) {
			SDL_DestroyWindow(window);
		}
	}

	/**
	void setSurface(Surface& new_surface) {

		surface = Surface(new_surface);
		SDL_UpdateWindowSurface(window);
	}**/

	SDL_Window* getWindow() {
		return window;
	}

	/**
	SDL_Surface* getSurface() {
		return surface.get();
	}**/

	SDL_Renderer* getRenderer() {
		return renderer;
	}
};