#pragma once

#include <stack>
#include "Scene.hpp"

class AudioPlayer;
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

	void hideAllPongBalls() const;
	void togglePlayersMovement(bool canTheyMove) const;
	void pushInactivePongBall(PongBall* pongBallToPush);
	void startFirstRound() const;
	void restartRound() const;
	void endRound() const;

	//gets - Sets
	PolygonTerrain* getPolygonTerrain() const;
	AudioPlayer* getAudioPlayer() const;
	void setPlayersToDefaultSpawnPoints() const;

private:
	std::shared_ptr<GameManager> _gameManager;
	std::unique_ptr<AnimatorManager> _animator;
	std::unique_ptr<AudioPlayer> _audioPlayer;

	//Game objects
	std::vector<PongBall*> _pongBalls;
	std::stack<PongBall*> _inactivePongBalls;
	std::vector<Character*> _players;

	std::unique_ptr<PolygonTerrain> _polygonTerrain;

	//Font, texts and textures..
	sf::Font _font;

	void initValues();
	void initFonts();
	void makePlayerShoot(int playerIndex);
	void checkPlayerPongBallCollision(const PongBall& pongBall) const;
};
