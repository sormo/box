#include "WorldActor.h"
#include <set>
#include "common.h"
#include "BodyActor.h"
#include "BoxActor.h"
#include "BoxData_generated.h"

WorldActor::WorldActor()
	: m_world(nullptr)
{
	setSize(oxygine::getStage()->getSize());

	m_world = new b2World(b2Vec2(0.0f, 10.0f));
	m_world->SetContactListener(&m_contactManager);

}

WorldActor::~WorldActor()
{
	std::vector<BodyActor*> toRemove;
	for (b2Body * body = m_world->GetBodyList(); body; body = body->GetNext())
		toRemove.push_back(reinterpret_cast<BodyActor*>(body->GetUserData()));

	for (BodyActor * actor : toRemove)
	{
		actor->m_world = nullptr;
		actor->m_body = nullptr;
	}

	delete m_world;
}

void WorldActor::doUpdate(const oxygine::UpdateState & us)
{
	//in real project you should make steps with fixed dt, check box2d documentation
	m_world->Step(us.dt / 1000.0f, 6, 2);

	std::set<b2Body*> toRemove;

	//update each body position on display
	b2Body* body = m_world->GetBodyList();
	while (body)
	{
		b2Body* next = body->GetNext();
		if (toRemove.find(body) != std::end(toRemove))
		{
			body = next;
			continue;
		}

		BodyActor* actor = (BodyActor*)body->GetUserData();
		if (actor)
		{
			const b2Vec2& pos = body->GetPosition();
			actor->setPosition(convert(pos));
			actor->setRotation(body->GetAngle());

			//remove fallen bodies
			if (actor->getY() > getHeight() + 50)
			{
				toRemove.insert(body);
			}
		}

		body = next;
	}

	for (auto b : toRemove)
	{
		BodyActor * actor = (BodyActor*)b->GetUserData();
		actor->detach();
	}
}

class MyRayCastCallback : public b2RayCastCallback
{
public:
	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, float32 fraction)
	{
		m_point = point;
		m_isSet = true;

		return fraction;
	}
	operator bool() { return m_isSet; }
	b2Vec2 m_point;
	bool m_isSet = false;
};

oxygine::Vector2 WorldActor::RayCast(const oxygine::Vector2 & p1, const oxygine::Vector2 & p2)
{
	MyRayCastCallback callback;

	m_world->RayCast(&callback, convert(p1), convert(p2));

	if (callback)
		return convert(callback.m_point);
	return p2;
}

ContactManager & WorldActor::GetContactManager()
{
	return m_contactManager;
}

BodyActor * WorldActor::GetActor(int id)
{
	for (b2Body * body = m_world->GetBodyList(); body; body = body->GetNext())
	{
		BodyActor * actor = reinterpret_cast<BodyActor*>(body->GetUserData());
		assert(actor);

		if (actor->GetId() == id)
			return actor;
	}

	return nullptr;
}

void WorldActor::Clear()
{
	std::vector<b2Body*> toRemove;
	for (b2Body * body = m_world->GetBodyList(); body; body = body->GetNext())
		toRemove.push_back(body);

	for (b2Body * body : toRemove)
	{
		BodyActor * actor = reinterpret_cast<BodyActor*>(body->GetUserData());
		assert(actor);
		
		actor->detach();
	}
}

void WorldActor::VisitBodies(std::function<void(const BodyActor&)> visitor) const
{
	for (b2Body * body = m_world->GetBodyList(); body; body = body->GetNext())
	{
		BodyActor * actor = reinterpret_cast<BodyActor*>(body->GetUserData());
		assert(actor);

		visitor(*actor);
	}
}

std::vector<BoxData::Point> Convert(b2Vec2 * points, size_t size)
{
	std::vector<BoxData::Point> ret;
	for (size_t i = 0; i < size; ++i)
	{
		auto convertedVertex = convert(points[i]);
		ret.push_back({ convertedVertex.x, convertedVertex.y });
	}
	return ret;
}

std::unique_ptr<BoxData::Point> Convert(const oxygine::Vector2 & point)
{
	return std::make_unique<BoxData::Point>(point.x, point.y);
}

std::vector<char> WorldActor::SaveWorld() const
{
	flatbuffers::FlatBufferBuilder fbb;

	std::vector<flatbuffers::Offset<void>> boxes;
	std::vector<uint8_t> boxesType;

	for (b2Body * body = m_world->GetBodyList(); body; body = body->GetNext())
	{
		BoxData::AnyUnion anyUnion;
		BodyActor * bodyActor = (BodyActor*)body->GetUserData();

		if (bodyActor->GetType() == BodyActor::Type::body)
		{
			BoxData::BodyT * newBody = new BoxData::BodyT;
			newBody->id = bodyActor->GetId();
			newBody->isStatic = bodyActor->IsStatic();
			newBody->position = Convert(bodyActor->GetPosition());
			newBody->rotation = bodyActor->getRotation();

			for (b2Fixture * fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
			{
				Shape * shape = (Shape*)fixture->GetUserData();
				b2PolygonShape * polygon = dynamic_cast<b2PolygonShape*>(fixture->GetShape());

				std::unique_ptr<BoxData::ShapeT> newShape(new BoxData::ShapeT);
				newShape->color = shape->GetFillColor().rgba();
				newShape->vertices = Convert(polygon->m_vertices, polygon->m_count);

				newBody->shapes.push_back(std::move(newShape));
			}
			
			anyUnion.type = BoxData::Any_Body;
			anyUnion.value = newBody;
			boxes.push_back(anyUnion.Pack(fbb));
		}
		else if (bodyActor->GetType() == BodyActor::Type::box)
		{
			BoxActor * boxActor = dynamic_cast<BoxActor*>(bodyActor);
			BoxData::BoxT * newBox = new BoxData::BoxT;
			newBox->id = boxActor->GetId();
			newBox->position = Convert(boxActor->GetPosition());
			newBox->rotation = boxActor->getRotation();
			newBox->type = (int32_t)boxActor->GetBoxType();
			newBox->color = boxActor->GetFillColor().rgba();

			anyUnion.type = BoxData::Any_Box;
			anyUnion.value = newBox;
			boxes.push_back(anyUnion.Pack(fbb));
		}

		boxesType.push_back(anyUnion.type);
	}

	//auto dataOffset = fbb.CreateVector(data);
	//BoxData::BoxesBuilder boxes(fbb);
	//boxes.add_boxes(dataOffset);

	//fbb.Finish(boxes.Finish());

	auto anyValueVector = fbb.CreateVector(boxes);
	auto anyTypeVector = fbb.CreateVector(boxesType);

	BoxData::BodiesBuilder builder(fbb);
	builder.add_boxes(anyValueVector);
	builder.add_boxes_type(anyTypeVector);

	fbb.Finish(builder.Finish());

	return std::vector<char>((char*)fbb.GetBufferPointer(), (char*)fbb.GetBufferPointer() + fbb.GetSize());
}

oxygine::Vector2 Convert(const BoxData::Point & point)
{
	return { point.x(), point.y() };
}

std::vector<oxygine::Vector2> Convert(const std::vector<BoxData::Point> & points)
{
	std::vector<oxygine::Vector2> ret(points.size());
	for (size_t i = 0; i < points.size(); ++i)
		ret[i] = Convert(points[i]);
	return ret;
}

bool WorldActor::LoadWorld(const std::vector<char> & data)
{
	std::unique_ptr<BoxData::BodiesT> unpackData = BoxData::UnPackBodies(data.data());

	for (BoxData::AnyUnion & body : unpackData->boxes)
	{
		if (body.type == BoxData::Any_Body)
		{
			spBodyActor newBody = new BodyActor(this, Convert(*body.AsBody()->position), body.AsBody()->rotation, body.AsBody()->isStatic, body.AsBody()->id);
			for (const std::unique_ptr<BoxData::ShapeT> & shape : body.AsBody()->shapes)
			{
				spShape newShape = new PolygonShape(Convert(shape->vertices));
				newShape->SetFillColor(oxygine::Color::fromRGBA(shape->color));
				newBody->AddShape(newShape);
			}
			newBody->attachTo(this);
		}
		else if (body.type == BoxData::Any_Box)
		{
			spBoxActor newBox = new BoxActor(this, (BoxActor::BoxType)body.AsBox()->type, Convert(*body.AsBox()->position), body.AsBox()->rotation, body.AsBox()->id);
			newBox->SetFillColor(oxygine::Color::fromRGBA(body.AsBox()->color));
			newBox->attachTo(this);
		}
	}

	return true;
}
