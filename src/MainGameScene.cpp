#include "MainGameScene.h"

MainGameScene::MainGameScene(sf::RenderWindow* window)
	: Scene(window)
{
	_window = std::make_unique<sf::RenderWindow*>(window);

	initValues();
	initFonts();
}

MainGameScene::~MainGameScene()
{

}

void MainGameScene::updateInputs()
{
	//C'EST DEGEULASSE
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		_pongBall->resetSpeedBonusRatio();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad0))
		_pongBall->createPhantomBalls();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))
		_pongBall->activatedErasingLastPhantomBall();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
		_pongBall->startPhantomBallEffect();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3))
		_pongBall->boostBall(8.f);
}

void MainGameScene::initValues()
{
	_terrain = std::make_unique<Terrain>(**_window);//QUELLE HORREUR
	_pongBall = std::make_unique<PongBall>(**_window, _terrain->getPlayableArea());
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
	_pongBall->update(**_window, deltaTime);
}

void MainGameScene::render(sf::RenderTarget* target)
{
	_terrain->render(*target);
	_pongBall->render(*target);
}
