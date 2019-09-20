#include "GSGameOver.h"


GSGameOver::GSGameOver()
{
	m_time = 0;
}


GSGameOver::~GSGameOver()
{
}


void GSGameOver::Init()
{
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("Game Over");

	m_logo = std::make_shared<Sprite2D>(model, shader, texture);
	m_logo->Set2DPosition(Application::screenWidth / 2, Application::screenHeight / 2);
	m_logo->SetSize(Application::screenWidth , Application::screenHeight);

	SoundManager::GetInstance()->PauseSound("Play");
	SoundManager::GetInstance()->PlaySound("Defeat");
}

void GSGameOver::Exit()
{
	
}


void GSGameOver::Pause()
{
	
}

void GSGameOver::Resume()
{
	
}


void GSGameOver::HandleEvents()
{
	
}

void GSGameOver::HandleKeyEvents(int key, bool bIsPressed)
{

}

void GSGameOver::HandleTouchEvents(int x, int y, bool bIsPressed)
{
}

void GSGameOver::HandleMouseEvents(int x, int y)
{
}

void GSGameOver::Update(float deltaTime)
{
	
	m_time += deltaTime;
	if (m_time > 6)
	{
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Menu);
		m_time = 0;
	}
}

void GSGameOver::Draw()
{
	m_logo->Draw();
}
