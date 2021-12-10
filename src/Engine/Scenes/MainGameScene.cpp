#include "MainGameScene.hpp"

#include <iostream>

#include "../../Game/Entities/PongBall.hpp"
#include "../../Game/Entities/Terrain.hpp"
#include "../../Game/Entities/Character.hpp"
#include "../../Game/PolygonTerrain.hpp"

MainGameScene::MainGameScene(PoPossibEngin& poPossibEngin)
	: Scene(poPossibEngin, SceneConfig())
{
	initValues();
	initFonts();
}

MainGameScene::~MainGameScene()
{

}

void MainGameScene::updateInputs(const float& deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		_pongBall->resetSpeedMultiplierBonus();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
		_pongBall->startBoostBall(8.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3))
		_pongBall->startBoostBall(16.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
		_pongBall->startBoostBall(32.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5))
		_pongBall->startBoostBall(64.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6))
		_pongBall->startBoostBall(128.f);

	
	// Joueur 1
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
	{
		_character->direction(true, false, _terrain->getPlayableArea());
	}
	else
	{
		_character->direction(false, false, _terrain->getPlayableArea());
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
	{
		_character->direction(false, true, _terrain->getPlayableArea());
	}
	else
	{
		_character->direction(false, false, _terrain->getPlayableArea());
	}


	// Joueur 2

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		_character->directionP2(true, false, _terrain->getPlayableArea());
	}
	else
	{
		_character->directionP2(false, false, _terrain->getPlayableArea());
	}



	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		_character->directionP2(false, true, _terrain->getPlayableArea());
	}
	else
	{
		_character->directionP2(false, false, _terrain->getPlayableArea());
	}


	clock += deltaTime;

	if (clock >= 2 || activeShieldAutorize == true )
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			_character->verifActiveShield(true);
			clock = 0;
			activeShieldAutorize = false;
		}
	}


		

	
}

void MainGameScene::start()
{

}

void MainGameScene::initValues()
{
	_terrain = std::make_unique<Terrain>(_poPossibEngin->getRenderWindow());
	_polygonTerrain = std::make_unique<PolygonTerrain>(_poPossibEngin->getRenderWindow());

	//_pongBall = std::make_unique<PongBall>(_poPossibEngin->getRenderWindow(), _terrain->getPlayableArea());
	_pongBall = std::make_unique<PongBall>(_poPossibEngin->getRenderWindow(), _terrain->getPlayableArea(), *_polygonTerrain);
	_character = std::make_unique<Character>(_poPossibEngin->getRenderWindow());
}

void MainGameScene::initFonts()
{
	if (!_font.loadFromFile("./Assets/arial.ttf"))
	{
		std::cout << "ERROR FONT NOT LOADED - MainGameScene.cpp" << std::endl;
	}



}

void MainGameScene::update(const float& deltaTime)
{
	updateInputs(deltaTime);
	_pongBall->update(_poPossibEngin->getRenderWindow(), deltaTime);
	_character->updateMouvement(_poPossibEngin->getRenderWindow(),_poPossibEngin->getRenderWindow(), clock);
	_character->updateMouvementP2(_poPossibEngin->getRenderWindow(),_poPossibEngin->getRenderWindow(),clock);
}

void MainGameScene::render(sf::RenderTarget* target)
{
	//_terrain->render(*target);
	_polygonTerrain->render(*target);
	_pongBall->render(*target);
	_character->render(*target);
	_character->renderP2(*target);
}