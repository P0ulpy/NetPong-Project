#pragma once
#include <vector>
#include <memory>

#include "SFML/Graphics/RenderTarget.hpp"

class GameUI;
class RoundStartCountdown;
class Character;

class GameManager
{
private	:
	Character* _player1;
	Character* _player2;

	int _numRoundsToWin;
	int _currentRound;

	std::unique_ptr<RoundStartCountdown> _roundStartCountdown;
	std::unique_ptr<GameUI> _gameUI;

	void initValues();

public:
	GameManager(Character* player1, Character* player2);
	~GameManager();

	void update(const float& deltaTime);
	void render(sf::RenderTarget& target) const;

	//TODO : � voir si on a besoin d'une m�thode g�n�rique pour les deux joueurs, ou bien deux m�thodes s�par�es (incrementScorePlayer1, incrementScorePlayer2...)
	void incrementScorePlayer(Character* pPlayer);
	void playerWinsRound(Character* pPlayer);
	void playerWinsGame(Character* pPlayer);

	void startRoundStartCountdown();
	void resetGame();
};