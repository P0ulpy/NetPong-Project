#include "MainGameScene.hpp"
#include "../../Game/Entities/PongBall.hpp"
#include "../../Game/Entities/Terrain.hpp"
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

void MainGameScene::updateInputs()
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
	updateInputs();
	_pongBall->update(deltaTime);
}

void MainGameScene::render(sf::RenderTarget* target)
{
	//_terrain->render(*target);
	_polygonTerrain->render(*target);
	_pongBall->render(*target);
}