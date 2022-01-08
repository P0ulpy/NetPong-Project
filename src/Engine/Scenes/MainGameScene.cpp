#include "MainGameScene.hpp"

#include <iostream>

#include "../../Game/Entities/PongBall.hpp"
#include "../../Game/Terrains/Terrain.hpp"
#include "../../Game/Entities/Character.hpp"
#include "../../Game/Terrains/PolygonTerrain.hpp"

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
	/*	sf::Event event;

	
	if (_poPossibEngin->getInputsManager().getEvent(sf::Event::KeyPressed, event))
	{
	
		switch (event.key.code)
		{
			case sf::Keyboard::Space:   _pongBall->resetSpeedMultiplierBonus(); break;

			case sf::Keyboard::Numpad2: _pongBall->startBoostBall(8.f); break;
			case sf::Keyboard::Numpad3: _pongBall->startBoostBall(16.f); break;
			case sf::Keyboard::Numpad4: _pongBall->startBoostBall(32.f); break;
			case sf::Keyboard::Numpad5: _pongBall->startBoostBall(64.f); break;
			case sf::Keyboard::Numpad6: _pongBall->startBoostBall(128.f); break;

				//Player 1
			//case sf::Keyboard::Left:  _character1->direction(true, false, false, false, _polygonTerrain->getPlayableArea(), deltaTime);
			case sf::Keyboard::Left:  _character1->direction(true, false, false, false, _polygonTerrain->getPlayableArea(), deltaTime); break;
			case sf::Keyboard::Right: _character1->direction(false, true, false, false, _polygonTerrain->getPlayableArea(), deltaTime); break;
			case sf::Keyboard::Up:    _character1->direction(false, false, true, false, _polygonTerrain->getPlayableArea(), deltaTime); break;
			case sf::Keyboard::Down:  _character1->direction(false, false, false, true, _polygonTerrain->getPlayableArea(), deltaTime); break;
				//Player 2
			case sf::Keyboard::Q: _character2->direction(true, false, false, false, _polygonTerrain->getPlayableArea(), deltaTime); break;
			case sf::Keyboard::D: _character2->direction(false, true, false, false, _polygonTerrain->getPlayableArea(), deltaTime); break;
			case sf::Keyboard::Z: _character2->direction(false, false, true, false, _polygonTerrain->getPlayableArea(), deltaTime); break;
			case sf::Keyboard::S: _character2->direction(false, false, false, true, _polygonTerrain->getPlayableArea(), deltaTime); break;

			default: break;
		}
	}
	*/

	// Joueur 1
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		_character1->direction(true, false, false, false, _polygonTerrain->getPlayableArea(), deltaTime);
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		_character1->direction(false, true, false, false, _polygonTerrain->getPlayableArea(), deltaTime);
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		_character1->direction(false, false, true, false, _polygonTerrain->getPlayableArea(), deltaTime);
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		_character1->direction(false, false, false, true, _polygonTerrain->getPlayableArea(), deltaTime);
	}
	else
	{
		_character1->direction(false, false, false, false, _polygonTerrain->getPlayableArea(), deltaTime);
	}


	// Joueur 2
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		_character2->direction(true, false, false, false, _polygonTerrain->getPlayableArea(), deltaTime);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		_character2->direction(false, true, false, false, _polygonTerrain->getPlayableArea(), deltaTime);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		_character2->direction(false, false, true, false, _polygonTerrain->getPlayableArea(), deltaTime);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		_character2->direction(false, false, false, true, _polygonTerrain->getPlayableArea(), deltaTime);
	}
	else
	{
		_character2->direction(false, false, false, false, _polygonTerrain->getPlayableArea(), deltaTime);
	}


}

void MainGameScene::start()
{

}

void MainGameScene::initValues()
{
	_polygonTerrain = std::make_unique<PolygonTerrain>(_poPossibEngin->getRenderWindow());
	_pongBall = std::make_unique<PongBall>(_poPossibEngin->getRenderWindow(), _polygonTerrain->getPlayableArea(), *_polygonTerrain);
	_character1 = std::make_unique<Character>(_poPossibEngin->getRenderWindow(),450,200,sf::Color::Red);
	_character2 = std::make_unique<Character>(_poPossibEngin->getRenderWindow(),450,700,sf::Color::Blue);

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
	_poPossibEngin->setMousePosition();
	
	_pongBall->update(deltaTime);
	_character1->setMousePosition(_poPossibEngin->getMousePosition());
	_character2->setMousePosition(_poPossibEngin->getMousePosition());

	_character1->update(deltaTime);
	_character2->update(deltaTime);
}

void MainGameScene::render(sf::RenderTarget* target)
{
	//_terrain->render(*target);
	_polygonTerrain->render(*target);
	_pongBall->render(*target);
	_character1->render(*target);
	_character2->render(*target);
}