#pragma once
#include "primitives\Polyline.h"
#include "Box2D\Box2D.h"
#include "common.h"

DECLARE_SMART(Shape, spShape);
class Shape : public oxygine::Actor
{
	friend class BodyActor;
public:
	enum class Type
	{
		rectangle,
		polygon
	};
	Shape(Type type);

	void SetFillColor(const oxygine::Color & color);
	const oxygine::Color & GetFillColor();
protected:
	primitive::spPolyline m_polyline;
	b2PolygonShape m_shape;
	b2Fixture * m_fixture = nullptr;
	const Type m_type;
};

DECLARE_SMART(RectangleShape, spRectangleShape);
class RectangleShape : public Shape
{
public:
	RectangleShape(float hx, float hy);
	RectangleShape(const oxygine::Vector2 & offset, float hx, float hy);
};

// not sure if we need to differentiate between polygon and rectangle
// possibly rectangle type is redundant
DECLARE_SMART(PolygonShape, spPolygonShape);
class PolygonShape : public Shape
{
public:
	PolygonShape(const std::vector<oxygine::Vector2> & vertices);
};
