#include "DxLib.h"
#include <vector>
#include <memory> // std::unique_ptrを使うためのヘッダー

// 定数定義
const int WINDOW_WIDTH = 800;  // ウィンドウの横幅
const int WINDOW_HEIGHT = 600; // ウィンドウの縦幅
const int PLAYER_SIZE = 20;    // プレイヤーのサイズ
const int ENEMY_SIZE = 20;     // 敵キャラクターのサイズ

// 敵キャラクターを表す構造体
struct Enemy {
    int x, y;   // 敵キャラクターの現在座標
    int speed;  // 敵キャラクターの移動速度

    // コンストラクタ：敵の初期化時に座標と速度を設定
    Enemy(int startX, int startY, int moveSpeed)
        : x(startX), y(startY), speed(moveSpeed) {}

    // 敵キャラクターの移動処理
    void move() {
        y += speed; // 敵キャラクターを下方向に移動させる
        if (y > WINDOW_HEIGHT) { // 画面外に出た場合、上部にリセットする
            y = -ENEMY_SIZE; // 上部の外に再配置
            x = rand() % (WINDOW_WIDTH - ENEMY_SIZE); // 新しい横位置をランダムに設定
        }
    }

    // 敵キャラクターの描画処理
    void draw() const {
        DrawBox(x, y, x + ENEMY_SIZE, y + ENEMY_SIZE, GetColor(255, 0, 0), TRUE); // 赤い四角形で描画
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // [1] DXライブラリ初期化関連処理
    // ウィンドウモードに設定（フルスクリーンにしない）
    ChangeWindowMode(TRUE);

    // DXライブラリの初期化（失敗したら終了）
    if (DxLib_Init() < 0) {
        return -1;
    }

    // 描画先を裏画面に設定（ダブルバッファリングのため）
    SetDrawScreen(DX_SCREEN_BACK);

    // 画面モードを設定（ウィンドウサイズ800x600、32bitカラー）
    SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);

    // [2] プレイヤーの初期設定
    int playerX = WINDOW_WIDTH / 2;         // プレイヤーの初期位置（横）
    int playerY = WINDOW_HEIGHT - PLAYER_SIZE * 2; // プレイヤーの初期位置（縦）

    // [3] 敵キャラクターの動的リスト（ポインタ管理）
    // 敵キャラクターを格納する動的配列（std::vector）を作成
    std::vector<std::unique_ptr<Enemy>> enemies;

    // 初期の敵キャラクターを5体生成してリストに追加
    for (int i = 0; i < 5; i++) {
        enemies.push_back(std::make_unique<Enemy>(
            rand() % (WINDOW_WIDTH - ENEMY_SIZE), // ランダムなX座標
            rand() % 200 - 200,                  // 画面外（上部）のランダムなY座標
            2 + rand() % 3                       // ランダムな速度（2～4）
        ));
    }

    // [4] ゲームループ
    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
        // ESCキーが押された場合、ゲームを終了
        if (CheckHitKey(KEY_INPUT_ESCAPE)) {
            break;
        }

        // [5] プレイヤーの移動処理
        if (CheckHitKey(KEY_INPUT_UP)) playerY -= 5;   // 上方向移動
        if (CheckHitKey(KEY_INPUT_DOWN)) playerY += 5; // 下方向移動
        if (CheckHitKey(KEY_INPUT_LEFT)) playerX -= 5; // 左方向移動
        if (CheckHitKey(KEY_INPUT_RIGHT)) playerX += 5; // 右方向移動

        // プレイヤーが画面外に出ないようにするための制限
        if (playerX < 0) playerX = 0;
        if (playerX > WINDOW_WIDTH - PLAYER_SIZE) playerX = WINDOW_WIDTH - PLAYER_SIZE;
        if (playerY < 0) playerY = 0;
        if (playerY > WINDOW_HEIGHT - PLAYER_SIZE) playerY = WINDOW_HEIGHT - PLAYER_SIZE;

        // [6] 敵キャラクターの移動と描画
        for (auto& enemy : enemies) {
            enemy->move(); // 敵キャラクターを移動
            enemy->draw(); // 敵キャラクターを描画

            // [7] 当たり判定（プレイヤーと敵の衝突を判定）
            if (playerX < enemy->x + ENEMY_SIZE && playerX + PLAYER_SIZE > enemy->x &&
                playerY < enemy->y + ENEMY_SIZE && playerY + PLAYER_SIZE > enemy->y) {
                // 衝突時の処理（ゲームオーバー画面表示）
                DrawString(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2, "Game Over!", GetColor(255, 0, 0));
                ScreenFlip(); // 表画面に反映
                WaitKey();    // キー入力を待機
                DxLib_End();  // DXライブラリ終了処理
                return 0;     // プログラム終了
            }
        }

        // [8] プレイヤーの描画
        DrawBox(playerX, playerY, playerX + PLAYER_SIZE, playerY + PLAYER_SIZE, GetColor(0, 255, 0), TRUE);

        // [9] ゲームの説明文を描画（画面左上に表示）
        DrawFormatString(0, 0, GetColor(255, 255, 255), "ESCキーで終了");
    }

    // [10] DXライブラリ終了処理
    DxLib_End();
    return 0;
}
