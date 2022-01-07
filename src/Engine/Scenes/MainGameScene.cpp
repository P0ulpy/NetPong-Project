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
		_gameManager->startRoundStartCountdown();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
		for (const auto pongBall : _pongBalls)
			pongBall->setActive(true);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
		for (const auto pongBall : _pongBalls)
			pongBall->setActive(false);
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

	//TODO : Renseigner les deux joueurs dans le constructeur de GameManager
	_gameManager = std::make_unique<GameManager>(_character.get(), _character.get());
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

	_gameManager->update(deltaTime);

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
	_gameManager->render(*target);

	_polygonTerrain->render(*target);

	for (const auto pongBall : _pongBalls)
	{
		pongBall->render(*target);
	}
	//_character->render(*target);
	//_character->renderP2(*target);
}