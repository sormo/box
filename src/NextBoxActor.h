#pragma once
#include "oxygine-framework.h"
#include "BoxActor.h"
#include <atomic>

#include "WorldSimmulator.h"

class BoxWorldActor;

DECLARE_SMART(NextBoxActor, spNextBoxActor);
class NextBoxActor : public oxygine::Actor
{
public:
	NextBoxActor(BoxWorldActor & reference);
	~NextBoxActor();

	void onTouchDown(oxygine::Event* event);
	void onTouchMove(oxygine::Event* event);
	void onTouchUp(oxygine::Event* event);

private:
	virtual void doUpdate(const oxygine::UpdateState& us) override;

	// this is next box in upper left
	spBoxActor m_nextBox;
	void ResetNextBox();
	void CreateNextBox(BoxWorldActor & world);

	// this box is shown under cursor
	spBoxActor m_cursorBox;

	// this box is shown on first collision with world
	spBoxActor m_simmulatedBox;
	oxygine::Vector2 m_simmulatedBoxPosition;
	float m_simmulatedBoxRotation;
	std::atomic<bool> m_simmulatedBoxUpdated;

	oxygine::Vector2 m_downPosition;

	WorldSimmulator m_simmulator;
	void ResetSimmulator();
};