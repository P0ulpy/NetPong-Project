#pragma once

#include <stack>
#include <memory>
#include "Scene.hpp"
#include "../../Game/Controllers/ControllerBase.hpp"

class GameManager;
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

    static MainGameScene* getInstance();

	void updateInputs(const float& deltaTime);
	void start() override {};
	void update(const float& deltaTime) override;
	void render(sf::RenderTarget* target) override;

	PolygonTerrain* getPolygonTerrain() const;
	void hideAllPongBalls() const;
	void pushInactivePongBall(PongBall* pongBallToPush);

	sf::Color _player0color = sf::Color(255, 40, 0);
	sf::Color _player1color = sf::Color(0, 40, 255);

	void setPlayersToDefaultSpawnPoints() const;

    // GET
    std::stack<PongBall*>& getInactivePongBalls();

private:
    static MainGameScene* _instance;

	std::vector<sf::Texture> _textures;
	std::shared_ptr<GameManager> _gameManager;

    // Controllers
    std::vector<Engine::ControllerBase*> _controllers;

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
