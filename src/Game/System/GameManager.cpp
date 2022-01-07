#include "GameManager.hpp"

#include <iostream>

#include "RoundStartCountdown.hpp"

GameManager::GameManager(Character* player1, Character* player2)
	:_player1(player1), _player2(player2)
{
	initValues();
}

GameManager::~GameManager()
{
}

void GameManager::initValues()
{
	_numRoundsToWin = 5;

	//_player1.setScore(0);
	//_player2.setScore(0);

	_roundStartCountdown = std::make_unique<RoundStartCountdown>();
}

void GameManager::update(const float& deltaTime)
{
	_roundStartCountdown->update(deltaTime);
}

void GameManager::render(sf::RenderTarget& target) const
{
	_roundStartCountdown->render(target);
}


void GameManager::incrementScorePlayer(Character* pPlayer)
{
	//pPlayer.setScore(pPlayer.getScore() + 1);

	//if (pPlayer.getScore() >= _numRoundsToWin)
	//{
	//	player1Wins();
	//}
}

void GameManager::playerWinsRound(Character* pPlayer)
{
	incrementScorePlayer(pPlayer);
}

void GameManager::playerWinsGame(Character* pPlayer)
{
	//std::cout << pPlayer.getName() << " wins ! " << std::endl;
}

void GameManager::startRoundStartCountdown()
{
	_roundStartCountdown->startBeginCountdown();
}

