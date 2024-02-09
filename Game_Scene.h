#pragma once

#undef UNICODE  // Unicodeではなく、マルチバイト文字を使う

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

