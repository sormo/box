#include "ContactManager.h"
#include "BodyActor.h"

void ContactManager::Register(BodyActor * subject, ContactCbk begin, ContactCbk end)
{
	auto it = m_registrations.find(subject->m_body);
	if (it == std::end(m_registrations))
		m_registrations.insert({ subject->m_body, {{ begin, end }} });
	else
		it->second.push_back({ begin, end });
}

void ContactManager::Register(BodyActor * subject)
{
	Register(subject, nullptr, nullptr);
}

void ContactManager::Unregister(BodyActor * subject)
{
	m_registrations.erase(subject->m_body);
}

void ContactManager::BeginContact(b2Contact* contact)
{
	Process(contact->GetFixtureA()->GetBody(), contact->GetFixtureB()->GetBody(), ProcessType::begin);
	Process(contact->GetFixtureB()->GetBody(), contact->GetFixtureA()->GetBody(), ProcessType::begin);
}

void ContactManager::EndContact(b2Contact* contact)
{
	Process(contact->GetFixtureA()->GetBody(), contact->GetFixtureB()->GetBody(), ProcessType::end);
	Process(contact->GetFixtureB()->GetBody(), contact->GetFixtureA()->GetBody(), ProcessType::end);
}

void ContactManager::Process(b2Body * registrantBody, b2Body * otherBody, ProcessType type)
{
	auto it = m_registrations.find(registrantBody);
	if (it == std::end(m_registrations))
		return;

	BodyActor * registrant = (BodyActor*)registrantBody->GetUserData();
	BodyActor * other = (BodyActor*)otherBody->GetUserData();

	if (type == ProcessType::begin)
	{
		registrant->BeginContact(other);
		for (RegistrationData & data : it->second)
		{
			if (data.beginCbk)
				data.beginCbk(other);
		}
	}
	else
	{
		registrant->EndContact(other);
		for (RegistrationData & data : it->second)
		{
			if (data.endCbk)
				data.endCbk(other);
		}
	}
}
