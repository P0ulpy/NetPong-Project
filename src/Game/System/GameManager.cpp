#include "GameManager.hpp"

#include <iostream>

#include "GameUI.hpp"
#include "RoundStartCountdown.hpp"
#include "../../Engine/Scenes/MainGameScene.hpp"
#include "../Terrains/PolygonTerrain.hpp"
#include "../../Utils/UI.hpp"

constexpr int NUM_ROUNDS_TO_WIN = 5;
constexpr float DURATION_BEFORE_STARTING_NEW_ROUND = 2.5f;

GameManager::GameManager(MainGameScene* pMainGameScene)
	:_mainGameScene(pMainGameScene)
{

	if (!_font.loadFromFile("./Assets/arial.ttf"))
	{
		std::cout << "No asset " << std::endl;
	}
	initValues();
	resetGame();

}

GameManager::~GameManager()
{ }

void GameManager::initValues()
{
	_UI = std::make_unique<UI>();
	_UI->CreateText("ScorePlayer1", sf::Color(255, 40, 0), "0", 100, sf::Vector2f(860, 50), _font);
	_UI->CreateText("BetweenScores", sf::Color::White, "|", 100, sf::Vector2f(960, 50), _font);
	_UI->CreateText("ScorePlayer2", sf::Color(0, 40, 255), "0", 100, sf::Vector2f(1060, 50), _font);
	_UI->CreateText("Winner", sf::Color::White, "Player x won the round !", 100, sf::Vector2f(960, 900), _font);
	_UI->Text("Winner").setOutlineColor(sf::Color::White);
	_UI->Text("Winner").setOutlineThickness(5);

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
	target.draw(_UI->Text("ScorePlayer1"));
	target.draw(_UI->Text("BetweenScores"));
	target.draw(_UI->Text("ScorePlayer2"));
	if (_hasRoundEnded && !_isFirstRound)
	{
		target.draw(_UI->Text("Winner"));
	}

}

void GameManager::makePlayerWin(int numPlayer)
{
	if (numPlayer == 1)
	{
		player1WinsRound();
	}

	if(numPlayer == 2)
	{
		player2WinsRound();
	}
	/*
	numPlayer = std::min(1, std::max(numPlayer, 2));

	numPlayer == 1 ? player1WinsRound() : player2WinsRound();*/
}

void GameManager::player1WinsRound()
{
	_scorePlayer1++;
	_UI->Text("ScorePlayer1").setString(std::to_string(_scorePlayer1));
	if(_scorePlayer1 >= NUM_ROUNDS_TO_WIN)
	{
		player1WinsGame();
	}
	else
	{
		_UI->Text("Winner").setString("Player 1 won the round !");
		_UI->Text("Winner").setFillColor(sf::Color(255, 40, 0));
		_UI->Text("Winner").setPosition(sf::Vector2f(960, 900));
		//std::cout << "Player 1 won the round !!\n\n";
	}

	endRound();
}

void GameManager::player2WinsRound()
{
	_scorePlayer2++;
	_UI->Text("ScorePlayer2").setString(std::to_string(_scorePlayer2));
	if (_scorePlayer2 >= NUM_ROUNDS_TO_WIN)
	{
		player2WinsGame();
	}
	else
	{
		_UI->Text("Winner").setString("Player 2 won the round !");
		_UI->Text("Winner").setFillColor(sf::Color(0, 40, 255));
		_UI->Text("Winner").setPosition(sf::Vector2f(960, 900));
		
		//std::cout << "Player 2 won the round !!\n\n";
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
	_UI->Text("Winner").setString("Player 1 won the game !!\n\n      Restarting game\n");
	_UI->Text("Winner").setFillColor(sf::Color(255, 40, 0));
	_UI->Text("Winner").setPosition(sf::Vector2f(960, 300));
//	std::cout << "Score player 1 : " << _scorePlayer1 << " | Score player 2 : " << _scorePlayer2 << std::endl;
	resetGame();
}

void GameManager::player2WinsGame()
{
	_UI->Text("Winner").setString("Player 2 won the game !!\n\n      Restarting game\n");
	_UI->Text("Winner").setFillColor(sf::Color(0, 40, 255));
	_UI->Text("Winner").setPosition(sf::Vector2f(960, 300));

	//std::cout << "Score player 1 : " << _scorePlayer1 << " | Score player 2 : " << _scorePlayer2 << std::endl;
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
	_UI->Text("ScorePlayer1").setString("0");
	_UI->Text("ScorePlayer2").setString("0");
	startFirstRound();
}

