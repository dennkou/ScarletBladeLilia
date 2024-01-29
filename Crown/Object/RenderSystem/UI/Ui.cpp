#include "Ui.h"
#include "./UiManager.h"

Crown::RenderObject::Ui::Ui()
	:
	m_priority(0)
{
	UiManager::GetInstance()->AddUi(this);
}

Crown::RenderObject::Ui::~Ui()
{
	UiManager::GetInstance()->DeleteUi(this);
}

void Crown::RenderObject::Ui::SetPriority(int priority) noexcept
{
	m_priority = priority;
	UiManager::GetInstance()->DeleteUi(this);
	UiManager::GetInstance()->AddUi(this);
}