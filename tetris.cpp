#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <unistd.h>
#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
    #define SLEEP(ms) Sleep(ms)
#else
    #include <unistd.h>
    #define SLEEP(ms) usleep(ms * 1000)
#endif

// Game constants
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;
const int PREVIEW_SIZE = 4;

// Tetromino shapes (4x4 grids)
const int TETROMINOES[7][4][4] = {
    // I-piece
    {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}},
    // O-piece
    {{0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0}},
    // T-piece
    {{0,0,0,0}, {0,1,0,0}, {1,1,1,0}, {0,0,0,0}},
    // S-piece
    {{0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0}},
    // Z-piece
    {{0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0}},
    // J-piece
    {{0,0,0,0}, {1,0,0,0}, {1,1,1,0}, {0,0,0,0}},
    // L-piece
    {{0,0,0,0}, {0,0,1,0}, {1,1,1,0}, {0,0,0,0}}
};

// Color pairs for different pieces
enum Colors {
    COLOR_CYAN_PAIR = 1,
    COLOR_YELLOW_PAIR,
    COLOR_MAGENTA_PAIR,
    COLOR_GREEN_PAIR,
    COLOR_RED_PAIR,
    COLOR_BLUE_PAIR,
    COLOR_WHITE_PAIR,
    COLOR_BORDER_PAIR
};

class Tetris {
private:
    int board[BOARD_HEIGHT][BOARD_WIDTH];
    int currentPiece[4][4];
    int nextPiece[4][4];
    int currentX, currentY;
    int currentType, nextType;
    int score;
    int level;
    int lines;
    bool gameOver;
    WINDOW *gameWin, *scoreWin, *nextWin;

public:
    Tetris() {
        // Initialize ncurses
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        curs_set(0);
        
        // Initialize colors
        start_color();
        init_pair(COLOR_CYAN_PAIR, COLOR_CYAN, COLOR_BLACK);
        init_pair(COLOR_YELLOW_PAIR, COLOR_YELLOW, COLOR_BLACK);
        init_pair(COLOR_MAGENTA_PAIR, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(COLOR_GREEN_PAIR, COLOR_GREEN, COLOR_BLACK);
        init_pair(COLOR_RED_PAIR, COLOR_RED, COLOR_BLACK);
        init_pair(COLOR_BLUE_PAIR, COLOR_BLUE, COLOR_BLACK);
        init_pair(COLOR_WHITE_PAIR, COLOR_WHITE, COLOR_BLACK);
        init_pair(COLOR_BORDER_PAIR, COLOR_WHITE, COLOR_BLUE);
        
        // Create windows (bigger UI)
        gameWin = newwin(BOARD_HEIGHT + 2, BOARD_WIDTH * 3 + 2, 8, 5);
        scoreWin = newwin(12, 25, 8, BOARD_WIDTH * 3 + 10);
        nextWin = newwin(8, 15, 21, BOARD_WIDTH * 3 + 10);
        
        // Initialize game state
        initGame();
    }
    
    ~Tetris() {
        delwin(gameWin);
        delwin(scoreWin);
        delwin(nextWin);
        endwin();
    }
    
    void initGame() {
        // Clear board
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            for (int x = 0; x < BOARD_WIDTH; x++) {
                board[y][x] = 0;
            }
        }
        
        score = 0;
        level = 1;
        lines = 0;
        gameOver = false;
        
        srand(time(nullptr));
        nextType = rand() % 7;
        spawnPiece();
    }
    
    void spawnPiece() {
        currentType = nextType;
        nextType = rand() % 7;
        
        // Copy piece to current
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                currentPiece[y][x] = TETROMINOES[currentType][y][x];
                nextPiece[y][x] = TETROMINOES[nextType][y][x];
            }
        }
        
        currentX = BOARD_WIDTH / 2 - 2;
        currentY = 0;
        
        // Check game over
        if (checkCollision(currentX, currentY)) {
            gameOver = true;
        }
    }
    
    bool checkCollision(int x, int y) {
        for (int py = 0; py < 4; py++) {
            for (int px = 0; px < 4; px++) {
                if (currentPiece[py][px]) {
                    int nx = x + px;
                    int ny = y + py;
                    
                    if (nx < 0 || nx >= BOARD_WIDTH || ny >= BOARD_HEIGHT) {
                        return true;
                    }
                    if (ny >= 0 && board[ny][nx]) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    
    void placePiece() {
        for (int py = 0; py < 4; py++) {
            for (int px = 0; px < 4; px++) {
                if (currentPiece[py][px]) {
                    int nx = currentX + px;
                    int ny = currentY + py;
                    if (ny >= 0) {
                        board[ny][nx] = currentType + 1;
                    }
                }
            }
        }
        
        // Check for complete lines
        checkLines();
        spawnPiece();
    }
    
    void checkLines() {
        int linesCleared = 0;
        std::vector<int> linesToClear;
        
        // Find all complete lines
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            bool fullLine = true;
            for (int x = 0; x < BOARD_WIDTH; x++) {
                if (!board[y][x]) {
                    fullLine = false;
                    break;
                }
            }
            if (fullLine) {
                linesToClear.push_back(y);
            }
        }
        
        // Clear complete lines and move lines down
        for (int lineY : linesToClear) {
            // Move all lines above this one down by one
            for (int y = lineY; y > 0; y--) {
                for (int x = 0; x < BOARD_WIDTH; x++) {
                    board[y][x] = board[y - 1][x];
                }
            }
            // Clear the top line
            for (int x = 0; x < BOARD_WIDTH; x++) {
                board[0][x] = 0;
            }
            linesCleared++;
            
            // Adjust remaining lines to clear (they've moved down)
            for (int i = 0; i < linesToClear.size(); i++) {
                if (linesToClear[i] < lineY) {
                    linesToClear[i]++;
                }
            }
        }
        
        if (linesCleared > 0) {
            lines += linesCleared;
            score += linesCleared * linesCleared * 100 * level;
            level = lines / 10 + 1;
        }
    }
    
    void rotatePiece() {
        int rotated[4][4];
        
        // Rotate 90 degrees clockwise
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                rotated[x][3 - y] = currentPiece[y][x];
            }
        }
        
        // Store original piece
        int original[4][4];
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                original[y][x] = currentPiece[y][x];
                currentPiece[y][x] = rotated[y][x];
            }
        }
        
        // Check if rotation is valid, try wall kicks
        int kicks[][2] = {{0, 0}, {-1, 0}, {1, 0}, {0, -1}, {-2, 0}, {2, 0}};
        bool rotationValid = false;
        
        for (int i = 0; i < 6; i++) {
            if (!checkCollision(currentX + kicks[i][0], currentY + kicks[i][1])) {
                currentX += kicks[i][0];
                currentY += kicks[i][1];
                rotationValid = true;
                break;
            }
        }
        
        // If no valid rotation, restore original
        if (!rotationValid) {
            for (int y = 0; y < 4; y++) {
                for (int x = 0; x < 4; x++) {
                    currentPiece[y][x] = original[y][x];
                }
            }
        }
    }
    
    void drawBoard() {
        // Draw border
        wattron(gameWin, COLOR_PAIR(COLOR_BORDER_PAIR));
        box(gameWin, 0, 0);
        wattroff(gameWin, COLOR_PAIR(COLOR_BORDER_PAIR));
        
        // Draw board
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            for (int x = 0; x < BOARD_WIDTH; x++) {
                int piece = board[y][x];
                wmove(gameWin, y + 1, x * 3 + 1);
                
                if (piece) {
                    wattron(gameWin, COLOR_PAIR(piece) | A_REVERSE);
                    waddstr(gameWin, "   ");
                    wattroff(gameWin, COLOR_PAIR(piece) | A_REVERSE);
                } else {
                    waddstr(gameWin, "   ");
                }
            }
        }
        
        // Draw current piece
        wattron(gameWin, COLOR_PAIR(currentType + 1) | A_REVERSE);
        for (int py = 0; py < 4; py++) {
            for (int px = 0; px < 4; px++) {
                if (currentPiece[py][px]) {
                    int drawX = (currentX + px) * 3 + 1;
                    int drawY = currentY + py + 1;
                    if (drawY > 0 && drawY <= BOARD_HEIGHT) {
                        wmove(gameWin, drawY, drawX);
                        waddstr(gameWin, "   ");
                    }
                }
            }
        }
        wattroff(gameWin, COLOR_PAIR(currentType + 1) | A_REVERSE);
        
        wrefresh(gameWin);
    }
    
    void drawScore() {
        werase(scoreWin);
        box(scoreWin, 0, 0);
        
        wattron(scoreWin, A_BOLD);
        mvwprintw(scoreWin, 1, 2, "RETRO TETRIS");
        wattroff(scoreWin, A_BOLD);
        
        mvwprintw(scoreWin, 3, 2, "Score: %d", score);
        mvwprintw(scoreWin, 4, 2, "Level: %d", level);
        mvwprintw(scoreWin, 5, 2, "Lines: %d", lines);
        
        mvwprintw(scoreWin, 7, 2, "Controls:");
        mvwprintw(scoreWin, 8, 2, "W - Rotate");
        mvwprintw(scoreWin, 9, 2, "A/D - Move");
        mvwprintw(scoreWin, 10, 2, "S - Drop");
        mvwprintw(scoreWin, 11, 2, "R - Restart");
        
        wrefresh(scoreWin);
    }
    
    void drawNext() {
        werase(nextWin);
        box(nextWin, 0, 0);
        mvwprintw(nextWin, 0, 2, "NEXT");
        
        wattron(nextWin, COLOR_PAIR(nextType + 1) | A_REVERSE);
        for (int py = 0; py < 4; py++) {
            for (int px = 0; px < 4; px++) {
                if (nextPiece[py][px]) {
                    wmove(nextWin, py + 2, px * 3 + 2);
                    waddstr(nextWin, "   ");
                }
            }
        }
        wattroff(nextWin, COLOR_PAIR(nextType + 1) | A_REVERSE);
        
        wrefresh(nextWin);
    }
    
    void drawTitle() {
        // Big bold title
        attron(COLOR_PAIR(COLOR_CYAN_PAIR) | A_BOLD);
        mvprintw(0, 15, "**************************************************");
        mvprintw(1, 15, "*                                                *");
        mvprintw(2, 15, "*         RETRO TERMINAL TETRIS GAME             *");
        mvprintw(3, 15, "*              Version 1.0                       *");
        mvprintw(4, 15, "*                                                *");
        mvprintw(5, 15, "**************************************************");
        attroff(COLOR_PAIR(COLOR_CYAN_PAIR) | A_BOLD);
        
        if (gameOver) {
            attron(COLOR_PAIR(COLOR_RED_PAIR) | A_BOLD | A_BLINK);
            mvprintw(BOARD_HEIGHT + 15, 5, ">>> GAME OVER! Press 'R' to restart or 'Q' to quit <<<");
            attroff(COLOR_PAIR(COLOR_RED_PAIR) | A_BOLD | A_BLINK);
        } else {
            attron(COLOR_PAIR(COLOR_GREEN_PAIR) | A_BOLD);
            mvprintw(BOARD_HEIGHT + 15, 5, "Controls: W-Rotate, A/D-Move, S-Drop, Q-Quit, R-Restart");
            attroff(COLOR_PAIR(COLOR_GREEN_PAIR) | A_BOLD);
        }
        
        refresh();
    }
    
    void run() {
        int dropTimer = 0;
        int dropSpeed = 50 - (level - 1) * 5;
        if (dropSpeed < 5) dropSpeed = 5;
        
        while (true) {
            clear();
            drawTitle();
            drawBoard();
            drawScore();
            drawNext();
            
            int ch = getch();
            
            if (ch == 'q' || ch == 'Q') {
                break;
            } else if (ch == 'r' || ch == 'R') {
                initGame();
                continue;
            }
            
            if (!gameOver) {
                switch (ch) {
                    case 'a':
                    case 'A':
                        if (!checkCollision(currentX - 1, currentY)) {
                            currentX--;
                        }
                        break;
                    case 'd':
                    case 'D':
                        if (!checkCollision(currentX + 1, currentY)) {
                            currentX++;
                        }
                        break;
                    case 's':
                    case 'S':
                        if (!checkCollision(currentX, currentY + 1)) {
                            currentY++;
                        } else {
                            placePiece();
                        }
                        break;
                    case 'w':
                    case 'W':
                        rotatePiece();
                        break;
                }
                
                // Auto drop
                dropTimer++;
                if (dropTimer >= dropSpeed) {
                    if (!checkCollision(currentX, currentY + 1)) {
                        currentY++;
                    } else {
                        placePiece();
                    }
                    dropTimer = 0;
                    dropSpeed = 50 - (level - 1) * 5;
                    if (dropSpeed < 5) dropSpeed = 5;
                }
            }
            
            usleep(20000); // 50 FPS
        }
    }
};

int main() {
    Tetris game;
    game.run();
    return 0;
}