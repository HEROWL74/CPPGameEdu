#include "DxLib.h"
#include <vector>
#include <memory> // std::unique_ptr���g�����߂̃w�b�_�[

// �萔��`
const int WINDOW_WIDTH = 800;  // �E�B���h�E�̉���
const int WINDOW_HEIGHT = 600; // �E�B���h�E�̏c��


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // [1] DX���C�u�����������֘A����
    // �E�B���h�E���[�h�ɐݒ�i�t���X�N���[���ɂ��Ȃ��j
    ChangeWindowMode(TRUE);

    // DX���C�u�����̏������i���s������I���j
    if (DxLib_Init() < 0) {
        return -1;
    }

    // �`���𗠉�ʂɐݒ�i�_�u���o�b�t�@�����O�̂��߁j
    SetDrawScreen(DX_SCREEN_BACK);

    // ��ʃ��[�h��ݒ�i�E�B���h�E�T�C�Y800x600�A32bit�J���[�j
    SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);

  

    

    // [4] �Q�[�����[�v
    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
        // ESC�L�[�������ꂽ�ꍇ�A�Q�[�����I��
        if (CheckHitKey(KEY_INPUT_ESCAPE)) {
            break;
        }

     
    }

    // [10] DX���C�u�����I������
    DxLib_End();
    return 0;
}
