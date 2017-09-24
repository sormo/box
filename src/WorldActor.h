 #pragma once
#include "oxygine-framework.h"
#include "Box2D/Box2D.h"
#include "primitives.h"
#include <vector>
#include "ContactManager.h"
#include <set>

DECLARE_SMART(WorldActor, spWorldActor);
class WorldActor : public oxygine::Actor
{
	friend class BodyActor;
	friend class BoxActor;
public:
	WorldActor();
	~WorldActor();

	std::vector<char> SaveWorld() const;
	bool LoadWorld(const std::vector<char> & data);

	oxygine::Vector2 RayCast(const oxygine::Vector2 & p1, const oxygine::Vector2 & p2);

	ContactManager & GetContactManager();

	BodyActor * GetActor(int id);

	void Clear();
	//void VisitBodies(std::function<void(BodyActor&)> visitor);
	void VisitBodies(std::function<void(const BodyActor&)> visitor) const;

protected:
	virtual void doUpdate(const oxygine::UpdateState& us) override;
	virtual void onBodyRemove(BodyActor * body) {}

	b2World * m_world;
	ContactManager m_contactManager;
};
