//==========================================================================
// X�C���v�b�g[XInput.cpp]
// author: MasyoLab
//==========================================================================
#include "XInput.h"

CXInputBool4::CXInputBool4()
{
    this->m_up = this->m_down = this->m_left = this->m_right = false;
}

CXInputBool4::~CXInputBool4()
{
}

CXInput::CXInput()
{
    this->m_state = nullptr;
    this->m_stateOld = nullptr;
    this->m_statedat = nullptr;
    this->m_trigger = nullptr;
    this->m_numdata = 0;
}

CXInput::~CXInput()
{
    this->Release();
}

//==========================================================================
/**
@brief ������
@param Num [in] ���R���g���[���[�̐�
@return ���s�� true
*/
bool CXInput::Init(int Num)
{
	DWORD dwResult;
	if (this->m_state != nullptr)
	{
		delete[]this->m_state;
        this->m_state = nullptr;
	}
	if (this->m_stateOld != nullptr)
	{
		delete[]this->m_stateOld;
        this->m_stateOld = nullptr;
	}
	if (this->m_statedat != nullptr)
	{
		delete[]this->m_statedat;
        this->m_statedat = nullptr;
	}
	if (this->m_trigger != nullptr)
	{
		delete[]this->m_trigger;
        this->m_trigger = nullptr;
	}

	// �o�^���̑��
    this->m_numdata = Num;
    this->m_statedat = new bool[m_numdata];
    this->m_state = new XINPUT_STATE[m_numdata];
    this->m_stateOld = new XINPUT_STATE[m_numdata];
    this->m_trigger = new CXInputAnalogTrigger[m_numdata];

	for (int i = 0; i < this->m_numdata; i++)
	{
		// ���g�����ꂢ�ɂ���
		ZeroMemory(&this->m_state[i], sizeof(XINPUT_STATE));

		// �R���g���[���[�̏��̎擾
		dwResult = XInputGetState((DWORD)i, &this->m_state[i]);
		if (dwResult != ERROR_SUCCESS)
		{
            this->m_statedat[i] = false;
			break;
		}
		else
		{
            this->m_statedat[i] = true;
		}
	}

	return false;
}

//==========================================================================
/**
@brief ���
*/
void CXInput::Release(void)
{
	if (this->m_state != nullptr)
	{
		delete[]this->m_state;
        this->m_state = nullptr;
	}
	if (this->m_stateOld != nullptr)
	{
		delete[]this->m_stateOld;
        this->m_stateOld = nullptr;
	}
	if (this->m_statedat != nullptr)
	{
		delete[]this->m_statedat;
        this->m_statedat = nullptr;
	}
	if (this->m_trigger != nullptr)
	{
		delete[]this->m_trigger;
        this->m_trigger = nullptr;
	}
}

//==========================================================================
/**
@brief �X�V
*/
void CXInput::Update(void)
{
	XINPUT_STATE * pnew = nullptr;
	XINPUT_STATE * pold = nullptr;
    CXInputAnalogTrigger * panatri = nullptr;

	for (int i = 0; i < this->m_numdata; i++)
	{
		// �A�N�Z�X���x�̌���
		pnew = &this->m_state[i];
		pold = &this->m_stateOld[i];
		panatri = &this->m_trigger[i];

		// �R���g���[�����̕���
		*pold = *pnew;

		// �R���g���[���[�̏��̎擾
		XInputGetState(i, pnew);

		// ���X�e�B�b�N�̃f�b�h�]�[��
		if ((pnew->Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && pnew->Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
			(pnew->Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && pnew->Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		{
			pnew->Gamepad.sThumbLX = 0;
			pnew->Gamepad.sThumbLY = 0;
		}

		// �E�X�e�B�b�N�̃f�b�h�]�[��
		if ((pnew->Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pnew->Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
			(pnew->Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pnew->Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
		{
			pnew->Gamepad.sThumbRX = 0;
			pnew->Gamepad.sThumbRY = 0;
		}

		// �A�i���O�X�e�B�b�N�g���K�[�p�X�V
		panatri->update();

		// ���A�i���O�̃g���K�[����
		if (pnew->Gamepad.sThumbLY > /*XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE*/(SHORT)31000 ||
			pnew->Gamepad.sThumbLY < -/*XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE*/(SHORT)31000 ||
			pnew->Gamepad.sThumbLX < -/*XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE*/(SHORT)31000 ||
			pnew->Gamepad.sThumbLX > /*XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE*/(SHORT)31000)
		{
			panatri->ifset(panatri->ifbool((1.41421356f / 2.0f), (float)pnew->Gamepad.sThumbLY), &panatri->m_Ltrigger.m_up);
			panatri->ifset(panatri->ifbool((float)pnew->Gamepad.sThumbLY, -(1.41421356f / 2.0f)), &panatri->m_Ltrigger.m_down);
			panatri->ifset(panatri->ifbool((float)pnew->Gamepad.sThumbLX, -(1.41421356f / 2.0f)), &panatri->m_Ltrigger.m_left);
			panatri->ifset(panatri->ifbool((1.41421356f / 2.0f), (float)pnew->Gamepad.sThumbLX), &panatri->m_Ltrigger.m_right);
		}
		else
		{
			panatri->m_Ltrigger = CXInputBool4();
		}

		// �E�A�i���O�̃g���K�[����
		if (pnew->Gamepad.sThumbRY > /*XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE*/(SHORT)31000 ||
			pnew->Gamepad.sThumbRY < -/*XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE*/(SHORT)31000 ||
			pnew->Gamepad.sThumbRX < -/*XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE*/(SHORT)31000 ||
			pnew->Gamepad.sThumbRX > /*XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE*/(SHORT)31000)
		{
			panatri->ifset(panatri->ifbool((1.41421356f / 2.0f), (float)pnew->Gamepad.sThumbRY), &panatri->m_Rtrigger.m_up);
			panatri->ifset(panatri->ifbool((float)pnew->Gamepad.sThumbRY, -(1.41421356f / 2.0f)), &panatri->m_Rtrigger.m_down);
			panatri->ifset(panatri->ifbool((float)pnew->Gamepad.sThumbRX, -(1.41421356f / 2.0f)), &panatri->m_Rtrigger.m_left);
			panatri->ifset(panatri->ifbool((1.41421356f / 2.0f), (float)pnew->Gamepad.sThumbRX), &panatri->m_Rtrigger.m_right);
		}
		else
		{
			panatri->m_Rtrigger = CXInputBool4();
		}
	}
}

//==========================================================================
/**
@brief �v���X
@param button [in] �{�^��ID
@param index [in] �����R���g���[���[
@return �����Ă���Ƃ� true
*/
bool CXInput::Press(EButton button, int index)
{
	return (this->m_state[index].Gamepad.wButtons & (WORD)button) ? true : false;
}

//==========================================================================
/**
@brief �g���K�[
@param button [in] �{�^��ID
@param index [in] �����R���g���[���[
@return �����Ă���Ƃ� true
*/
bool CXInput::Trigger(EButton button, int index)
{
	return this->KeyTrigger((bool)((this->m_state[index].Gamepad.wButtons & (WORD)button) ? true : false), (bool)((this->m_stateOld[index].Gamepad.wButtons & (WORD)button) ? true : false));
}

//==========================================================================
/**
@brief �����[�X
@param button [in] �{�^��ID
@param index [in] �����R���g���[���[
@return �����Ă���Ƃ� true
*/
bool CXInput::Release(EButton button, int index)
{
	return this->KeyRelease((this->m_state[index].Gamepad.wButtons & (WORD)button) ? true : false, (this->m_stateOld[index].Gamepad.wButtons & (WORD)button) ? true : false);
}

//==========================================================================
/**
@brief ���A�i���O�X�e�B�b�N
@param index [in] �����R���g���[���[
@param Out [in] �X���x�N�g��
@return �����Ă���Ƃ� true
*/
bool CXInput::AnalogL(int index, D3DXVECTOR3 & Out)
{
	XINPUT_STATE * pstate = &this->m_state[index];

	Out = D3DXVECTOR3(pstate->Gamepad.sThumbLX, 0.0f, pstate->Gamepad.sThumbLY);
	D3DXVec3Normalize(&Out, &Out);

	return this->AnalogL(index);
}

//==========================================================================
/**
@brief ���A�i���O�X�e�B�b�N
@param index [in] �����R���g���[���[
@return �����Ă���Ƃ� true
*/
bool CXInput::AnalogL(int index)
{
	XINPUT_STATE * pstate = &this->m_state[index];

	if (pstate->Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		pstate->Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		pstate->Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
		pstate->Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		return true;
	}

	return false;
}

//==========================================================================
/**
@brief �E�A�i���O�X�e�B�b�N
@param index [in] �����R���g���[���[
@param Out [in] �X���x�N�g��
@return �����Ă���Ƃ� true
*/
bool CXInput::AnalogR(int index, D3DXVECTOR3 & Out)
{
	XINPUT_STATE * pstate = &this->m_state[index];

	Out = D3DXVECTOR3(pstate->Gamepad.sThumbRX, 0.0f, pstate->Gamepad.sThumbRY);
	D3DXVec3Normalize(&Out, &Out);

	return this->AnalogR(index);
}

//==========================================================================
/**
@brief �E�A�i���O�X�e�B�b�N
@param index [in] �����R���g���[���[
@return �����Ă���Ƃ� true
*/
bool CXInput::AnalogR(int index)
{
	XINPUT_STATE * pstate = &this->m_state[index];

	if (pstate->Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
		pstate->Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
		pstate->Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ||
		pstate->Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		return true;
	}

	return false;
}

//==========================================================================
/**
@brief ���A�i���O�X�e�B�b�N�̃g���K�[
@param key [in] �{�^��ID
@param index [in] �����R���g���[���[
@return �����Ă���Ƃ� true
*/
bool CXInput::AnalogLTrigger(EAnalog key, int index)
{
	switch (key)
	{
	case EAnalog::UP:
		return this->KeyTrigger(this->m_trigger[index].m_Ltrigger.m_up, this->m_trigger[index].m_LtriggerOld.m_up);
		break;
	case EAnalog::DOWN:
		return this->KeyTrigger(this->m_trigger[index].m_Ltrigger.m_down, this->m_trigger[index].m_LtriggerOld.m_down);
		break;
	case EAnalog::LEFT:
		return this->KeyTrigger(this->m_trigger[index].m_Ltrigger.m_left, this->m_trigger[index].m_LtriggerOld.m_left);
		break;
	case EAnalog::RIGHT:
		return this->KeyTrigger(this->m_trigger[index].m_Ltrigger.m_right, this->m_trigger[index].m_LtriggerOld.m_right);
		break;
	default:
		break;
	}
	return false;
}

//==========================================================================
/**
@brief ���A�i���O�X�e�B�b�N�̃g���K�[
@param index [in] �����R���g���[���[
@param key [in] �{�^��ID
@param Out [in] �X���x�N�g��
@return �����Ă���Ƃ� true
*/
bool CXInput::AnalogLTrigger(EAnalog key, int index, D3DXVECTOR3 * Out)
{
	XINPUT_STATE * pstate = &this->m_state[index];

	*Out = D3DXVECTOR3(pstate->Gamepad.sThumbLX, 0.0f, pstate->Gamepad.sThumbLY);
	D3DXVec3Normalize(Out, Out);

	return this->AnalogLTrigger(key, index);
}

//==========================================================================
/**
@brief �E�A�i���O�X�e�B�b�N�̃g���K�[
@param index [in] �����R���g���[���[
@param key [in] �{�^��ID
@return �����Ă���Ƃ� true
*/
bool CXInput::AnalogRTrigger(EAnalog key, int index)
{
	switch (key)
	{
	case EAnalog::UP:
		return this->KeyTrigger(this->m_trigger[index].m_Rtrigger.m_up, this->m_trigger[index].m_RtriggerOld.m_up);
		break;
	case EAnalog::DOWN:
		return this->KeyTrigger(this->m_trigger[index].m_Rtrigger.m_down, this->m_trigger[index].m_RtriggerOld.m_down);
		break;
	case EAnalog::LEFT:
		return this->KeyTrigger(this->m_trigger[index].m_Rtrigger.m_left, this->m_trigger[index].m_RtriggerOld.m_left);
		break;
	case EAnalog::RIGHT:
		return this->KeyTrigger(this->m_trigger[index].m_Rtrigger.m_right, this->m_trigger[index].m_RtriggerOld.m_right);
		break;
	default:
		break;
	}
	return false;
}

//==========================================================================
/**
@brief �E�A�i���O�X�e�B�b�N�̃g���K�[
@param index [in] �����R���g���[���[
@param key [in] �{�^��ID
@param Out [in] �X���x�N�g��
@return �����Ă���Ƃ� true
*/
bool CXInput::AnalogRTrigger(EAnalog key, int index, D3DXVECTOR3 * Out)
{
	XINPUT_STATE * pstate = &this->m_state[index];

	*Out = D3DXVECTOR3(pstate->Gamepad.sThumbRX, 0.0f, pstate->Gamepad.sThumbRY);
	D3DXVec3Normalize(Out, Out);

	return this->AnalogRTrigger(key, index);
}

//==========================================================================
/**
@brief ���g���K�[�{�^��
@param index [in] �����R���g���[���[
@return �����Ă���Ƃ� true
*/
bool CXInput::LT(int index)
{
	return (this->m_state[index].Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;
}

//==========================================================================
/**
@brief �E�g���K�[�{�^��
@param index [in] �����R���g���[���[
@return �����Ă���Ƃ� true
*/
bool CXInput::RT(int index)
{
	return (this->m_state[index].Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;
}

//==========================================================================
/**
@brief �R���g���[���[�̑��݂̊m�F
@param index [in] �����R���g���[���[
@return ���݂���ꍇ true
*/
bool CXInput::Check(int index)
{
    return this->m_statedat[index];
}

//==========================================================================
/**
@brief �R���g���[���[�̌��݂̏�Ԃ��擾
@param index [in] �����R���g���[���[
@return �C���X�^���X
*/
XINPUT_STATE * CXInput::GetState(int index)
{
    return &this->m_state[index];
}

//==========================================================================
/**
@brief �T�C�Y
@return �R���g���[���[��
*/
int CXInput::Size(void)
{
    return this->m_numdata;
}

//==========================================================================
/**
@brief �R���g���[���̃L�[�g���K�[
@param bNew [in] �V��������L�[
@param bOld [in] �Â�����L�[
@return ����
*/
bool CXInput::KeyTrigger(bool bNew, bool bOld)
{
    return (bOld ^ bNew) & bNew;
}

//==========================================================================
/**
@brief �R���g���[���̃L�[�����[�X
@param bNew [in] �V��������L�[
@param bOld [in] �Â�����L�[
@return ����
*/
bool CXInput::KeyRelease(bool bNew, bool bOld)
{
    return bOld ^ bNew & bOld;
}

CXInputAnalogTrigger::CXInputAnalogTrigger()
{
    this->m_Ltrigger = CXInputBool4();
    this->m_LtriggerOld = CXInputBool4();
    this->m_Rtrigger = CXInputBool4();
    this->m_RtriggerOld = CXInputBool4();
}

CXInputAnalogTrigger::~CXInputAnalogTrigger()
{
}

//==========================================================================
/**
@brief �X�V
*/
void CXInputAnalogTrigger::update(void)
{
    this->m_LtriggerOld = this->m_Ltrigger;
    this->m_RtriggerOld = this->m_Rtrigger;
}

//==========================================================================
/**
@brief �؂�ւ�
@param input [in] ����L�[
@param Out [in/out] �Ώ�
*/
void CXInputAnalogTrigger::ifset(bool input, bool * Out)
{
    (*Out) = ((input == true) ? true : false);
}

//==========================================================================
/**
@brief ��r in1 < in2
@param in1 [in] �Ώ�1
@param in2 [in/out] �Ώ�2
@return in2 ���傫���ꍇ true
*/
bool CXInputAnalogTrigger::ifbool(float in1, float in2)
{
    return (in1<in2) ? true : false;
}