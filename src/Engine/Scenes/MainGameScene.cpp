#include "MainGameScene.hpp"

#include <iostream>

#include "../../Game/Entities/PongBall.hpp"
#include "../../Game/Terrains/Terrain.hpp"
#include "../../Game/Entities/Character.hpp"
#include "../../Game/Terrains/PolygonTerrain.hpp"

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

	
	// Joueur 1
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
	{
		_character->direction(true, false, _polygonTerrain->getPlayableArea());
	}
	else
	{
		_character->direction(false, false, _polygonTerrain->getPlayableArea());
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
	{
		_character->direction(false, true, _polygonTerrain->getPlayableArea());
	}
	else
	{
		_character->direction(false, false, _polygonTerrain->getPlayableArea());
	}


	// Joueur 2

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		_character->directionP2(true, false, _polygonTerrain->getPlayableArea());
	}
	else
	{
		_character->directionP2(false, false, _polygonTerrain->getPlayableArea());
	}



	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		_character->directionP2(false, true, _polygonTerrain->getPlayableArea());
	}
	else
	{
		_character->directionP2(false, false, _polygonTerrain->getPlayableArea());
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
	_polygonTerrain = std::make_unique<PolygonTerrain>(_poPossibEngin->getRenderWindow(), _pongBalls);

	for (int i = 0 ; i < NUM_MAX_PONGBALL ; i++)
	{
		_pongBalls.emplace_back(new PongBall(_poPossibEngin->getRenderWindow(), _polygonTerrain->getPlayableArea(), *_polygonTerrain));
	}

	_pongBall = std::make_unique<PongBall>(_poPossibEngin->getRenderWindow(), _polygonTerrain->getPlayableArea(), *_polygonTerrain);
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
	_polygonTerrain->update(deltaTime);
	//_pongBall->update(deltaTime);
	for (const auto pongBall : _pongBalls)
	{
		pongBall->update(deltaTime);
	}
	_character->updateMouvement(_poPossibEngin->getRenderWindow(),_poPossibEngin->getRenderWindow(), clock);
	_character->updateMouvementP2(_poPossibEngin->getRenderWindow(),_poPossibEngin->getRenderWindow(),clock);
}

void MainGameScene::render(sf::RenderTarget* target)
{
	//_terrain->render(*target);
	_polygonTerrain->render(*target);
	//_pongBall->render(*target);

	for (const auto pongBall : _pongBalls)
	{
		pongBall->render(*target);
	}
	//_character->render(*target);
	//_character->renderP2(*target);
}