#pragma once
#include <vector>
#include <memory>

#include "SFML/Graphics/RenderTarget.hpp"

class RoundStartCountdown;
class Character;

class GameManager
{
private	:
	Character* _player1;
	Character* _player2;

	float _numRoundsToWin;

	std::unique_ptr<RoundStartCountdown> _roundStartCountdown;

	void initValues();

public:
	GameManager(Character* player1, Character* player2);
	~GameManager();

	void update(const float& deltaTime);
	void render(sf::RenderTarget& target) const;

	void incrementScorePlayer(Character* pPlayer);
	void playerWinsRound(Character* pPlayer);
	void playerWinsGame(Character* pPlayer);

	void startRoundStartCountdown();
};