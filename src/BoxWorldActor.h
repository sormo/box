#pragma once
#include "oxygine-framework.h"
#include "Box2D/Box2D.h"
#include "primitives.h"
#include "GameScoreActor.h"
#include "BoxActor.h"
#include <vector>
#include "TargetLine.h"
#include "ContactManager.h"
#include <set>
#include "NextBoxActor.h"
#include "WorldActor.h"

DECLARE_SMART(BoxWorldActor, spBoxWorldActor);
class BoxWorldActor : public WorldActor
{
public:
	BoxWorldActor();

	bool LoadBoxes();
	void SaveBoxes();

	spBoxActor CreateRandomBox();

	spGameScoreActor & GetGameScore();

private:
	virtual void doUpdate(const oxygine::UpdateState& us) override;
	virtual void onBodyRemove(BodyActor * body) override;

	void onTouchDown(oxygine::Event* event);
	void onTouchMove(oxygine::Event* event);
	void onTouchUp(oxygine::Event* event);
	
	b2World * m_world;
	ContactManager m_contactManager;

	spGameScoreActor m_score;
	spTargetLine m_target;

	spNextBoxActor m_nextBoxActor;
};
extern spBoxWorldActor g_worldActor;
