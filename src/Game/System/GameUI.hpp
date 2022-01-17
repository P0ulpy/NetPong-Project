#pragma once

#include <string>

#include "SFML/Graphics/RenderTarget.hpp"

class GameUI
{
private:
	std::string _player1Score;
	std::string _player2Score;
	std::string _currentRound;
//etc etc
public:
	GameUI();
	~GameUI();

	void update(const float& deltaTime);
	void render(sf::RenderTarget& target) const;
};
