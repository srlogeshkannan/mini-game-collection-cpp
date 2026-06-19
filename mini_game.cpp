#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <iomanip>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
    #define CLEAR "cls"
    char getKey() { return _getch(); }
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
    #define CLEAR "clear"
    char getKey() {
        struct termios oldt, newt;
        char ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        read(STDIN_FILENO, &ch, 1);
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#endif

using namespace std;

// ─────────────────────────────────────────────
//  Shared utilities
// ─────────────────────────────────────────────
void clearScreen() { system(CLEAR); }

void waitEnter() {
    cout << "\n  Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int getInt(const string& prompt, int lo, int hi) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= lo && val <= hi) { cin.ignore(); return val; }
        cout << "  ✗ Enter a number between " << lo << " and " << hi << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void printBanner(const string& title) {
    cout << "\n"
         << "╔══════════════════════════════════════════════════════╗\n"
         << "║              MINI GAME PROJECT  v1.0                 ║\n"
         << "╠══════════════════════════════════════════════════════╣\n"
         << "║  " << left << setw(52) << title << "║\n"
         << "╚══════════════════════════════════════════════════════╝\n";
}

// ═══════════════════════════════════════════════════════
//
//   GAME 1 : TIC TAC TOE
//
// ═══════════════════════════════════════════════════════
class TicTacToe {
private:
    char  board[3][3];
    char  currentPlayer;
    int   winsX, winsO, draws;
    bool  vsAI;

    void initBoard() {
        for (int r = 0; r < 3; r++)
            for (int c = 0; c < 3; c++)
                board[r][c] = '1' + r * 3 + c;   // '1'..'9'
        currentPlayer = 'X';
    }

    void drawBoard() {
        clearScreen();
        printBanner("TIC TAC TOE");
        cout << "\n";
        cout << "  Score ── X: " << winsX << "  O: " << winsO << "  Draws: " << draws << "\n\n";
        cout << "     1   2   3\n";
        cout << "   ┌───┬───┬───┐\n";
        for (int r = 0; r < 3; r++) {
            cout << " " << (r + 1) << " │";
            for (int c = 0; c < 3; c++) {
                char ch = board[r][c];
                if      (ch == 'X') cout << " \033[1;31mX\033[0m ";
                else if (ch == 'O') cout << " \033[1;34mO\033[0m ";
                else                cout << " " << ch << " ";
                cout << "│";
            }
            cout << "\n";
            if (r < 2) cout << "   ├───┼───┼───┤\n";
        }
        cout << "   └───┴───┴───┘\n";
        cout << "\n  Current Player: " << currentPlayer << "\n";
    }

    bool makeMove(int cell) {
        int r = (cell - 1) / 3;
        int c = (cell - 1) % 3;
        if (board[r][c] == 'X' || board[r][c] == 'O') return false;
        board[r][c] = currentPlayer;
        return true;
    }

    char checkWinner() {
        // Rows & cols
        for (int i = 0; i < 3; i++) {
            if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) return board[i][0];
            if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) return board[0][i];
        }
        // Diagonals
        if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) return board[0][0];
        if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) return board[0][2];
        return ' ';
    }

    bool isBoardFull() {
        for (int r = 0; r < 3; r++)
            for (int c = 0; c < 3; c++)
                if (board[r][c] != 'X' && board[r][c] != 'O') return false;
        return true;
    }

    // ── Minimax AI ───────────────────────────
    int minimax(bool isMaximising) {
        char w = checkWinner();
        if (w == 'O') return  10;
        if (w == 'X') return -10;
        if (isBoardFull()) return 0;

        int best = isMaximising ? -1000 : 1000;
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {
                if (board[r][c] != 'X' && board[r][c] != 'O') {
                    char orig = board[r][c];
                    board[r][c] = isMaximising ? 'O' : 'X';
                    int score = minimax(!isMaximising);
                    board[r][c] = orig;
                    best = isMaximising ? max(best, score) : min(best, score);
                }
            }
        }
        return best;
    }

    void aiMove() {
        int bestScore = -1000, bestR = -1, bestC = -1;
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {
                if (board[r][c] != 'X' && board[r][c] != 'O') {
                    char orig = board[r][c];
                    board[r][c] = 'O';
                    int score = minimax(false);
                    board[r][c] = orig;
                    if (score > bestScore) { bestScore = score; bestR = r; bestC = c; }
                }
            }
        }
        board[bestR][bestC] = 'O';
    }

    void switchPlayer() { currentPlayer = (currentPlayer == 'X') ? 'O' : 'X'; }

public:
    TicTacToe() : winsX(0), winsO(0), draws(0), vsAI(false) {}

    void run() {
        clearScreen();
        printBanner("TIC TAC TOE — SETUP");
        cout << "\n  1. Two Players (PvP)\n  2. vs Computer (AI)\n";
        vsAI = (getInt("  Choose: ", 1, 2) == 2);

        bool playAgain = true;
        while (playAgain) {
            initBoard();
            bool gameOver = false;

            while (!gameOver) {
                drawBoard();

                if (vsAI && currentPlayer == 'O') {
                    cout << "  AI is thinking...\n";
                    aiMove();
                } else {
                    int cell = getInt("  Enter cell (1-9): ", 1, 9);
                    while (!makeMove(cell)) {
                        cout << "  ✗ Cell already taken. Try again.\n";
                        cell = getInt("  Enter cell (1-9): ", 1, 9);
                    }
                }

                char winner = checkWinner();
                if (winner != ' ') {
                    drawBoard();
                    if (winner == 'X') { cout << "\n  🎉 Player X WINS!\n"; winsX++; }
                    else               { cout << "\n  🎉 " << (vsAI ? "AI" : "Player O") << " WINS!\n"; winsO++; }
                    gameOver = true;
                } else if (isBoardFull()) {
                    drawBoard();
                    cout << "\n  🤝 It's a DRAW!\n";
                    draws++;
                    gameOver = true;
                } else {
                    switchPlayer();
                }
            }

            cout << "\n  Play again? (1=Yes / 0=No): ";
            int again;
            cin >> again; cin.ignore();
            playAgain = (again == 1);
        }
    }
};

// ═══════════════════════════════════════════════════════
//
//   GAME 2 : SNAKE
//
// ═══════════════════════════════════════════════════════
class Snake {
private:
    static const int WIDTH  = 20;
    static const int HEIGHT = 15;

    struct Point { int x, y; };

    vector<Point> snake;
    Point  food;
    char   dir;       // W A S D
    bool   gameOver;
    int    score;
    int    highScore;
    int    speed;     // ms delay (lower = faster)

    void placeFood() {
        srand((unsigned)time(nullptr) + score);
        bool onSnake;
        do {
            onSnake = false;
            food.x = rand() % (WIDTH  - 2) + 1;
            food.y = rand() % (HEIGHT - 2) + 1;
            for (const auto& s : snake)
                if (s.x == food.x && s.y == food.y) { onSnake = true; break; }
        } while (onSnake);
    }

    void drawFrame() {
        clearScreen();
        printBanner("SNAKE GAME");
        cout << "\n  Score: " << score << "   High Score: " << highScore
             << "   Speed: " << (speed <= 80 ? "FAST" : speed <= 150 ? "MEDIUM" : "SLOW") << "\n\n";

        // Build grid
        char grid[HEIGHT][WIDTH];
        for (int y = 0; y < HEIGHT; y++)
            for (int x = 0; x < WIDTH; x++) {
                if (x == 0 || x == WIDTH-1) grid[y][x] = '|';
                else if (y == 0 || y == HEIGHT-1) grid[y][x] = '-';
                else grid[y][x] = ' ';
            }

        // Place food
        grid[food.y][food.x] = '*';

        // Place snake
        for (int i = 0; i < (int)snake.size(); i++)
            grid[snake[i].y][snake[i].x] = (i == 0) ? 'O' : 'o';

        // Print grid
        for (int y = 0; y < HEIGHT; y++) {
            cout << "  ";
            for (int x = 0; x < WIDTH; x++) cout << grid[y][x];
            cout << "\n";
        }
        cout << "\n  Controls: W=Up  A=Left  S=Down  D=Right  Q=Quit\n";
    }

    void sleepMs(int ms) {
#ifdef _WIN32
        Sleep(ms);
#else
        usleep(ms * 1000);
#endif
    }

    bool kbhit_nonblock() {
#ifdef _WIN32
        return _kbhit();
#else
        struct termios oldt, newt;
        int ch;
        int oldf;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
        if (ch != EOF) { ungetc(ch, stdin); return true; }
        return false;
#endif
    }

public:
    Snake() : gameOver(false), score(0), highScore(0), speed(200) {}

    void run() {
        clearScreen();
        printBanner("SNAKE — SETUP");
        cout << "\n  Select Speed:\n  1. Slow\n  2. Medium\n  3. Fast\n";
        int sp = getInt("  Choose: ", 1, 3);
        speed = (sp == 1) ? 250 : (sp == 2) ? 150 : 80;

        bool playAgain = true;
        while (playAgain) {
            // Init snake (3 segments, center)
            snake.clear();
            snake.push_back({WIDTH/2,     HEIGHT/2});
            snake.push_back({WIDTH/2 - 1, HEIGHT/2});
            snake.push_back({WIDTH/2 - 2, HEIGHT/2});
            dir      = 'D';
            gameOver = false;
            score    = 0;
            placeFood();

            while (!gameOver) {
                // Non-blocking key input
                if (kbhit_nonblock()) {
                    char key = tolower(getKey());
                    if (key == 'q') { gameOver = true; break; }
                    // Prevent reverse direction
                    if ((key=='w' && dir!='s') || (key=='s' && dir!='w') ||
                        (key=='a' && dir!='d') || (key=='d' && dir!='a'))
                        dir = key;
                }

                // Calculate next head
                Point newHead = snake[0];
                if      (dir == 'w') newHead.y--;
                else if (dir == 's') newHead.y++;
                else if (dir == 'a') newHead.x--;
                else if (dir == 'd') newHead.x++;

                // Wall collision
                if (newHead.x <= 0 || newHead.x >= WIDTH-1 ||
                    newHead.y <= 0 || newHead.y >= HEIGHT-1) {
                    gameOver = true; break;
                }

                // Self collision
                for (const auto& s : snake)
                    if (s.x == newHead.x && s.y == newHead.y) { gameOver = true; break; }
                if (gameOver) break;

                // Move snake
                snake.insert(snake.begin(), newHead);
                if (newHead.x == food.x && newHead.y == food.y) {
                    score += 10;
                    if (score > highScore) highScore = score;
                    // Speed up every 50 pts
                    if (score % 50 == 0 && speed > 60) speed -= 20;
                    placeFood();
                } else {
                    snake.pop_back();
                }

                drawFrame();
                sleepMs(speed);
            }

            // Game over screen
            clearScreen();
            printBanner("SNAKE — GAME OVER");
            cout << "\n  Final Score : " << score     << "\n"
                 << "  High Score  : " << highScore   << "\n";
            cout << "\n  Play again? (1=Yes / 0=No): ";
            int again; cin >> again; cin.ignore();
            playAgain = (again == 1);
            speed = (sp == 1) ? 250 : (sp == 2) ? 150 : 80;  // reset speed
        }
    }
};

// ═══════════════════════════════════════════════════════
//   MAIN MENU
// ═══════════════════════════════════════════════════════
int main() {
    srand((unsigned)time(nullptr));
    int choice;

    do {
        clearScreen();
        printBanner("MAIN MENU");
        cout << "\n"
             << "  1.  Tic Tac Toe  (PvP or vs AI)\n"
             << "  2.  Snake Game\n"
             << "  0.  Exit\n\n";

        choice = getInt("  Choose: ", 0, 2);

        if (choice == 1) {
            TicTacToe game;
            game.run();
        } else if (choice == 2) {
            Snake game;
            game.run();
        }
    } while (choice != 0);

    clearScreen();
    cout << "\n  Thanks for playing! Goodbye.\n\n";
    return 0;
}
