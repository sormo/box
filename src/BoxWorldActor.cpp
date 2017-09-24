#include "BoxWorldActor.h"
#include <set>

static const oxygine::Color BOX_PALETTE[] =
{
	oxygine::Color::OliveDrab,
	oxygine::Color::Coral,
	oxygine::Color::DarkOrange,
	oxygine::Color::MediumSlateBlue,
	oxygine::Color::Teal
};

spBoxWorldActor g_worldActor;

BoxWorldActor::BoxWorldActor()
	: m_world(nullptr)
{
	addEventListener(oxygine::TouchEvent::TOUCH_DOWN, CLOSURE(this, &BoxWorldActor::onTouchDown));
	addEventListener(oxygine::TouchEvent::MOVE, CLOSURE(this, &BoxWorldActor::onTouchMove));
	addEventListener(oxygine::TouchEvent::TOUCH_UP, CLOSURE(this, &BoxWorldActor::onTouchUp));

	m_score = new GameScoreActor;
	m_score->attachTo(this);

	primitive::spLine clickBoundary = new primitive::Line({ 0.0f, getSize().y * CLICK_BOUNDARY_HEIGHT_FACTOR },
		{ getSize().x, getSize().y * CLICK_BOUNDARY_HEIGHT_FACTOR });
	clickBoundary->SetLineWidth(5.0f);
	clickBoundary->SetLineColor({ 255, 255, 255, 5 });
	clickBoundary->attachTo(this);

	m_target = new TargetLine;
	m_target->attachTo(this);

	m_nextBoxActor = new NextBoxActor(*this);
	m_nextBoxActor->attachTo(this);
}

void BoxWorldActor::doUpdate(const oxygine::UpdateState & us)
{
	WorldActor::doUpdate(us);
}

void BoxWorldActor::onBodyRemove(BodyActor * body)
{
	m_score->Decrease();
}

void BoxWorldActor::onTouchDown(oxygine::Event* event)
{
	if (event->target.get() != this)
		return;

	m_target->onTouchDown(event);
	m_nextBoxActor->onTouchDown(event);
}

void BoxWorldActor::onTouchMove(oxygine::Event* event)
{
	if (event->target.get() != this)
		return;
	oxygine::TouchEvent* te = oxygine::safeCast<oxygine::TouchEvent*>(event);

	m_target->onTouchMove(event);
	m_nextBoxActor->onTouchMove(event);
}

void BoxWorldActor::onTouchUp(oxygine::Event* event)
{
	if (event->target.get() != this)
		return;

	m_target->onTouchUp(event);
	m_nextBoxActor->onTouchUp(event);
}

bool BoxWorldActor::LoadBoxes()
{
	if (!oxygine::file::exists(WORLD_SAVE_FILE))
		return false;
	return LoadWorldFromFile(*this, WORLD_SAVE_FILE);
}

void BoxWorldActor::SaveBoxes()
{
	m_nextBoxActor->detach();
	m_nextBoxActor = nullptr;

	SaveWorldToFile(*this, WORLD_SAVE_FILE);
}

spBoxActor BoxWorldActor::CreateRandomBox()
{
	size_t colorIndex = rand() % COUNTOF(BOX_PALETTE);
	BoxActor::BoxType boxType = (BoxActor::BoxType)(rand() % (int)BoxActor::BoxType::count);

	spBoxActor ret = new BoxActor(this, boxType, { 0.0f, 0.0f }, 0.0f);
	ret->attachTo(this);
	ret->SetFillColor(BOX_PALETTE[colorIndex]);

	return ret;
}

spGameScoreActor & BoxWorldActor::GetGameScore()
{
	return m_score;
}
