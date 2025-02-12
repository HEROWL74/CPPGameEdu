#include "DxLib.h"
#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <mutex>
#include <condition_variable>

constexpr int GRID_SIZE = 8;
constexpr int TILE_SIZE = 64;
constexpr int TILE_TYPES = 5;

std::vector<std::vector<int>> board(GRID_SIZE, std::vector<int>(GRID_SIZE));
std::mutex boardMutex;

int selectedX = -1, selectedY = -1;

void InitBoard() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, TILE_TYPES);
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            board[y][x] = dis(gen);
        }
    }
}

bool CheckMatch() {
    std::lock_guard<std::mutex> lock(boardMutex);
    bool matched = false;
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE - 2; ++x) {
            if (board[y][x] == board[y][x + 1] && board[y][x] == board[y][x + 2]) {
                board[y][x] = board[y][x + 1] = board[y][x + 2] = 0;
                matched = true;
            }
        }
    }
    return matched;
}

void DropTiles() {
    for (int x = 0; x < GRID_SIZE; ++x) {
        for (int y = GRID_SIZE - 1; y > 0; --y) {
            if (board[y][x] == 0) {
                for (int k = y; k > 0; --k) {
                    board[k][x] = board[k - 1][x];
                }
                board[0][x] = (rand() % TILE_TYPES) + 1;
            }
        }
    }
}

void SwapTiles(int x1, int y1, int x2, int y2) {
    std::swap(board[y1][x1], board[y2][x2]);
    if (!CheckMatch()) {
        std::swap(board[y1][x1], board[y2][x2]); // Œ³‚É–ß‚·
    }
    else {
        DropTiles();
    }
}

void HandleMouseInput() {
    int mx, my;
    if (GetMouseInput() & MOUSE_INPUT_LEFT) {
        GetMousePoint(&mx, &my);
        int x = mx / TILE_SIZE;
        int y = my / TILE_SIZE;
        if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
            if (selectedX == -1 && selectedY == -1) {
                selectedX = x;
                selectedY = y;
            }
            else {
                if ((abs(selectedX - x) == 1 && selectedY == y) || (abs(selectedY - y) == 1 && selectedX == x)) {
                    SwapTiles(selectedX, selectedY, x, y);
                }
                selectedX = -1;
                selectedY = -1;
            }
        }
    }
}

void DrawBoard() {
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            int color;
            if (board[y][x] == 1) color = GetColor(255, 0, 0);    // Ô
            else if (board[y][x] == 2) color = GetColor(0, 0, 255); // Â
            else if (board[y][x] == 3) color = GetColor(0, 255, 0); // —Î
            else color = GetColor(0, 0, 0); // Á‹ŽŒã‚Ì•

            DrawBox(x * TILE_SIZE, y * TILE_SIZE, (x + 1) * TILE_SIZE, (y + 1) * TILE_SIZE, color, TRUE);
        }
    }
}

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE , _In_ LPSTR,_In_ int)
{
    ChangeWindowMode(TRUE);
    if (DxLib_Init() == -1) return -1;
    SetDrawScreen(DX_SCREEN_BACK);
    SetGraphMode(800, 600, 32);

    InitBoard();

    while (ProcessMessage() == 0) {
        ClearDrawScreen();
        HandleMouseInput();
        DrawBoard();
        ScreenFlip();

        if (CheckMatch()) {
            DropTiles();
        }
    }

    DxLib_End();
    return 0;
}