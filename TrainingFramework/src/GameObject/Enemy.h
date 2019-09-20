#pragma once

#include "Sprite2D.h"
#include <Bullet.h>

class Enemy : public Sprite2D
{
public:
	Enemy(std::shared_ptr<Models>& model, std::shared_ptr<Shaders>& shader, std::shared_ptr<Texture>& texture);
	~Enemy();
	bool		IsActive();
	void		SetActive(bool status);

	void		Update(float deltaTime) override;

	void		CheckCollider(std::vector<std::shared_ptr<Bullet>>& listBullet);

	void		SetColliderSize(float size);
	float		GetColliderSize();

	void		SetDamage(float damage);
	float		GetDamage();

	void		Explosive();
	bool		IsExplosive();

private:
	int		m_Health;
	bool	m_active;
	bool	m_Explosive;
	float	m_speed;
	float	m_MaxSpeed;
	float	m_MaxCooldown;
	float	m_Cooldown;
	float	m_Damage;
	float	m_SizeCollider;

	float distance(Vector2 pos, Vector2 target);

};
