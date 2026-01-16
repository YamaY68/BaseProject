#pragma once
class InputComponent
{
public:
	InputComponent(int front, int back, int left, int right, int up, int down);
	~InputComponent(void) = default;

	int frontKey_;	//前進キー
	int backKey_;	//後退キー
	int leftKey_;	//左移動キー
	int rightKey_;	//右移動キー
	int upKey_;		//上移動キー
	int downKey_;	//下移動キー
};

