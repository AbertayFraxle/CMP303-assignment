#include "Player.h"
#define MOVESPEED 100
#define COOLDOWN 1

Player::Player() {

	srand(time(NULL));

	team = Team::red;

	redTex.loadFromFile("textures/red.png");
	blueTex.loadFromFile("textures/blue.png");
	yellowTex.loadFromFile("textures/yellow.png");

	setTexture(&redTex);

	spawn();

	setSize(sf::Vector2f(100, 100));
	outline.setSize(sf::Vector2f(106, 106));

	fPos = sf::Vector2i(getPosition().x, getPosition().y);

	setOrigin(sf::Vector2f(25, 50));
	outline.setOrigin(sf::Vector2f(31, 53));

	outline.setTexture(&yellowTex);

	outline.setFillColor(sf::Color::Yellow);

	line[0].color = sf::Color::Yellow;
	line[1].color = sf::Color::Yellow;

	firing = false;
	fFiring = false;
	fInvuln = true;

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

	
	if (invulnTimer <= 0) {
		if (input->isLeftMousePressed()) {
			if (fireTimer <= 0) {
				fireTimer = COOLDOWN;
				setFiring(true);
				input->setLeftMouse(Input::MouseState::UP);
			}
		}
	}
}

void Player::update(float dt)
{
	updated = false;
	if (drawTimer > 0) {
		drawTimer -= dt;
	}

	if (invulnTimer > 0) {
		invulnerable = true;
		invulnTimer -= dt;
	}
	else {
		invulnerable = false;
	}

	sf::Vector2f movement = sf::Vector2f(MOVESPEED*inputVec.x, MOVESPEED* inputVec.y);

	setRotation(angle);

	if (getPosition().x + (movement.x * dt) < 0 || getPosition().x + (movement.x * dt) > 1920) {
		setPosition(sf::Vector2f(getPosition().x, getPosition().y));
	}
	else {
		setPosition(sf::Vector2f(getPosition().x + (movement.x * dt), getPosition().y));
	};

	if (getPosition().y + (movement.y * dt) < 0 || getPosition().y + (movement.y * dt) > 1080) {
		setPosition(sf::Vector2f(getPosition().x, getPosition().y));
	}
	else {
		setPosition(sf::Vector2f(getPosition().x , getPosition().y + (movement.y * dt)));
	};
	
	outline.setPosition(getPosition());
	outline.setRotation(getRotation());

	if (sf::Vector2i(getPosition().x, getPosition().y) != fPos || angle != fAngle || firing != fFiring || invulnerable != fInvuln) {
		updated = true;
	}
	
	
}

void Player::netUpdate(float dt)
{
	if (drawTimer > 0) {
		drawTimer -= dt;
	}
	outline.setPosition(getPosition());
	outline.setRotation(getRotation());

	angleR = getRotation() * 0.0174533;

}

void Player::render() {

	if (drawTimer > 0) {
		window->draw(line, 2, sf::Lines);
	}
	if (invulnerable) {
		window->draw(outline);
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

void Player::spawn() {
	int spawnX = rand() % 1920 + 1;
	int spawnY = rand() % 1080 + 1;

	setPosition(sf::Vector2f(spawnX, spawnY));

	invulnTimer = 4.f;

}