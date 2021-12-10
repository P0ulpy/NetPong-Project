#pragma once

#include "Scene.hpp"

class Terrain;
class PongBall;
class PoPossibEngin;
class Character;

class MainGameScene : public Scene
{
public:
	MainGameScene(PoPossibEngin& poPossibEngin);
	~MainGameScene();

	void updateInputs(const float& deltaTime);
	void start();
	void update(const float& deltaTime);
	void render(sf::RenderTarget* target = nullptr);
private:
	std::vector<sf::Texture> _textures;

	//Game objects
	std::unique_ptr<Terrain> _terrain;
	std::unique_ptr<PongBall> _pongBall;
	std::unique_ptr<Character> _character;
	
	bool activeShieldAutorize = true;
	float delay = 2.f;
	//Font and texts
	sf::Font _font;
	//sf::Clock clock;
	float clock = 0;

	void initValues();
	void initFonts();
};