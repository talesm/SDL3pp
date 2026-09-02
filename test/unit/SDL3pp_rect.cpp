#include "SDL3pp/SDL3pp_rect.h"
#include "doctest.h"

namespace SDL {

TEST_CASE("Point")
{
  Point p{10, 20};

  CHECK(p.GetX() == 10);
  CHECK(p.GetY() == 20);

  p.SetX(11).SetY(21);

  CHECK(p.GetX() == 11);
  CHECK(p.GetY() == 21);

  FPoint fp = static_cast<FPointRaw>(p);
  CHECK(fp.GetX() == 11);
  CHECK(fp.GetY() == 21);

  // p = fp; // This should not compile, as the conversion is explicit
  p = Point{fp};
  CHECK(p.GetX() == 11);
  CHECK(p.GetY() == 21);

  fp = 2.f * p;
  CHECK(fp.GetX() == 22);
  CHECK(fp.GetY() == 42);

  fp = 2.5f * p;
  CHECK(fp.GetX() == 27.5f);
  CHECK(fp.GetY() == 52.5f);

  p = 2 * p;
  CHECK(p.GetX() == 22);
  CHECK(p.GetY() == 42);
}

TEST_CASE("Rect")
{
  Rect r{10, 20, 30, 40};

  CHECK(r.GetX() == 10);
  CHECK(r.GetY() == 20);
  CHECK(r.GetW() == 30);
  CHECK(r.GetH() == 40);

  r.SetX(11).SetY(21).SetW(31).SetH(41);

  CHECK(r.GetX() == 11);
  CHECK(r.GetY() == 21);
  CHECK(r.GetW() == 31);
  CHECK(r.GetH() == 41);

  FRect fr = static_cast<FRectRaw>(r);
  CHECK(fr.GetX() == 11);
  CHECK(fr.GetY() == 21);
  CHECK(fr.GetW() == 31);
  CHECK(fr.GetH() == 41);

  // r = fr; // This should not compile, as the conversion is explicit
  r = Rect{fr};
  CHECK(r.GetX() == 11);
  CHECK(r.GetY() == 21);
  CHECK(r.GetW() == 31);
  CHECK(r.GetH() == 41);
}

}
