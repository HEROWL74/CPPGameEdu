#include "DxLib.h"




// ******************************
// ���C���֐�
// ******************************
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // DX���C�u�����̏�����
    ChangeWindowMode(TRUE);  // �E�B���h�E���[�h
    if (DxLib_Init() == -1) return -1;  // ���������s���͏I��
    SetDrawScreen(DX_SCREEN_BACK); // �_�u���o�b�t�@�����O�ݒ�

    // ���f���̃��[�h
    int modelHandle = MV1LoadModel("player.x");
    if (modelHandle == -1) {
        DxLib_End();
        return -1;
    }

    // �J�����̐ݒ�
    SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 20.0f, -50.0f), VGet(0.0f, 10.0f, 0.0f));

    VECTOR playerPos = VGet(0.0f, 0.0f, 0.0f);
    
    const float moveSpeed = 2.0f;

    // ���C�����[�v
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        if (CheckHitKey(KEY_INPUT_UP))    playerPos.z += moveSpeed; // �O�i
        if (CheckHitKey(KEY_INPUT_DOWN))  playerPos.z -= moveSpeed; // ���
        if (CheckHitKey(KEY_INPUT_LEFT))  playerPos.x -= moveSpeed; // ���ֈړ�
        if (CheckHitKey(KEY_INPUT_RIGHT)) playerPos.x += moveSpeed; // �E�ֈړ�

        // ���f���̈ʒu���X�V
        MV1SetPosition(modelHandle, playerPos);

        ClearDrawScreen();  // ��ʃN���A

        // ���f���̕`��
        MV1DrawModel(modelHandle);

        ScreenFlip();
    }

    // ���f���̍폜
    MV1DeleteModel(modelHandle);

    // DX���C�u�����̏I��
    DxLib_End();

    return 0;
}