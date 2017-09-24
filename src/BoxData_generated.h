// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_BOXDATA_BOXDATA_H_
#define FLATBUFFERS_GENERATED_BOXDATA_BOXDATA_H_

#include "flatbuffers/flatbuffers.h"

namespace BoxData {

struct Point;

struct Shape;
struct ShapeT;

struct Body;
struct BodyT;

struct Box;
struct BoxT;

struct Bodies;
struct BodiesT;

enum Any {
  Any_NONE = 0,
  Any_Body = 1,
  Any_Box = 2,
  Any_MIN = Any_NONE,
  Any_MAX = Any_Box
};

inline Any (&EnumValuesAny())[3] {
  static Any values[] = {
    Any_NONE,
    Any_Body,
    Any_Box
  };
  return values;
}

inline const char **EnumNamesAny() {
  static const char *names[] = {
    "NONE",
    "Body",
    "Box",
    nullptr
  };
  return names;
}

inline const char *EnumNameAny(Any e) {
  const size_t index = static_cast<int>(e);
  return EnumNamesAny()[index];
}

template<typename T> struct AnyTraits {
  static const Any enum_value = Any_NONE;
};

template<> struct AnyTraits<Body> {
  static const Any enum_value = Any_Body;
};

template<> struct AnyTraits<Box> {
  static const Any enum_value = Any_Box;
};

struct AnyUnion {
  Any type;
  void *value;

  AnyUnion() : type(Any_NONE), value(nullptr) {}
  AnyUnion(AnyUnion&& u) FLATBUFFERS_NOEXCEPT :
    type(Any_NONE), value(nullptr)
    { std::swap(type, u.type); std::swap(value, u.value); }
  AnyUnion(const AnyUnion &) FLATBUFFERS_NOEXCEPT;
  AnyUnion &operator=(const AnyUnion &u) FLATBUFFERS_NOEXCEPT
    { AnyUnion t(u); std::swap(type, t.type); std::swap(value, t.value); return *this; }
  AnyUnion &operator=(AnyUnion &&u) FLATBUFFERS_NOEXCEPT
    { std::swap(type, u.type); std::swap(value, u.value); return *this; }
  ~AnyUnion() { Reset(); }

  void Reset();

#ifndef FLATBUFFERS_CPP98_STL
  template <typename T>
  void Set(T&& val) {
    Reset();
    type = AnyTraits<typename T::TableType>::enum_value;
    if (type != Any_NONE) {
      value = new T(std::forward<T>(val));
    }
  }
#endif  // FLATBUFFERS_CPP98_STL

  static void *UnPack(const void *obj, Any type, const flatbuffers::resolver_function_t *resolver);
  flatbuffers::Offset<void> Pack(flatbuffers::FlatBufferBuilder &_fbb, const flatbuffers::rehasher_function_t *_rehasher = nullptr) const;

  BodyT *AsBody() {
    return type == Any_Body ?
      reinterpret_cast<BodyT *>(value) : nullptr;
  }
  BoxT *AsBox() {
    return type == Any_Box ?
      reinterpret_cast<BoxT *>(value) : nullptr;
  }
};

bool VerifyAny(flatbuffers::Verifier &verifier, const void *obj, Any type);
bool VerifyAnyVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types);

MANUALLY_ALIGNED_STRUCT(4) Point FLATBUFFERS_FINAL_CLASS {
 private:
  float x_;
  float y_;

 public:
  Point() {
    memset(this, 0, sizeof(Point));
  }
  Point(const Point &_o) {
    memcpy(this, &_o, sizeof(Point));
  }
  Point(float _x, float _y)
      : x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)) {
  }
  float x() const {
    return flatbuffers::EndianScalar(x_);
  }
  float y() const {
    return flatbuffers::EndianScalar(y_);
  }
};
STRUCT_END(Point, 8);

struct ShapeT : public flatbuffers::NativeTable {
  typedef Shape TableType;
  uint32_t color;
  std::vector<Point> vertices;
  ShapeT()
      : color(0) {
  }
};

struct Shape FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ShapeT NativeTableType;
  enum {
    VT_COLOR = 4,
    VT_VERTICES = 6
  };
  uint32_t color() const {
    return GetField<uint32_t>(VT_COLOR, 0);
  }
  const flatbuffers::Vector<const Point *> *vertices() const {
    return GetPointer<const flatbuffers::Vector<const Point *> *>(VT_VERTICES);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_COLOR) &&
           VerifyOffset(verifier, VT_VERTICES) &&
           verifier.Verify(vertices()) &&
           verifier.EndTable();
  }
  ShapeT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(ShapeT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<Shape> Pack(flatbuffers::FlatBufferBuilder &_fbb, const ShapeT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct ShapeBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_color(uint32_t color) {
    fbb_.AddElement<uint32_t>(Shape::VT_COLOR, color, 0);
  }
  void add_vertices(flatbuffers::Offset<flatbuffers::Vector<const Point *>> vertices) {
    fbb_.AddOffset(Shape::VT_VERTICES, vertices);
  }
  ShapeBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ShapeBuilder &operator=(const ShapeBuilder &);
  flatbuffers::Offset<Shape> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Shape>(end);
    return o;
  }
};

inline flatbuffers::Offset<Shape> CreateShape(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t color = 0,
    flatbuffers::Offset<flatbuffers::Vector<const Point *>> vertices = 0) {
  ShapeBuilder builder_(_fbb);
  builder_.add_vertices(vertices);
  builder_.add_color(color);
  return builder_.Finish();
}

inline flatbuffers::Offset<Shape> CreateShapeDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t color = 0,
    const std::vector<const Point *> *vertices = nullptr) {
  return BoxData::CreateShape(
      _fbb,
      color,
      vertices ? _fbb.CreateVector<const Point *>(*vertices) : 0);
}

flatbuffers::Offset<Shape> CreateShape(flatbuffers::FlatBufferBuilder &_fbb, const ShapeT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct BodyT : public flatbuffers::NativeTable {
  typedef Body TableType;
  std::unique_ptr<Point> position;
  float rotation;
  std::vector<std::unique_ptr<ShapeT>> shapes;
  bool isStatic;
  int32_t id;
  BodyT()
      : rotation(0.0f),
        isStatic(false),
        id(0) {
  }
};

struct Body FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef BodyT NativeTableType;
  enum {
    VT_POSITION = 4,
    VT_ROTATION = 6,
    VT_SHAPES = 8,
    VT_ISSTATIC = 10,
    VT_ID = 12
  };
  const Point *position() const {
    return GetStruct<const Point *>(VT_POSITION);
  }
  float rotation() const {
    return GetField<float>(VT_ROTATION, 0.0f);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Shape>> *shapes() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Shape>> *>(VT_SHAPES);
  }
  bool isStatic() const {
    return GetField<uint8_t>(VT_ISSTATIC, 0) != 0;
  }
  int32_t id() const {
    return GetField<int32_t>(VT_ID, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<Point>(verifier, VT_POSITION) &&
           VerifyField<float>(verifier, VT_ROTATION) &&
           VerifyOffset(verifier, VT_SHAPES) &&
           verifier.Verify(shapes()) &&
           verifier.VerifyVectorOfTables(shapes()) &&
           VerifyField<uint8_t>(verifier, VT_ISSTATIC) &&
           VerifyField<int32_t>(verifier, VT_ID) &&
           verifier.EndTable();
  }
  BodyT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(BodyT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<Body> Pack(flatbuffers::FlatBufferBuilder &_fbb, const BodyT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct BodyBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_position(const Point *position) {
    fbb_.AddStruct(Body::VT_POSITION, position);
  }
  void add_rotation(float rotation) {
    fbb_.AddElement<float>(Body::VT_ROTATION, rotation, 0.0f);
  }
  void add_shapes(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Shape>>> shapes) {
    fbb_.AddOffset(Body::VT_SHAPES, shapes);
  }
  void add_isStatic(bool isStatic) {
    fbb_.AddElement<uint8_t>(Body::VT_ISSTATIC, static_cast<uint8_t>(isStatic), 0);
  }
  void add_id(int32_t id) {
    fbb_.AddElement<int32_t>(Body::VT_ID, id, 0);
  }
  BodyBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  BodyBuilder &operator=(const BodyBuilder &);
  flatbuffers::Offset<Body> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Body>(end);
    return o;
  }
};

inline flatbuffers::Offset<Body> CreateBody(
    flatbuffers::FlatBufferBuilder &_fbb,
    const Point *position = 0,
    float rotation = 0.0f,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Shape>>> shapes = 0,
    bool isStatic = false,
    int32_t id = 0) {
  BodyBuilder builder_(_fbb);
  builder_.add_id(id);
  builder_.add_shapes(shapes);
  builder_.add_rotation(rotation);
  builder_.add_position(position);
  builder_.add_isStatic(isStatic);
  return builder_.Finish();
}

inline flatbuffers::Offset<Body> CreateBodyDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const Point *position = 0,
    float rotation = 0.0f,
    const std::vector<flatbuffers::Offset<Shape>> *shapes = nullptr,
    bool isStatic = false,
    int32_t id = 0) {
  return BoxData::CreateBody(
      _fbb,
      position,
      rotation,
      shapes ? _fbb.CreateVector<flatbuffers::Offset<Shape>>(*shapes) : 0,
      isStatic,
      id);
}

flatbuffers::Offset<Body> CreateBody(flatbuffers::FlatBufferBuilder &_fbb, const BodyT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct BoxT : public flatbuffers::NativeTable {
  typedef Box TableType;
  std::unique_ptr<Point> position;
  float rotation;
  int32_t type;
  int32_t id;
  uint32_t color;
  BoxT()
      : rotation(0.0f),
        type(0),
        id(0),
        color(0) {
  }
};

struct Box FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef BoxT NativeTableType;
  enum {
    VT_POSITION = 4,
    VT_ROTATION = 6,
    VT_TYPE = 8,
    VT_ID = 10,
    VT_COLOR = 12
  };
  const Point *position() const {
    return GetStruct<const Point *>(VT_POSITION);
  }
  float rotation() const {
    return GetField<float>(VT_ROTATION, 0.0f);
  }
  int32_t type() const {
    return GetField<int32_t>(VT_TYPE, 0);
  }
  int32_t id() const {
    return GetField<int32_t>(VT_ID, 0);
  }
  uint32_t color() const {
    return GetField<uint32_t>(VT_COLOR, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<Point>(verifier, VT_POSITION) &&
           VerifyField<float>(verifier, VT_ROTATION) &&
           VerifyField<int32_t>(verifier, VT_TYPE) &&
           VerifyField<int32_t>(verifier, VT_ID) &&
           VerifyField<uint32_t>(verifier, VT_COLOR) &&
           verifier.EndTable();
  }
  BoxT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(BoxT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<Box> Pack(flatbuffers::FlatBufferBuilder &_fbb, const BoxT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct BoxBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_position(const Point *position) {
    fbb_.AddStruct(Box::VT_POSITION, position);
  }
  void add_rotation(float rotation) {
    fbb_.AddElement<float>(Box::VT_ROTATION, rotation, 0.0f);
  }
  void add_type(int32_t type) {
    fbb_.AddElement<int32_t>(Box::VT_TYPE, type, 0);
  }
  void add_id(int32_t id) {
    fbb_.AddElement<int32_t>(Box::VT_ID, id, 0);
  }
  void add_color(uint32_t color) {
    fbb_.AddElement<uint32_t>(Box::VT_COLOR, color, 0);
  }
  BoxBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  BoxBuilder &operator=(const BoxBuilder &);
  flatbuffers::Offset<Box> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Box>(end);
    return o;
  }
};

inline flatbuffers::Offset<Box> CreateBox(
    flatbuffers::FlatBufferBuilder &_fbb,
    const Point *position = 0,
    float rotation = 0.0f,
    int32_t type = 0,
    int32_t id = 0,
    uint32_t color = 0) {
  BoxBuilder builder_(_fbb);
  builder_.add_color(color);
  builder_.add_id(id);
  builder_.add_type(type);
  builder_.add_rotation(rotation);
  builder_.add_position(position);
  return builder_.Finish();
}

flatbuffers::Offset<Box> CreateBox(flatbuffers::FlatBufferBuilder &_fbb, const BoxT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

struct BodiesT : public flatbuffers::NativeTable {
  typedef Bodies TableType;
  std::vector<AnyUnion> boxes;
  BodiesT() {
  }
};

struct Bodies FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef BodiesT NativeTableType;
  enum {
    VT_BOXES_TYPE = 4,
    VT_BOXES = 6
  };
  const flatbuffers::Vector<uint8_t> *boxes_type() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_BOXES_TYPE);
  }
  const flatbuffers::Vector<flatbuffers::Offset<void>> *boxes() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<void>> *>(VT_BOXES);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_BOXES_TYPE) &&
           verifier.Verify(boxes_type()) &&
           VerifyOffset(verifier, VT_BOXES) &&
           verifier.Verify(boxes()) &&
           VerifyAnyVector(verifier, boxes(), boxes_type()) &&
           verifier.EndTable();
  }
  BodiesT *UnPack(const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  void UnPackTo(BodiesT *_o, const flatbuffers::resolver_function_t *_resolver = nullptr) const;
  static flatbuffers::Offset<Bodies> Pack(flatbuffers::FlatBufferBuilder &_fbb, const BodiesT* _o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);
};

struct BodiesBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_boxes_type(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> boxes_type) {
    fbb_.AddOffset(Bodies::VT_BOXES_TYPE, boxes_type);
  }
  void add_boxes(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<void>>> boxes) {
    fbb_.AddOffset(Bodies::VT_BOXES, boxes);
  }
  BodiesBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  BodiesBuilder &operator=(const BodiesBuilder &);
  flatbuffers::Offset<Bodies> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Bodies>(end);
    return o;
  }
};

inline flatbuffers::Offset<Bodies> CreateBodies(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> boxes_type = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<void>>> boxes = 0) {
  BodiesBuilder builder_(_fbb);
  builder_.add_boxes(boxes);
  builder_.add_boxes_type(boxes_type);
  return builder_.Finish();
}

inline flatbuffers::Offset<Bodies> CreateBodiesDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<uint8_t> *boxes_type = nullptr,
    const std::vector<flatbuffers::Offset<void>> *boxes = nullptr) {
  return BoxData::CreateBodies(
      _fbb,
      boxes_type ? _fbb.CreateVector<uint8_t>(*boxes_type) : 0,
      boxes ? _fbb.CreateVector<flatbuffers::Offset<void>>(*boxes) : 0);
}

flatbuffers::Offset<Bodies> CreateBodies(flatbuffers::FlatBufferBuilder &_fbb, const BodiesT *_o, const flatbuffers::rehasher_function_t *_rehasher = nullptr);

inline ShapeT *Shape::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = new ShapeT();
  UnPackTo(_o, _resolver);
  return _o;
}

inline void Shape::UnPackTo(ShapeT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = color(); _o->color = _e; };
  { auto _e = vertices(); if (_e) { _o->vertices.resize(_e->size()); for (flatbuffers::uoffset_t _i = 0; _i < _e->size(); _i++) { _o->vertices[_i] = *_e->Get(_i); } } };
}

inline flatbuffers::Offset<Shape> Shape::Pack(flatbuffers::FlatBufferBuilder &_fbb, const ShapeT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateShape(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<Shape> CreateShape(flatbuffers::FlatBufferBuilder &_fbb, const ShapeT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const ShapeT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _color = _o->color;
  auto _vertices = _o->vertices.size() ? _fbb.CreateVectorOfStructs(_o->vertices) : 0;
  return BoxData::CreateShape(
      _fbb,
      _color,
      _vertices);
}

inline BodyT *Body::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = new BodyT();
  UnPackTo(_o, _resolver);
  return _o;
}

inline void Body::UnPackTo(BodyT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = position(); if (_e) _o->position = std::unique_ptr<Point>(new Point(*_e)); };
  { auto _e = rotation(); _o->rotation = _e; };
  { auto _e = shapes(); if (_e) { _o->shapes.resize(_e->size()); for (flatbuffers::uoffset_t _i = 0; _i < _e->size(); _i++) { _o->shapes[_i] = std::unique_ptr<ShapeT>(_e->Get(_i)->UnPack(_resolver)); } } };
  { auto _e = isStatic(); _o->isStatic = _e; };
  { auto _e = id(); _o->id = _e; };
}

inline flatbuffers::Offset<Body> Body::Pack(flatbuffers::FlatBufferBuilder &_fbb, const BodyT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateBody(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<Body> CreateBody(flatbuffers::FlatBufferBuilder &_fbb, const BodyT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const BodyT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _position = _o->position ? _o->position.get() : 0;
  auto _rotation = _o->rotation;
  auto _shapes = _o->shapes.size() ? _fbb.CreateVector<flatbuffers::Offset<Shape>> (_o->shapes.size(), [](size_t i, _VectorArgs *__va) { return CreateShape(*__va->__fbb, __va->__o->shapes[i].get(), __va->__rehasher); }, &_va ) : 0;
  auto _isStatic = _o->isStatic;
  auto _id = _o->id;
  return BoxData::CreateBody(
      _fbb,
      _position,
      _rotation,
      _shapes,
      _isStatic,
      _id);
}

inline BoxT *Box::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = new BoxT();
  UnPackTo(_o, _resolver);
  return _o;
}

inline void Box::UnPackTo(BoxT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = position(); if (_e) _o->position = std::unique_ptr<Point>(new Point(*_e)); };
  { auto _e = rotation(); _o->rotation = _e; };
  { auto _e = type(); _o->type = _e; };
  { auto _e = id(); _o->id = _e; };
  { auto _e = color(); _o->color = _e; };
}

inline flatbuffers::Offset<Box> Box::Pack(flatbuffers::FlatBufferBuilder &_fbb, const BoxT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateBox(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<Box> CreateBox(flatbuffers::FlatBufferBuilder &_fbb, const BoxT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const BoxT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _position = _o->position ? _o->position.get() : 0;
  auto _rotation = _o->rotation;
  auto _type = _o->type;
  auto _id = _o->id;
  auto _color = _o->color;
  return BoxData::CreateBox(
      _fbb,
      _position,
      _rotation,
      _type,
      _id,
      _color);
}

inline BodiesT *Bodies::UnPack(const flatbuffers::resolver_function_t *_resolver) const {
  auto _o = new BodiesT();
  UnPackTo(_o, _resolver);
  return _o;
}

inline void Bodies::UnPackTo(BodiesT *_o, const flatbuffers::resolver_function_t *_resolver) const {
  (void)_o;
  (void)_resolver;
  { auto _e = boxes_type(); if (_e) { _o->boxes.resize(_e->size()); for (flatbuffers::uoffset_t _i = 0; _i < _e->size(); _i++) { _o->boxes[_i].type = (Any)_e->Get(_i); } } };
  { auto _e = boxes(); if (_e) { _o->boxes.resize(_e->size()); for (flatbuffers::uoffset_t _i = 0; _i < _e->size(); _i++) { _o->boxes[_i].value = AnyUnion::UnPack(_e->Get(_i), boxes_type()->GetEnum<Any>(_i), _resolver); } } };
}

inline flatbuffers::Offset<Bodies> Bodies::Pack(flatbuffers::FlatBufferBuilder &_fbb, const BodiesT* _o, const flatbuffers::rehasher_function_t *_rehasher) {
  return CreateBodies(_fbb, _o, _rehasher);
}

inline flatbuffers::Offset<Bodies> CreateBodies(flatbuffers::FlatBufferBuilder &_fbb, const BodiesT *_o, const flatbuffers::rehasher_function_t *_rehasher) {
  (void)_rehasher;
  (void)_o;
  struct _VectorArgs { flatbuffers::FlatBufferBuilder *__fbb; const BodiesT* __o; const flatbuffers::rehasher_function_t *__rehasher; } _va = { &_fbb, _o, _rehasher}; (void)_va;
  auto _boxes_type = _o->boxes.size() ? _fbb.CreateVector<uint8_t>(_o->boxes.size(), [](size_t i, _VectorArgs *__va) { return static_cast<uint8_t>(__va->__o->boxes[i].type); }, &_va) : 0;
  auto _boxes = _o->boxes.size() ? _fbb.CreateVector<flatbuffers::Offset<void>>(_o->boxes.size(), [](size_t i, _VectorArgs *__va) { return __va->__o->boxes[i].Pack(*__va->__fbb, __va->__rehasher); }, &_va) : 0;
  return BoxData::CreateBodies(
      _fbb,
      _boxes_type,
      _boxes);
}

inline bool VerifyAny(flatbuffers::Verifier &verifier, const void *obj, Any type) {
  switch (type) {
    case Any_NONE: {
      return true;
    }
    case Any_Body: {
      auto ptr = reinterpret_cast<const Body *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case Any_Box: {
      auto ptr = reinterpret_cast<const Box *>(obj);
      return verifier.VerifyTable(ptr);
    }
    default: return false;
  }
}

inline bool VerifyAnyVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types) {
  if (values->size() != types->size()) return false;
  for (flatbuffers::uoffset_t i = 0; i < values->size(); ++i) {
    if (!VerifyAny(
        verifier,  values->Get(i), types->GetEnum<Any>(i))) {
      return false;
    }
  }
  return true;
}

inline void *AnyUnion::UnPack(const void *obj, Any type, const flatbuffers::resolver_function_t *resolver) {
  switch (type) {
    case Any_Body: {
      auto ptr = reinterpret_cast<const Body *>(obj);
      return ptr->UnPack(resolver);
    }
    case Any_Box: {
      auto ptr = reinterpret_cast<const Box *>(obj);
      return ptr->UnPack(resolver);
    }
    default: return nullptr;
  }
}

inline flatbuffers::Offset<void> AnyUnion::Pack(flatbuffers::FlatBufferBuilder &_fbb, const flatbuffers::rehasher_function_t *_rehasher) const {
  switch (type) {
    case Any_Body: {
      auto ptr = reinterpret_cast<const BodyT *>(value);
      return CreateBody(_fbb, ptr, _rehasher).Union();
    }
    case Any_Box: {
      auto ptr = reinterpret_cast<const BoxT *>(value);
      return CreateBox(_fbb, ptr, _rehasher).Union();
    }
    default: return 0;
  }
}

inline AnyUnion::AnyUnion(const AnyUnion &u) FLATBUFFERS_NOEXCEPT : type(u.type), value(nullptr) {
  switch (type) {
    case Any_Body: {
      assert(false);  // BodyT not copyable.
      break;
    }
    case Any_Box: {
      assert(false);  // BoxT not copyable.
      break;
    }
    default:
      break;
  }
}

inline void AnyUnion::Reset() {
  switch (type) {
    case Any_Body: {
      auto ptr = reinterpret_cast<BodyT *>(value);
      delete ptr;
      break;
    }
    case Any_Box: {
      auto ptr = reinterpret_cast<BoxT *>(value);
      delete ptr;
      break;
    }
    default: break;
  }
  value = nullptr;
  type = Any_NONE;
}

inline const BoxData::Bodies *GetBodies(const void *buf) {
  return flatbuffers::GetRoot<BoxData::Bodies>(buf);
}

inline bool VerifyBodiesBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<BoxData::Bodies>(nullptr);
}

inline void FinishBodiesBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<BoxData::Bodies> root) {
  fbb.Finish(root);
}

inline std::unique_ptr<BodiesT> UnPackBodies(
    const void *buf,
    const flatbuffers::resolver_function_t *res = nullptr) {
  return std::unique_ptr<BodiesT>(GetBodies(buf)->UnPack(res));
}

}  // namespace BoxData

#endif  // FLATBUFFERS_GENERATED_BOXDATA_BOXDATA_H_