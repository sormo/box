#include "TargetLine.h"
#include "common.h"
#include "BoxWorldActor.h"

extern spBoxWorldActor g_worldActor;

void TargetLine::Line::SetPosition(const oxygine::Vector2 & position)
{
	oxygine::Vector2 farPosition = g_worldActor->RayCast(position, { position.x, position.y + 1000.0f });

	m_line->Set(position, farPosition);
	m_point->setPosition(farPosition);
}

TargetLine::Line::Line()
{
	m_line = new primitive::Line(10.0f);
	m_point = new primitive::Circle(2.5f);

	m_line->SetLineWidth(5.0f);
	m_line->SetLineColor({ 255, 255, 255, 5 });
	m_line->attachTo(this);
	m_point->SetFillColor({ 255, 255, 255, 60 });
	m_point->attachTo(this);
}

void TargetLine::onTouchDown(oxygine::Event* event)
{
	oxygine::TouchEvent* te = oxygine::safeCast<oxygine::TouchEvent*>(event);
	
	m_activeTouches[te->index] = new Line;
	m_activeTouches[te->index]->attachTo(this);
	m_activeTouches[te->index]->setVisible(te->localPosition.y < g_worldActor->getSize().y * CLICK_BOUNDARY_HEIGHT_FACTOR);

	if (m_activeTouches[te->index]->getVisible())
		m_activeTouches[te->index]->SetPosition(te->localPosition);
}

void TargetLine::onTouchMove(oxygine::Event* event)
{
	oxygine::TouchEvent* te = oxygine::safeCast<oxygine::TouchEvent*>(event);
	if (m_activeTouches.find(te->index) == std::end(m_activeTouches))
		return;

	m_activeTouches[te->index]->setVisible(te->localPosition.y < g_worldActor->getSize().y * CLICK_BOUNDARY_HEIGHT_FACTOR);

	if (m_activeTouches[te->index]->getVisible())
		m_activeTouches[te->index]->SetPosition(te->localPosition);
}

void TargetLine::onTouchUp(oxygine::Event* event)
{
	oxygine::TouchEvent* te = oxygine::safeCast<oxygine::TouchEvent*>(event);

	m_activeTouches[te->index]->detach();
	m_activeTouches.erase(te->index);
}
