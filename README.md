
**Tetris**

I tried to follow the Tetris wiki as close as possible when it came to spawning pieces, rotating them and game speed.

## Controls:

	Q / E      - left / right rotation
	W          - drop
	arrow keys - movement
	ESC        - quit
	
## Compilation

sdl-2.0.4 (or newer)  
llvm-5.0.0 (or newer)  
cmake-3.6.2 (or newer)  
clang  
clang++  

## Other

Some features I should talk about; I implemented a simple wall kick, which means if a tetromino can't be rotated,
it moves to the right and left by one space and tries to rotate there.

Scoring and levels are taken from Tetris Worlds, values are taken from Tetris wiki. You need to score 5 points in each level
to progress into the next one. There are different amounts of points for 1, .. , 4 cleared rows and 2x 4 cleared rows in a row.

The game loads and saves high score into *high_score.txt* file. 
