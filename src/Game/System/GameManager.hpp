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

	int _tempScorePlayer1;
	int _tempScorePlayer2;
	bool _isRoundEnded;

	int _currentRound;

	float _currentTimeBeforeStartingNewRound;

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

	void player1WinsGame();
	void player2WinsGame();

	void startRoundStartCountdown();
	void endRound();
	void startRoundEndTimer();
	void restartRound(const float& deltaTime);
	void resetGame();
};