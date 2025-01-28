#ifndef SDL3PP_RECT_HPP_
#define SDL3PP_RECT_HPP_

#include <cmath>
#include <optional>
#include <span>
#include <SDL3/SDL_rect.h>

namespace SDL {

// Forward decl
struct FPoint;
struct FRect;
struct Rect;

/**
 * @brief The structure that defines a point (using integers)
 *
 * Based on https://github.com/libSDL2pp/libSDL2pp/blob/master/SDL2pp/Point.hh
 */
struct Point : SDL_Point
{
  constexpr Point(const SDL_Point& point)
    : SDL_Point(point)
  {
  }
  constexpr Point()
    : Point({0})
  {
  }

  constexpr Point(int x, int y)
    : SDL_Point{x, y}
  {
  }

  /**
   * @brief Get X coordinate of the point
   *
   * @returns X coordinate of the point
   *
   */
  constexpr int GetX() const { return x; }

  /**
   * @brief Set X coordinate of the point
   *
   * @param[in] nx New X coordinate value
   *
   * @returns Reference to self
   *
   */
  constexpr Point& SetX(int nx)
  {
    x = nx;
    return *this;
  }

  /**
   * @brief Get Y coordinate of the point
   *
   * @returns Y coordinate of the point
   *
   */
  constexpr int GetY() const { return y; }

  /**
   * @brief Set Y coordinate of the point
   *
   * @param[in] ny New Y coordinate value
   *
   * @returns Reference to self
   *
   */
  constexpr Point& SetY(int ny)
  {
    y = ny;
    return *this;
  }

  /**
   * @brief Determine whether a point resides inside a rectangle.
   *
   * A point is considered part of a rectangle if both `p` and `r` are not NULL,
   * and `p`'s x and y coordinates are >= to the rectangle's top left corner,
   * and < the rectangle's x+w and y+h. So a 1x1 rectangle considers point (0,0)
   * as "inside" and (0,1) as not.
   *
   * Note that this is a forced-inline function in a header, and not a public
   * API function available in the SDL library (which is to say, the code is
   * embedded in the calling program and the linker and dynamic loader will not
   * be able to find this function inside SDL itself).
   *
   * @param r the rectangle to test.
   * @return true if `p` is contained by `r`, false otherwise.
   */
  constexpr bool IsInRect(const Rect& r) const;

  /**
   * @brief Get point's memberwise negation
   *
   * @returns New Point representing memberwise negation
   *
   */
  constexpr Point operator-() const { return Point(-x, -y); }

  /**
   * @brief Get point's memberwise addition with another point
   *
   * @param[in] other Point to add
   *
   * @returns New Point representing memberwise addition with another point
   *
   */
  constexpr Point operator+(const Point& other) const
  {
    return Point(x + other.x, y + other.y);
  }

  /**
   * @brief Get point's memberwise subtraction with another point
   *
   * @param[in] other Point to subtract
   *
   * @returns New Point representing memberwise subtraction of another point
   *
   */
  constexpr Point operator-(const Point& other) const
  {
    return Point(x - other.x, y - other.y);
  }

  /**
   * @brief Get point's memberwise division by an integer
   *
   * @param[in] value Divisor
   *
   * @returns New Point representing memberwise division of
   *          point by an integer
   *
   */
  constexpr Point operator/(int value) const
  {
    return Point(x / value, y / value);
  }

  /**
   * @brief Get point's memberwise division by another point
   *
   * @param[in] other Divisor
   *
   * @returns New Point representing memberwise division of
   *          point by another point
   *
   */
  constexpr Point operator/(const Point& other) const
  {
    return Point(x / other.x, y / other.y);
  }

  /**
   * @brief Get point's memberwise remainder from division
   *        by an integer
   *
   * @param[in] value Divisor
   *
   * @returns New Point representing memberwise remainder
   *          from division by an integer
   *
   */
  constexpr Point operator%(int value) const
  {
    return Point(x % value, y % value);
  }

  /**
   * @brief Get point's memberwise remainder from division
   *        by another point
   *
   * @param[in] other Divisor
   *
   * @returns New Point representing memberwise remainder
   *          from division by another point
   *
   */
  constexpr Point operator%(const Point& other) const
  {
    return Point(x % other.x, y % other.y);
  }

  /**
   * @brief Get point's memberwise multiplication by an
   *        integer
   *
   * @param[in] value Multiplier
   *
   * @returns New Point representing memberwise multiplication
   *          of point by an integer
   *
   */
  constexpr Point operator*(int value) const
  {
    return Point(x * value, y * value);
  }

  /**
   * @brief Get point's memberwise multiplication by another
   *        point
   *
   * @param[in] other Multiplier
   *
   * @returns New Point representing memberwise multiplication
   *          of point by another point
   *
   */
  constexpr Point operator*(const Point& other) const
  {
    return Point(x * other.x, y * other.y);
  }

  /**
   * @brief Memberwise add another point
   *
   * @param[in] other Point to add to the current one
   *
   * @returns Reference to self
   *
   */
  constexpr Point& operator+=(const Point& other)
  {
    x += other.x;
    y += other.y;
    return *this;
  }

  /**
   * @brief Memberwise subtract another point
   *
   * @param[in] other Point to subtract from the current one
   *
   * @returns Reference to self
   *
   */
  constexpr Point& operator-=(const Point& other)
  {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  /**
   * @brief Memberwise divide by an integer
   *
   * @param[in] value Divisor
   *
   * @returns Reference to self
   *
   */
  constexpr Point& operator/=(int value)
  {
    x /= value;
    y /= value;
    return *this;
  }

  /**
   * @brief Memberwise divide by another point
   *
   * @param[in] other Divisor
   *
   * @returns Reference to self
   *
   */
  constexpr Point& operator/=(const Point& other)
  {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  /**
   * @brief Memberwise remainder from division by an integer
   *
   * @param[in] value Divisor
   *
   * @returns Reference to self
   *
   */
  constexpr Point& operator%=(int value)
  {
    x %= value;
    y %= value;
    return *this;
  }

  /**
   * @brief Memberwise remainder from division by another
   *        point
   *
   * @param[in] other Divisor
   *
   * @returns Reference to self
   *
   */
  constexpr Point& operator%=(const Point& other)
  {
    x %= other.x;
    y %= other.y;
    return *this;
  }

  /**
   * @brief Memberwise multiply by an integer
   *
   * @param[in] value Multiplier
   *
   * @returns Reference to self
   *
   */
  constexpr Point& operator*=(int value)
  {
    x *= value;
    y *= value;
    return *this;
  }

  /**
   * @brief Memberwise multiply by another point
   *
   * @param[in] other Multiplier
   *
   * @returns Reference to self
   *
   */
  constexpr Point& operator*=(const Point& other)
  {
    x *= other.x;
    y *= other.y;
    return *this;
  }

  /**
   * @brief Get a point with coordinates modified so it fits
   *        into a given rect
   *
   * @param[in] rect Rectangle to clamp with
   *
   * @returns Clamped point
   *
   */
  constexpr Point GetClamped(const Rect& rect) const;

  /**
   * @brief Clamp point coordinates to make it fit into a
   *        given rect
   *
   * @param[in] rect Rectangle to clamp with
   *
   * @returns Reference to self
   *
   */
  constexpr Point& Clamp(const Rect& rect);

  /**
   * @brief Get a point wrapped within a specified rect
   *
   * @param[in] rect Rectangle to wrap with
   *
   * @returns Wrapped point
   *
   */
  constexpr Point GetWrapped(const Rect& rect) const;

  /**
   * @brief Wrap point coordinates within a specified rect
   *
   * @param[in] rect Rectangle to wrap with
   *
   * @returns Reference to self
   *
   */
  constexpr Point& Wrap(const Rect& rect);

  // Auto comparison operator
  auto operator<=>(const Point&) const = default;

  /**
   * @brief Converts to FPoint
   *
   * @return FPoint
   */
  constexpr operator FPoint() const;
};

/**
 * @brief The structure that defines a point (using floating point values).
 */
struct FPoint : SDL_FPoint
{
  constexpr FPoint(const SDL_FPoint& point)
    : SDL_FPoint(point)
  {
  }
  constexpr FPoint()
    : FPoint({0})
  {
  }

  constexpr FPoint(float x, float y)
    : SDL_FPoint{x, y}
  {
  }

  /**
   * @brief Get X coordinate of the point
   *
   * @returns X coordinate of the point
   *
   */
  constexpr int GetX() const { return x; }

  /**
   * @brief Set X coordinate of the point
   *
   * @param[in] nx New X coordinate value
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& SetX(int nx)
  {
    x = nx;
    return *this;
  }

  /**
   * @brief Get Y coordinate of the point
   *
   * @returns Y coordinate of the point
   *
   */
  constexpr int GetY() const { return y; }

  /**
   * @brief Set Y coordinate of the point
   *
   * @param[in] ny New Y coordinate value
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& SetY(int ny)
  {
    y = ny;
    return *this;
  }

  /**
   * @brief Determine whether a point resides inside a rectangle.
   *
   * A point is considered part of a rectangle if both `p` and `r` are not NULL,
   * and `p`'s x and y coordinates are >= to the rectangle's top left corner,
   * and < the rectangle's x+w and y+h. So a 1x1 rectangle considers point (0,0)
   * as "inside" and (0,1) as not.
   *
   * Note that this is a forced-inline function in a header, and not a public
   * API function available in the SDL library (which is to say, the code is
   * embedded in the calling program and the linker and dynamic loader will not
   * be able to find this function inside SDL itself).
   *
   * @param r the rectangle to test.
   * @return true if `p` is contained by `r`, false otherwise.
   */
  constexpr bool IsInRect(const FRect& r) const;

  /**
   * @brief Get point's memberwise negation
   *
   * @returns New Point representing memberwise negation
   *
   */
  constexpr FPoint operator-() const { return FPoint(-x, -y); }

  /**
   * @brief Get point's memberwise addition with another point
   *
   * @param[in] other Point to add
   *
   * @returns New Point representing memberwise addition with another point
   *
   */
  constexpr FPoint operator+(const FPoint& other) const
  {
    return FPoint(x + other.x, y + other.y);
  }

  /**
   * @brief Get point's memberwise subtraction with another point
   *
   * @param[in] other Point to subtract
   *
   * @returns New Point representing memberwise subtraction of another point
   *
   */
  constexpr FPoint operator-(const FPoint& other) const
  {
    return FPoint(x - other.x, y - other.y);
  }

  /**
   * @brief Get point's memberwise division by an float
   *
   * @param[in] value Divisor
   *
   * @returns New Point representing memberwise division of
   *          point by an float
   *
   */
  constexpr FPoint operator/(float value) const
  {
    return FPoint(x / value, y / value);
  }

  /**
   * @brief Get point's memberwise division by another point
   *
   * @param[in] other Divisor
   *
   * @returns New Point representing memberwise division of
   *          point by another point
   *
   */
  constexpr FPoint operator/(const FPoint& other) const
  {
    return FPoint(x / other.x, y / other.y);
  }

  /**
   * @brief Get point's memberwise multiplication by an
   *        float
   *
   * @param[in] value Multiplier
   *
   * @returns New Point representing memberwise multiplication
   *          of point by an float
   *
   */
  constexpr FPoint operator*(float value) const
  {
    return FPoint(x * value, y * value);
  }

  /**
   * @brief Get point's memberwise multiplication by another
   *        point
   *
   * @param[in] other Multiplier
   *
   * @returns New Point representing memberwise multiplication
   *          of point by another point
   *
   */
  constexpr FPoint operator*(const FPoint& other) const
  {
    return FPoint(x * other.x, y * other.y);
  }

  /**
   * @brief Memberwise add another point
   *
   * @param[in] other Point to add to the current one
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& operator+=(const Point& other)
  {
    x += other.x;
    y += other.y;
    return *this;
  }

  /**
   * @brief Memberwise subtract another point
   *
   * @param[in] other Point to subtract from the current one
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& operator-=(const Point& other)
  {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  /**
   * @brief Memberwise divide by an float
   *
   * @param[in] value Divisor
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& operator/=(float value)
  {
    x /= value;
    y /= value;
    return *this;
  }

  /**
   * @brief Memberwise divide by another point
   *
   * @param[in] other Divisor
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& operator/=(const Point& other)
  {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  /**
   * @brief Memberwise multiply by an float
   *
   * @param[in] value Multiplier
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& operator*=(float value)
  {
    x *= value;
    y *= value;
    return *this;
  }

  /**
   * @brief Memberwise multiply by another point
   *
   * @param[in] other Multiplier
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& operator*=(const FPoint& other)
  {
    x *= other.x;
    y *= other.y;
    return *this;
  }

  /**
   * @brief Get a point with coordinates modified so it fits
   *        into a given rect
   *
   * @param[in] rect Rectangle to clamp with
   *
   * @returns Clamped point
   *
   */
  constexpr FPoint GetClamped(const FRect& rect) const;

  /**
   * @brief Clamp point coordinates to make it fit into a
   *        given rect
   *
   * @param[in] rect Rectangle to clamp with
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& Clamp(const FRect& rect);

  /**
   * @brief Get a point wrapped within a specified rect
   *
   * @param[in] rect Rectangle to wrap with
   *
   * @returns Wrapped point
   *
   */
  constexpr FPoint GetWrapped(const FRect& rect) const;

  /**
   * @brief Wrap point coordinates within a specified rect
   *
   * @param[in] rect Rectangle to wrap with
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& Wrap(const FRect& rect);

  // Auto comparison operator
  auto operator<=>(const FPoint&) const = default;
};

/**
 * @brief A rectangle, with the origin at the upper left (using integers).
 */
struct Rect : SDL_Rect
{
  constexpr Rect(const SDL_Rect& rect)
    : SDL_Rect(rect)
  {
  }

  constexpr Rect()
    : Rect({0})
  {
  }

  constexpr Rect(int x, int y, int w, int h)
    : SDL_Rect({x, y, w, h})
  {
  }

  constexpr Rect(const SDL_Point& corner, const SDL_Point& size)
    : Rect{corner.x, corner.y, size.x, size.y}
  {
  }

  static Rect GetEnclosingPoints(std::span<const SDL_Point> points)
  {
    return GetEnclosingPoints(points.data(), points.size());
  }

  static Rect GetEnclosingPoints(std::span<const SDL_Point> points,
                                 const SDL_Rect& clip)
  {
    return GetEnclosingPoints(points.data(), points.size(), clip);
  }

  static Rect GetEnclosingPoints(const SDL_Point* points, int count)
  {
    Rect result;
    SDL_GetRectEnclosingPoints(points, count, nullptr, &result);
    return result;
  }
  static Rect GetEnclosingPoints(const SDL_Point* points,
                                 int count,
                                 const SDL_Rect& clip)
  {
    Rect result;
    SDL_GetRectEnclosingPoints(points, count, &clip, &result);
    return result;
  }

  /**
   * @brief Construct the rect from given center coordinates, width and height
   *
   * @param[in] cx X coordinate of the rectangle center
   * @param[in] cy Y coordinate of the rectangle center
   * @param[in] w Width of the rectangle
   * @param[in] h Height of the rectangle
   *
   */
  static constexpr Rect FromCenter(int cx, int cy, int w, int h)
  {
    return Rect(cx - w / 2, cy - h / 2, w, h);
  }

  /**
   * @brief Construct the rect from given center coordinates and size
   *
   * @param[in] center Coordinates of the rectangle center
   * @param[in] size Dimensions of the rectangle
   *
   */
  static constexpr Rect FromCenter(const Point& center, const Point& size)
  {
    return Rect(center - size / 2, size);
  }

  /**
   * @brief Construct the rect from given corners coordinates
   *
   * @param[in] x1 X coordinate of the top left rectangle corner
   * @param[in] y1 Y coordinate of the top left rectangle corner
   * @param[in] x2 X coordinate of the bottom right rectangle corner
   * @param[in] y2 Y coordinate of the bottom right rectangle corner
   *
   */
  static constexpr Rect FromCorners(int x1, int y1, int x2, int y2)
  {
    return Rect(x1, y1, x2 - x1 + 1, y2 - y1 + 1);
  }

  /**
   * @brief Construct the rect from given centers coordinates
   *
   * @param[in] p1 Coordinates of the top left rectangle corner
   * @param[in] p2 Coordinates of the bottom right rectangle corner
   *
   */
  static constexpr Rect FromCorners(const Point& p1, const Point& p2)
  {
    return Rect(p1, p2 - p1 + Point(1, 1));
  }

  /**
   * @brief Get X coordinate of the rect corner
   *
   * @returns X coordinate of the rect corner
   *
   */
  constexpr int GetX() const { return x; }

  /**
   * @brief Set X coordinate of the rect corner
   *
   * @param[in] nx New X coordinate value
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& SetX(int nx)
  {
    x = nx;
    return *this;
  }

  /**
   * @brief Get Y coordinate of the rect corner
   *
   * @returns Y coordinate of the rect corner
   *
   */
  constexpr int GetY() const { return y; }

  /**
   * @brief Set Y coordinate of the rect corner
   *
   * @param[in] ny New Y coordinate value
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& SetY(int ny)
  {
    y = ny;
    return *this;
  }

  /**
   * @brief Get width of the rect
   *
   * @returns Width of the rect
   *
   */
  constexpr int GetW() const { return w; }

  /**
   * @brief Set width of the rect
   *
   * @param[in] nw New width of the rect
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& SetW(int nw)
  {
    w = nw;
    return *this;
  }

  /**
   * @brief Get height of the rect
   *
   * @returns Height of the rect
   *
   */
  constexpr int GetH() const { return h; }

  /**
   * @brief Set height of the rect
   *
   * @param[in] nh New height of the rect
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& SetH(int nh)
  {
    h = nh;
    return *this;
  }

  /**
   * @brief Get X coordinate of the rect second corner
   *
   * @returns X coordinate of the rect second corner
   *
   */
  constexpr int GetX2() const { return x + w - 1; }

  /**
   * @brief Set X coordinate of the rect second corner
   *
   * @param[in] x2 New X coordinate value
   *
   * This modifies rectangle width internally
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& SetX2(int x2)
  {
    w = x2 - x + 1;
    return *this;
  }

  /**
   * @brief Get Y coordinate of the rect second corner
   *
   * @returns Y coordinate of the rect second corner
   *
   */
  constexpr int GetY2() const { return y + h - 1; }

  /**
   * @brief Set Y coordinate of the rect second corner
   *
   * @param[in] y2 New Y coordinate value
   *
   * This modifies rectangle height internally
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& SetY2(int y2)
  {
    h = y2 - y + 1;
    return *this;
  }

  /**
   * @brief Get top left corner of the rect
   *
   * @returns Top left corner of the rect
   *
   */
  constexpr Point GetTopLeft() const { return Point(x, y); }

  /**
   * @brief Get top right corner of the rect
   *
   * @returns Top right corner of the rect
   *
   */
  constexpr Point GetTopRight() const { return Point(GetX2(), y); }

  /**
   * @brief Get bottom left corner of the rect
   *
   * @returns bottom left corner of the rect
   *
   */
  constexpr Point GetBottomLeft() const { return Point(x, GetY2()); }

  /**
   * @brief Get bottom right corner of the rect
   *
   * @returns Bottom right corner of the rect
   *
   */
  constexpr Point GetBottomRight() const { return Point(GetX2(), GetY2()); }

  /**
   * @brief Get size of the rect
   *
   * @returns Size of the rect
   *
   */
  constexpr Point GetSize() const { return Point(w, h); }

  /**
   * @brief Get centroid of the rect
   *
   * @returns Centroid of the rect
   *
   */
  constexpr Point GetCentroid() const { return Point(x + w / 2, y + h / 2); }

  /**
   * @brief Check whether the rect contains given point
   *
   * @param[in] px X coordinate of a point
   * @param[in] py Y coordinate of a point
   *
   * @returns True if the point is contained in the rect
   *
   */
  constexpr bool Contains(int px, int py) const
  {
    return px >= x && py >= y && px <= GetX2() && py <= GetY2();
  }

  /**
   * @brief Check whether the rect contains given point
   *
   * @param[in] point Point to check
   *
   * @returns True if the point is contained in the rect
   *
   */
  constexpr bool Contains(const Point& point) const
  {
    return point.x >= x && point.y >= y && point.x <= GetX2() &&
           point.y <= GetY2();
  }

  /**
   * @brief Check whether the rect contains another rect
   *
   * @param[in] rect Rect to check
   *
   * @returns True if the checked rect is contained in this rect
   *
   */
  constexpr bool Contains(const Rect& rect) const
  {
    return rect.x >= x && rect.y >= y && rect.GetX2() <= GetX2() &&
           rect.GetY2() <= GetY2();
  }

  /**
   * @brief Check whether the rect intersects another rect
   *
   * @param[in] rect Rect to check
   *
   * @returns True if rectangles intersect
   *
   */
  constexpr bool Intersects(const Rect& rect) const
  {
    return !(rect.GetX2() < x || rect.GetY2() < y || rect.x > GetX2() ||
             rect.y > GetY2());
  }

  /**
   * @brief Calculate union with another rect
   *
   * @param[in] rect Rect to union with
   *
   * @returns Rect representing union of two rectangles
   *
   */
  constexpr Rect GetUnion(const Rect& rect) const
  {
    return Rect::FromCorners(std::min(x, rect.x),
                             std::min(y, rect.y),
                             std::max(GetX2(), rect.GetX2()),
                             std::max(GetY2(), rect.GetY2()));
  }

  /**
   * @brief Union rect with another rect
   *
   * @param[in] rect Rect to union with
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& Union(const Rect& rect)
  {
    int nx = std::min(x, rect.x);
    int ny = std::min(y, rect.y);
    int nx2 = std::max(GetX2(), rect.GetX2());
    int ny2 = std::max(GetY2(), rect.GetY2());
    x = nx;
    y = ny;
    SetX2(nx2);
    SetY2(ny2);
    return *this;
  }

  /**
   * @brief Get a rect extended by specified amount of pixels
   *
   * @param[in] amount Number of pixels to extend by
   *
   * @returns Extended rect
   *
   */
  constexpr Rect GetExtension(unsigned int amount) const
  {
    Rect r = *this;
    r.Extend(amount);
    return r;
  }

  /**
   * @brief Get a rect extended by specified amount of pixels
   *
   * @param[in] hAmount Number of pixels to extend by
   *                    in horizontal direction
   * @param[in] vAmount Number of pixels to extend by
   *                    in vertical direction
   *
   * @returns Extended rect
   *
   */
  constexpr Rect GetExtension(unsigned int hAmount, unsigned int vAmount) const
  {
    Rect r = *this;
    r.Extend(hAmount, vAmount);
    return r;
  }

  /**
   * @brief Extend a rect by specified amount of pixels
   *
   * @param[in] amount Number of pixels to extend by
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& Extend(unsigned int amount) { return Extend(amount, amount); }

  /**
   * @brief Extend a rect by specified amount of pixels
   *
   * @param[in] hAmount Number of pixels to extend by
   *                    in horizontal direction
   * @param[in] vAmount Number of pixels to extend by
   *                    in vertical direction
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& Extend(unsigned int hAmount, unsigned int vAmount)
  {
    x -= hAmount;
    y -= vAmount;
    w += hAmount * 2;
    h += vAmount * 2;
    return *this;
  }

  /**
   * @brief Calculate intersection with another rect
   *
   * @param[in] rect Rect to intersect with
   *
   * @returns Rect representing intersection area or NullOpt if there was no
   * intersection
   *
   */
  constexpr std::optional<Rect> GetIntersection(const Rect& rect) const
  {
    if (!Intersects(rect)) return std::nullopt;

    return Rect::FromCorners(std::max(x, rect.x),
                             std::max(y, rect.y),
                             std::min(GetX2(), rect.GetX2()),
                             std::min(GetY2(), rect.GetY2()));
  }

  /**
   * @brief Calculate the intersection of a rectangle and line segment
   *
   * @param[in,out] x1 Starting X-coordinate of the line
   * @param[in,out] y1 Starting Y-coordinate of the line
   * @param[in,out] x2 Ending X-coordinate of the line
   * @param[in,out] y2 Ending Y-coordinate of the line
   *
   * @returns True if there is an intersection, false otherwise
   *
   * This function is used to clip a line segment to a
   * rectangle. A line segment contained entirely within the
   * rectangle or that does not intersect will remain unchanged.
   * A line segment that crosses the rectangle at either or both
   * ends will be clipped to the boundary of the rectangle and
   * the new coordinates saved in x1, y1, x2, and/or y2 as
   * necessary.
   *
   */
  bool IntersectLine(int& x1, int& y1, int& x2, int& y2) const
  {
    return SDL_GetRectAndLineIntersection(this, &x1, &y1, &x2, &y2);
  }

  /**
   * @brief Calculate the intersection of a rectangle and line segment
   *
   * @param[in,out] p1 Starting coordinates of the line
   * @param[in,out] p2 Ending coordinates of the line
   *
   * @returns True if there is an intersection, false otherwise
   *
   * This function is used to clip a line segment to a
   * rectangle. A line segment contained entirely within the
   * rectangle or that does not intersect will remain unchanged.
   * A line segment that crosses the rectangle at either or both
   * ends will be clipped to the boundary of the rectangle and
   * the new coordinates saved in p1 and/or p2 as necessary.
   *
   */
  bool IntersectLine(Point& p1, Point& p2) const
  {
    return IntersectLine(p1.x, p1.y, p2.x, p2.y);
  }

  /**
   * @brief Get rectangle moved by a given offset
   *
   * @param[in] offset Point specifying an offset
   *
   * @returns Moved rectangle
   *
   */
  constexpr Rect operator+(const Point& offset) const
  {
    return Rect(x + offset.x, y + offset.y, w, h);
  }

  /**
   * @brief Get rectangle moved by an opposite of given offset
   *
   * @param[in] offset Point specifying an offset
   *
   * @returns Moved rectangle
   *
   */
  constexpr Rect operator-(const Point& offset) const
  {
    return Rect(x - offset.x, y - offset.y, w, h);
  }

  /**
   * @brief Move by then given offset
   *
   * @param[in] offset Point specifying an offset
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& operator+=(const Point& offset)
  {
    x += offset.x;
    y += offset.y;
    return *this;
  }

  /**
   * @brief Move by an opposite of the given offset
   *
   * @param[in] offset Point specifying an offset
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& operator-=(const Point& offset)
  {
    x -= offset.x;
    y -= offset.y;
    return *this;
  }

  // Auto comparison operator
  auto operator<=>(const Rect&) const = default;

  /**
   * @brief Converts to FRect
   *
   * @return FRect
   */
  constexpr operator FRect() const;

  constexpr bool IsEmpty() const { return w <= 0 || h <= 0; }

  constexpr operator bool() const { return IsEmpty(); }
};

/**
 * @brief A rectangle, with the origin at the upper left (using floats).
 */
struct FRect : SDL_FRect
{
  constexpr FRect(const SDL_FRect& rect)
    : SDL_FRect(rect)
  {
  }

  constexpr FRect()
    : FRect({0})
  {
  }

  constexpr FRect(float x, float y, float w, float h)
    : SDL_FRect({x, y, w, h})
  {
  }

  constexpr FRect(const SDL_FPoint& corner, const SDL_FPoint& size)
    : FRect{corner.x, corner.y, size.x, size.y}
  {
  }

  static FRect GetEnclosingPoints(std::span<const SDL_FPoint> points)
  {
    return GetEnclosingPoints(points.data(), points.size());
  }

  static FRect GetEnclosingPoints(std::span<const SDL_FPoint> points,
                                  const SDL_FRect& clip)
  {
    return GetEnclosingPoints(points.data(), points.size(), clip);
  }

  static FRect GetEnclosingPoints(const SDL_FPoint* points, int count)
  {
    FRect result;
    SDL_GetRectEnclosingPointsFloat(points, count, nullptr, &result);
    return result;
  }
  static FRect GetEnclosingPoints(const SDL_FPoint* points,
                                  int count,
                                  const SDL_FRect& clip)
  {
    FRect result;
    SDL_GetRectEnclosingPointsFloat(points, count, &clip, &result);
    return result;
  }

  /**
   * @brief Construct the rect from given center coordinates, width and height
   *
   * @param[in] cx X coordinate of the rectangle center
   * @param[in] cy Y coordinate of the rectangle center
   * @param[in] w Width of the rectangle
   * @param[in] h Height of the rectangle
   *
   */
  static constexpr FRect FromCenter(float cx, float cy, float w, float h)
  {
    return FRect(cx - w / 2, cy - h / 2, w, h);
  }

  /**
   * @brief Construct the rect from given center coordinates and size
   *
   * @param[in] center Coordinates of the rectangle center
   * @param[in] size Dimensions of the rectangle
   *
   */
  static constexpr FRect FromCenter(const FPoint& center, const FPoint& size)
  {
    return FRect(center - size / 2, size);
  }

  /**
   * @brief Construct the rect from given corners coordinates
   *
   * @param[in] x1 X coordinate of the top left rectangle corner
   * @param[in] y1 Y coordinate of the top left rectangle corner
   * @param[in] x2 X coordinate of the bottom right rectangle corner
   * @param[in] y2 Y coordinate of the bottom right rectangle corner
   *
   */
  static constexpr FRect FromCorners(float x1, float y1, float x2, float y2)
  {
    return FRect(x1, y1, x2 - x1 + 1, y2 - y1 + 1);
  }

  /**
   * @brief Construct the rect from given centers coordinates
   *
   * @param[in] p1 Coordinates of the top left rectangle corner
   * @param[in] p2 Coordinates of the bottom right rectangle corner
   *
   */
  static constexpr FRect FromCorners(const FPoint& p1, const FPoint& p2)
  {
    return FRect(p1, p2 - p1 + FPoint(1, 1));
  }

  /**
   * @brief Get X coordinate of the rect corner
   *
   * @returns X coordinate of the rect corner
   *
   */
  constexpr float GetX() const { return x; }

  /**
   * @brief Set X coordinate of the rect corner
   *
   * @param[in] nx New X coordinate value
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& SetX(float nx)
  {
    x = nx;
    return *this;
  }

  /**
   * @brief Get Y coordinate of the rect corner
   *
   * @returns Y coordinate of the rect corner
   *
   */
  constexpr float GetY() const { return y; }

  /**
   * @brief Set Y coordinate of the rect corner
   *
   * @param[in] ny New Y coordinate value
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& SetY(float ny)
  {
    y = ny;
    return *this;
  }

  /**
   * @brief Get width of the rect
   *
   * @returns Width of the rect
   *
   */
  constexpr float GetW() const { return w; }

  /**
   * @brief Set width of the rect
   *
   * @param[in] nw New width of the rect
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& SetW(float nw)
  {
    w = nw;
    return *this;
  }

  /**
   * @brief Get height of the rect
   *
   * @returns Height of the rect
   *
   */
  constexpr float GetH() const { return h; }

  /**
   * @brief Set height of the rect
   *
   * @param[in] nh New height of the rect
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& SetH(float nh)
  {
    h = nh;
    return *this;
  }

  /**
   * @brief Get X coordinate of the rect second corner
   *
   * @returns X coordinate of the rect second corner
   *
   */
  constexpr float GetX2() const { return x + w - 1; }

  /**
   * @brief Set X coordinate of the rect second corner
   *
   * @param[in] x2 New X coordinate value
   *
   * This modifies rectangle width internally
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& SetX2(float x2)
  {
    w = x2 - x + 1;
    return *this;
  }

  /**
   * @brief Get Y coordinate of the rect second corner
   *
   * @returns Y coordinate of the rect second corner
   *
   */
  constexpr float GetY2() const { return y + h - 1; }

  /**
   * @brief Set Y coordinate of the rect second corner
   *
   * @param[in] y2 New Y coordinate value
   *
   * This modifies rectangle height internally
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& SetY2(float y2)
  {
    h = y2 - y + 1;
    return *this;
  }

  /**
   * @brief Get top left corner of the rect
   *
   * @returns Top left corner of the rect
   *
   */
  constexpr FPoint GetTopLeft() const { return FPoint(x, y); }

  /**
   * @brief Get top right corner of the rect
   *
   * @returns Top right corner of the rect
   *
   */
  constexpr FPoint GetTopRight() const { return FPoint(GetX2(), y); }

  /**
   * @brief Get bottom left corner of the rect
   *
   * @returns bottom left corner of the rect
   *
   */
  constexpr FPoint GetBottomLeft() const { return FPoint(x, GetY2()); }

  /**
   * @brief Get bottom right corner of the rect
   *
   * @returns Bottom right corner of the rect
   *
   */
  constexpr FPoint GetBottomRight() const { return FPoint(GetX2(), GetY2()); }

  /**
   * @brief Get size of the rect
   *
   * @returns Size of the rect
   *
   */
  constexpr FPoint GetSize() const { return FPoint(w, h); }

  /**
   * @brief Get centroid of the rect
   *
   * @returns Centroid of the rect
   *
   */
  constexpr FPoint GetCentroid() const { return FPoint(x + w / 2, y + h / 2); }

  /**
   * @brief Check whether the rect contains given point
   *
   * @param[in] px X coordinate of a point
   * @param[in] py Y coordinate of a point
   *
   * @returns True if the point is contained in the rect
   *
   */
  constexpr bool Contains(float px, float py) const
  {
    return px >= x && py >= y && px <= GetX2() && py <= GetY2();
  }

  /**
   * @brief Check whether the rect contains given point
   *
   * @param[in] point Point to check
   *
   * @returns True if the point is contained in the rect
   *
   */
  constexpr bool Contains(const FPoint& point) const
  {
    return point.x >= x && point.y >= y && point.x <= GetX2() &&
           point.y <= GetY2();
  }

  /**
   * @brief Check whether the rect contains another rect
   *
   * @param[in] rect FRect to check
   *
   * @returns True if the checked rect is contained in this rect
   *
   */
  constexpr bool Contains(const FRect& rect) const
  {
    return rect.x >= x && rect.y >= y && rect.GetX2() <= GetX2() &&
           rect.GetY2() <= GetY2();
  }

  /**
   * @brief Check whether the rect intersects another rect
   *
   * @param[in] rect FRect to check
   *
   * @returns True if rectangles intersect
   *
   */
  constexpr bool Intersects(const FRect& rect) const
  {
    return !(rect.GetX2() < x || rect.GetY2() < y || rect.x > GetX2() ||
             rect.y > GetY2());
  }

  /**
   * @brief Calculate union with another rect
   *
   * @param[in] rect FRect to union with
   *
   * @returns FRect representing union of two rectangles
   *
   */
  constexpr FRect GetUnion(const FRect& rect) const
  {
    return FRect::FromCorners(std::min(x, rect.x),
                              std::min(y, rect.y),
                              std::max(GetX2(), rect.GetX2()),
                              std::max(GetY2(), rect.GetY2()));
  }

  /**
   * @brief Union rect with another rect
   *
   * @param[in] rect FRect to union with
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& Union(const FRect& rect)
  {
    float nx = std::min(x, rect.x);
    float ny = std::min(y, rect.y);
    float nx2 = std::max(GetX2(), rect.GetX2());
    float ny2 = std::max(GetY2(), rect.GetY2());
    x = nx;
    y = ny;
    SetX2(nx2);
    SetY2(ny2);
    return *this;
  }

  /**
   * @brief Get a rect extended by specified amount of pixels
   *
   * @param[in] amount Number of pixels to extend by
   *
   * @returns Extended rect
   *
   */
  constexpr FRect GetExtension(float amount) const
  {
    FRect r = *this;
    r.Extend(amount);
    return r;
  }

  /**
   * @brief Get a rect extended by specified amount of pixels
   *
   * @param[in] hAmount Number of pixels to extend by
   *                    in horizontal direction
   * @param[in] vAmount Number of pixels to extend by
   *                    in vertical direction
   *
   * @returns Extended rect
   *
   */
  constexpr FRect GetExtension(float hAmount, float vAmount) const
  {
    FRect r = *this;
    r.Extend(hAmount, vAmount);
    return r;
  }

  /**
   * @brief Extend a rect by specified amount of pixels
   *
   * @param[in] amount Number of pixels to extend by
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& Extend(float amount) { return Extend(amount, amount); }

  /**
   * @brief Extend a rect by specified amount of pixels
   *
   * @param[in] hAmount Number of pixels to extend by
   *                    in horizontal direction
   * @param[in] vAmount Number of pixels to extend by
   *                    in vertical direction
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& Extend(float hAmount, float vAmount)
  {
    x -= hAmount;
    y -= vAmount;
    w += hAmount * 2;
    h += vAmount * 2;
    return *this;
  }

  /**
   * @brief Calculate intersection with another rect
   *
   * @param[in] rect FRect to intersect with
   *
   * @returns FRect representing intersection area or NullOpt if there was no
   * intersection
   *
   */
  constexpr std::optional<FRect> GetIntersection(const FRect& rect) const
  {
    if (!Intersects(rect)) return std::nullopt;

    return FRect::FromCorners(std::max(x, rect.x),
                              std::max(y, rect.y),
                              std::min(GetX2(), rect.GetX2()),
                              std::min(GetY2(), rect.GetY2()));
  }

  /**
   * @brief Calculate the intersection of a rectangle and line segment
   *
   * @param[in,out] x1 Starting X-coordinate of the line
   * @param[in,out] y1 Starting Y-coordinate of the line
   * @param[in,out] x2 Ending X-coordinate of the line
   * @param[in,out] y2 Ending Y-coordinate of the line
   *
   * @returns True if there is an intersection, false otherwise
   *
   * This function is used to clip a line segment to a
   * rectangle. A line segment contained entirely within the
   * rectangle or that does not intersect will remain unchanged.
   * A line segment that crosses the rectangle at either or both
   * ends will be clipped to the boundary of the rectangle and
   * the new coordinates saved in x1, y1, x2, and/or y2 as
   * necessary.
   *
   */
  bool IntersectLine(float& x1, float& y1, float& x2, float& y2) const
  {
    return SDL_GetRectAndLineIntersectionFloat(this, &x1, &y1, &x2, &y2);
  }

  /**
   * @brief Calculate the intersection of a rectangle and line segment
   *
   * @param[in,out] p1 Starting coordinates of the line
   * @param[in,out] p2 Ending coordinates of the line
   *
   * @returns True if there is an intersection, false otherwise
   *
   * This function is used to clip a line segment to a
   * rectangle. A line segment contained entirely within the
   * rectangle or that does not intersect will remain unchanged.
   * A line segment that crosses the rectangle at either or both
   * ends will be clipped to the boundary of the rectangle and
   * the new coordinates saved in p1 and/or p2 as necessary.
   *
   */
  bool IntersectLine(FPoint& p1, FPoint& p2) const
  {
    return IntersectLine(p1.x, p1.y, p2.x, p2.y);
  }

  /**
   * @brief Get rectangle moved by a given offset
   *
   * @param[in] offset Point specifying an offset
   *
   * @returns Moved rectangle
   *
   */
  constexpr FRect operator+(const FPoint& offset) const
  {
    return FRect(x + offset.x, y + offset.y, w, h);
  }

  /**
   * @brief Get rectangle moved by an opposite of given offset
   *
   * @param[in] offset Point specifying an offset
   *
   * @returns Moved rectangle
   *
   */
  constexpr FRect operator-(const FPoint& offset) const
  {
    return FRect(x - offset.x, y - offset.y, w, h);
  }

  /**
   * @brief Move by then given offset
   *
   * @param[in] offset Point specifying an offset
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& operator+=(const FPoint& offset)
  {
    x += offset.x;
    y += offset.y;
    return *this;
  }

  /**
   * @brief Move by an opposite of the given offset
   *
   * @param[in] offset Point specifying an offset
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& operator-=(const FPoint& offset)
  {
    x -= offset.x;
    y -= offset.y;
    return *this;
  }

  // Auto comparison operator
  auto operator<=>(const FRect&) const = default;

  constexpr bool IsEmpty() const { return w <= 0 || h <= 0; }

  constexpr operator bool() const { return IsEmpty(); }
};

constexpr bool Point::IsInRect(const Rect& r) const
{
  return r.Contains(*this);
}

constexpr Point::operator FPoint() const { return {float(x), float(y)}; }

constexpr Point Point::GetClamped(const Rect& rect) const
{
  Point p = *this;
  p.Clamp(rect);
  return p;
}

constexpr Point& Point::Clamp(const Rect& rect)
{
  if (x < rect.x) x = rect.x;
  if (x > rect.GetX2()) x = rect.GetX2();
  if (y < rect.y) y = rect.y;
  if (y > rect.GetY2()) y = rect.GetY2();
  return *this;
}

constexpr Point Point::GetWrapped(const Rect& rect) const
{
  Point p = *this;
  p.Wrap(rect);
  return p;
}

constexpr Point& Point::Wrap(const Rect& rect)
{
  if (x < rect.x)
    x = rect.x + rect.w - 1 - (rect.x - x + rect.w - 1) % rect.w;
  else if (x >= rect.x + rect.w)
    x = rect.x + (x - rect.x - rect.w) % rect.w;

  if (y < rect.y)
    y = rect.y + rect.h - 1 - (rect.y - y + rect.h - 1) % rect.h;
  else if (y >= rect.y + rect.h)
    y = rect.y + (y - rect.y - rect.h) % rect.h;

  return *this;
}

constexpr bool FPoint::IsInRect(const FRect& r) const
{
  return r.Contains(*this);
}

constexpr FPoint FPoint::GetClamped(const FRect& rect) const
{
  FPoint p = *this;
  p.Clamp(rect);
  return p;
}

constexpr FPoint& FPoint::Clamp(const FRect& rect)
{
  if (x < rect.x) x = rect.x;
  if (x > rect.GetX2()) x = rect.GetX2();
  if (y < rect.y) y = rect.y;
  if (y > rect.GetY2()) y = rect.GetY2();
  return *this;
}

constexpr FPoint FPoint::GetWrapped(const FRect& rect) const
{
  FPoint p = *this;
  p.Wrap(rect);
  return p;
}

constexpr FPoint& FPoint::Wrap(const FRect& rect)
{
  if (x < rect.x)
    x = rect.x + rect.w - 1 - fmod(rect.x - x + rect.w - 1, rect.w);
  else if (x >= rect.x + rect.w)
    x = rect.x + fmod(x - rect.x - rect.w, rect.w);

  if (y < rect.y)
    y = rect.y + rect.h - 1 - fmod(rect.y - y + rect.h - 1, rect.h);
  else if (y >= rect.y + rect.h)
    y = rect.y + fmod(y - rect.y - rect.h, rect.h);

  return *this;
}

constexpr Rect::operator FRect() const
{
  return {float(x), float(y), float(w), float(h)};
}

} // namespace SDL

#endif /* SDL3PP_RECT_HPP_ */
