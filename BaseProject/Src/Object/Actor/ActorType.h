#pragma once

enum class ActorType
{
	Player,	//入力で動くオブジェクト
	Object,	//外部の影響をうけて動くオブジェクト
	Kinematic,	//外部の影響をうけずに動くオブジェクト
	Static	//動かないオブジェクト
};