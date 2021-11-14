#pragma once

#include "Scene.h"
#include "Terrain.h"
#include "PongBall.h"

class MainGameScene : public Scene
{
private:
	std::unique_ptr<sf::RenderWindow*> _window;
	std::vector<sf::Texture> _textures;

	//Game objects
	std::unique_ptr<Terrain> _terrain;
	std::unique_ptr<PongBall> _pongBall;

	//Font and texts
	sf::Font _font;

	void initValues();
	void initFonts();
public:
	MainGameScene(sf::RenderWindow* window);
	virtual ~MainGameScene();

	void updateInputs();
	void update(const float& deltaTime);
	void render(sf::RenderTarget* target = nullptr);
};
