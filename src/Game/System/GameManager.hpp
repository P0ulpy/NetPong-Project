#pragma once
#include <vector>
#include <memory>

#include "SFML/Graphics/RenderTarget.hpp"

class MainGameScene;
class GameUI;
class RoundStartCountdown;

class GameManager
{
private	:
	MainGameScene* _mainGameScene;

	int _scorePlayer1 = 0;
	int _scorePlayer2 = 0;
	bool _isRoundEnded = false;

	int _currentRound = 0;

	float _currentTimeBeforeStartingNewRound = 0.0f;

	std::unique_ptr<RoundStartCountdown> _roundStartCountdown;
	std::unique_ptr<GameUI> _gameUI;

	void initValues();

public:
	GameManager(MainGameScene* pMainGameScene);
	~GameManager();

	void update(const float& deltaTime);
	void updateRoundEndTimer(const float& deltaTime);
	void render(sf::RenderTarget& target) const;

	void player1WinsRound();
	void player2WinsRound();
	void playerDrawRound();

	void player1WinsGame();
	void player2WinsGame();

	void startRoundStartCountdown() const;
	void endRound();
	void startRoundEndTimer();
	void restartRound(const float& deltaTime);
	void resetGame();
};