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
class AnimatorManager;

class MainGameScene : public Scene
{
public:
	MainGameScene(PoPossibEngin& poPossibEngin);
	~MainGameScene();

	void updateInputs(const float& deltaTime);
	void start() override {};
	void update(const float& deltaTime) override;
	void render(sf::RenderTarget* target = nullptr) override;

	PolygonTerrain* getPolygonTerrain() const;
	void displayPlayers(bool isDisplayed) const;
	void hideAllPongBalls() const;
	void togglePlayersMovement(bool canTheyMove) const;
	void pushInactivePongBall(PongBall* pongBallToPush);

	void setPlayersToDefaultSpawnPoints() const;

private:
	std::vector<sf::Texture> _textures;
	std::shared_ptr<GameManager> _gameManager;

	//Game objects
	std::vector<PongBall*> _pongBalls;
	std::stack<PongBall*> _inactivePongBalls;
	std::vector<Character*> _players;
	
	
	std::unique_ptr<PolygonTerrain> _polygonTerrain;
	std::unique_ptr<AnimatorManager> _animator;

	//Font and texts
	sf::Font _font;

	void initValues();
	void initFonts();
	void makePlayerShoot(int playerIndex);
	void checkPlayerPongBallCollision(const PongBall& pongBall) const;
};
