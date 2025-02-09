#include "DxLib.h"




// ******************************
// メイン関数
// ******************************
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // DXライブラリの初期化
    ChangeWindowMode(TRUE);  // ウィンドウモード
    if (DxLib_Init() == -1) return -1;  // 初期化失敗時は終了
    SetDrawScreen(DX_SCREEN_BACK); // ダブルバッファリング設定

    // モデルのロード
    int modelHandle = MV1LoadModel("player.x");
    if (modelHandle == -1) {
        DxLib_End();
        return -1;
    }

    // カメラの設定
    SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 20.0f, -50.0f), VGet(0.0f, 10.0f, 0.0f));

    VECTOR playerPos = VGet(0.0f, 0.0f, 0.0f);
    
    const float moveSpeed = 2.0f;

    // メインループ
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        if (CheckHitKey(KEY_INPUT_UP))    playerPos.z += moveSpeed; // 前進
        if (CheckHitKey(KEY_INPUT_DOWN))  playerPos.z -= moveSpeed; // 後退
        if (CheckHitKey(KEY_INPUT_LEFT))  playerPos.x -= moveSpeed; // 左へ移動
        if (CheckHitKey(KEY_INPUT_RIGHT)) playerPos.x += moveSpeed; // 右へ移動

        // モデルの位置を更新
        MV1SetPosition(modelHandle, playerPos);

        ClearDrawScreen();  // 画面クリア

        // モデルの描画
        MV1DrawModel(modelHandle);

        ScreenFlip();
    }

    // モデルの削除
    MV1DeleteModel(modelHandle);

    // DXライブラリの終了
    DxLib_End();

    return 0;
}