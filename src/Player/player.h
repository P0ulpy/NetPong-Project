#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#define			PI					3.14159265
#define         SPRITE_SPEED        10

class player {

public:
	player();
	~player();
	virtual void start();
	virtual void update(float deltaTime);
	virtual void render(RenderTarget& renderTarget);

private:

	void Charater(int speed= SPRITE_SPEED);
	

};