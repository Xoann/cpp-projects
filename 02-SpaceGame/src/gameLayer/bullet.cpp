#include <bullet.h>

void Bullet::render(gl2d::Renderer2D& renderer, gl2d::Texture bulletsTexture, gl2d::TextureAtlasPadding bulletsAtlas) {

	float bulletAngle = atan2(fireDirection.y, -fireDirection.x);
	bulletAngle = glm::degrees(bulletAngle) + 90.f;

	renderer.renderRectangle({ position + glm::vec2(25,25), 50,50 }, bulletsTexture, Colors_White, {}, bulletAngle, bulletsAtlas.get(1, 1));
}

void Bullet::update(float deltaTime) {
	position += fireDirection * deltaTime * 1500.f;
}