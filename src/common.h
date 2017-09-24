#pragma once
#include "oxygine-framework.h"
#include "MainActor.h"
#include "Box2D/Box2D.h"

class WorldActor;

#define COUNTOF(arr) sizeof(arr)/sizeof(arr[0])
#define PI 3.141592654

#define CLICK_BOUNDARY_HEIGHT_FACTOR 0.33f
#define BOX_ALPHA_DELETE_THRESHOLD 20
#define WORLD_SAVE_FILE "save.data"

const float SCALE = 100.0f;
b2Vec2 convert(const oxygine::Vector2& pos);
oxygine::Vector2 convert(const b2Vec2& pos);

static const oxygine::pointer_index INVALID_TOUCH = 255;

double fromDegToRad(double deg);
double fromRadToDeg(double rad);

template <class T>
T & rotate(T & v, float angle);
template <class T>
T rotate(T && v, float angle);

template <class T>
T getAngle(const oxygine::VectorT2<T> & v);
template <class T>
T getAngle(const oxygine::VectorT2<T> & v1, const oxygine::VectorT2<T> & v2);

double crossProduct(const oxygine::VectorD2 & v1, const oxygine::VectorD2 & v2);
// -PI , PI
double normalizeAngle(double radians);

template<class T>
bool sigmaCompare(T a, T b, double sigma = 0.00000001);

oxygine::VectorD2 convertVec(const oxygine::VectorD2 & v);

// global gui object
extern oxygine::Resources g_gameResources;
// global actors (in order of event propagation)
extern spMainActor g_mainActor;

double frand(double fMin, double fMax);

void SaveWorldToFile(const WorldActor & world, const char * fileName);
bool LoadWorldFromFile(WorldActor & world, const char * fileName);

std::string DumpDebugData(const WorldActor & world);

// --- template implementations ---

template <class T>
T getAngle(const oxygine::VectorT2<T> & v)
{
	return atan2(v.y, v.x);
}

template <class T>
T getAngle(const oxygine::VectorT2<T> & v1, const oxygine::VectorT2<T> & v2)
{
	return atan2(v2.y, v2.x) - atan2(v1.y, v1.x);
}

template<class T>
bool sigmaCompare(T a, T b, double sigma)
{
	return fabs(a - b) < sigma;
}
bool sigmaCompare(const oxygine::VectorD2 & a, const oxygine::VectorD2 & b, double sigma);

template<class T>
T rotate(T && v, float angle)
{
	float s = std::sin(angle);
	float c = std::cos(angle);

	T t;

	t.x = v.x * c - v.y * s;
	t.y = v.x * s + v.y * c;

	return t;
}

template<class T>
T & rotate(T & v, float angle)
{
	float s = std::sin(angle);
	float c = std::cos(angle);

	T t;

	t.x = v.x * c - v.y * s;
	t.y = v.x * s + v.y * c;

	v = t;

	return v;
}

//#define USE_CAMERA
