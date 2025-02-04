#include "DxLib.h"
#include <vector>
#include <memory> // std::unique_ptrを使うためのヘッダー

// 定数定義
const int WINDOW_WIDTH = 800;  // ウィンドウの横幅
const int WINDOW_HEIGHT = 600; // ウィンドウの縦幅


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

  

    

    // [4] ゲームループ
    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
        // ESCキーが押された場合、ゲームを終了
        if (CheckHitKey(KEY_INPUT_ESCAPE)) {
            break;
        }

     
    }

    // [10] DXライブラリ終了処理
    DxLib_End();
    return 0;
}
