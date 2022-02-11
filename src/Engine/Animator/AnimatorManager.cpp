#include "AnimatorManager.hpp"
#include "iostream"

AnimatorManager::AnimatorManager()
{
	//Animation d'explosion
	if (!_imageExplosion.loadFromFile("./Assets/Animations/Explosion.png"))std::cout << "No Explosion asset" << std::endl;
	_explosionTexture.loadFromImage(_imageExplosion);
	//Anime 1 = Explosion
	_playAnim.push_back(false);
}

void AnimatorManager::DeathAnimation(sf::Vector2f position)
{

	_frameExplosion.setPosition(position);
	_frameExplosion.setTexture(_explosionTexture);
	// Nombre de frame par ligne cf Assets/Animations/Explosion.png
	_nbrFrameRow = 6;
	// Nombre de frame par colonne
	_nbrFrameCol = 8;
	_playAnim.at(0) = true;
}



void AnimatorManager::render(sf::RenderTarget& target)
{
	if (_playAnim.at(0))
	{
		BalayageTexture(_imageExplosion, _frameExplosion);
		target.draw(_frameExplosion);
	}
}

void AnimatorManager::BalayageTexture(sf::Image image, sf::Sprite& frame)
{
	//Colonne et ligne où la frame commence
	_col = image.getSize().x / _nbrFrameCol;
	_row = image.getSize().y / _nbrFrameRow;
	//Préparation d'un rectangle qui entoure la frame à render
	frame.setTextureRect(sf::IntRect(_col * _indexBalayageCol, _row * _indexBalayageRow, _col, _row));
	frame.setOrigin(frame.getLocalBounds().width / 2, frame.getLocalBounds().height / 2);
	//Scale 1.5 car trop petit sinon
	frame.setScale(sf::Vector2f(1.5f, 1.5f));

	//Si on dépasse le nombre de frame par colonne maximum
	if (_indexBalayageCol > _nbrFrameRow)
	{
		//Retour à la ligne
		_indexBalayageCol = 0;
		_indexBalayageRow++;
		//Si on dépasse le nombre de frame par ligne max
		if (_indexBalayageRow > _nbrFrameCol)
		{
			//Fin de l'animation
			_playAnim.at(0) = false;
			_indexBalayageRow = 0;
			_indexBalayageCol = 0;
		}
	}
	_indexBalayageCol++;
}





