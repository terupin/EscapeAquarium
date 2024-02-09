#pragma once

#undef UNICODE  // Unicodeではなく、マルチバイト文字を使う

class Title_Scene
{
public:
	Title_Scene();
	~Title_Scene();

	void Update();
	void Draw();
};

