#include "MainGameScene.hpp"

#include <iostream>

#include "../../Game/Entities/PongBall.hpp"
#include "../../Game/Terrains/Terrain.hpp"
#include "../../Game/Entities/Character.hpp"
#include "../../Game/Terrains/PolygonTerrain.hpp"
#include "../../Game/System/GameManager.hpp"

constexpr int NUM_MAX_PONGBALL = 1;

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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
		_gameManager->player1WinsRound();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
		_gameManager->player2WinsRound();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
		for (const auto pongBall : _pongBalls)
			pongBall->setActive(true);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
		hideAllPongBalls();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
		for (const auto pongBall : _pongBalls)
			pongBall->startBoostBall(8.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3))
		for (const auto pongBall : _pongBalls)
			pongBall->startBoostBall(16.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
		for (const auto pongBall : _pongBalls)
			pongBall->startBoostBall(32.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5))
		for (const auto pongBall : _pongBalls)
			pongBall->startBoostBall(64.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6))
		for (const auto pongBall : _pongBalls)
			pongBall->startBoostBall(128.f);
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
		_players[0]->direction(true, false, false, false, _polygonTerrain->getPlayableArea(), deltaTime);
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		_players[0]->direction(false, true, false, false, _polygonTerrain->getPlayableArea(), deltaTime);
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		_players[0]->direction(false, false, true, false, _polygonTerrain->getPlayableArea(), deltaTime);
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		_players[0]->direction(false, false, false, true, _polygonTerrain->getPlayableArea(), deltaTime);
	}
	else
	{
		_players[0]->direction(false, false, false, false, _polygonTerrain->getPlayableArea(), deltaTime);
	}


	// Joueur 2
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		_players[1]->direction(true, false, false, false, _polygonTerrain->getPlayableArea(), deltaTime);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		_players[1]->direction(false, true, false, false, _polygonTerrain->getPlayableArea(), deltaTime);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		_players[1]->direction(false, false, true, false, _polygonTerrain->getPlayableArea(), deltaTime);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		_players[1]->direction(false, false, false, true, _polygonTerrain->getPlayableArea(), deltaTime);
	}
	else
	{
		_players[1]->direction(false, false, false, false, _polygonTerrain->getPlayableArea(), deltaTime);
	}


}

void MainGameScene::initValues()
{
	_polygonTerrain = std::make_unique<PolygonTerrain>(_poPossibEngin->getRenderWindow(), _pongBalls, _players);

	for (int i = 0 ; i < NUM_MAX_PONGBALL ; i++)
	{
		_pongBalls.emplace_back(new PongBall(_poPossibEngin->getRenderWindow(), _polygonTerrain->getPlayableArea(), *_polygonTerrain));
	}

	_players.emplace_back(new Character(_poPossibEngin->getRenderWindow(), 450, 200, sf::Color::Red));
	_players.emplace_back(new Character(_poPossibEngin->getRenderWindow(), 450, 700, sf::Color::Blue));

	_gameManager = std::make_shared<GameManager>(this);
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
	
	_gameManager->update(deltaTime);

	_polygonTerrain->update(deltaTime);

	for (const auto pongBall : _pongBalls)
	{
		pongBall->update(deltaTime);
	}

	for (const auto player : _players)
	{
		player->setMousePosition(_poPossibEngin->getMousePosition());
		player->update(deltaTime);
	}
}

void MainGameScene::render(sf::RenderTarget* target)
{
	_gameManager->render(*target);
	_polygonTerrain->render(*target);

	for (const auto pongBall : _pongBalls)
	{
		pongBall->render(*target);
	}

	for (const auto player : _players)
	{
		player->render(*target);
	}
}

PolygonTerrain* MainGameScene::getPolygonTerrain() const
{
	return _polygonTerrain.get();
}

void MainGameScene::hideAllPongBalls()
{
	for (auto pongBall : _pongBalls)
	{
		pongBall->setActive(false);
	}
}
