//Player.cpp
#include "Player.h"
#include"../GameObject.h"

Player::Player()
	: deltaTime(0.0f), speed(150.0f), playerSpeed(0.0f, 0.0f), counterLeft(0), counterRight(0),
	counterIdle(0), isMovingRight(false), isMovingLeft(false), isJumping(false), isJumpBoost(false),
	animationTime(0), animationSpeed(8.0f * 0.0166667f), jumpTime(0), jumpRate(50.0f * 0.0166667f),
	jumpAnimationcounter(0) , jumpAnimationRate(18.0f * 0.0166667f) , jumpAnimationTime(0){

	if (!texture.loadFromFile("Assets/Adventure/newSpriteplayer.png")) {
		std::cout << "failed to load player texture " << std::endl;
	}

	for (unsigned int i = 0; i < GameMagicNumbers::idleFrame; i++) {
		this->idleAnimation.push_back(sf::IntRect(i * GameMagicNumbers::spriteSize, GameMagicNumbers::zero, GameMagicNumbers::spriteSize, GameMagicNumbers::spriteSize));
	}
	for (unsigned int i = 0; i < GameMagicNumbers::runFrame; i++) {
		this->rightAnimation.push_back(sf::IntRect(i * GameMagicNumbers::spriteSize, GameMagicNumbers::spriteSize, GameMagicNumbers::spriteSize, GameMagicNumbers::spriteSize));
	}
	for (unsigned int i = 0; i < GameMagicNumbers::runFrame; i++) {
		this->jumpAnimation.push_back(sf::IntRect(i * GameMagicNumbers::spriteSize, 2 * GameMagicNumbers::spriteSize, GameMagicNumbers::spriteSize, GameMagicNumbers::spriteSize));
	}

	this->sprite.setTexture(texture);
	this->sprite.setTextureRect(this->idleAnimation[GameMagicNumbers::zero]);

	this->playerCollisionBox.setOutlineThickness(GameMagicNumbers::collisionBoxThickness);
	this->playerCollisionBox.setOutlineColor(sf::Color::Transparent);
	this->playerCollisionBox.setFillColor(sf::Color::Transparent);
	this->playerCollisionBox.setSize(sf::Vector2f(GameMagicNumbers::playerScale * GameMagicNumbers::spriteSize * GameMagicNumbers::collisionBoxSizeScale, GameMagicNumbers::playerScale * GameMagicNumbers::spriteSize));

	this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 2, this->sprite.getGlobalBounds().height / 2);
	this->playerCollisionBox.setOrigin(this->playerCollisionBox.getGlobalBounds().width / 2, this->playerCollisionBox.getGlobalBounds().height/ 2);
}



void Player::Load() {
	this->sprite.setPosition(sf::Vector2f(200.f, 200.f)); // Random starting point, todo: change it to GameMagicNumbers
	this->sprite.setScale(sf::Vector2f(GameMagicNumbers::playerScale, GameMagicNumbers::playerScale));
	this->playerCollisionBox.setPosition(sprite.getPosition());

}


void Player::Update( const float& deltaTime){
	this->deltaTime = deltaTime;
	GravityAffect();
	InputHandle();
	AnimationHandle();
	InputJump();
}

void Player::GravityAffect() {
	this->sprite.move(playerSpeed + gameObject.physic.AffectGravity(playerCollisionBox, gameObject.backGroundPath->GetPathSprite(), deltaTime));
	this->playerCollisionBox.setPosition(sprite.getPosition());
	this->playerSpeed = sf::Vector2f(0.0f, 0.0f);
}

void Player::InputHandle(){
	InputMovement();
	InputJump();
}
void Player::AnimationHandle() {
	AnimationMovement();
	JumpAnimation();
}

void Player::InputMovement()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		this->playerSpeed.x = -this->speed * this->deltaTime;
		this->isMovingRight = false;
		this->isMovingLeft = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		this->playerSpeed.x = this->speed * this->deltaTime;
		this->isMovingRight = true;
		this->isMovingLeft = false;
	}
	else {
		this->isMovingRight = false;
		this->isMovingLeft = false;
	}
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		this->playerSpeed.y = -this->speed * this->deltaTime;    needed for debugging

	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		this->playerSpeed.y = this->speed * this->deltaTime;
	}*/
}

void Player::AnimationMovement() {
	if (!isJumping) {
		this->animationTime += this->deltaTime;
		if (this->animationTime >= this->animationSpeed) {
			this->animationTime -= this->animationSpeed;

			if (this->isMovingLeft || this->isMovingRight) {
				auto& counter = this->isMovingLeft ? this->counterLeft : this->counterRight;
				gameObject.utility.UpdateAnimation(sprite, counter, rightAnimation);
			}
			else {
				gameObject.utility.UpdateAnimation(sprite, counterIdle, idleAnimation);
			}
		}
	}
	float desiredScale = this->isMovingLeft ? -GameMagicNumbers::playerScale : GameMagicNumbers::playerScale;
	sprite.setScale(desiredScale, GameMagicNumbers::playerScale);
}



void Player::InputJump() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !this->isJumping) {
		this->isJumping = true;
		this->isJumpBoost = true;
		this->jumpTime = GameMagicNumbers::zero;
		jumpAnimationcounter = GameMagicNumbers::zero;
	}
	if (isJumpBoost) {
		this->jumpTime += this->deltaTime;
		this->playerSpeed.y = (this->jumpTime <= this->jumpRate) ? -GameMagicNumbers::gravityCounter * this->deltaTime : GameMagicNumbers::zero;
		this->isJumpBoost = (this->jumpTime <= this->jumpRate);
	}
}


void Player::JumpAnimation(){
	if (this->isJumping) {
		this->jumpAnimationTime += this->deltaTime;
		if (this->jumpAnimationTime >= this->jumpAnimationRate) {
			this->jumpAnimationTime -= this->jumpAnimationRate;
			gameObject.utility.UpdateAnimation(sprite, jumpAnimationcounter, jumpAnimation);
		}
	}
}


void Player::Draw(std::shared_ptr<sf::RenderWindow> window){
	window->draw(this->sprite);
	window->draw(this->playerCollisionBox);
}
