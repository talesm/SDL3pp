#include "doctest.h"
#include "SDL3pp/SDL3pp_resource.h"

namespace SDL {

struct DummyResourceRaw
{
  int count;
};

void DestroyDummyResource(DummyResourceRaw* resource)
{
  if (resource) resource->count--;
}

struct DummyResourceRef : ResourceBaseT<DummyResourceRaw*>
{
  using ResourceBaseT::ResourceBaseT;

  void Destroy() { DestroyDummyResource(release()); }
};

struct DummyResource : DummyResourceRef
{
  using DummyResourceRef::DummyResourceRef;

  explicit DummyResource(DummyResourceRaw* resource)
    : DummyResourceRef(resource)
  {
  }

  /// Move ctor
  DummyResource(DummyResource&& other) noexcept
    : DummyResourceRef(other.release())
  {
  }

  DummyResource& operator=(DummyResource&& other) noexcept
  {
    if (this != &other) { DummyResourceRef::operator=(other.release()); }
    return *this;
  }

  ~DummyResource() { DestroyDummyResource(get()); }
};

DummyResource CreateDummyResource(DummyResourceRaw* resource)
{
  resource->count++;
  return DummyResource(resource);
}

TEST_CASE("ResourceBaseT")
{
  DummyResourceRaw raw{0};

  SUBCASE("Owned resource")
  {
    {
      auto res{CreateDummyResource(&raw)};
      CHECK(res);
      CHECK(res.get() == &raw);

      auto res2{std::move(res)};
      CHECK(res2);
      CHECK(res2.get() == &raw);
      CHECK(raw.count == 1);
    }
    CHECK(raw.count == 0);
    ResourceBase<DummyResourceRaw*> res1;
    CHECK(!res1);
    CHECK(res1.get() == nullptr);
    ResourceBase<DummyResourceRaw*> res2(nullptr);
    CHECK(!res2);
    CHECK(res2.get() == nullptr);
  }
  SUBCASE("Reference wrapper")
  {
    {
      auto res = CreateDummyResource(&raw);
      CHECK(res);
      CHECK(res.get() == &raw);

      DummyResourceRef res2(res);
      CHECK(res2);
      CHECK(res2.get() == &raw);
      CHECK(raw.count == 1);
    }
    CHECK(raw.count == 0);
  }
  SUBCASE("Create into reference wrapper")
  {
    DummyResourceRef res = CreateDummyResource(&raw).release();
    CHECK(res);
    CHECK(res.get() == &raw);
    CHECK(raw.count == 1);
    DestroyDummyResource(res.release());
    CHECK(raw.count == 0);
  }
}

} // namespace SDL
