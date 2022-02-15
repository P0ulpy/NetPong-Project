#pragma once
#include "SFML/Graphics.hpp"


class AnimatorManager
{
private:
	//Explosion
	sf::Image _imageExplosion;
	sf::Texture _explosionTexture;
	sf::Sprite _frameExplosion;


	// 0 = Animation Explosion 
	std::vector<bool> _playAnim;
	
	//Pour le stylsheet
	int _nbrFrameRow = 0;
	int _nbrFrameCol = 0;
	int _indexBalayageRow = 0;
	int _indexBalayageCol = 0;

	int _col = 0;
	int _row = 0;

public:
	//Pour l'animation de mort
	void DeathAnimation(sf::Vector2f position);

	void render(sf::RenderTarget& target);
	void BalayageTexture(sf::Image image, sf::Sprite& frame);
	


	AnimatorManager();

};