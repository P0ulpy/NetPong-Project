#pragma once

#include "Scene.hpp"

class Terrain;
class PongBall;
class PoPossibEngin;

class MainGameScene : public Scene
{
public:
	MainGameScene(PoPossibEngin& poPossibEngin);
	~MainGameScene();

	void updateInputs();
	void start();
	void update(const float& deltaTime);
	void render(sf::RenderTarget* target = nullptr);
private:
	std::vector<sf::Texture> _textures;

	//Game objects
	std::unique_ptr<Terrain> _terrain;
	std::unique_ptr<PongBall> _pongBall;

	//Font and texts
	sf::Font _font;

	void initValues();
	void initFonts();
};