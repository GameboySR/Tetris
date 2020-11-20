#pragma once
#include <cstring>
#include "SDL.h"
#include <vector>

enum class Shape { I,
	O,
	T,
	S,
	Z,
	J,
	L };

enum class Color { black,
	red,
	orange,
	yellow,
	green,
	cyan,
	blue,
	purple };

class Tetromino {
private:
	std::vector<std::vector<std::vector<bool>>> layout; //layout[rotation][x][y]
	Shape shape;
	Color color;
	int rotation = 0; //0 - 0 degrees, 1 - 90 degrees, 2 - 180 degrees, 3 - 270 degrees

	void fillLayout();

public:
	Tetromino(Shape s)
	    : layout(4, std::vector<std::vector<bool>>(4, std::vector<bool>(4, false))) //initializing a 3d vector (bleh)
	    , shape(s) {
		fillLayout();
	}

	void rotateRight();
	void rotateLeft();
	Color getColor();

	const std::vector<std::vector<bool>>& getCoords() const {
		return layout[rotation];
	}
};
