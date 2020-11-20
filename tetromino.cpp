#include "tetromino.h"

//https://tetris.wiki/images/3/3d/SRS-pieces.png rotation coordinates reference
void Tetromino::fillLayout() {
	switch (shape) {
	case (Shape::I):
		layout[0][0][1] = true;
		layout[0][1][1] = true;
		layout[0][2][1] = true;
		layout[0][3][1] = true;

		layout[1][2][0] = true;
		layout[1][2][1] = true;
		layout[1][2][2] = true;
		layout[1][2][3] = true;

		layout[2][0][2] = true;
		layout[2][1][2] = true;
		layout[2][2][2] = true;
		layout[2][3][2] = true;

		layout[3][1][0] = true;
		layout[3][1][1] = true;
		layout[3][1][2] = true;
		layout[3][1][3] = true;
		color = Color::cyan;
		break;

	case (Shape::O):
		layout[0][1][0] = true;
		layout[0][1][1] = true;
		layout[0][2][0] = true;
		layout[0][2][1] = true;

		layout[1][1][0] = true;
		layout[1][1][1] = true;
		layout[1][2][0] = true;
		layout[1][2][1] = true;

		layout[2][1][0] = true;
		layout[2][1][1] = true;
		layout[2][2][0] = true;
		layout[2][2][1] = true;

		layout[3][1][0] = true;
		layout[3][1][1] = true;
		layout[3][2][0] = true;
		layout[3][2][1] = true;
		color = Color::yellow;
		break;

	case (Shape::T):
		layout[0][1][0] = true;
		layout[0][0][1] = true;
		layout[0][1][1] = true;
		layout[0][2][1] = true;

		layout[1][1][0] = true;
		layout[1][1][1] = true;
		layout[1][1][2] = true;
		layout[1][2][1] = true;

		layout[2][0][1] = true;
		layout[2][1][1] = true;
		layout[2][1][2] = true;
		layout[2][2][1] = true;

		layout[3][0][1] = true;
		layout[3][1][0] = true;
		layout[3][1][1] = true;
		layout[3][1][2] = true;
		color = Color::purple;
		break;

	case (Shape::S):
		layout[0][0][1] = true;
		layout[0][1][1] = true;
		layout[0][1][0] = true;
		layout[0][2][0] = true;

		layout[1][1][0] = true;
		layout[1][1][1] = true;
		layout[1][2][1] = true;
		layout[1][2][2] = true;

		layout[2][0][2] = true;
		layout[2][1][1] = true;
		layout[2][1][2] = true;
		layout[2][2][1] = true;

		layout[3][0][0] = true;
		layout[3][0][1] = true;
		layout[3][1][1] = true;
		layout[3][1][2] = true;
		color = Color::green;
		break;

	case (Shape::Z):
		layout[0][0][0] = true;
		layout[0][1][0] = true;
		layout[0][1][1] = true;
		layout[0][2][1] = true;

		layout[1][1][1] = true;
		layout[1][1][2] = true;
		layout[1][2][1] = true;
		layout[1][2][0] = true;

		layout[2][0][1] = true;
		layout[2][1][1] = true;
		layout[2][1][2] = true;
		layout[2][2][2] = true;

		layout[3][0][1] = true;
		layout[3][0][2] = true;
		layout[3][1][1] = true;
		layout[3][1][0] = true;
		color = Color::red;
		break;

	case (Shape::J):
		layout[0][0][0] = true;
		layout[0][0][1] = true;
		layout[0][1][1] = true;
		layout[0][2][1] = true;

		layout[1][1][0] = true;
		layout[1][1][1] = true;
		layout[1][1][2] = true;
		layout[1][2][0] = true;

		layout[2][0][1] = true;
		layout[2][1][1] = true;
		layout[2][2][1] = true;
		layout[2][2][2] = true;

		layout[3][0][2] = true;
		layout[3][1][0] = true;
		layout[3][1][1] = true;
		layout[3][1][2] = true;
		color = Color::blue;
		break;

	case (Shape::L):
		layout[0][2][0] = true;
		layout[0][0][1] = true;
		layout[0][1][1] = true;
		layout[0][2][1] = true;

		layout[1][1][0] = true;
		layout[1][1][1] = true;
		layout[1][1][2] = true;
		layout[1][2][2] = true;

		layout[2][0][2] = true;
		layout[2][0][1] = true;
		layout[2][1][1] = true;
		layout[2][2][1] = true;

		layout[3][0][0] = true;
		layout[3][1][0] = true;
		layout[3][1][1] = true;
		layout[3][1][2] = true;
		color = Color::orange;
		break;

	default:
		break;
	}
}

void Tetromino::rotateRight() {
	rotation = (rotation + 1) % 4;
}

void Tetromino::rotateLeft() {
	if (rotation == 0) {
		rotation = 3;
		return;
	}

	rotation -= 1;
}

Color Tetromino::getColor() {
	return color;
}
