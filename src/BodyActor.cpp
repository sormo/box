#include "BodyActor.h"
#include "common.h"
#include "WorldActor.h"

#include <cmath>

int BodyActor::s_idCounter = 1;

BodyActor::BodyActor(WorldActor * world, const oxygine::Vector2 & pos, float rotation, bool staticBody, int id)
	: BodyActor(world, pos, rotation, staticBody, Type::body, id)
{
}

BodyActor::BodyActor(WorldActor * world, const oxygine::Vector2& pos, float rotation, bool staticBody, Type type, int id)
	: m_world(world), m_type(type)
{
	if (id == 0)
		m_id = s_idCounter++;
	else
		m_id = id;

	b2BodyDef bodyDef;
	bodyDef.type = staticBody ? b2_staticBody : b2_dynamicBody;
	bodyDef.position = convert(pos);
	bodyDef.angle = rotation;

	m_body = m_world->m_world->CreateBody(&bodyDef);
	m_body->SetUserData(this);

	setPosition(pos);
	setRotation(rotation);

	m_centerOfGravity = new primitive::Circle(2.0f);
	m_centerOfGravity->SetFillColor(oxygine::Color{ (unsigned int)oxygine::Color::Red, 20 });
	m_centerOfGravity->attachTo(this);
}

BodyActor::~BodyActor()
{
	if (m_world)
	{
		if (m_body && m_body->IsActive())
			m_world->onBodyRemove(this);
		m_world->GetContactManager().Unregister(this);
	}

	if (m_body)
	{
		m_body->SetUserData(nullptr);
		if (m_world->m_world)
			m_world->m_world->DestroyBody(m_body);
	}
}

void BodyActor::SetPosition(const oxygine::Vector2 & pos)
{
	oxygine::Vector2 anchorPos = pos - convert(m_body->GetLocalCenter());

	m_body->SetTransform(convert(anchorPos), m_body->GetAngle());
}

void BodyActor::SetRotation(float rad)
{
	b2Vec2 rotvec = m_body->GetLocalCenter();

	rotate(rotvec, rad);

	m_body->SetTransform(m_body->GetPosition() - (rotvec - m_body->GetLocalCenter()) , rad);
}

oxygine::Vector2 BodyActor::GetPosition() const
{
	b2Vec2 localCenter = m_body->GetWorldCenter();

	return convert(localCenter);
}

spShape BodyActor::AddShape(spShape shape)
{
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape->m_shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.userData = shape.get();

	shape->m_fixture = m_body->CreateFixture(&fixtureDef);
	shape->m_fixture->SetUserData(shape.get());
	shape->setUserData(shape->m_fixture);

	shape->attachTo(this);

	m_shapes.push_back(shape);

	return shape;
}

int BodyActor::GetId() const
{
	return m_id;
}

bool BodyActor::IsStatic() const
{
	return m_body->GetType() == b2_staticBody;
}

BodyActor::Type BodyActor::GetType()
{
	return m_type;
}
