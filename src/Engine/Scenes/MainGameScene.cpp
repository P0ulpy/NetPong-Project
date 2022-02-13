#include "MainGameScene.hpp"

#include <iostream>

#include "../../Game/Entities/PongBall.hpp"
#include "../../Game/Entities/Character.hpp"
#include "../../Game/Terrains/PolygonTerrain.hpp"
#include "../../Game/System/GameManager.hpp"

constexpr int NUM_MAX_PONGBALL = 500;

constexpr int PLAYERS_SPAWN_POINT_X_OFFSET = 75;


MainGameScene::MainGameScene(PoPossibEngin& poPossibEngin)
	: Scene(poPossibEngin, SceneConfig())
{
	initValues();
	initFonts();

	setPlayersToDefaultSpawnPoints();
}

MainGameScene::~MainGameScene() = default;

void MainGameScene::updateInputs(const float& deltaTime)
{
#pragma region DEBUG_KEYS

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
		_gameManager->player1WinsRound();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
		_gameManager->player2WinsRound();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
		for (const auto pongBall : _pongBalls)
			pongBall->startPhantomBallEffect();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
		for (const auto pongBall : _pongBalls)
			pongBall->stopPhantomBallEffect();
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

#pragma endregion

    // POSITION
    {
        float x = 0;
        float y = 0;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) x = 1;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) x = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) y = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))	y = -1;

        _players[0]->moveEntity(sf::Vector2f(x, y), deltaTime);
    }

    {
        float x = 0;
        float y = 0;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) x = 1;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) x = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) y = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))	y = -1;

        _players[1]->moveEntity(sf::Vector2f(x, y), deltaTime);
    }

	//Shoot
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		makePlayerShoot(0);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
		makePlayerShoot(1);
}

void MainGameScene::initValues()
{
	_polygonTerrain = std::make_unique<PolygonTerrain>(_poPossibEngin->getRenderWindow(), _pongBalls, _players);

	for (int i = 0 ; i < NUM_MAX_PONGBALL ; i++)
	{
		_pongBalls.emplace_back(new PongBall(_poPossibEngin->getRenderWindow(), *this));
	}
	
	_players.emplace_back(new Character(_player0color));
	_players.back()->setAmmosColor(sf::Color(255, 40, 0), sf::Color(255, 160, 160));

	_players.emplace_back(new Character(_player1color));
	_players.back()->setAmmosColor(sf::Color(0, 40, 255), sf::Color(160, 160, 235));

	_gameManager = std::make_shared<GameManager>(this);
}

void MainGameScene::initFonts()
{
	if (!_font.loadFromFile("./Assets/arial.ttf"))
	{
		std::cout << "ERROR FONT NOT LOADED - MainGameScene.cpp" << std::endl;
	}
}

void MainGameScene::makePlayerShoot(int playerIndex)
{
	if (playerIndex < 0 || playerIndex > _players.size() - 1) return;
	if (_inactivePongBalls.empty()) return;

	if (!_players[playerIndex]->isInCooldown() && !_players[playerIndex]->isReloading())
	{
		_inactivePongBalls.top()->shoot(
			_players[playerIndex]->shootDepart(),
			_players[playerIndex]->shootDirection(_poPossibEngin->getInputsManager().getMousePosition()),
			_players[playerIndex]->getNormalAmmoColor(),
			_players[playerIndex]->getInactiveAmmoColor()
		);

		_players[playerIndex]->ammoCount(-1);
		_players[playerIndex]->activateCooldown(true);

		_inactivePongBalls.pop();
	}
}

void MainGameScene::update(const float& deltaTime)
{
	updateInputs(deltaTime);
	_poPossibEngin->getInputsManager().update();
	_gameManager->update(deltaTime);
	_polygonTerrain->update(deltaTime);

	for (const auto pongBall : _pongBalls)
	{
		pongBall->update(deltaTime);

        // TODO : REMOVE THIS
		/*if (pongBall->hitPlayer(_players[0]->getPositionAndRadiusCharac().x, _players[0]->getPositionAndRadiusCharac().y, _players[0]->getPositionAndRadiusCharac().z, _player0color))
		{
			std::cout << "Player red hit" << std::endl;
		}

		if (pongBall->hitPlayer(_players[1]->getPositionAndRadiusCharac().x, _players[1]->getPositionAndRadiusCharac().y, _players[1]->getPositionAndRadiusCharac().z, _player1color))
		{
			std::cout << "Player blue hit" << std::endl;
		}*/
	}

	for (const auto player : _players)
	{
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

PolygonTerrain* MainGameScene::getPolygonTerrain() const { return _polygonTerrain.get(); }
void MainGameScene::hideAllPongBalls() const
{
	for (auto pongBall : _pongBalls)
	{
		pongBall->setActive(false);
	}
}
void MainGameScene::pushInactivePongBall(PongBall* pongBallToPush) { _inactivePongBalls.push(pongBallToPush); }
void MainGameScene::setPlayersToDefaultSpawnPoints() const
{
    sf::Vector2i renderWindowSize = (sf::Vector2i)_poPossibEngin->getRenderWindow().getSize();

    _players[0]->setPosition(renderWindowSize.x / 2 - PLAYERS_SPAWN_POINT_X_OFFSET,
                             renderWindowSize.y / 2);

    _players[1]->setPosition(renderWindowSize.x / 2 + PLAYERS_SPAWN_POINT_X_OFFSET,
                             renderWindowSize.y / 2);
}