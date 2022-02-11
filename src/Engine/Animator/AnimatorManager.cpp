#include "AnimatorManager.hpp"
#include "iostream"

AnimatorManager::AnimatorManager()
{
	//Animation d'explosion
	if (!_imageExplosion.loadFromFile("./Assets/Animations/Explosion.png"))std::cout << "Image explosion pas la" << std::endl;
	_explosionTexture.loadFromImage(_imageExplosion);
}

void AnimatorManager::DeathAnimation(sf::Vector2f position)
{
	_styleSheetExplosion.setPosition(position);
	_styleSheetExplosion.setTexture(_explosionTexture);
	_playAnimExplosion = true;
	_nbrFrameRow = 6;
	_nbrFrameCol = 8;
}

void AnimatorManager::render(sf::RenderTarget& target)
{
	if (_playAnimExplosion)
	{
		BalayageTexture();
		target.draw(_styleSheetExplosion);
	}
}

void AnimatorManager::BalayageTexture()
{
	_col = _imageExplosion.getSize().x / _nbrFrameCol;
	_row = _imageExplosion.getSize().y / _nbrFrameRow;

	_styleSheetExplosion.setTextureRect(sf::IntRect(_col * _indexBalayageCol, _row * _indexBalayageRow, _col, _row));
	_styleSheetExplosion.setOrigin(_styleSheetExplosion.getLocalBounds().width / 2, _styleSheetExplosion.getLocalBounds().height / 2);
	_styleSheetExplosion.setScale(sf::Vector2f(1.5f, 1.5f));

	if (_indexBalayageCol > _nbrFrameRow)
	{
		_indexBalayageCol = 0;
		_indexBalayageRow++;

		if (_indexBalayageRow > _nbrFrameCol)
		{
			_playAnimExplosion = false;
			_indexBalayageRow = 0;
			_indexBalayageCol = 0;
		}
	}
	_indexBalayageCol++;
}





