#pragma once
#include "SFML/Graphics.hpp"


class AnimatorManager
{
private:
	sf::Image _imageExplosion;
	sf::Texture _explosionTexture;
	sf::Sprite _styleSheetExplosion;

	bool _playAnimExplosion = false;

	//Pour le stylsheet de l'explosion
	int _nbrFrameRow;
	int _nbrFrameCol;
	int _indexBalayageRow = 0;
	int _indexBalayageCol = 0;

	int _col = 0;
	int _row = 0;

public:

	void DeathAnimation(sf::Vector2f position);
	void render(sf::RenderTarget& target);
	void BalayageTexture();
	AnimatorManager();

};