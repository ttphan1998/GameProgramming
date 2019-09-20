#include "GSPlay.h"

#include <sstream>
#include <iomanip>
#include <thread>
#include "Shaders.h"
#include "Texture.h"
#include "Models.h"
#include "Camera.h"
#include "Font.h"
#include "Text.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "ExplosiveEffect.h"

int GSPlay::m_score = 0;
GSPlay::GSPlay()
{
	m_SpawnCooldown = 0.5;
	m_score = 0;
	m_time = 0;
}


GSPlay::~GSPlay()
{

}


void GSPlay::Init()
{
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("bg_play");

	//BackGround
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	m_BackGround = std::make_shared<Sprite2D>(model, shader, texture);
	m_BackGround->Set2DPosition(Application::screenWidth / 2, Application::screenHeight / 2);
	m_BackGround->SetSize(Application::screenWidth, Application::screenHeight);
	m_BackGround->SetRotation(180);

	texture = ResourceManagers::GetInstance()->GetTexture("Player");
	m_Player = std::make_shared<Player >(model, shader, texture);
	m_Player->Set2DPosition(Application::screenWidth / 2, Application::screenHeight - 100);
	m_Player->MoveToPossition(Vector2(Application::screenWidth / 2, Application::screenHeight - 100));
	m_Player->SetSize(50, 50);

	//text game title
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_scoreText = std::make_shared< Text>(shader, font, "SCORE: ", TEXT_COLOR::RED, 1.0);
	m_scoreText->Set2DPosition(Vector2(5, 25));
	m_playerHealthText = std::make_shared< Text>(shader, font, "HP: ", TEXT_COLOR::RED, 1.0);
	m_playerHealthText->Set2DPosition(Vector2(5, 50));

	//init effect
	model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	texture = ResourceManagers::GetInstance()->GetTexture("explosive");
	shader = ResourceManagers::GetInstance()->GetShader("SpriteShader");
	std::shared_ptr<ExplosiveEffect> exp = std::make_shared<ExplosiveEffect>(model, shader, texture, Vector2(960, 768), Vector2(192, 192), 0, 19, 0.7);
	exp->SetSize(100, 100);
	exp->SetActive(false);
	m_listExplosiveEffect.push_back(exp);

	//init sound
	SoundManager::GetInstance()->AddSound("explosive");
	SoundManager::GetInstance()->AddSound("explosive_2");
	SoundManager::GetInstance()->AddSound("fire");
	SoundManager::GetInstance()->PlaySound("Play");

}

void GSPlay::Exit()
{

}


void GSPlay::Pause()
{

}

void GSPlay::Resume()
{

}


void GSPlay::HandleEvents()
{

}

void GSPlay::HandleKeyEvents(int key, bool bIsPressed)
{

}

void GSPlay::HandleMouseEvents(int x, int y)
{
	m_Player->MoveToPossition(Vector2(x, y));
}


void GSPlay::HandleTouchEvents(int x, int y, bool bIsPressed)
{
}

void GSPlay::Update(float deltaTime)
{
	m_time += deltaTime;
	if (m_time>700)
	{
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Credit);
	}

	if (m_SpawnCooldown > 0)
	{
		m_SpawnCooldown -= deltaTime;
	}
	if (m_SpawnCooldown <= 0)
	{
		CreateRandomEnemy();
		m_SpawnCooldown = 0.5;
		if (m_score >= 0 && m_score < 50) {
			CreateRandomEnemy();
			m_SpawnCooldown = 0.4;
		}
		else if (m_score >= 50 && m_score < 100) {
			CreateRandomEnemy();
			m_SpawnCooldown = 0.35;
		}
		else if (m_score >= 100 && m_score < 150) {
			CreateRandomEnemy();
			m_SpawnCooldown = 0.3;
		}
		else if (m_score >= 150 && m_score < 200) {
			CreateRandomEnemy();
			m_SpawnCooldown = 0.25;
		}
	}


	//update player
	if (m_Player->IsAlive())
	{
		m_Player->Update(deltaTime);
		if (m_Player->CanShoot()) {
			if (m_score < 15) {
				m_Player->Shoot(m_listBullet);
			}
			else if (m_score >= 15 && m_score < 51) {
				m_Player->Shoot(m_listBullet);
			}
			else if(m_score >=51 && m_score <200) {
				m_Player->Shoot(m_listBullet);
			}
			else {
				GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Victory);
			}
		}
		m_Player->CheckCollider(m_listBullet, m_listEnemy);
	}
	else
	{
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_GameOver);
	}

	//update enemies
	for (auto enemy : m_listEnemy)
	{
		if (enemy->IsActive())
		{
			if (enemy->IsExplosive())
			{
				enemy->SetActive(false);
				//SpawnExplosive(enemy->Get2DPosition());
				continue;
			}
			enemy->Update(deltaTime);
			enemy->CheckCollider(m_listBullet);
		}
	}


	for (auto exp : m_listExplosiveEffect)
	{
		if (exp->IsActive())
		{
			exp->Update(deltaTime);
		}
	}

	//update bullets
	for (auto bullet : m_listBullet)
	{
		if (bullet->IsActive())
			bullet->Update(deltaTime);
	}


	//update Score
	std::stringstream stream;
	stream << std::fixed << std::setprecision(0) << m_score;
	std::string score = "SCORE: " + stream.str();
	m_scoreText->setText(score);
	std::stringstream stream2;
	stream2 << std::fixed << std::setprecision(0) << m_Player->GetHealth();
	std::string heal = "HP: " + stream2.str();
	m_playerHealthText->setText(heal);
}

void GSPlay::Draw()
{
	//ground
	m_BackGround->Draw();

	for (auto enemy : m_listEnemy)
		if (enemy->IsActive())
			enemy->Draw();

	

	if (m_Player->IsAlive())
		m_Player->Draw();

	for (auto bullet : m_listBullet)
		if (bullet->IsActive())
			bullet->Draw();


	for (auto exp : m_listExplosiveEffect)
	{
		if (exp->IsActive())
		{
			exp->Draw();
		}
	}

	//UI
	m_scoreText->Draw();
	m_playerHealthText->Draw();
}



void GSPlay::CreateRandomEnemy()
{

	int range = Application::screenWidth - 10 + 1;
	int num = rand() % range + 10;

	Vector2 pos;
	pos.x = num;
	pos.y = 10;

	for (auto enemy : m_listEnemy)
	{
		if (!enemy->IsActive())
		{
			enemy->SetActive(true);
			enemy->Set2DPosition(pos);
			return;
		}
	}
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("Enemy_1");

	std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(model, shader, texture);
	enemy->Set2DPosition(pos);
	enemy->SetSize(40, 40);
	m_listEnemy.push_back(enemy);
}

void GSPlay::SpawnExplosive(Vector2 pos)
{
	for (auto exp : m_listExplosiveEffect)
	{
		if (!exp->IsActive())
		{
			exp->SetActive(true);
			exp->Set2DPosition(pos);
			return;
		}
	}

	//animation
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("explosive");
	auto shader = ResourceManagers::GetInstance()->GetShader("SpriteShader");
	std::shared_ptr<ExplosiveEffect> exp = std::make_shared<ExplosiveEffect>(model, shader, texture, Vector2(960, 768), Vector2(192, 192), 0, 19, 0.7);
	exp->SetSize(100, 100);
	exp->Set2DPosition(pos);
	m_listExplosiveEffect.push_back(exp);
}
