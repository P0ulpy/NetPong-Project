#include "MainGameScene.hpp"

#include <iostream>

#include "../../Game/Entities/PongBall.hpp"
#include "../../Game/Entities/Character.hpp"
#include "../../Engine/Animator/AnimatorManager.hpp"
#include "../../Game/Terrains/PolygonTerrain.hpp"
#include "../../Engine/Scenes/MainMenuScene.hpp"
#include "../../Game/System/GameManager.hpp"
#include "../../Logger/Logger.hpp"

#define COLOR_PLAYER_1 sf::Color(255, 40, 0)
#define INACTIVE_AMMO_COLOR_PLAYER1 sf::Color(255, 160, 160)
#define NORMAL_AMMO_COLOR_PLAYER1 sf::Color(220, 40, 0)

#include "../../Game/Controllers/LocalCharacterController/LocalCharacterController.hpp"
#include "../../Game/Controllers/NetworkCharacterController/NetworkCharacterController.hpp"
#define COLOR_PLAYER_2 sf::Color(0, 40, 255)
#define INACTIVE_AMMO_COLOR_PLAYER2 sf::Color(160, 160, 235)
#define NORMAL_AMMO_COLOR_PLAYER2 sf::Color(0, 40, 220)

constexpr int NUM_MAX_PONGBALL = 15;
constexpr int PLAYERS_SPAWN_POINT_X_OFFSET = 75;

MainGameScene* MainGameScene::_instance = nullptr;

MainGameScene::MainGameScene(PoPossibEngin& poPossibEngin)
	: Scene(poPossibEngin, SceneConfig())
{
    MainGameScene::_instance = this;

	initValues();
	initFonts();

	setPlayersToDefaultSpawnPoints();

}

MainGameScene::~MainGameScene()
{
    MainGameScene::_instance = nullptr;
};

MainGameScene *MainGameScene::getInstance() { return _instance; }

void MainGameScene::updateInputs(const float& deltaTime)
{
#pragma region DEBUG_KEYS

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
		_gameManager->makePlayerWin(1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
		_gameManager->makePlayerWin(2);
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
}

void MainGameScene::initValues()
{
	_polygonTerrain = std::make_unique<PolygonTerrain>(_poPossibEngin->getRenderWindow(), _pongBalls, _players);
	_animator = std::make_unique<AnimatorManager>();

	for (int i = 0 ; i < NUM_MAX_PONGBALL ; i++)
	{
		_pongBalls.emplace_back(new PongBall(_poPossibEngin->getRenderWindow(), *this));
	}

    auto p0 = new Character(_player0color);
	_players.emplace_back(p0);
	_players.back()->setAmmosColor(sf::Color(255, 40, 0), sf::Color(255, 160, 160));

    auto p1 = new Character(_player1color);
    _players.emplace_back(p1);
	_players.back()->setAmmosColor(sf::Color(0, 40, 255), sf::Color(160, 160, 235));

   /* _controllers.push_back(new LocalCharacterController(0, Client::Remote, Client::Character, *p0, {
            sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right, sf::Mouse::Button::Right
    }));

    _controllers.push_back(new LocalCharacterController(0, Client::Remote, Client::Character, *p1, {
            sf::Keyboard::Z, sf::Keyboard::S, sf::Keyboard::Q, sf::Keyboard::D, sf::Mouse::Button::Left
    }));*/

	_gameManager = std::make_shared<GameManager>(this);
}

void MainGameScene::initFonts()
{
	if (!_font.loadFromFile("./Assets/arial.ttf"))
	{
		Logger::Err("ERROR FONT NOT LOADED - MainGameScene.cpp");
	}
}

void MainGameScene::makePlayerShoot(int playerIndex)
{
	if (playerIndex < 0 ||
		playerIndex > _players.size() - 1 ||
		_inactivePongBalls.empty() ||
		!_players[playerIndex]->canCharacterMove())
	{
		return;
	}

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

void MainGameScene::checkPlayerPongBallCollision(const PongBall& pongBall) const
{
	for (int currPlayerIndex = 0; currPlayerIndex < _players.size(); ++currPlayerIndex)
	{
		if (pongBall.hitPlayer(_players[currPlayerIndex]->getPosition().x, _players[currPlayerIndex]->getPosition().y, _players[currPlayerIndex]->getRadius(), _players[currPlayerIndex]->getNormalAmmoColor()))
		{
			_players[currPlayerIndex]->setPlayerAlive(false);
			_animator->DeathAnimation(_players[currPlayerIndex]->getPosition());
			_gameManager->makePlayerWin(currPlayerIndex+1);

			std::cout << "Player hit !" << std::endl;
		}
	}
}

void MainGameScene::update(const float& deltaTime)
{
	
	
	updateInputs(deltaTime);

	_poPossibEngin->getInputsManager().update();
	_gameManager->update(deltaTime);
	_polygonTerrain->update(deltaTime);

    for(const auto controller : _controllers)
        controller->update(deltaTime);

	for (const auto pongBall : _pongBalls)
	{
		pongBall->update(deltaTime);

		checkPlayerPongBallCollision(*pongBall);
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

	_animator->render(*target);
}

PolygonTerrain* MainGameScene::getPolygonTerrain() const { return _polygonTerrain.get(); }
void MainGameScene::displayPlayers(bool isDisplayed) const
{
	for (const auto player : _players)
	{
		player->setPlayerAlive(isDisplayed);
		player->resetAmmos();
	}
}
void MainGameScene::hideAllPongBalls() const
{
	for (const auto pongBall : _pongBalls)
	{
		pongBall->setActive(false);
	}
}
void MainGameScene::pushInactivePongBall(PongBall* pongBallToPush) { _inactivePongBalls.push(pongBallToPush); }

void MainGameScene::togglePlayersMovement(bool canTheyMove) const
{
	for (const auto player : _players)
	{
		player->toggleCharacterMove(canTheyMove);
	}
}

void MainGameScene::pushInactivePongBall(PongBall* pongBallToPush)
{
	_inactivePongBalls.push(pongBallToPush);
}

void MainGameScene::setPlayersToDefaultSpawnPoints() const
{
    sf::Vector2i renderWindowSize = (sf::Vector2i)_poPossibEngin->getRenderWindow().getSize();

    _players[0]->setPosition({
        renderWindowSize.x / 2 - PLAYERS_SPAWN_POINT_X_OFFSET,
        renderWindowSize.y / 2
    });

    _players[1]->setPosition({
        renderWindowSize.x / 2 + PLAYERS_SPAWN_POINT_X_OFFSET,
        renderWindowSize.y / 2
    });
}

std::stack<PongBall *> &MainGameScene::getInactivePongBalls() { return _inactivePongBalls; }