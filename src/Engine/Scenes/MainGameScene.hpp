#pragma once

#include "Scene.hpp"

class PolygonCollisionResult;
class PolygonTerrain;
class Terrain;
class PongBall;
class PoPossibEngin;
class Character;

class MainGameScene : public Scene
{
public:
	MainGameScene(PoPossibEngin& poPossibEngin);
	~MainGameScene();

	void updateInputs(const float& deltaTime);
	void start();
	void update(const float& deltaTime);
	void render(sf::RenderTarget* target = nullptr);
private:
	std::vector<sf::Texture> _textures;

	//Game objects
	std::unique_ptr<PongBall> _pongBall;
	std::unique_ptr<Character> _character1;
	std::unique_ptr<Character> _character2;
	
	
	
	std::unique_ptr<PolygonTerrain> _polygonTerrain;

	//Font and texts
	sf::Font _font;
	

	void initValues();
	void initFonts();
};