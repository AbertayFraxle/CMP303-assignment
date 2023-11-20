#include "Player.h"
#define MOVESPEED 100
#define COOLDOWN 1

Player::Player() {

	srand(time(NULL));

	team = Team::red;

	redTex.loadFromFile("textures/red.png");
	blueTex.loadFromFile("textures/blue.png");

	setTexture(&redTex);

	int spawnX = rand() % 1920+1;
	int spawnY = rand() % 1080 + 1;

	setSize(sf::Vector2f(100, 100));
	setPosition(sf::Vector2f(spawnX, spawnY));
	fPos = sf::Vector2i(getPosition().x, getPosition().y);

	setOrigin(sf::Vector2f(25, 50));

	line[0].color = sf::Color::Yellow;
	line[1].color = sf::Color::Yellow;

	firing = false;
	fFiring = false;

	drawTimer = 0;
}

Player::~Player() {

}

void Player::handleInput(float dt)
{

	fireTimer -= dt;

	float iX = input->isKeyDown(sf::Keyboard::D) - input->isKeyDown(sf::Keyboard::A);
	float iY = input->isKeyDown(sf::Keyboard::S) - input->isKeyDown(sf::Keyboard::W);

	inputVec = VectorHelper::normalise(sf::Vector2f(iX, iY));

	sf::Vector2f diff = getPosition() - sf::Vector2f(input->getMouseX(), input->getMouseY());

	angle = atan2f(diff.y, diff.x) * (180 / 3.14) + 180;
	angleR = atan2f(diff.y, diff.x) + 3.14;

	

	if (input->isLeftMousePressed()) {
		if (fireTimer <= 0) {
			fireTimer = COOLDOWN;
			setFiring(true);
			input->setLeftMouse(Input::MouseState::UP);
		}
	}
}

void Player::update(float dt)
{
	if (drawTimer > 0) {
		drawTimer -= dt;
	}

	sf::Vector2f movement = sf::Vector2f(MOVESPEED*inputVec.x, MOVESPEED* inputVec.y);

	setRotation(angle);

	move(movement * dt);

	if (sf::Vector2i(getPosition().x, getPosition().y) != fPos || angle != fAngle || firing != fFiring) {
		updated = true;
	}
	else {
		updated = false;
	}
	
}

void Player::netUpdate(float dt)
{
	if (drawTimer > 0) {
		drawTimer -= dt;
	}


	angleR = getRotation() * 0.0174533;

}

void Player::render() {

	if (drawTimer > 0) {
		window->draw(line, 2, sf::Lines);
	}
}

void Player::setWindow(sf::RenderWindow* hwnd)
{
	window = hwnd;
}

void Player::setInput(Input * in)
{
	input = in;
}

void Player::setFPos(sf::Vector2f pos)
{
	fPos = sf::Vector2i(pos.x, pos.y);
}

void Player::setFAngle(float nAngle)
{
	fAngle = nAngle;

}

void Player::setFiring(bool nFire)
{

	firing = nFire;

	if (firing) {
		drawTimer = 0.2f;

		line[0].position = getPosition() + sf::Vector2f(cosf(angleR) * 50, sinf(angleR) * 50);
		line[1].position = getPosition() + sf::Vector2f(cosf(angleR) * 10000, sinf(angleR) * 10000);
	}
}

void Player::setFFiring(bool nFFire)
{
	fFiring = nFFire;
}

void Player::setTeam(sf::Uint8 nTeam)
{
	team = nTeam;
	
	sf::Texture* nTex = new sf::Texture;

	if (team == red) {
		setTexture(&redTex);
	}
	else {
		setTexture(&blueTex);
	}


}

sf::Uint8 Player::getTeam()
{
	return team; 
}

