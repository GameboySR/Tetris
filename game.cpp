#include "game.h"
#include <fstream>
#include <iostream>

/////////////////////////////////////////////////////////////////////////////////////
// GAME LOGIC
/////////////////////////////////////////////////////////////////////////////////////

void Game::run() {
	while (play_again) {
		play_again = false;
		score = 0;
		last_score = 0;
		last_tetris = false;
		curr_x = 3;
		curr_y = 0;
		level = 0;
		grid = std::vector<std::vector<Color>>(10, std::vector<Color>(20, Color::black));
		time_passed = SDL_GetTicks();

		while (step()) {
			if (SDL_GetTicks() > time_passed) {
				time_passed += delays[level];
				if (canMoveDown()) {

					curr_y += 1;
					redrawAll();
					SDL_RenderPresent(wr.getRenderer());
				} else {
					lockTetromino();
					moveRows();
					updateDifficulty();
					redrawAll();
					updateCurrentAndNext();
					SDL_RenderPresent(wr.getRenderer());
					if (isGameOver()) {
						saveHighScore();
						printGameOver();
						play_again = playAgain();
						break;
					}
				}
			}
		}
	}
}



bool Game::step() {
	redrawAll();
	SDL_RenderPresent(wr.getRenderer());
	return manageKeys();
}

bool Game::manageKeys() {
	if (SDL_WaitEventTimeout(&e, 250) && e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
		case SDLK_p:
			renderPause(true);
			return pauseGame();
			break;

		case SDLK_q: //weird if structures perform wall kicks
			if (canRotate(false)) {
				current.rotateLeft();
				redrawAll();
				SDL_RenderPresent(wr.getRenderer());
			} else {
				if (canMoveRight()) {
					curr_x++;
					if (canRotate(false)) {
						current.rotateLeft();
						redrawAll();
						SDL_RenderPresent(wr.getRenderer());
					} else {
						curr_x--;
					}
				} else {
					if (canMoveLeft()) {
						curr_x--;
						if (canRotate(false)) {
							current.rotateLeft();
							redrawAll();
							SDL_RenderPresent(wr.getRenderer());
						} else {
							curr_x++;
						}
					}
				}
			}
			break;

		case SDLK_e:
			if (canRotate(true)) {
				current.rotateRight();
				redrawAll();
				SDL_RenderPresent(wr.getRenderer());
			} else {
				if (canMoveRight()) {
					curr_x++;
					if (canRotate(true)) {
						current.rotateRight();
						redrawAll();
						SDL_RenderPresent(wr.getRenderer());
					} else {
						curr_x--;
					}
				} else {
					if (canMoveLeft()) {
						curr_x--;
						if (canRotate(true)) {
							current.rotateRight();
							redrawAll();
							SDL_RenderPresent(wr.getRenderer());
						} else {
							curr_x++;
						}
					}
				}
			}
			break;

		case SDLK_w:

			while (canMoveDown()) {
				curr_y += 1;
			}

			lockTetromino();
			moveRows();
			updateDifficulty();
			redrawAll();
			updateCurrentAndNext();
			SDL_RenderPresent(wr.getRenderer());

			if (isGameOver()) {
				saveHighScore();
				printGameOver();
				play_again = playAgain();
				return false;
			}

			break;

		case SDLK_LEFT:
			if (canMoveLeft()) {
				curr_x -= 1;
				redrawAll();
				SDL_RenderPresent(wr.getRenderer());
			}
			break;

		case SDLK_RIGHT:
			if (canMoveRight()) {
				curr_x += 1;
				redrawAll();
				SDL_RenderPresent(wr.getRenderer());
			}
			break;

		case SDLK_DOWN:
			if (canMoveDown()) {
				curr_y += 1;
				redrawAll();
				SDL_RenderPresent(wr.getRenderer());
			}
			break;

		case SDLK_ESCAPE:
			return false;

		default:
			break;
		}
	}
	return true;
}

void Game::lockTetromino() {
	const auto& coords = current.getCoords();

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (coords[x][y]) {
				grid[x + curr_x][y + curr_y] = current.getColor();
			}
		}
	}
}

void Game::addScore(int rows) {
	switch (rows) {
	case 1:
		score += 1;
		last_tetris = false;
		break;
	case 2:
		score += 3;
		last_tetris = false;
		break;
	case 3:
		score += 5;
		last_tetris = false;
		break;
	case 4:
		if (last_tetris) {
			score += 12;
		} else {
			score += 8;
		} //if the last clear was also a tetris
		last_tetris = true;
		break;
	default:
		break;
	}
}

std::pair<int, int> Game::findFullRowsAndEmpty() {
	bool clear_row;
	int rows = 0;
	int index_add = 0;

	for (int y = 0; y < 20; y++) {
		clear_row = true;
		for (int x = 0; x < 10; x++) {
			if (!static_cast<bool>(grid[x][y])) {
				clear_row = false;
				break;
			}
		}

		if (clear_row) {
			rows++;
			for (int x = 0; x < 10; x++) {
				grid[x][y] = Color::black;
			}

			for (int y2 = 1; y2 < 4 && y2 + y < 20; y2++) {
				clear_row = true;
				index_add++;
				for (int x = 0; x < 10; x++) {
					if (!static_cast<bool>(grid[x][y + y2])) {
						clear_row = false;
						break;
					}
				}

				if (clear_row) {
					rows++;
					for (int x = 0; x < 10; x++) {
						grid[x][y + y2] = Color::black;
					}
				}
			}

			return { rows, y + index_add };
		}
	}

	return { 0, -1 };
}

bool Game::isGameOver() {
	const auto& coords = current.getCoords();

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (coords[x][y]) {
				if (grid[x + curr_x][y + curr_y] != Color::black) {
					return true;
				}
			}
		}
	}

	return false;
}

void Game::updateDifficulty() {
	if (score - last_score >= 5) {
		level++;
		last_score = score;
	}
}

void Game::saveHighScore() {
	if (high_score > score) {
		return;
	}

	std::ofstream score_file;
	score_file.open("high_score.txt", std::ofstream::trunc);

	if (!score_file) {
		std::cerr << "High score file couldn't be opened." << std::endl;
		return;
	}

	score_file << score;
	score_file.close();
}

unsigned int Game::loadHighScore() {
	unsigned int result;
	std::string input;

	std::ifstream score_file;
	score_file.open("high_score.txt");

	if (!score_file) {
		std::cerr << "High score file couldn't be opened." << std::endl;
		return 0;
	}

	getline(score_file, input);
	score_file.close();

	result = std::stoul(input);
	return result;
}

bool Game::playAgain() {
	while (true) {
		if (SDL_WaitEventTimeout(&e, 250) && e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
			case SDLK_y:
				return true;
				break;

			case SDLK_n:
				return false;
				break;

			default:
				break;
			}
		}
	}
}

bool Game::pauseGame() {

	while (true) {
		if (SDL_WaitEventTimeout(&e, 250) && e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
			case SDLK_p:
				time_passed = SDL_GetTicks() - time_left_to_move;
				renderPause(false);
				return true;
				break;

			case SDLK_ESCAPE:
				return false;
				break;

			default:
				break;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// MOVEMENT
/////////////////////////////////////////////////////////////////////////////////////

bool Game::canMoveLeft() {
	const auto& coords = current.getCoords();

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (coords[x][y]) {
				if (x + curr_x - 1 < 0 || static_cast<bool>(grid[x + curr_x - 1][y + curr_y])) {
					return false;
				}
			}
		}
	}

	return true;
}

bool Game::canMoveRight() {
	const auto& coords = current.getCoords();

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (coords[x][y]) {
				if (x + curr_x + 1 > 9 || static_cast<bool>(grid[x + curr_x + 1][y + curr_y])) {
					return false;
				}
			}
		}
	}

	return true;
}

bool Game::canMoveDown() {
	const auto& coords = current.getCoords();

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (coords[x][y]) {
				if (y + curr_y + 1 > 19 || static_cast<bool>(grid[x + curr_x][y + curr_y + 1])) {
					return false;
				}
			}
		}
	}

	return true;
}

bool Game::canRotate(bool right) {
	if (right) {
		current.rotateRight();
	} else {
		current.rotateLeft();
	}

	const auto& coords = current.getCoords();

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (coords[x][y]) {
				if (x + curr_x < 0 || x + curr_x > 9 || y + curr_y > 19 || static_cast<bool>(grid[x + curr_x][y + curr_y])) {
					if (right) {
						current.rotateLeft();
					} else {
						current.rotateRight();
					}
					return false;
				}
			}
		}
	}

	if (right) {
		current.rotateLeft();
	} else {
		current.rotateRight();
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////
// DRAWING
/////////////////////////////////////////////////////////////////////////////////////

void Game::drawNext(bool destroy) {
	const auto& coords = next.getCoords();
	if (destroy) {
		setRendererColor(Color::black);
	} else {
		setRendererColor(next.getColor());
	}

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (coords[x][y]) {
				SDL_Rect rectangle { x * grid_w / 12 + 1 + grid_w + grid_w / 12, y * grid_w / 12 + 1 + grid_h / 20 + grid_w / 6,
					grid_w / 12 - 2, grid_w / 12 - 2 };
				SDL_RenderFillRect(wr.getRenderer(), &rectangle);
			}
		}
	}
}

void Game::updateCurrentAndNext() {
	drawNext(true);
	current = next;
	curr_x = 3;
	curr_y = 0;
	next = Tetromino(static_cast<Shape>(rand() % 7));
	drawNext(false);
}

void Game::redrawTetromino() {
	const auto& coords = current.getCoords();

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (coords[x][y]) {
				drawSquare(x + curr_x, y + curr_y, true);
			}
		}
	}
}

void Game::setRendererColor(Color color) {
	switch (color) {
	case (Color::black):
		SDL_SetRenderDrawColor(wr.getRenderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
		break;

	case (Color::red):
		SDL_SetRenderDrawColor(wr.getRenderer(), 241, 1, 0, SDL_ALPHA_OPAQUE);
		break;

	case (Color::orange):
		SDL_SetRenderDrawColor(wr.getRenderer(), 237, 161, 0, SDL_ALPHA_OPAQUE);
		break;

	case (Color::yellow):
		SDL_SetRenderDrawColor(wr.getRenderer(), 240, 241, 0, SDL_ALPHA_OPAQUE);
		break;

	case (Color::green):
		SDL_SetRenderDrawColor(wr.getRenderer(), 0, 240, 0, SDL_ALPHA_OPAQUE);
		break;

	case (Color::cyan):
		SDL_SetRenderDrawColor(wr.getRenderer(), 1, 241, 235, SDL_ALPHA_OPAQUE);
		break;

	case (Color::blue):
		SDL_SetRenderDrawColor(wr.getRenderer(), 0, 2, 238, SDL_ALPHA_OPAQUE);
		break;

	case (Color::purple):
		SDL_SetRenderDrawColor(wr.getRenderer(), 160, 3, 240, SDL_ALPHA_OPAQUE);
		break;

	default:
		break;
	}
}

void Game::drawGridBorders() {

	//SDL_RenderDrawLine(wr.getRenderer(), 0, 3 * grid_w / 10, grid_w, grid_h / 20 * 3);
	//horizontal and vertical grid lines
	SDL_SetRenderDrawColor(wr.getRenderer(), 57, 57, 57, SDL_ALPHA_OPAQUE);
	for (int i = 1; i < 20; i++) {
		SDL_RenderDrawLine(wr.getRenderer(), 1, grid_h / 20 * i, grid_w, grid_h / 20 * i);
	}
	for (int i = 1; i < 10; i++) {
		SDL_RenderDrawLine(wr.getRenderer(), grid_w / 10 * i, 1, grid_w / 10 * i, grid_h);
	}

	//grid borders
	SDL_Point points[5] = { { 1, 1 }, { grid_w, 1 }, { grid_w, grid_h - 1 }, { 1, grid_h - 1 }, { 1, 1 } };
	SDL_SetRenderDrawColor(wr.getRenderer(), 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLines(wr.getRenderer(), points, 5);

	//next tetromino borders
	SDL_Point pts[5] = { { static_cast<int>(grid_w + grid_w / 12), static_cast<int>(grid_h / 20 + grid_w / 12) },
		{ static_cast<int>(grid_w * 1.5 - grid_w / 12), static_cast<int>(grid_h / 20 + grid_w / 12) },
		{ static_cast<int>(grid_w * 1.5 - grid_w / 12), static_cast<int>(grid_h / 20 + grid_w * 5 / 12) },
		{ static_cast<int>(static_cast<int>(grid_w + grid_w / 12)), static_cast<int>(grid_h / 20 + grid_w * 5 / 12) },
		{ static_cast<int>(grid_w + grid_w / 12), static_cast<int>(grid_h / 20 + grid_w / 12) } };

	SDL_RenderDrawLines(wr.getRenderer(), pts, 5);

	//score borders
	SDL_Point score_pts[5] = { { static_cast<int>(grid_w + grid_w / 12), static_cast<int>(grid_h - grid_h * 4 / 12) },
		{ static_cast<int>(grid_w * 1.5 - grid_w / 12), static_cast<int>(grid_h - grid_h * 4 / 12) },
		{ static_cast<int>(grid_w * 1.5 - grid_w / 12), static_cast<int>(grid_h - grid_h / 12) },
		{ static_cast<int>(static_cast<int>(grid_w + grid_w / 12)), static_cast<int>(grid_h - grid_h / 12) },
		{ static_cast<int>(grid_w + grid_w / 12), static_cast<int>(grid_h - grid_h * 4 / 12) } };

	SDL_RenderDrawLines(wr.getRenderer(), score_pts, 5);

	//rendering score textures
	SDL_Rect score_string_rect = { static_cast<int>(grid_w + grid_w / 12 + 1), static_cast<int>(grid_h - grid_h * 4 / 12 + 1),
		grid_h / 6 - 2, grid_h / 20 - 2 };
	Surface score_sur(TTF_RenderText_Solid(font.getFontPointer(), "Score", white));
	Texture texture(wr.getRenderer(), score_sur);
	SDL_RenderCopy(wr.getRenderer(), texture.getTexture(), nullptr, &score_string_rect);

	SDL_Rect high_score_string_rect = { static_cast<int>(grid_w + grid_w / 12 + 1), static_cast<int>(grid_h - grid_h / 20 - grid_h / 6 - grid_h / 120),
		grid_h / 6 - 2, grid_h / 20 - 2 };
	Surface high_score_sur(TTF_RenderText_Solid(font.getFontPointer(), "High Score", white));
	Texture high_texture(wr.getRenderer(), high_score_sur);
	SDL_RenderCopy(wr.getRenderer(), high_texture.getTexture(), nullptr, &high_score_string_rect);

	SDL_Rect hs_rect = { static_cast<int>(grid_w + grid_w / 12 + 1), static_cast<int>(grid_h - grid_h * 17 / 120),
		grid_h / 6 - 2, grid_h / 20 - 2 };
	Surface hs_sur(TTF_RenderText_Solid(font.getFontPointer(), std::to_string(high_score).c_str(), white));
	Texture hs_texture(wr.getRenderer(), hs_sur);
	SDL_RenderCopy(wr.getRenderer(), hs_texture.getTexture(), nullptr, &hs_rect);
}

void Game::renderPause(bool pause) {
	redrawAll();
	time_left_to_move = SDL_GetTicks() - time_passed;
	if (pause) {
		SDL_Rect pause_rect = { static_cast<int>(grid_w + grid_w / 12 + 1), static_cast<int>(grid_h / 2),
			grid_h / 6 - 2, grid_h / 20 - 2 };
		Surface pause_sur = TTF_RenderText_Solid(font.getFontPointer(), "PAUSED", white);
		Texture pause_texture(wr.getRenderer(), pause_sur);
		SDL_RenderCopy(wr.getRenderer(), pause_texture.getTexture(), nullptr, &pause_rect);
	} else {
		setRendererColor(Color::black);
		SDL_Rect pause_rect = { static_cast<int>(grid_w + grid_w / 12 + 1), static_cast<int>(grid_h / 2),
			grid_h / 6 - 2, grid_h / 20 - 2 };
		SDL_RenderFillRect(wr.getRenderer(), &pause_rect);
	}
	SDL_RenderPresent(wr.getRenderer());
}

void Game::drawSquare(int x, int y, bool tetromino) {
	if (tetromino) {
		setRendererColor(current.getColor());
	} else {
		setRendererColor(grid[x][y]);
	}
	SDL_Rect rectangle { x * grid_w / 10 + 2, y * grid_w / 10 + 2, grid_w / 10 - 4, grid_w / 10 - 4 };
	SDL_RenderFillRect(wr.getRenderer(), &rectangle);
}

void Game::eraseTetromino() {
	const auto& coords = current.getCoords();

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			if (coords[x][y]) {
				drawSquare(x + curr_x, y + curr_y, false);
			}
		}
	}
}

void Game::moveRows() {
	std::pair<int, int> rows_and_index = findFullRowsAndEmpty();
	bool is_empty = true;
	int rows_to_empty = rows_and_index.first;
	addScore(rows_and_index.first);
	updateScore();

	if (rows_and_index.first == 0) {
		return;
	}

	for (int y = rows_and_index.second; y >= 0 && rows_to_empty != 0; y--) {
		is_empty = true;

		for (int x = 0; x < 10; x++) {
			if (static_cast<bool>(grid[x][y])) {
				is_empty = false;
			}
		}

		while (is_empty) {
			rows_to_empty--;

			for (int y2 = y; y2 - 1 >= 0; y2--) {
				for (int x = 0; x < 10; x++) {
					grid[x][y2] = grid[x][y2 - 1];
				}

				if (y2 - 1 == 0) {
					for (int x = 0; x < 10; x++) {
						grid[x][0] = Color::black;
					}

					break;
				}
			}

			for (int x = 0; x < 10; x++) {
				if (static_cast<bool>(grid[x][y])) {
					is_empty = false;
				}
			}
		}
	}
}

void Game::updateGrid() {
	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 20; y++) {
			drawSquare(x, y, false);
		}
	}
}

void Game::updateScore() {

	SDL_SetRenderDrawColor(wr.getRenderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(wr.getRenderer(), &score_rect);

	Surface score_sur(TTF_RenderText_Solid(font.getFontPointer(), std::to_string(score).c_str(), white));
	Texture texture(wr.getRenderer(), score_sur);
	SDL_RenderCopy(wr.getRenderer(), texture.getTexture(), nullptr, &score_rect);
}

void Game::printGameOver() {
	setRendererColor(Color::black);
	SDL_RenderClear(wr.getRenderer());

	SDL_Rect gameover = { 0, grid_h / 4, static_cast<int>(grid_w * 1.5), grid_h / 4 };
	SDL_SetRenderDrawColor(wr.getRenderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(wr.getRenderer(), &gameover);

	SDL_Rect again = { 0, grid_h / 2, static_cast<int>(grid_w * 1.5), grid_h / 4 };
	SDL_RenderFillRect(wr.getRenderer(), &again);

	Surface sur_over = TTF_RenderText_Solid(font.getFontPointer(), "Game Over", white);
	Texture texture(wr.getRenderer(), sur_over);
	SDL_RenderCopy(wr.getRenderer(), texture.getTexture(), nullptr, &gameover);

	Surface sur_again = TTF_RenderText_Solid(font.getFontPointer(), "Try again? Y/N", white);
	Texture texture_again(wr.getRenderer(), sur_again);
	SDL_RenderCopy(wr.getRenderer(), texture_again.getTexture(), nullptr, &again);

	SDL_RenderPresent(wr.getRenderer());
}

void Game::redrawAll() {
	time_left_to_move = SDL_GetTicks() - time_passed;
	setRendererColor(Color::black);
	SDL_RenderClear(wr.getRenderer());
	drawGridBorders();
	updateGrid();
	updateScore();
	redrawTetromino();
	drawNext(false);
	time_passed = SDL_GetTicks() - time_left_to_move;

}