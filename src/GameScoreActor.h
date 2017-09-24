#pragma once
#include "oxygine-framework.h"
#include "primitives/Text.h"

DECLARE_SMART(GameScoreActor, spGameScoreActor);
class GameScoreActor : public oxygine::Actor
{
public:
	GameScoreActor();
	void Increase();
	void Decrease();

private:
	void UpdateScore();

	virtual void doUpdate(const oxygine::UpdateState& us) override;

	size_t m_currentCount = 0;
	size_t m_maxCount = 0;
	primitive::spText m_text;

	bool m_updateNeeded = false;
};
