#include "GameManager.hpp"

#include <iostream>

#include "GameUI.hpp"
#include "RoundStartCountdown.hpp"
#include "../../Engine/Scenes/MainGameScene.hpp"
#include "../Terrains/PolygonTerrain.hpp"

constexpr int NUM_ROUNDS_TO_WIN = 5;
constexpr float DURATION_BEFORE_STARTING_NEW_ROUND = 2.5f;

GameManager::GameManager(MainGameScene* pMainGameScene)
	:_mainGameScene(pMainGameScene)
{
	initValues();
	resetGame();
}

GameManager::~GameManager()
{ }

void GameManager::initValues()
{
	_gameUI = std::make_unique<GameUI>();
	_roundStartCountdown = std::make_unique<RoundStartCountdown>(*_mainGameScene);

	_currentTimeBeforeStartingNewRound = 0;

	_hasRoundEnded = false;
	_isFirstRound = true;
}

void GameManager::update(const float& deltaTime)
{
	_roundStartCountdown->update(deltaTime);
	_gameUI->update(deltaTime);

	updateRoundEndTimer(deltaTime);
}

void GameManager::updateRoundEndTimer(const float& deltaTime)
{
	if (_hasRoundEnded)
	{
		_currentTimeBeforeStartingNewRound += deltaTime;

		if (_currentTimeBeforeStartingNewRound >= DURATION_BEFORE_STARTING_NEW_ROUND)
		{
			_currentTimeBeforeStartingNewRound = 0;
			restartRound();
		}
	}
}

void GameManager::render(sf::RenderTarget& target) const
{
	_roundStartCountdown->render(target);
	_gameUI->render(target);
}

void GameManager::makePlayerWin(int numPlayer)
{
	numPlayer = std::min(1, std::max(numPlayer, 2));

	numPlayer == 1 ? player1WinsRound() : player2WinsRound();
}

void GameManager::player1WinsRound()
{
	_scorePlayer1++;
	if(_scorePlayer1 >= NUM_ROUNDS_TO_WIN)
	{
		player1WinsGame();
	}
	else
	{
		std::cout << "Player 1 won the round !!\n\n";
	}

	endRound();
}

void GameManager::player2WinsRound()
{
	_scorePlayer2++;
	if (_scorePlayer2 >= NUM_ROUNDS_TO_WIN)
	{
		player2WinsGame();
	}
	else
	{
		std::cout << "Player 2 won the round !!\n\n";
	}

	endRound();
}

void GameManager::playerDrawRound()
{
	std::cout << "No one wins !\n";
	endRound();
}

void GameManager::player1WinsGame()
{
	std::cout << "Player 1 won the game !!\n\nRestarting game\n";
	std::cout << "Score player 1 : " << _scorePlayer1 << " | Score player 2 : " << _scorePlayer2 << std::endl;
	resetGame();
}

void GameManager::player2WinsGame()
{
	std::cout << "Player 2 won the game !!\n\nRestarting game\n";
	std::cout << "Score player 1 : " << _scorePlayer1 << " | Score player 2 : " << _scorePlayer2 << std::endl;
	resetGame();
}

void GameManager::startRoundStartCountdown() const
{
	_roundStartCountdown->startBeginCountdown();
}

void GameManager::endRound()
{
	_mainGameScene->endRound();
	startRoundEndTimer();
}

void GameManager::startRoundEndTimer()
{
	_hasRoundEnded = true;
}

void GameManager::startFirstRound()
{
	//We re-use roundEndTimer to wait a bit before the first countdown plays.
	startRoundEndTimer();
}

void GameManager::restartRound()
{
	_hasRoundEnded = false;
	_currentTimeBeforeStartingNewRound = 0;
	_currentRound++;

	if(!_isFirstRound)
	{
		_mainGameScene->restartRound();
	}
	else
	{
		_isFirstRound = false;
		_mainGameScene->startFirstRound();
	}

	startRoundStartCountdown();
}

void GameManager::resetGame()
{
	_currentRound = 1;
	_scorePlayer1 = _scorePlayer2 = 0;
	startFirstRound();
}

