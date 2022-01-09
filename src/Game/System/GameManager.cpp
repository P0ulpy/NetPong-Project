#include "GameManager.hpp"

#include <iostream>

#include "GameUI.hpp"
#include "RoundStartCountdown.hpp"
#include "../../Engine/Scenes/MainGameScene.hpp"
#include "../Terrains/PolygonTerrain.hpp"

constexpr int NUM_ROUNDS_TO_WIN = 2;
constexpr float DURATION_BEFORE_STARTING_NEW_ROUND = 2.5f;

GameManager::GameManager(MainGameScene* pMainGameScene)
	:_mainGameScene(pMainGameScene)
{
	initValues();
	resetGame();
}

GameManager::~GameManager()
{

}

void GameManager::initValues()
{
	_gameUI = std::make_unique<GameUI>();
	_roundStartCountdown = std::make_unique<RoundStartCountdown>();

	_currentTimeBeforeStartingNewRound = 0;

	_isRoundEnded = false;
}

void GameManager::update(const float& deltaTime)
{
	_roundStartCountdown->update(deltaTime);
	_gameUI->update(deltaTime);

	updateRoundEndTimer(deltaTime);
}

void GameManager::updateRoundEndTimer(const float& deltaTime)
{
	if (_isRoundEnded)
	{
		_currentTimeBeforeStartingNewRound += deltaTime;

		if (_currentTimeBeforeStartingNewRound >= DURATION_BEFORE_STARTING_NEW_ROUND)
		{
			_currentTimeBeforeStartingNewRound = 0;
			restartRound(deltaTime);
		}
	}
}

void GameManager::render(sf::RenderTarget& target) const
{
	_roundStartCountdown->render(target);
	_gameUI->render(target);
}

void GameManager::player1WinsRound()
{
	//Incrémenter le score
	//std::cout << _player1.getName() << " wins ! " << std::endl;
	_tempScorePlayer1++;
	if(_tempScorePlayer1 >= NUM_ROUNDS_TO_WIN)
	{
		std::cout << "Player 1 won the game !!\n\nRestarting game\n";
		resetGame();
	}
	else
	{
		std::cout << "Player 1 won the round !!\n\n";
	}
	endRound();
}

void GameManager::player2WinsRound()
{
	//Incrémenter le score
	//std::cout << _player2.getName() << " wins ! " << std::endl;
	_tempScorePlayer2++;
	if (_tempScorePlayer2 >= NUM_ROUNDS_TO_WIN)
	{
		std::cout << "Player 2 won the game !!\n\nRestarting game\n";
		resetGame();
	}
	else
	{
		std::cout << "Player 2 won the round !!\n\n";
	}
	endRound();
}

void GameManager::startRoundStartCountdown()
{
	_roundStartCountdown->startBeginCountdown();
}

void GameManager::endRound()
{
	_mainGameScene->hideAllPongBalls();
	startRoundEndTimer();
}

void GameManager::startRoundEndTimer()
{
	_isRoundEnded = true;
}

void GameManager::restartRound(const float& deltaTime)
{
	_isRoundEnded = false;
	_currentTimeBeforeStartingNewRound = 0;
	std::cout << "Oulala je change le terrain" << std::endl;
	_mainGameScene->getPolygonTerrain()->drawRandomTerrain();
	startRoundStartCountdown();
}

void GameManager::resetGame()
{
	_currentRound = 0;
	_tempScorePlayer1 = _tempScorePlayer2 = 0;
	//_player1.setScore(0);
	//_player2.setScore(0);
}

