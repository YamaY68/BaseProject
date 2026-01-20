#pragma once
#include"../ComponentBase.h"

class PlayerInputComponent :public ComponentBase
{
public:
	PlayerInputComponent(int front, int back, int left, int right, int up, int down);
	~PlayerInputComponent(void) = default;

	void SetJumpKey(int key) { jumpKey_ = key; }

	int frontKey_=-1;	//前進キー
	int backKey_ =- 1;	//後退キー
	int leftKey_ = -1;	//左移動キー
	int rightKey_ = -1;	//右移動キー
	int upKey_ = -1;		//上移動キー
	int downKey_ = -1;	//下移動キー

	int jumpKey_ = -1; //ジャンプキー
};

