//==========================================================================
// X�C���v�b�g[XInput.h]
// author: MasyoLab
//==========================================================================
#pragma once

//==========================================================================
// include
//==========================================================================
#pragma comment (lib, "xinput.lib") // ���C�u�����̃C���|�[�g
#include <Windows.h>
#include <XInput.h>
#include <d3d9.h>
#include <d3dx9.h>

//==========================================================================
//
// class  : CXInputBool4
// Content: CXInputBool4
//
//==========================================================================
class CXInputBool4
{
public:
    CXInputBool4();
    ~CXInputBool4();
public:
    bool m_up, m_down, m_left, m_right;
};

//==========================================================================
//
// class  : CXInputAnalogTrigger
// Content: �A�i���O�X�e�B�b�N�̃g���K�[
//
//==========================================================================
class CXInputAnalogTrigger
{
public:
    CXInputAnalogTrigger();
    ~CXInputAnalogTrigger();
public:
    /**
    @brief �X�V
    */
    void update(void);

    /**
    @brief �؂�ւ�
    @param input [in] ����L�[
    @param Out [in/out] �Ώ�
    */
    void ifset(bool input, bool * Out);

    /**
    @brief ��r in1 < in2
    @param in1 [in] �Ώ�1
    @param in2 [in/out] �Ώ�2
    @return in2 ���傫���ꍇ true
    */
    bool ifbool(float in1, float in2);
public:
    CXInputBool4 m_Ltrigger; // ���A�i���O�̃g���K�[
    CXInputBool4 m_LtriggerOld; // ���A�i���O�̌Â��g���K�[
    CXInputBool4 m_Rtrigger; // �E�A�i���O�̃g���K�[
    CXInputBool4 m_RtriggerOld; // �E�A�i���O�̌Â��g���K�[
};

//==========================================================================
//
// class  : CXInput
// Content: X�C���v�b�g
//
//==========================================================================
class CXInput
{
private:
    // �R�s�[�֎~ (C++11)
    CXInput(const CXInput &) = delete;
    CXInput &operator=(const CXInput &) = delete;
public:
    // �A�i���O�̃{�^��
    enum class EAnalog
    {
        UP, // ��
        DOWN, // ��
        LEFT, // ��
        RIGHT, // �E
    };
    // xBox�̉�����{�^��
    enum class EButton
    {
        DPAD_UP = XINPUT_GAMEPAD_DPAD_UP, // �\���{�^�� ��
        DPAD_DOWN = XINPUT_GAMEPAD_DPAD_DOWN, // �\���{�^�� ��
        DPAD_LEFT = XINPUT_GAMEPAD_DPAD_LEFT, // �\���{�^�� ��
        DPAD_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT, // �\���{�^�� �E
        START = XINPUT_GAMEPAD_START, // START�{�^��
        BACK = XINPUT_GAMEPAD_BACK, // BACK�{�^��
        LEFT_THUMB = XINPUT_GAMEPAD_LEFT_THUMB, // ���A�i���O�X�e�B�b�N�̃{�^��
        RIGHT_THUMB = XINPUT_GAMEPAD_RIGHT_THUMB, // �E�A�i���O�X�e�B�b�N�̃{�^��
        LEFT_LB = XINPUT_GAMEPAD_LEFT_SHOULDER, // LB�{�^��
        RIGHT_RB = XINPUT_GAMEPAD_RIGHT_SHOULDER, // RB�{�^��
        A = XINPUT_GAMEPAD_A, // A�{�^��
        B = XINPUT_GAMEPAD_B, // B�{�^��
        X = XINPUT_GAMEPAD_X, // X�{�^��
        Y = XINPUT_GAMEPAD_Y, // Y�{�^��
    };
public:
    CXInput();
    ~CXInput();

    /**
    @brief ������
    @param Num [in] ���R���g���[���[�̐�
    @return ���s�� true
    */
    bool Init(int Num);

    /**
    @brief ���
    */
    void Release(void);

    /**
    @brief �X�V
    */
    void Update(void);

    /**
    @brief �v���X
    @param button [in] �{�^��ID
    @param index [in] �����R���g���[���[
    @return �����Ă���Ƃ� true
    */
    bool Press(EButton button, int index);

    /**
    @brief �g���K�[
    @param button [in] �{�^��ID
    @param index [in] �����R���g���[���[
    @return �����Ă���Ƃ� true
    */
    bool Trigger(EButton button, int index);

    /**
    @brief �����[�X
    @param button [in] �{�^��ID
    @param index [in] �����R���g���[���[
    @return �����Ă���Ƃ� true
    */
    bool Release(EButton button, int index);

    /**
    @brief ���A�i���O�X�e�B�b�N
    @param index [in] �����R���g���[���[
    @param Out [in] �X���x�N�g��
    @return �����Ă���Ƃ� true
    */
    bool AnalogL(int index, D3DXVECTOR3 & Out);

    /**
    @brief ���A�i���O�X�e�B�b�N
    @param index [in] �����R���g���[���[
    @return �����Ă���Ƃ� true
    */
    bool AnalogL(int index);

    /**
    @brief �E�A�i���O�X�e�B�b�N
    @param index [in] �����R���g���[���[
    @param Out [in] �X���x�N�g��
    @return �����Ă���Ƃ� true
    */
    bool AnalogR(int index, D3DXVECTOR3 & Out);

    /**
    @brief �E�A�i���O�X�e�B�b�N
    @param index [in] �����R���g���[���[
    @return �����Ă���Ƃ� true
    */
    bool AnalogR(int index);

    /**
    @brief ���A�i���O�X�e�B�b�N�̃g���K�[
    @param key [in] �{�^��ID
    @param index [in] �����R���g���[���[
    @return �����Ă���Ƃ� true
    */
    bool AnalogLTrigger(EAnalog key, int index);

    /**
    @brief ���A�i���O�X�e�B�b�N�̃g���K�[
    @param index [in] �����R���g���[���[
    @param key [in] �{�^��ID
    @param Out [in] �X���x�N�g��
    @return �����Ă���Ƃ� true
    */
    bool AnalogLTrigger(EAnalog key, int index, D3DXVECTOR3 * Out);

    /**
    @brief �E�A�i���O�X�e�B�b�N�̃g���K�[
    @param index [in] �����R���g���[���[
    @param key [in] �{�^��ID
    @return �����Ă���Ƃ� true
    */
    bool AnalogRTrigger(EAnalog key, int index);

    /**
    @brief �E�A�i���O�X�e�B�b�N�̃g���K�[
    @param index [in] �����R���g���[���[
    @param key [in] �{�^��ID
    @param Out [in] �X���x�N�g��
    @return �����Ă���Ƃ� true
    */
    bool AnalogRTrigger(EAnalog key, int index, D3DXVECTOR3 * Out);

    /**
    @brief ���g���K�[�{�^��
    @param index [in] �����R���g���[���[
    @return �����Ă���Ƃ� true
    */
    bool LT(int index);

    /**
    @brief �E�g���K�[�{�^��
    @param index [in] �����R���g���[���[
    @return �����Ă���Ƃ� true
    */
    bool RT(int index);

    /**
    @brief �R���g���[���[�̑��݂̊m�F
    @param index [in] �����R���g���[���[
    @return ���݂���ꍇ true
    */
    bool Check(int index);

    /**
    @brief �R���g���[���[�̌��݂̏�Ԃ��擾
    @param index [in] �����R���g���[���[
    @return �C���X�^���X
    */
    XINPUT_STATE* GetState(int index);

    /**
    @brief �T�C�Y
    @return �R���g���[���[��
    */
    int Size(void);
private:
    /**
    @brief �R���g���[���̃L�[�g���K�[
    @param bNew [in] �V��������L�[
    @param bOld [in] �Â�����L�[
    @return ����
    */
    bool KeyTrigger(bool bNew, bool bOld);

    /**
    @brief �R���g���[���̃L�[�����[�X
    @param bNew [in] �V��������L�[
    @param bOld [in] �Â�����L�[
    @return ����
    */
    bool KeyRelease(bool bNew, bool bOld);
private:
    XINPUT_STATE * m_state; // �R���g���[���[�̃X�e�[�^�X
    XINPUT_STATE * m_stateOld; // �Â����
    bool * m_statedat; //�p�b�h�̗L��
    CXInputAnalogTrigger * m_trigger; // �A�i���O�̃g���K�[
    int m_numdata; // �o�^���ꂽ�f�[�^��
};
