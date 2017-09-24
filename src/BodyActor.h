#pragma once
#include "oxygine-framework.h"
#include "Box2D/Box2D.h"
#include "primitives/Circle.h"
#include "Shape.h"

class WorldActor;

DECLARE_SMART(BodyActor, spBodyActor);
class BodyActor : public oxygine::Actor
{
	friend class ContactManager;
	friend class WorldActor;
public:
	enum class Type
	{
		body,
		box
	};

	BodyActor(WorldActor * world, const oxygine::Vector2& pos, float rotation, bool staticBody, int id = 0);
	virtual ~BodyActor();

	void SetPosition(const oxygine::Vector2 & pos);
	void SetRotation(float rad);

	oxygine::Vector2 GetPosition() const;

	spShape AddShape(spShape shape);

	int GetId() const;
	bool IsStatic() const;

	Type GetType();

protected:
	BodyActor(WorldActor * world, const oxygine::Vector2& pos, float rotation, bool staticBody, Type type, int id = 0);

	virtual void BeginContact(BodyActor * other) {};
	virtual void EndContact(BodyActor * other) {};

	WorldActor * m_world;

	b2Body * m_body;

	primitive::spCircle m_centerOfGravity;

	std::vector<spShape> m_shapes;

	static int s_idCounter;
	int m_id;

	const Type m_type;
};
