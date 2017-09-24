#include "Shape.h"
#include "WorldActor.h"

RectangleShape::RectangleShape(float hx, float hy)
	: Shape(Type::rectangle)
{
	m_polyline = new primitive::Polyline({ { -hx, -hy },{ hx, -hy },{ hx, hy },{ -hx, hy },{ -hx, -hy } });
	m_polyline->setTouchChildrenEnabled(false);
	m_polyline->attachTo(this);

	m_shape.SetAsBox(hx / SCALE, hy / SCALE);
}

RectangleShape::RectangleShape(const oxygine::Vector2 & offset, float hx, float hy)
	: Shape(Type::rectangle)
{
	m_polyline = new primitive::Polyline({ { -hx, -hy },{ hx, -hy },{ hx, hy },{ -hx, hy },{ -hx, -hy } });
	m_polyline->setPosition(offset);
	m_polyline->setTouchChildrenEnabled(false);
	m_polyline->attachTo(this);

	m_shape.SetAsBox(hx / SCALE, hy / SCALE);
	for (int32 i = 0; i < m_shape.m_count; i++)
		m_shape.m_vertices[i] += convert(offset);
}

PolygonShape::PolygonShape(const std::vector<oxygine::Vector2> & vertices)
	: Shape(Type::polygon)
{
	m_polyline = new primitive::Polyline(vertices);
	m_polyline->setTouchChildrenEnabled(false);
	m_polyline->attachTo(this);

	std::vector<b2Vec2> b2vertices(vertices.size());
	for (size_t i = 0; i < vertices.size(); ++i)
		b2vertices[i] = convert(vertices[i]);

	m_shape.Set(b2vertices.data(), b2vertices.size());
}

Shape::Shape(Type type)
	: m_type(type)
{

}

void Shape::SetFillColor(const oxygine::Color & color)
{
	m_polyline->SetFillColor(color);
}

const oxygine::Color & Shape::GetFillColor()
{
	return m_polyline->GetFillColor();
}
