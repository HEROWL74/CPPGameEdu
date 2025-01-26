#include "DxLib.h"
#include <vector>
#include <memory>

// 定数定義
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int PLAYER_SIZE = 20;
const int ENEMY_SIZE = 20;

// 敵キャラクターの構造体
struct Enemy {
    int x, y;   // 座標
    int speed;  // 移動速度

    // 敵の初期化
    Enemy(int startX, int startY, int moveSpeed) : x(startX), y(startY), speed(moveSpeed) {}

    // 敵の移動
    void move() {
        y += speed; // 下方向に移動
        if (y > WINDOW_HEIGHT) { // 画面外に出たら再配置
            y = -ENEMY_SIZE;
            x = rand() % (WINDOW_WIDTH - ENEMY_SIZE);
        }
    }

    // 敵の描画
    void draw() const {
        DrawBox(x, y, x + ENEMY_SIZE, y + ENEMY_SIZE, GetColor(255, 0, 0), TRUE);
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // ウィンドウモードに設定
    ChangeWindowMode(TRUE);

    // DXライブラリ初期化処理
    if (DxLib_Init() < 0) {
        return -1; // 初期化失敗で終了
    }

    // 描画先を裏画面に設定
    SetDrawScreen(DX_SCREEN_BACK);

    // 画面モードを設定
    SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);

    // プレイヤーの初期設定
    int playerX = WINDOW_WIDTH / 2;
    int playerY = WINDOW_HEIGHT - PLAYER_SIZE * 2;

    // 敵キャラクターの動的リスト（ポインタ管理）
    std::vector<std::unique_ptr<Enemy>> enemies;

    // 敵キャラクターを5体生成
    for (int i = 0; i < 5; i++) {
        enemies.push_back(std::make_unique<Enemy>(
            rand() % (WINDOW_WIDTH - ENEMY_SIZE), rand() % 200 - 200, 2 + rand() % 3));
    }

    // ゲームループ
    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
        // ESCキーで終了
        if (CheckHitKey(KEY_INPUT_ESCAPE)) {
            break;
        }

        // プレイヤーの操作
        if (CheckHitKey(KEY_INPUT_UP)) playerY -= 5;
        if (CheckHitKey(KEY_INPUT_DOWN)) playerY += 5;
        if (CheckHitKey(KEY_INPUT_LEFT)) playerX -= 5;
        if (CheckHitKey(KEY_INPUT_RIGHT)) playerX += 5;

        // プレイヤーが画面外に出ないよう制限
        if (playerX < 0) playerX = 0;
        if (playerX > WINDOW_WIDTH - PLAYER_SIZE) playerX = WINDOW_WIDTH - PLAYER_SIZE;
        if (playerY < 0) playerY = 0;
        if (playerY > WINDOW_HEIGHT - PLAYER_SIZE) playerY = WINDOW_HEIGHT - PLAYER_SIZE;

        // 敵の移動と当たり判定
        for (auto& enemy : enemies) {
            enemy->move(); // 敵の移動
            enemy->draw(); // 敵の描画

            // 当たり判定
            if (playerX < enemy->x + ENEMY_SIZE && playerX + PLAYER_SIZE > enemy->x &&
                playerY < enemy->y + ENEMY_SIZE && playerY + PLAYER_SIZE > enemy->y) {
                DrawString(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2, "Game Over!", GetColor(255, 0, 0));
                ScreenFlip();
                WaitKey();
                DxLib_End();
                return 0;
            }
        }

        // プレイヤーの描画
        DrawBox(playerX, playerY, playerX + PLAYER_SIZE, playerY + PLAYER_SIZE, GetColor(0, 255, 0), TRUE);

        // ゲームの説明文
        DrawFormatString(0, 0, GetColor(255, 255, 255), "ESCキーで終了");
    }

    // DXライブラリ終了処理
    DxLib_End();
    return 0;
}
