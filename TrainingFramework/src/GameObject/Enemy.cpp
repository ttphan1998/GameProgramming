#include "Enemy.h"
#include "GameManager/ResourceManagers.h"
#include <GameStates\GSPlay.h>

Enemy::Enemy(std::shared_ptr<Models>& model, std::shared_ptr<Shaders>& shader, std::shared_ptr<Texture>& texture)
	:Sprite2D(model, shader, texture)
{
	m_active = false;
	m_MaxCooldown = 0.3;
	m_Cooldown = 0.0;
	m_speed = 250;
	m_MaxSpeed = 500;
	m_Health = 5;
	m_Damage = 10;
	m_Explosive = false;
	m_SizeCollider = 20;
}

Enemy::~Enemy()
{
}


void Enemy::Update(float deltaTime)
{
	if (!m_active)
		return;

	if (m_Health <= 0 || m_Explosive)
	{
		SoundManager::GetInstance()->PlaySound("explosive");
		m_Explosive = true;
		GSPlay::m_score ++;
		return;
	}

	if (m_Cooldown > 0)
	{
		m_Cooldown -= deltaTime;
	}

	Vector2 pos = Get2DPosition();
	pos.y = pos.y + m_speed * deltaTime;
	Set2DPosition(pos);

	if (pos.y > Application::screenHeight)
		m_active = false;
}




float Enemy::distance(Vector2 pos, Vector2 target)
{
	return sqrt((pos.x - target.x) * (pos.x - target.x) + (pos.y - target.y) * (pos.y - target.y));
}


void Enemy::CheckCollider(std::vector<std::shared_ptr<Bullet>>& listBullet)
{
	Vector2 pos = Get2DPosition();

	for (auto bullet : listBullet)
	{
		if (bullet->IsActive() && bullet->GetType() == BULLET_TYPE::Player)
		{
			if (distance(pos, bullet->Get2DPosition()) < m_SizeCollider + bullet->GetColliderSize())
			{
				bullet->SetActive(false);
				m_Health -= bullet->GetDamage();
				if (m_Health <= 0)
					return;
			}
		}
	}
}




bool Enemy::IsActive()
{
	return m_active;
}

void Enemy::SetActive(bool status)
{
	m_active = status;
	m_Explosive = false;
	m_Health = 10;
}

void Enemy::SetDamage(float damage)
{
	m_Damage = damage;
}

float Enemy::GetDamage()
{
	return m_Damage;
}

void Enemy::Explosive()
{
	m_Explosive = true;
}

bool Enemy::IsExplosive()
{
	return m_Explosive;
}


void Enemy::SetColliderSize(float size)
{
	m_SizeCollider = size;
}
float Enemy::GetColliderSize()
{
	return m_SizeCollider;
}