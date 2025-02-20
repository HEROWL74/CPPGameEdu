#include "DxLib.h"

typedef struct {
    VECTOR pos;
	unsigned int color; 
} Vertex;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    // DXライブラリの初期化
    if (DxLib_Init() == -1) {
        return -1; // 初期化に失敗した場合は終了
    }
    // 画面の描画先を裏画面に設定
    SetDrawScreen(DX_SCREEN_BACK);

    Vertex vertices[3]{
        {VGet(0.0f, 0.0f, 0.0f), GetColor(255, 0, 0) },
        { VGet(100.0f, 0.0f, 0.0f), GetColor(0, 255, 0) },
        { VGet(50.0f, 100.0f, 0.0f), GetColor(0, 0, 255)}
        };
    // メインループ
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
        // 描画処理
        ClearDrawScreen(); // 画面をクリア
        // ポリゴン描画処理をここに記述
		DrawTriangle(vertices[0].pos.x, vertices[0].pos.y,
			         vertices[1].pos.x, vertices[1].pos.y, 
			         vertices[2].pos.x, vertices[2].pos.y, vertices[0].color, TRUE);
        ScreenFlip(); // 裏画面を表画面に反映
    }
    // DXライブラリの終了処理
    DxLib_End();
    return 0;
}