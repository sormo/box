#pragma once
#include "Box2D/Box2D.h"
#include <map>
#include <vector>
#include <functional>

class BodyActor;

class ContactManager : public b2ContactListener
{
public:
	using ContactCbk = std::function<void(BodyActor*)>;

	void Register(BodyActor * subject);
	void Register(BodyActor * subject, ContactCbk begin, ContactCbk end);
	void Unregister(BodyActor * subject);

private:
	virtual void BeginContact(b2Contact* contact) override;
	virtual void EndContact(b2Contact* contact) override;

	enum class ProcessType { begin, end };
	void Process(b2Body * registrantBody, b2Body * otherBody, ProcessType type);

	struct RegistrationData
	{
		ContactCbk beginCbk;
		ContactCbk endCbk;
	};
	std::map<b2Body*, std::vector<RegistrationData>> m_registrations;
};
