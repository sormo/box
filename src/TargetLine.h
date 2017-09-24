#pragma once
#include "oxygine-framework.h"
#include "primitives.h"
#include "Box2D/Box2D.h"
#include <map>

DECLARE_SMART(TargetLine, spTargetLine);
class TargetLine : public oxygine::Actor
{
public:
	void onTouchDown(oxygine::Event* event);
	void onTouchMove(oxygine::Event* event);
	void onTouchUp(oxygine::Event* event);

private:

	DECLARE_SMART(Line, spLine);
	class Line : public oxygine::Actor
	{
	public:
		Line();
		void SetPosition(const oxygine::Vector2 & pos);
	private:
		primitive::spLine m_line;
		primitive::spCircle m_point;
	};

	std::map<oxygine::pointer_index, spLine> m_activeTouches;
};
