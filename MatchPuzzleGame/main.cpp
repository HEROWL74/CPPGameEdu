#include "DxLib.h"

// �萔��`
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int PLAYER_SIZE = 20;
const int ENEMY_SIZE = 20;
const int ENEMY_COUNT = 5;

struct Enemy {
    int x, y;
    int speed;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // �E�B���h�E���[�h�ɐݒ�
    ChangeWindowMode(TRUE);

    // DX���C�u��������������
    if (DxLib_Init() < 0) {
        return -1; // ���������s�ŏI��
    }

    // �`���𗠉�ʂɐݒ�
    SetDrawScreen(DX_SCREEN_BACK);

    // ��ʃ��[�h��ݒ�
    SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);

    // �v���C���[�̏����ݒ�
    int playerX = WINDOW_WIDTH / 2;
    int playerY = WINDOW_HEIGHT - PLAYER_SIZE * 2;

    // �G�L�����N�^�[�̏����ݒ�i�z����g�p�j
    Enemy enemies[ENEMY_COUNT];
    for (int i = 0; i < ENEMY_COUNT; i++) {
        enemies[i] = { rand() % (WINDOW_WIDTH - ENEMY_SIZE), rand() % 200 - 200, 2 + rand() % 3 };
    }

    // �Q�[�����[�v
    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
        // ESC�L�[�ŏI��
        if (CheckHitKey(KEY_INPUT_ESCAPE)) {
            break;
        }

        // �v���C���[�̑���
        if (CheckHitKey(KEY_INPUT_UP)) playerY -= 5;
        if (CheckHitKey(KEY_INPUT_DOWN)) playerY += 5;
        if (CheckHitKey(KEY_INPUT_LEFT)) playerX -= 5;
        if (CheckHitKey(KEY_INPUT_RIGHT)) playerX += 5;

        // �v���C���[����ʊO�ɏo�Ȃ��悤����
        if (playerX < 0) playerX = 0;
        if (playerX > WINDOW_WIDTH - PLAYER_SIZE) playerX = WINDOW_WIDTH - PLAYER_SIZE;
        if (playerY < 0) playerY = 0;
        if (playerY > WINDOW_HEIGHT - PLAYER_SIZE) playerY = WINDOW_HEIGHT - PLAYER_SIZE;

        // �G�̈ړ��ƃ��Z�b�g
        for (int i = 0; i < ENEMY_COUNT; i++) {
            enemies[i].y += enemies[i].speed;
            if (enemies[i].y > WINDOW_HEIGHT) {
                enemies[i].y = -ENEMY_SIZE;
                enemies[i].x = rand() % (WINDOW_WIDTH - ENEMY_SIZE);
            }
        }

        // �����蔻��
        for (int i = 0; i < ENEMY_COUNT; i++) {
            if (playerX < enemies[i].x + ENEMY_SIZE && playerX + PLAYER_SIZE > enemies[i].x &&
                playerY < enemies[i].y + ENEMY_SIZE && playerY + PLAYER_SIZE > enemies[i].y) {
                DrawString(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2, "Game Over!", GetColor(255, 0, 0));
                ScreenFlip();
                WaitKey();
                DxLib_End();
                return 0;
            }
        }

        // �v���C���[�̕`��
        DrawBox(playerX, playerY, playerX + PLAYER_SIZE, playerY + PLAYER_SIZE, GetColor(0, 255, 0), TRUE);

        // �G�L�����N�^�[�̕`��
        for (int i = 0; i < ENEMY_COUNT; i++) {
            DrawBox(enemies[i].x, enemies[i].y, enemies[i].x + ENEMY_SIZE, enemies[i].y + ENEMY_SIZE, GetColor(255, 0, 0), TRUE);
        }

        // �Q�[���̐�����
        DrawFormatString(0, 0, GetColor(255, 255, 255), "ESC�L�[�ŏI��");
    }

    // DX���C�u�����I������
    DxLib_End();
    return 0;
}
