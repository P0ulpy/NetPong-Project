#pragma once

#include <stack>
#include <memory>
#include "Scene.hpp"
#include "../../Game/Controllers/ControllerBase.hpp"
#include "../SocketsManagement/Client/SyncableObjectManagement/SyncableObject.hpp"

class AudioPlayer;
class GameManager;
class PolygonTerrain;
class Terrain;
class PongBall;
class PoPossibEngin;
class Character;
class AnimatorManager;


class MainGameScene : public Scene
{
public:
	MainGameScene(PoPossibEngin& poPossibEngin);
	~MainGameScene();

    static MainGameScene* getInstance();

	void updateInputs(const float& deltaTime);
	void start() override {};
	void update(const float& deltaTime) override;
	void render(sf::RenderTarget* target) override;

	void hideAllPongBalls() const;
	void togglePlayersMovement(bool canTheyMove) const;
    std::stack<PongBall*>& getInactivePongBalls();
    void pushInactivePongBall(PongBall* pongBallToPush);
	void startFirstRound() const;
	void restartRound() const;
	void endRound() const;

	//gets - Sets
	PolygonTerrain* getPolygonTerrain() const;
    void displayPlayers(bool isDisplayed) const;
	AudioPlayer* getAudioPlayer() const;

    Client::SyncableObject* createPlayer(SyncableObjectOptions options);

    // GET

private:
    static MainGameScene* _instance;

	std::shared_ptr<GameManager> _gameManager;
	std::unique_ptr<AnimatorManager> _animator;
	std::unique_ptr<AudioPlayer> _audioPlayer;

    // Controllers
    std::vector<Engine::ControllerBase*> _controllers;

	//Game objects
	std::vector<PongBall*> _pongBalls;
	std::stack<PongBall*> _inactivePongBalls;
	std::vector<Character*> _players;

	std::unique_ptr<PolygonTerrain> _polygonTerrain;

	//Font, texts and textures..
	sf::Font _font;

	void initValues();
	void initFonts();
	//void makePlayerShoot(int playerIndex);
	void checkPlayerPongBallCollision(const PongBall& pongBall) const;

    void setPlayersToDefaultSpawnPoints(Character *p1 = nullptr, Character *p2 = nullptr) const;
};
