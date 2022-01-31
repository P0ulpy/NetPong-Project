#pragma once

#include <stack>
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
	void pushInactivePongBall(PongBall* pongBallToPush);

private:
	std::vector<sf::Texture> _textures;
	std::shared_ptr<GameManager> _gameManager;

	//Game objects
	std::vector<PongBall*> _pongBalls;
	std::stack<PongBall*> _inactivePongBalls;
	std::vector<Character*> _players;
	
	std::unique_ptr<PolygonTerrain> _polygonTerrain;

	//Font and texts
	sf::Font _font;

	void initValues();
	void initFonts();
	void makePlayerShoot(int playerIndex);
};
