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
#define COLOR_PLAYER_2 sf::Color(0, 40, 255)

constexpr int NUM_MAX_PONGBALL = 15;
constexpr int PLAYERS_SPAWN_POINT_X_OFFSET = 75;

#include "../../Game/Controllers/LocalCharacterController/LocalCharacterController.hpp"
#include "../../Game/Controllers/NetworkCharacterController/NetworkCharacterController.hpp"
#include "../../Game/Controllers/PongBallController/NetworkPongBallController.hpp"

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

    for(auto* controller : _controllers)
    {
        delete controller;
        controller = nullptr;
    }
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
	_polygonTerrain = std::make_unique<PolygonTerrain>(_poPossibEngin->getRenderWindow(), _players);
	_animator = std::make_unique<AnimatorManager>();
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

void MainGameScene::update(const float& deltaTime)
{
	updateInputs(deltaTime);

	_gameManager->update(deltaTime);
	_polygonTerrain->update(deltaTime);

    for(const auto controller : _controllers)
    {
        controller->update(deltaTime);
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

	for (const auto player : _players)
	{
		player->render(*target);
	}

	_animator->render(*target);
}

PolygonTerrain* MainGameScene::getPolygonTerrain() const    { return _polygonTerrain.get(); }
std::vector<Character *>& MainGameScene::getPlayers()       { return _players; }
AudioPlayer* MainGameScene::getAudioPlayer() const          { return _audioPlayer.get(); }
AnimatorManager* MainGameScene::getAnimatorManager() const  { return _animator.get(); }
GameManager* MainGameScene::getGameManager() const          { return _gameManager.get(); }

void MainGameScene::displayPlayers(bool isDisplayed) const
{
    for (const auto player : _players)
    {
        player->setPlayerAlive(isDisplayed);
        player->resetAmmos();
    }
}

void MainGameScene::togglePlayersMovement(bool canTheyMove) const
{
	for (const auto player : _players)
	{
		player->toggleCharacterMove(canTheyMove);
	}
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
	_polygonTerrain->drawRandomTerrain();
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

void MainGameScene::hideAllPongBalls() const
{
    for(auto* pongBall : _polygonTerrain->getPongBalls())
    {
        pongBall->setActive(false);
    }
}

Client::SyncableObject *MainGameScene::createPlayer(const SyncableObjectOptions& options, const PlayerState& playerState)
{
    // TODO : differentiation P1 / P2

    Logger::Log("Creating new player id: " + std::to_string(options.id));

    Character* newPlayer = nullptr;

    // FIXME : TEMP
    // P1
    if(options.id == 0)
    {
        Logger::Log("Creating player 1");

        newPlayer = new Character(COLOR_PLAYER_1);
        newPlayer->setAmmosColor(sf::Color(255, 40, 0), sf::Color(255, 160, 160));

        setPlayersToDefaultSpawnPoints(newPlayer, nullptr);
    }
    // P2
    else if(options.id >= 1)
    {
        Logger::Log("Creating player 2");

        newPlayer = new Character(COLOR_PLAYER_2);
        newPlayer->setAmmosColor(sf::Color(0, 40, 255), sf::Color(160, 160, 235));

        setPlayersToDefaultSpawnPoints(nullptr, newPlayer);
    }

    _players.emplace_back(newPlayer);

    Engine::ControllerBase* newPlayerController = nullptr;

    switch (options.control)
    {
        case Local:
            Logger::Log("Creating LocalCharacterController for id: " + std::to_string(options.id));

            newPlayerController = new LocalCharacterController(options, *newPlayer, {
                    sf::Keyboard::Z,
                    sf::Keyboard::S,
                    sf::Keyboard::Q,
                    sf::Keyboard::D,
                    sf::Mouse::Button::Left
            }, playerState);

            break;
        case Remote:
            Logger::Log("Creating NetworkCharacterController for id: " + std::to_string(options.id));

            newPlayerController = new NetworkCharacterController(options, *newPlayer);
            break;
    }

     _controllers.push_back(newPlayerController);

     return newPlayerController;
 }

void MainGameScene::setPlayersToDefaultSpawnPoints(Character* p1, Character* p2) const
{
    if (p1)
    {
        sf::Vector2i spawnPosition(
            (int)_poPossibEngin->getRenderWindow().getSize().x / 2 - PLAYERS_SPAWN_POINT_X_OFFSET,
            (int)_poPossibEngin->getRenderWindow().getSize().y / 2
        );

        p1->setPosition(spawnPosition);
    }

    if (p2)
    {
        sf::Vector2i spawnPosition(
            (int)_poPossibEngin->getRenderWindow().getSize().x / 2 + PLAYERS_SPAWN_POINT_X_OFFSET,
            (int)_poPossibEngin->getRenderWindow().getSize().y / 2
        );

        p2->setPosition(spawnPosition);
    }
}

Client::SyncableObject *MainGameScene::createPongBall(const SyncableObjectOptions& options, const PongBallState& pongBallState)
{
    auto& syncableEntieManager = _poPossibEngin->getSocketManager()->getSocketClient()->getSyncableObjectManager();

    auto* character = syncableEntieManager.getCharacter(options.controllerID);

    if(!character) return nullptr;

    auto* newPongBall = new PongBall(_poPossibEngin->getRenderWindow(), *this);
    auto* pongBallController = new NetworkPongBallController(options, *newPongBall, pongBallState);

    character->addPongBall(newPongBall);
    _controllers.push_back(pongBallController);

    return pongBallController;
}