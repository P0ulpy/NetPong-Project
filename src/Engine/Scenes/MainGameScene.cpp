#include "MainGameScene.hpp"
#include "../../Game/Entities/PongBall.hpp"
#include "../../Game/Entities/Terrain.hpp"

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
		_pongBall->resetSpeedBonusRatio();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
		_pongBall->startPhantomBallEffect();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3))
		_pongBall->boostBall(8.f);
}

void MainGameScene::start()
{

}

void MainGameScene::initValues()
{
	_terrain = std::make_unique<Terrain>(_poPossibEngin->getRenderWindow());
	_pongBall = std::make_unique<PongBall>(_poPossibEngin->getRenderWindow(), _terrain->getPlayableArea());
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
	_pongBall->update(_poPossibEngin->getRenderWindow(), deltaTime);
}

void MainGameScene::render(sf::RenderTarget* target)
{
	_terrain->render(*target);
	_pongBall->render(*target);
}