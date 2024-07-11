#include "Physic.h"

Physic::Physic():gravity(0,GameMagicNumbers::gravity), velocityOfGravity(0,0){}

sf::Vector2f Physic::AffectGravity(const sf::Sprite& sprite, const sf::RectangleShape& path, const float& deltaTime) {
	velocityOfGravity = gravity * deltaTime;
	if (sprite.getGlobalBounds().intersects(path.getGlobalBounds())) {
		velocityOfGravity.y = GameMagicNumbers::zero;
		return velocityOfGravity;
	}
	return velocityOfGravity;
}