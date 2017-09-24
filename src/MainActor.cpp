#include "MainActor.h"
#include "primitives.h"

spMainActor g_mainActor;

MainActor::MainActor()
{
	setName("Main");

	enableMove(true);
	enableZoom(true);
	
	oxygine::getStage()->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, CLOSURE(this, &MainActor::OnTouchDown));
	oxygine::getStage()->addEventListener(oxygine::TouchEvent::MOVE, CLOSURE(this, &MainActor::OnMove));
	oxygine::getStage()->addEventListener(oxygine::TouchEvent::TOUCH_UP, CLOSURE(this, &MainActor::OnTouchUp));
	oxygine::getStage()->addEventListener(oxygine::TouchEvent::WHEEL_UP, CLOSURE(this, &MainActor::OnWheelUp));
	oxygine::getStage()->addEventListener(oxygine::TouchEvent::WHEEL_DOWN, CLOSURE(this, &MainActor::OnWheelDown));
}

void MainActor::OnMove(oxygine::Event* event)
{
	if (m_touches.size() == 1 && !m_enable[(size_t)EnableBit::move])
		return;
	if (m_touches.size() > 1 && !m_enable[(size_t)EnableBit::zoom])
		return;

	oxygine::TouchEvent* touch = oxygine::safeCast<oxygine::TouchEvent*>(event);
	m_position = touch->localPosition;

	if (m_touches.find(touch->index) == std::end(m_touches))
		return;

	m_touches[touch->index].previous = m_touches[touch->index].current;
	m_touches[touch->index].current = touch->localPosition;

	//oxygine::log::messageln("move local %d %f %f", touch->index, touch->localPosition.x, touch->localPosition.y);
	//oxygine::log::messageln("move global %d %f %f", touch->index, touch->position.x, touch->position.y);

	if (m_touches.size() == 1)
	{
		oxygine::Vector2 offset = m_touches[touch->index].current - m_touches[touch->index].previous;

		setPosition(getPosition() + offset);
	}
	else if (m_touches.size() > 1)
	{
		Touch p1, p2;
		for (const auto & t : m_touches)
		{
			if (t.first == touch->index)
				p1 = t.second;
			else
				p2 = t.second;
		}
		ZoomMulti(p1, p2);
	}
}

void MainActor::ZoomMulti(const Touch & p1, const Touch & p2)
{
	// Calculate new scale
	oxygine::Vector2 prevScale = getScale();
	oxygine::Vector2 curScale = getScale() * p1.current.distance(p2.current) / p1.previous.distance(p2.previous);
	if (curScale.x > m_maximumScale)
		curScale = { m_maximumScale, m_maximumScale };

	//oxygine::log::messageln("zoom multi scale %f %f", curScale.x, curScale.y);
	
	NotifyOnScaleChange();

	oxygine::Vector2 curPosLayer = (p1.current + p2.current) / 2.0f;
	oxygine::Vector2 prevPosLayer = (p1.previous + p2.previous) / 2.0f;
	oxygine::Vector2 t = stage2local(curPosLayer);

	setScale(curScale);

	if (curScale != prevScale)
	{
		float deltaX = (t.x) * (getScale() - prevScale).x;
		float deltaY = (t.y) * (getScale() - prevScale).x;

		setPosition(getPosition() - oxygine::Vector2{ deltaX, deltaY });
	}

	if (prevPosLayer != curPosLayer)
	{
		setPosition(getPosition() + oxygine::Vector2{ curPosLayer.x - prevPosLayer.x, curPosLayer.y - prevPosLayer.y });
	}
}

void MainActor::OnTouchDown(oxygine::Event* event)
{
	oxygine::TouchEvent * touch = dynamic_cast<oxygine::TouchEvent*>(event);
	//oxygine::log::messageln("touch down %d %f %f", touch->index, touch->localPosition.x, touch->localPosition.y);

	m_touches[touch->index].current = m_touches[touch->index].previous = touch->localPosition;
}

void MainActor::OnTouchUp(oxygine::Event* event)
{
	oxygine::TouchEvent * touch = dynamic_cast<oxygine::TouchEvent*>(event);
	//oxygine::log::messageln("touch up %f %f", touch->localPosition.x, touch->localPosition.y);

	auto it = m_touches.find(touch->index);
	if (it != std::end(m_touches))
		m_touches.erase(it);
}

void MainActor::OnWheelUp(oxygine::Event* event)
{
	if (!m_enable[(size_t)EnableBit::zoom])
		return;

	Zoom(event, 1.0f / m_zoomFactor);
}

void MainActor::OnWheelDown(oxygine::Event* event)
{
	if (!m_enable[(size_t)EnableBit::zoom])
		return;

	Zoom(event, m_zoomFactor);
}

void MainActor::Zoom(oxygine::Event* event, float factor)
{
	//oxygine::log::message("Zoom: %s\t", event->target->getName().c_str());
	//oxygine::log::messageln("%s", event->currentTarget->getName().c_str());
	oxygine::TouchEvent* touch = oxygine::safeCast<oxygine::TouchEvent*>(event);

	auto shift = m_position - getPosition();
	shift *= (1.0f - factor);

	//oxygine::log::messageln("factor %f scale %f", factor, getScale().x * factor);

	float newScale = getScale().x * factor;
	if (newScale > m_maximumScale)
		newScale = m_maximumScale;

	setScale(newScale);
	NotifyOnScaleChange();

	setPosition(getPosition() + shift);
	auto const newPos = getPosition();
	//oxygine::log::messageln("%f %f", newPos.x, newPos.y);
}

void MainActor::update(const oxygine::UpdateState & us)
{
	oxygine::Actor::update(us);

	UpdateFollowPosition();

	oxygine::DebugActor::addDebugString("screen: %.0f %.0f", m_position.x, m_position.y);
	auto convertedPosition = localFromStage(m_position);
	oxygine::DebugActor::addDebugString("camera: %.3f %.3f", convertedPosition.x, convertedPosition.y);
	oxygine::DebugActor::addDebugString("zoom: %.6f", getScale().x);
}

oxygine::Vector2 MainActor::localFromChild(oxygine::spActor & child)
{
	auto pos = child->getParent()->local2stage(child->getPosition());
	return getTransformInvert().transform(pos);
}

oxygine::Vector2 MainActor::localFromStage(const oxygine::Vector2 & position)
{
	return getTransformInvert().transform(position);
}

void MainActor::setScaleCamera(float scale)
{
	setScale(scale > m_maximumScale ? m_maximumScale : scale);
}

float MainActor::getScaleCamera()
{
	return getScale().x;
}

void MainActor::setMaximumScale(float scale)
{
	m_maximumScale = scale > MAXIMUM_SCALE ? MAXIMUM_SCALE : scale;
}

// --- on scale change --- 

void MainActor::registerOnScaleChange(void * key, std::function<void(float)> cbk)
{
	m_onScaleChangeRegistrations[key] = cbk;
}

void MainActor::unregisterOnScaleChange(void * key)
{
	auto it = m_onScaleChangeRegistrations.find(key);
	if (it != std::end(m_onScaleChangeRegistrations))
		m_onScaleChangeRegistrations.erase(it);
}

void MainActor::NotifyOnScaleChange()
{
	for (auto & c : m_onScaleChangeRegistrations)
		c.second(getScaleCamera());
}

// --- follow child ---

void MainActor::followChild(oxygine::spActor child)
{
	std::lock_guard<std::mutex> lock(m_followLock);

	m_follow = child;
}

void MainActor::UpdateFollowPosition()
{
	std::lock_guard<std::mutex> lock(m_followLock);

	if (m_follow)
	{
		if (m_follow->_ref_counter == 1)
			m_follow = nullptr;
		else
			setPositionFromLocal(localFromChild(m_follow));
	}
}

void MainActor::setPositionFromLocal(const oxygine::Vector2 & localPosition)
{
	oxygine::Vector2 center = oxygine::getStage()->getSize() / 2.0f;
	oxygine::Vector2 p = center - localPosition * getScale().x;

	setPosition(p);
}

// ---

void MainActor::enableMove(bool enable)
{
	m_enable[(size_t)EnableBit::move] = enable;
}

void MainActor::enableZoom(bool enable)
{
	m_enable[(size_t)EnableBit::zoom] = enable;
}
