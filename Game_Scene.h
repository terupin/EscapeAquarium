#pragma once

#undef UNICODE  // Unicode�ł͂Ȃ��A�}���`�o�C�g�������g��

enum Scene
{
	Title,
	Stage,
	Game_,
	Gameover,
	Goal,
};

enum level
{
	l_easy,
	l_normal,
	l_hard,
};

class Game_Scene
{
public:
	Game_Scene();
	~Game_Scene();

	void Update();
	void Draw();
};

