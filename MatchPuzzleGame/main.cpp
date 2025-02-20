#include "DxLib.h"

typedef struct {
    VECTOR pos;
	unsigned int color; 
} Vertex;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    // DX���C�u�����̏�����
    if (DxLib_Init() == -1) {
        return -1; // �������Ɏ��s�����ꍇ�͏I��
    }
    // ��ʂ̕`���𗠉�ʂɐݒ�
    SetDrawScreen(DX_SCREEN_BACK);

    Vertex vertices[3]{
        {VGet(0.0f, 0.0f, 0.0f), GetColor(255, 0, 0) },
        { VGet(100.0f, 0.0f, 0.0f), GetColor(0, 255, 0) },
        { VGet(50.0f, 100.0f, 0.0f), GetColor(0, 0, 255)}
        };
    // ���C�����[�v
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
        // �`�揈��
        ClearDrawScreen(); // ��ʂ��N���A
        // �|���S���`�揈���������ɋL�q
		DrawTriangle(vertices[0].pos.x, vertices[0].pos.y,
			         vertices[1].pos.x, vertices[1].pos.y, 
			         vertices[2].pos.x, vertices[2].pos.y, vertices[0].color, TRUE);
        ScreenFlip(); // ����ʂ�\��ʂɔ��f
    }
    // DX���C�u�����̏I������
    DxLib_End();
    return 0;
}