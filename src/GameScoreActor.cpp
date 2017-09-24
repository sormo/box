#include "GameScoreActor.h"

GameScoreActor::GameScoreActor()
{
	m_text = new primitive::Text("0 / 0");
	m_text->SetFontSize(30);
	m_text->SetColor(oxygine::Color::Gainsboro);
	m_text->setPosition({ oxygine::getStage()->getWidth() / 2.0f, 0.0f });
	m_text->attachTo(this);
}

void GameScoreActor::Increase()
{
	m_currentCount++;
	if (m_currentCount > m_maxCount)
		m_maxCount = m_currentCount;

	m_updateNeeded = true;
}

void GameScoreActor::Decrease()
{
	if (m_currentCount == 0)
		return;
	m_currentCount--;

	m_updateNeeded = true;
}

void GameScoreActor::UpdateScore()
{
	char buffer[256];
	sprintf(buffer, "%d / %d", m_maxCount, m_currentCount);
	m_text->SetText(buffer);
}

void GameScoreActor::doUpdate(const oxygine::UpdateState & us)
{
	if (!m_updateNeeded)
		return;
	UpdateScore();
	m_updateNeeded = false;
}
