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

struct DummyResourceBase : ResourceBaseT<DummyResourceRaw*>
{
  using ResourceBaseT::ResourceBaseT;

  void Destroy() { DestroyDummyResource(release()); }
};

using DummyResourceRef = ResourceRefT<DummyResourceBase>;

struct DummyResource : DummyResourceBase
{
  using DummyResourceBase::DummyResourceBase;

  explicit DummyResource(DummyResourceRaw* resource)
    : DummyResourceBase(resource)
  {
  }

  /// Move ctor
  DummyResource(DummyResource&& other) noexcept
    : DummyResourceBase(other.release())
  {
  }

  DummyResource& operator=(DummyResource&& other) noexcept
  {
    if (this != &other) { DummyResourceBase::operator=(other.release()); }
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
    DummyResource res1;
    CHECK(!res1);
    CHECK(res1.get() == nullptr);
    DummyResource res2(nullptr);
    CHECK(!res2);
    CHECK(res2.get() == nullptr);
  }
  SUBCASE("Reference wrapper")
  {
    {
      auto res = CreateDummyResource(&raw);
      CHECK(res);
      CHECK(res.get() == &raw);

      DummyResourceRef ref(res);
      CHECK(ref);
      CHECK(ref.get() == &raw);
      CHECK(raw.count == 1);

      DummyResourceRaw* rawPtr = ref;
      CHECK(rawPtr == &raw);
    }
    CHECK(raw.count == 0);
    DummyResourceRef ref1;
    CHECK(!ref1);
    CHECK(ref1.get() == nullptr);
    DummyResourceRef ref2(nullptr);
    CHECK(!ref2);
    CHECK(ref2.get() == nullptr);
  }
  SUBCASE("Create into reference wrapper")
  {
    DummyResourceRef ref = CreateDummyResource(&raw).release();
    CHECK(ref);
    CHECK(ref.get() == &raw);
    CHECK(raw.count == 1);
    DestroyDummyResource(ref.release());
    CHECK(raw.count == 0);
  }
}

} // namespace SDL
