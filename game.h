#pragma once
#include "SDL_wrapper.h"
#include "tetromino.h"
#include <vector>

class Game {
private:
	SDL sdl {};
	TTF ttf {};
	SDL_Event e {};
	std::vector<std::vector<Color>> grid {};
	int grid_w;
	int grid_h;
	WindowRenderer wr {};
	Tetromino current;
	Tetromino next;
	int curr_x = 3;
	int curr_y = 0;
	int level = 0;
	unsigned int score = 0;
	unsigned int last_score = 0; //determines if 5 points have been earned in the current level
	unsigned int high_score = 0;
	double time_passed;
	double time_left_to_move = 0.0;
	std::vector<double> delays = {
		1000.00, 793.00, 617.8, 472.73, 355.20, 262.00, 189.68, 134.73, 93.88, 64.15, 42.98, 28.22, 18.15, 11.44, 7.06, 4.26, 2.52, 1.46, 0.82, 0.46
	}; //Tetris Worlds official delays
	bool last_tetris = false; //to calculate back-to-back tetris'
	Font font;
	SDL_Rect score_rect;
	SDL_Color white;
	bool play_again = true;

	bool step();

	//updates current tetromino's position on screen
	void redrawTetromino();

	//sets the correct color for drawing
	void setRendererColor(Color color);

	//draws everything that will be permanent on screen, borders, grid, high score and score strings
	void drawGridBorders();

	//draws a square into game grid, if tetromine is true, the color is taken from the current tetromino, otherwise the color will be black
	void drawSquare(int x, int y, bool tetromino);

	//erases current tetromino from the screen
	void eraseTetromino();

	bool canMoveLeft();
	bool canMoveRight();
	bool canMoveDown();

	//if right is true, checks for right rotation, otherwise for left
	bool canRotate(bool right);

	//method that checks, which key is pressed or held
	bool manageKeys();

	//draws the tetromino that comes after the current one, if destroy is true, the next tetromino will be erased
	void drawNext(bool destroy);

	//updates current and next tetrominos both on screen and internally
	void updateCurrentAndNext();

	//tetromino is saved into grid
	void lockTetromino();

	//method that moves all rows after clearing some down
	void moveRows();

	//adds points based on cleared rows
	void addScore(int rows);

	//finds full rows after locking a tetromino, counts them and empties them
	//return a pair of {number_of_clear_rows, the_lowest_clear_row_index}
	std::pair<int, int> findFullRowsAndEmpty();

	//checks if the new tetromino spawns inside a full block, if so, the game end (returns true)
	bool isGameOver();

	//if 5 points were earned in the current level, the game speeds up
	void updateDifficulty();

	//redraws score
	void updateScore();

	//if score is greater than high score, the new score is saved
	void saveHighScore();

	//loads high score from file
	unsigned int loadHighScore();

	void printGameOver();

	void updateGrid();

	bool pauseGame();

	void renderPause(bool pause);//true if we want to pause the game, false if we want the game to continue

	bool playAgain();

	void redrawAll();

public:
	Game(int grid_width)
	    : grid(10, std::vector<Color>(20, Color::black))
	    , grid_w(grid_width)
	    , grid_h(grid_width * 2)
	    , wr("HW05 Tetris", static_cast<int>(1.5 * grid_w), grid_h)
	    , current(static_cast<Shape>(rand() % 7))
	    , next(static_cast<Shape>(rand() % 7))
	    , high_score(loadHighScore())
	    , time_passed(SDL_GetTicks())
	    , font("Tetris.ttf")
	    , score_rect({ static_cast<int>(grid_w + grid_w / 12 + 1), static_cast<int>(grid_h * 2 / 3 + grid_h / 15),
	          grid_h / 6 - 2, grid_h / 20 - 2 }) {
		white = { 255, 255, 255, SDL_ALPHA_OPAQUE };
		if (!font.getFontPointer()) {
			throw std::runtime_error("Font not initialized.");
		}
	};

	void run();
};
