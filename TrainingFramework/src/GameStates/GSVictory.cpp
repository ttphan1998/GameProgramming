#include "GSVictory.h"


GSVictory::GSVictory()
{
	m_time = 0;
}


GSVictory::~GSVictory()
{
}


void GSVictory::Init()
{
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("Victory");

	m_logo = std::make_shared<Sprite2D>(model, shader, texture);
	m_logo->Set2DPosition(Application::screenWidth / 2, Application::screenHeight / 2);
	m_logo->SetSize(Application::screenWidth , Application::screenHeight);
	m_logo->SetRotation(180);

	SoundManager::GetInstance()->PauseSound("Play");
	SoundManager::GetInstance()->PlaySound("Victory");
}

void GSVictory::Exit()
{
	
}


void GSVictory::Pause()
{
	
}

void GSVictory::Resume()
{
	
}


void GSVictory::HandleEvents()
{
	
}

void GSVictory::HandleKeyEvents(int key, bool bIsPressed)
{

}

void GSVictory::HandleTouchEvents(int x, int y, bool bIsPressed)
{
}

void GSVictory::HandleMouseEvents(int x, int y)
{
}

void GSVictory::Update(float deltaTime)
{
	m_time += deltaTime;
	if (m_time > 6)
	{
		
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Menu);
		
		m_time = 0;
	}
}

void GSVictory::Draw()
{
	m_logo->Draw();
}
