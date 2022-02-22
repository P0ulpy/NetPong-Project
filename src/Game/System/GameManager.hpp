#pragma once
#include <vector>
#include <memory>
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

class MainGameScene;
class GameUI;
class RoundStartCountdown;
class UI;

class GameManager
{
private	:
	MainGameScene* _mainGameScene;

	int _scorePlayer1 = 0;
	int _scorePlayer2 = 0;
	bool _hasRoundEnded = false;
	sf::Font _font;
	int _currentRound = 0;
	bool _isFirstRound { true };

	float _currentTimeBeforeStartingNewRound = 0.0f;

	std::unique_ptr<RoundStartCountdown> _roundStartCountdown;
	std::unique_ptr<GameUI> _gameUI;
	std::unique_ptr<UI> _UI;

	void initValues();

	void player1WinsRound();
	void player2WinsRound();
	void playerDrawRound();

	void player1WinsGame();
	void player2WinsGame();


public:
	GameManager(MainGameScene* pMainGameScene);
	~GameManager();

	void update(const float& deltaTime);
	void updateRoundEndTimer(const float& deltaTime);
	void render(sf::RenderTarget& target) const;

	void makePlayerWin(int numPlayer);

	void startRoundStartCountdown() const;
	void endRound();
	void startRoundEndTimer();

	void startFirstRound();
	void restartRound();
	void resetGame();
};