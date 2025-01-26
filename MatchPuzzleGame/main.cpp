#include "DxLib.h"
#include <vector>
#include <memory> // std::unique_ptr���g�����߂̃w�b�_�[

// �萔��`
const int WINDOW_WIDTH = 800;  // �E�B���h�E�̉���
const int WINDOW_HEIGHT = 600; // �E�B���h�E�̏c��
const int PLAYER_SIZE = 20;    // �v���C���[�̃T�C�Y
const int ENEMY_SIZE = 20;     // �G�L�����N�^�[�̃T�C�Y

// �G�L�����N�^�[��\���\����
struct Enemy {
    int x, y;   // �G�L�����N�^�[�̌��ݍ��W
    int speed;  // �G�L�����N�^�[�̈ړ����x

    // �R���X�g���N�^�F�G�̏��������ɍ��W�Ƒ��x��ݒ�
    Enemy(int startX, int startY, int moveSpeed)
        : x(startX), y(startY), speed(moveSpeed) {}

    // �G�L�����N�^�[�̈ړ�����
    void move() {
        y += speed; // �G�L�����N�^�[���������Ɉړ�������
        if (y > WINDOW_HEIGHT) { // ��ʊO�ɏo���ꍇ�A�㕔�Ƀ��Z�b�g����
            y = -ENEMY_SIZE; // �㕔�̊O�ɍĔz�u
            x = rand() % (WINDOW_WIDTH - ENEMY_SIZE); // �V�������ʒu�������_���ɐݒ�
        }
    }

    // �G�L�����N�^�[�̕`�揈��
    void draw() const {
        DrawBox(x, y, x + ENEMY_SIZE, y + ENEMY_SIZE, GetColor(255, 0, 0), TRUE); // �Ԃ��l�p�`�ŕ`��
    }
};

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

    // [2] �v���C���[�̏����ݒ�
    int playerX = WINDOW_WIDTH / 2;         // �v���C���[�̏����ʒu�i���j
    int playerY = WINDOW_HEIGHT - PLAYER_SIZE * 2; // �v���C���[�̏����ʒu�i�c�j

    // [3] �G�L�����N�^�[�̓��I���X�g�i�|�C���^�Ǘ��j
    // �G�L�����N�^�[���i�[���铮�I�z��istd::vector�j���쐬
    std::vector<std::unique_ptr<Enemy>> enemies;

    // �����̓G�L�����N�^�[��5�̐������ă��X�g�ɒǉ�
    for (int i = 0; i < 5; i++) {
        enemies.push_back(std::make_unique<Enemy>(
            rand() % (WINDOW_WIDTH - ENEMY_SIZE), // �����_����X���W
            rand() % 200 - 200,                  // ��ʊO�i�㕔�j�̃����_����Y���W
            2 + rand() % 3                       // �����_���ȑ��x�i2�`4�j
        ));
    }

    // [4] �Q�[�����[�v
    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
        // ESC�L�[�������ꂽ�ꍇ�A�Q�[�����I��
        if (CheckHitKey(KEY_INPUT_ESCAPE)) {
            break;
        }

        // [5] �v���C���[�̈ړ�����
        if (CheckHitKey(KEY_INPUT_UP)) playerY -= 5;   // ������ړ�
        if (CheckHitKey(KEY_INPUT_DOWN)) playerY += 5; // �������ړ�
        if (CheckHitKey(KEY_INPUT_LEFT)) playerX -= 5; // �������ړ�
        if (CheckHitKey(KEY_INPUT_RIGHT)) playerX += 5; // �E�����ړ�

        // �v���C���[����ʊO�ɏo�Ȃ��悤�ɂ��邽�߂̐���
        if (playerX < 0) playerX = 0;
        if (playerX > WINDOW_WIDTH - PLAYER_SIZE) playerX = WINDOW_WIDTH - PLAYER_SIZE;
        if (playerY < 0) playerY = 0;
        if (playerY > WINDOW_HEIGHT - PLAYER_SIZE) playerY = WINDOW_HEIGHT - PLAYER_SIZE;

        // [6] �G�L�����N�^�[�̈ړ��ƕ`��
        for (auto& enemy : enemies) {
            enemy->move(); // �G�L�����N�^�[���ړ�
            enemy->draw(); // �G�L�����N�^�[��`��

            // [7] �����蔻��i�v���C���[�ƓG�̏Փ˂𔻒�j
            if (playerX < enemy->x + ENEMY_SIZE && playerX + PLAYER_SIZE > enemy->x &&
                playerY < enemy->y + ENEMY_SIZE && playerY + PLAYER_SIZE > enemy->y) {
                // �Փˎ��̏����i�Q�[���I�[�o�[��ʕ\���j
                DrawString(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2, "Game Over!", GetColor(255, 0, 0));
                ScreenFlip(); // �\��ʂɔ��f
                WaitKey();    // �L�[���͂�ҋ@
                DxLib_End();  // DX���C�u�����I������
                return 0;     // �v���O�����I��
            }
        }

        // [8] �v���C���[�̕`��
        DrawBox(playerX, playerY, playerX + PLAYER_SIZE, playerY + PLAYER_SIZE, GetColor(0, 255, 0), TRUE);

        // [9] �Q�[���̐�������`��i��ʍ���ɕ\���j
        DrawFormatString(0, 0, GetColor(255, 255, 255), "ESC�L�[�ŏI��");
    }

    // [10] DX���C�u�����I������
    DxLib_End();
    return 0;
}
