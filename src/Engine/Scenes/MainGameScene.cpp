#include "MainGameScene.hpp"

#include <iostream>

#include "../../Game/Entities/PongBall.hpp"
#include "../../Game/Entities/Character.hpp"
#include "../../Engine/Animator/AnimatorManager.hpp"
#include "../../Game/Terrains/PolygonTerrain.hpp"
#include "../../Engine/Scenes/MainMenuScene.hpp"
#include "../../Game/System/GameManager.hpp"
#include "../../Game/System/Audio/AudioPlayer.hpp"

#define COLOR_PLAYER_1 sf::Color(255, 40, 0)

#include "../../Game/Controllers/LocalCharacterController/LocalCharacterController.hpp"
#include "../../Game/Controllers/NetworkCharacterController/NetworkCharacterController.hpp"
#define COLOR_PLAYER_2 sf::Color(0, 40, 255)

constexpr int NUM_MAX_PONGBALL = 15;
constexpr int PLAYERS_SPAWN_POINT_X_OFFSET = 75;

MainGameScene* MainGameScene::_instance = nullptr;

MainGameScene::MainGameScene(PoPossibEngin& poPossibEngin)
	: Scene(poPossibEngin, SceneConfig())
{
    MainGameScene::_instance = this;

	initValues();
	initFonts();

	getPolygonTerrain()->drawRandomTerrain();
	setPlayersToDefaultSpawnPoints();
	togglePlayersMovement(false);
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

	_audioPlayer = std::make_unique<AudioPlayer>();
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
	if (playerIndex < 0 || playerIndex > _players.size() - 1 ||
		_inactivePongBalls.empty() ||
		!_players[playerIndex]->canCharacterMove())
	{
		return;
	}

	if (_players[playerIndex]->canCharacterShoot())
	{
		_inactivePongBalls.top()->shoot(
			_players[playerIndex]->shootDepart(),
			_players[playerIndex]->shootDirection(_poPossibEngin->getInputsManager().getMousePosition()),
			_players[playerIndex]->getNormalAmmoColor(),
			_players[playerIndex]->getInactiveAmmoColor()
		);

		_players[playerIndex]->ammoCount(-1);
		_players[playerIndex]->activateCooldown(true);
		getAudioPlayer()->playSound("Shoot");

		_inactivePongBalls.pop();
	}
}

void MainGameScene::checkPlayerPongBallCollision(const PongBall& pongBall) const
{
	for (int currPlayerIndex = 0; currPlayerIndex < _players.size(); ++currPlayerIndex)
	{
		if (pongBall.hitPlayer(_players[currPlayerIndex]->getPosition().x,
								_players[currPlayerIndex]->getPosition().y,	
							_players[currPlayerIndex]->getRadius(),
								_players[currPlayerIndex]->getNormalAmmoColor()))
		{
			_players[currPlayerIndex]->setPlayerAlive(false);
			getAudioPlayer()->playSound("Explosion");
			_animator->DeathAnimation(_players[currPlayerIndex]->getPosition());
			_gameManager->makePlayerWin(currPlayerIndex+1);
		}
	}
}

void MainGameScene::update(const float& deltaTime)
{
	updateInputs(deltaTime);

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
AudioPlayer* MainGameScene::getAudioPlayer() const
{
	return _audioPlayer.get();
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

void MainGameScene::setPlayersToDefaultSpawnPoints() const
{
    sf::Vector2i renderWindowSize = (sf::Vector2i)_poPossibEngin->getRenderWindow().getSize();

   /*_players[0]->setPosition({
        renderWindowSize.x / 2 - PLAYERS_SPAWN_POINT_X_OFFSET,
        renderWindowSize.y / 2
    });

    _players[1]->setPosition({
        renderWindowSize.x / 2 + PLAYERS_SPAWN_POINT_X_OFFSET,
        renderWindowSize.y / 2
    });*/
}


void MainGameScene::startFirstRound() const
{
	setPlayersToDefaultSpawnPoints();

	for (const auto player : _players)
	{
		player->setPlayerAlive(true);
		player->resetAmmos();
	}
}

void MainGameScene::restartRound() const
{
	getPolygonTerrain()->drawRandomTerrain();
	setPlayersToDefaultSpawnPoints();

	for (const auto player : _players)
	{
		player->setPlayerAlive(true);
		player->resetAmmos();
	}
}

void MainGameScene::endRound() const
{
	hideAllPongBalls();
	togglePlayersMovement(false);
}

Client::SyncableObject *MainGameScene::createPlayer(SyncableObjectOptions options)
{
    // TODO : differentiation P1 / P2

    Logger::Log("Creating new player id: " + std::to_string(options.id));

    sf::Vector2i renderWindowSize = (sf::Vector2i)_poPossibEngin->getRenderWindow().getSize();

    Character* newPlayer = nullptr;

    // FIXME : TEMP
    if(options.id == 0)
    {
        Logger::Log("Creating player 1");

        newPlayer = new Character(COLOR_PLAYER_1);
        newPlayer->setAmmosColor(sf::Color(255, 40, 0), sf::Color(255, 160, 160));

        // FIXME : APPLY JULEN FIX
        newPlayer->setPosition({
               renderWindowSize.x / 2 - PLAYERS_SPAWN_POINT_X_OFFSET,
               renderWindowSize.y / 2
        });
    }
    else if(options.id >= 1)
    {
        Logger::Log("Creating player 2");

        newPlayer = new Character(COLOR_PLAYER_2);
        newPlayer->setAmmosColor(sf::Color(0, 40, 255), sf::Color(160, 160, 235));

        // FIXME : APPLY JULEN FIX
        newPlayer->setPosition({
               renderWindowSize.x / 2 + PLAYERS_SPAWN_POINT_X_OFFSET,
               renderWindowSize.y / 2
       });
    }

    _players.emplace_back(newPlayer);

    Engine::ControllerBase* newPlayerController = nullptr;

    switch (options.control)
    {
        case Local:
            Logger::Log("Creating LocalCharacterController for id: " + std::to_string(options.id));

            newPlayerController = new LocalCharacterController(options, *newPlayer, {
                    sf::Keyboard::Up,
                    sf::Keyboard::Down,
                    sf::Keyboard::Left,
                    sf::Keyboard::Right,
                    sf::Mouse::Button::Right
            });
            break;
        case Remote:
            Logger::Log("Creating NetworkCharacterController for id: " + std::to_string(options.id));

            newPlayerController = new NetworkCharacterController(options, *newPlayer);
            break;
    }

     _controllers.push_back(newPlayerController);

     return newPlayerController;
 }
