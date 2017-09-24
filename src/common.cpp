#include "common.h"
#include <fstream>
#include <cmath>
#include <sstream>

#include "WorldActor.h"
#include "BodyActor.h"

b2Vec2 convert(const oxygine::Vector2 & pos)
{
	return b2Vec2(pos.x / SCALE, pos.y / SCALE);
}

oxygine::Vector2 convert(const b2Vec2 & pos)
{
	return oxygine::Vector2(pos.x * SCALE, pos.y * SCALE);
}

double fromDegToRad(double deg)
{
	return deg * 0.0174533;
}

double fromRadToDeg(double rad)
{
	return rad * 57.2958;
}

oxygine::VectorD2 convertVec(const oxygine::VectorD2 & v)
{
	return{ (float)v.x, (float)v.y };
}

double crossProduct(const oxygine::VectorD2 & v1, const oxygine::VectorD2 & v2)
{
	return v1.x*v2.y - v1.y*v2.x;
}

double normalizeAngle(double radians)
{
	if (radians > 0.0)
	{
		while (radians > PI)
			radians -= 2.0*PI;
	}
	else
	{
		while (radians < PI)
			radians += 2.0*PI;
	}
	return radians;
}

bool sigmaCompare(const oxygine::VectorD2 & a, const oxygine::VectorD2 & b, double sigma)
{
	return fabs(a.x - b.x) < sigma && fabs(a.y - b.y) < sigma;
}

double frand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

void SaveWorldToFile(const WorldActor & world, const char * fileName)
{
	std::vector<char> data = world.SaveWorld();
	std::ofstream file(fileName, std::ios_base::binary);

	file.write(data.data(), data.size());
}

bool LoadWorldFromFile(WorldActor & world, const char * fileName)
{
	std::ifstream file(fileName, std::ios_base::binary);
	if (!file.is_open())
		return false;

	std::vector<char> data;
	data.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

	return world.LoadWorld(data);
}

std::string DumpDebugData(const WorldActor & world)
{
	std::stringstream str;

	world.VisitBodies([&str](const BodyActor& body)
	{
		str << "ID " << body.GetId() << std::endl;
		str << "\tposition: " << body.GetPosition().x << " " << body.GetPosition().y << std::endl;
	});

	return str.str();
}
