#include "NextBoxActor.h"
#include "BoxWorldActor.h"

#define PIXELS_TO_RADIANS_FACTOR 0.075f

NextBoxActor::NextBoxActor(BoxWorldActor & reference)
	: m_simmulator(reference, nullptr), m_simmulatedBoxUpdated(false)
{
	CreateNextBox(reference);
}

NextBoxActor::~NextBoxActor()
{
	if (m_nextBox)
	{
		m_nextBox->detach();
		m_nextBox = nullptr;
	}
	if (m_cursorBox)
	{
		m_cursorBox->detach();
		m_cursorBox = nullptr;
	}
	if (m_simmulatedBox)
	{
		m_simmulatedBox->detach();
		m_simmulatedBox = nullptr;
	}
}

void NextBoxActor::CreateNextBox(BoxWorldActor & world)
{
	m_nextBox = world.CreateRandomBox();
	ResetNextBox();
}

void NextBoxActor::ResetNextBox()
{
	m_nextBox->SetGhost(true);
	m_nextBox->SetPosition({ oxygine::getStage()->getSize().x - 100.0f, 100.0f });
}

void NextBoxActor::onTouchDown(oxygine::Event * event)
{
	oxygine::TouchEvent* te = oxygine::safeCast<oxygine::TouchEvent*>(event);

	m_downPosition = te->localPosition;

	m_cursorBox = m_nextBox;
	m_cursorBox->SetPosition(te->localPosition);
	m_cursorBox->setVisible(te->localPosition.y < g_worldActor->getSize().y * CLICK_BOUNDARY_HEIGHT_FACTOR);
	m_cursorBox->SetGhost(true);

	CreateNextBox(*g_worldActor);

	m_simmulatedBox = new BoxActor(g_worldActor.get(), m_cursorBox->GetBoxType(), te->localPosition, 0.0f);
	m_simmulatedBox->SetFillColor(m_cursorBox->GetFillColor());
	m_simmulatedBox->SetGhost(true);
	m_simmulatedBox->attachTo(g_worldActor);

	m_simmulatedBoxUpdated = false;
	ResetSimmulator();
}

void NextBoxActor::onTouchMove(oxygine::Event * event)
{
	if (!m_cursorBox)
		return;

	oxygine::TouchEvent* te = oxygine::safeCast<oxygine::TouchEvent*>(event);

	m_cursorBox->SetPosition(te->localPosition);
	m_cursorBox->SetRotation((m_downPosition.y - te->localPosition.y)*PIXELS_TO_RADIANS_FACTOR);
	m_cursorBox->setVisible(te->localPosition.y < g_worldActor->getSize().y * CLICK_BOUNDARY_HEIGHT_FACTOR);

	m_simmulator.Reset();
	m_simmulatedBoxUpdated = false;
}

void NextBoxActor::onTouchUp(oxygine::Event * event)
{
	if (!m_cursorBox)
		return;

	oxygine::TouchEvent* te = oxygine::safeCast<oxygine::TouchEvent*>(event);

	if (te->localPosition.y < g_worldActor->getSize().y * CLICK_BOUNDARY_HEIGHT_FACTOR)
	{
		m_cursorBox->SetGhost(false);
		g_worldActor->GetGameScore()->Increase();
	}
	else
	{
		// return cursor box as next box
		m_nextBox->detach();
		m_nextBox = m_cursorBox;
		ResetNextBox();
	}

	m_cursorBox = nullptr;

	m_simmulator.Reset(nullptr);

	if (m_simmulatedBox)
	{
		m_simmulatedBox->detach();
		m_simmulatedBox = nullptr;
	}
}

void NextBoxActor::ResetSimmulator()
{
	m_simmulator.Reset([this](WorldActor & world, std::atomic<bool> & running)
	{
		// find actor in copied world (it is not ghost)
		BodyActor * copyActor = world.GetActor(m_cursorBox->GetId());
		// find simmulation actor in copied world and remove it
		BodyActor * simmulationActor = world.GetActor(m_simmulatedBox->GetId());
		if (simmulationActor)
			simmulationActor->detach();

		// register for collision of that actor, callback call result and set running
		world.GetContactManager().Register(copyActor, [this, copyActor, &running](BodyActor*) 
		{
			// copy simmulated actor
			m_simmulatedBoxPosition = copyActor->GetPosition();
			m_simmulatedBoxRotation = copyActor->getRotation();
			m_simmulatedBoxUpdated = true;

			running = false;
		}, nullptr);
	});
}

void NextBoxActor::doUpdate(const oxygine::UpdateState& us)
{
	if (m_simmulatedBoxUpdated && m_simmulatedBox)
	{
		m_simmulatedBox->SetPosition(m_simmulatedBoxPosition);
		m_simmulatedBox->SetRotation(m_simmulatedBoxRotation);
		m_simmulatedBoxUpdated = false;
	}
}
