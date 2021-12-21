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
	sf::Event event;

	
	if (_poPossibEngin->getInputsManager().getEvent(sf::Event::KeyPressed, event))
	{
		switch (event.key.code)
		{
			case sf::Keyboard::Space:   _pongBall->resetSpeedMultiplierBonus();
			case sf::Keyboard::Numpad2: _pongBall->startBoostBall(8.f);
			case sf::Keyboard::Numpad3: _pongBall->startBoostBall(16.f);
			case sf::Keyboard::Numpad4: _pongBall->startBoostBall(32.f);
			case sf::Keyboard::Numpad5: _pongBall->startBoostBall(64.f);
			case sf::Keyboard::Numpad6: _pongBall->startBoostBall(128.f);

				//Player 1
			case sf::Keyboard::Left:  _character->direction(1, false, false, false, _polygonTerrain->getPlayableArea(), deltaTime);
			case sf::Keyboard::Right: _character->direction(false, 1, false, false, _polygonTerrain->getPlayableArea(), deltaTime);
			case sf::Keyboard::Up:    _character->direction(false, false, 1, false, _polygonTerrain->getPlayableArea(), deltaTime);
			case sf::Keyboard::Down:  _character->direction(false, false, false, 1, _polygonTerrain->getPlayableArea(), deltaTime);
				//Player 2
			case sf::Keyboard::Q: _character->directionP2(1, false, false, false, _polygonTerrain->getPlayableArea(), deltaTime);
			case sf::Keyboard::D: _character->directionP2(false, 1, false, false, _polygonTerrain->getPlayableArea(), deltaTime);
			case sf::Keyboard::Z: _character->directionP2(false, false, 1, false, _polygonTerrain->getPlayableArea(), deltaTime);
			case sf::Keyboard::S: _character->directionP2(false, false, false, 1, _polygonTerrain->getPlayableArea(), deltaTime);

			default: break;
		}
	}
	//Si une touche est relachée
	if (_poPossibEngin->getInputsManager().getEvent(sf::Event::KeyReleased, event))
	{
		_character->direction(false, false, false, false, _polygonTerrain->getPlayableArea(), deltaTime);
	}

	
	
}

void MainGameScene::start()
{

}

void MainGameScene::initValues()
{
	_polygonTerrain = std::make_unique<PolygonTerrain>(_poPossibEngin->getRenderWindow());

	_pongBall = std::make_unique<PongBall>(_poPossibEngin->getRenderWindow(), _polygonTerrain->getPlayableArea(), *_polygonTerrain);
	_character = std::make_unique<Character>(_poPossibEngin->getRenderWindow());
	_character2 = std::make_unique<Character>(_poPossibEngin->getRenderWindow());

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
	_poPossibEngin->setMousePosition();
	updateInputs(deltaTime);
	_pongBall->update(deltaTime);
	_character->updateMouvement(deltaTime);
	_character->updateMouvementP2(deltaTime);
	_character1->updateMouvement(deltaTime);
	_character2->updateMouvement(deltaTime);
}

void MainGameScene::render(sf::RenderTarget* target)
{
	//_terrain->render(*target);
	_polygonTerrain->render(*target);
	_pongBall->render(*target);
	_character->render(*target);
}