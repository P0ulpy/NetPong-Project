#pragma once

#include "Scene.hpp"

class GameManager;
class PolygonCollisionResult;
class PolygonTerrain;
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
	void start() override {};
	void update(const float& deltaTime);
	void render(sf::RenderTarget* target = nullptr);

	PolygonTerrain* getPolygonTerrain() const;
	void hideAllPongBalls();
private:
	std::vector<sf::Texture> _textures;
	std::shared_ptr<GameManager> _gameManager;

	//Game objects
	std::vector<PongBall*> _pongBalls;
	std::unique_ptr<Character> _character;
	
	bool activeShieldAutorize = true;
	float delay = 2.f;

	std::unique_ptr<PolygonTerrain> _polygonTerrain;

	//Font and texts
	sf::Font _font;
	//sf::Clock clock;
	float clock = 0;

	void initValues();
	void initFonts();
};