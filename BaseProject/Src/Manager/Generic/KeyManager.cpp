#include"KeyManager.h"

#include<DxLib.h>

KeyManager* KeyManager::ins = nullptr;

KeyManager::KeyManager():
	keyInfo(),
	keyboardFormat(),
	controllerButtonFormat()
{
}



void KeyManager::Init(void)
{
	// キーボードを割り振るとき
#define SET_KEYBOARD(type,key)keyboardFormat[(int)type].emplace_back(key)

	// コントローラーのボタンを割り振るとき
#define SET_C_BUTTON(type,key)controllerButtonFormat[(int)type].emplace_back(key)

	// コントローラーのボタン以外(スティックやトリガーなど)を割り振るとき
#define SET_C_OTHERS(type,key)controllerOthersFormat[(int)type].emplace_back(key)

	SET_KEYBOARD(KEY_TYPE::MOVE_UP, KEY_INPUT_W);
	SET_KEYBOARD(KEY_TYPE::MOVE_UP, KEY_INPUT_UP);
	SET_C_OTHERS(KEY_TYPE::MOVE_UP, CONTROLLER_OTHERS::LEFTSTICK_UP);


	SET_KEYBOARD(KEY_TYPE::MOVE_DOWN, KEY_INPUT_S);
	SET_KEYBOARD(KEY_TYPE::MOVE_DOWN, KEY_INPUT_DOWN);
	SET_C_OTHERS(KEY_TYPE::MOVE_DOWN, CONTROLLER_OTHERS::LEFTSTICK_DOWN);


	SET_KEYBOARD(KEY_TYPE::MOVE_RIGHT, KEY_INPUT_D);
	SET_KEYBOARD(KEY_TYPE::MOVE_RIGHT, KEY_INPUT_RIGHT);
	SET_C_OTHERS(KEY_TYPE::MOVE_RIGHT, CONTROLLER_OTHERS::LEFTSTICK_RIGHT);


	SET_KEYBOARD(KEY_TYPE::MOVE_LEFT, KEY_INPUT_A);
	SET_KEYBOARD(KEY_TYPE::MOVE_LEFT, KEY_INPUT_LEFT);
	SET_C_OTHERS(KEY_TYPE::MOVE_LEFT, CONTROLLER_OTHERS::LEFTSTICK_LEFT);


	SET_KEYBOARD(KEY_TYPE::JUMP, KEY_INPUT_SPACE);
	SET_KEYBOARD(KEY_TYPE::JUMP, KEY_INPUT_B);
	SET_C_BUTTON(KEY_TYPE::JUMP, XINPUT_BUTTON_A);


	SET_KEYBOARD(KEY_TYPE::ATTACK, KEY_INPUT_J);
	SET_KEYBOARD(KEY_TYPE::ATTACK, KEY_INPUT_RETURN);
	SET_C_BUTTON(KEY_TYPE::ATTACK, XINPUT_BUTTON_X);
	SET_C_BUTTON(KEY_TYPE::ATTACK, XINPUT_BUTTON_RIGHT_SHOULDER);
	SET_C_OTHERS(KEY_TYPE::ATTACK, CONTROLLER_OTHERS::RIGHT_TRIGGER);


	SET_KEYBOARD(KEY_TYPE::EVASION, KEY_INPUT_K);
	SET_KEYBOARD(KEY_TYPE::EVASION, KEY_INPUT_LSHIFT);
	SET_C_BUTTON(KEY_TYPE::EVASION, XINPUT_BUTTON_B);
	SET_C_BUTTON(KEY_TYPE::EVASION, XINPUT_BUTTON_LEFT_SHOULDER);
	SET_C_OTHERS(KEY_TYPE::EVASION, CONTROLLER_OTHERS::LEFT_TRIGGER);

	SET_KEYBOARD(KEY_TYPE::PAUSE, KEY_INPUT_ESCAPE);
	SET_C_BUTTON(KEY_TYPE::PAUSE, XINPUT_BUTTON_START);

	SET_KEYBOARD(KEY_TYPE::ENTER, KEY_INPUT_RETURN);
	SET_KEYBOARD(KEY_TYPE::ENTER, KEY_INPUT_SPACE);
	SET_KEYBOARD(KEY_TYPE::ENTER, KEY_INPUT_J);
	SET_C_BUTTON(KEY_TYPE::ENTER, XINPUT_BUTTON_B);
	SET_C_BUTTON(KEY_TYPE::ENTER, XINPUT_BUTTON_A);

	SET_KEYBOARD(KEY_TYPE::TUTORIAL_NEXT, KEY_INPUT_RETURN);
	SET_KEYBOARD(KEY_TYPE::TUTORIAL_NEXT, KEY_INPUT_N);
	SET_KEYBOARD(KEY_TYPE::TUTORIAL_NEXT, KEY_INPUT_B);
	SET_KEYBOARD(KEY_TYPE::TUTORIAL_NEXT, KEY_INPUT_V);
	SET_KEYBOARD(KEY_TYPE::TUTORIAL_NEXT, KEY_INPUT_F);
	SET_KEYBOARD(KEY_TYPE::TUTORIAL_NEXT, KEY_INPUT_G);
	SET_KEYBOARD(KEY_TYPE::TUTORIAL_NEXT, KEY_INPUT_R);
	SET_C_BUTTON(KEY_TYPE::TUTORIAL_NEXT, XINPUT_BUTTON_Y);


	SET_C_OTHERS(KEY_TYPE::BP_ATTACK_VEC_UP, CONTROLLER_OTHERS::RIGHTSTICK_UP);
	SET_C_OTHERS(KEY_TYPE::BP_ATTACK_VEC_DOWN, CONTROLLER_OTHERS::RIGHTSTICK_DOWN);
	SET_C_OTHERS(KEY_TYPE::BP_ATTACK_VEC_RIGHT, CONTROLLER_OTHERS::RIGHTSTICK_RIGHT);
	SET_C_OTHERS(KEY_TYPE::BP_ATTACK_VEC_LEFT, CONTROLLER_OTHERS::RIGHTSTICK_LEFT);

}

void KeyManager::Update(void)
{
	for (int i = 0; i < (int)KEY_TYPE::MAX; i++) {
		keyInfo[i].prev = keyInfo[i].now;

		bool b = false;

		for (auto& input : keyboardFormat[i]) {
			if (b) { break; }

			if (CheckHitKey(input) != 0) { b = true; }
		}
		for (auto& input : controllerButtonFormat[i]) {
			if (b) { break; }

			XINPUT_STATE state = {};
			if (GetJoypadXInputState(DX_INPUT_PAD1, &state) != 0) { state = {}; }

			if (state.Buttons[input] != 0) { b = true; }
		}
		for (CONTROLLER_OTHERS input : controllerOthersFormat[i]) {
			if (b) { break; }

			b = ControllerOthersInput(input);
		}

		keyInfo[i].now = b;

		keyInfo[i].up = (keyInfo[i].prev && !keyInfo[i].now);
		keyInfo[i].down = (!keyInfo[i].prev && keyInfo[i].now);
	}
}

void KeyManager::Release(void)
{
	for (auto& input : keyboardFormat) { input.clear(); }
	for (auto& input : controllerButtonFormat) { input.clear(); }
	for (auto& input : controllerOthersFormat) { input.clear(); }
}

bool KeyManager::ControllerOthersInput(const CONTROLLER_OTHERS& input)
{
	XINPUT_STATE state = {};
	if (GetJoypadXInputState(DX_INPUT_PAD1, &state) != 0) { state = {}; }

	short lenge = 10000;

	switch (input)
	{
	case KeyManager::CONTROLLER_OTHERS::LEFTSTICK_UP:
		if (state.ThumbLY > lenge) { return true; }
		break;
	case KeyManager::CONTROLLER_OTHERS::LEFTSTICK_DOWN:
		if (state.ThumbLY < -lenge) { return true; }
		break;
	case KeyManager::CONTROLLER_OTHERS::LEFTSTICK_RIGHT:
		if (state.ThumbLX > lenge) { return true; }
		break;
	case KeyManager::CONTROLLER_OTHERS::LEFTSTICK_LEFT:
		if (state.ThumbLX < -lenge) { return true; }
		break;


	case KeyManager::CONTROLLER_OTHERS::RIGHTSTICK_UP:
		if (state.ThumbRY > lenge) { return true; }
		break;
	case KeyManager::CONTROLLER_OTHERS::RIGHTSTICK_DOWN:
		if (state.ThumbRY < -lenge) { return true; }
		break;
	case KeyManager::CONTROLLER_OTHERS::RIGHTSTICK_RIGHT:
		if (state.ThumbRX > lenge) { return true; }
		break;
	case KeyManager::CONTROLLER_OTHERS::RIGHTSTICK_LEFT:
		if (state.ThumbRX < -lenge) { return true; }
		break;


	case KeyManager::CONTROLLER_OTHERS::LEFT_TRIGGER:
		if (state.LeftTrigger > 0) { return true; }
		break;
	case KeyManager::CONTROLLER_OTHERS::RIGHT_TRIGGER:
		if (state.RightTrigger > 0) { return true; }
		break;
	default:
		return false;
		break;
	}

	return false;
}


const Vector2F KeyManager::GetRightStickVec(void)
{
	XINPUT_STATE state = {};
	if (GetJoypadXInputState(DX_INPUT_PAD1, &state) != 0) { return { 0.0f,0.0f }; }
	short lenge = 10000;

	Vector2F vec = { (abs(state.ThumbRX) > lenge) ? (float)state.ThumbRX : 0.0f,(abs(state.ThumbRY) > lenge) ? (float)-state.ThumbRY : 0.0f };

	if (vec == 0.0f) { return{ 0.0f,0.0f }; }

	return vec / sqrtf(vec.x * vec.x + vec.y * vec.y);
}