#pragma once

#undef UNICODE  // Unicodeではなく、マルチバイト文字を使う

class Gole_Scene
{
public:
	Gole_Scene();
	~Gole_Scene();

	void Update();
	void Draw();
};

