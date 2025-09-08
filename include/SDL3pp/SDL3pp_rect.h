#ifndef SDL3PP_RECT_H_
#define SDL3PP_RECT_H_

#include <SDL3/SDL_rect.h>
#include "SDL3pp_error.h"
#include "SDL3pp_optionalRef.h"
#include "SDL3pp_spanRef.h"

namespace SDL {

/**
 * @defgroup CategoryRect Rectangle Functions
 *
 * Some helper functions for managing rectangles and 2D points, in both
 * integer and floating point versions.
 *
 * @{
 */

// Forward decl
struct FPoint;

// Forward decl
struct Rect;

// Forward decl
struct FRect;

/**
 * The structure that defines a point (using integers).
 *
 * Inspired by
 * https://github.com/libSDL2pp/libSDL2pp/blob/master/SDL2pp/Point.hh
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat wrap-extending-struct
 *
 * @sa Rect.GetEnclosingPoints
 * @sa Point.InRect
 */
struct Point : SDL_Point
{
  /**
   * Wraps Point.
   *
   * @param p the value to be wrapped
   */
  constexpr Point(const SDL_Point& p = {})
    : SDL_Point(p)
  {
  }

  /**
   * Constructs from its fields.
   *
   * @param x the value for x.
   * @param y the value for y.
   */
  constexpr Point(int x, int y)
    : SDL_Point{x, y}
  {
  }

  /**
   * Wraps Point.
   *
   * @param p the value to be wrapped
   */
  constexpr explicit Point(const SDL_FPoint& p)
    : SDL_Point{int(p.x), int(p.y)}
  {
  }

  /**
   * Default comparison operator
   */
  constexpr bool operator==(const Point& other) const
  {
    return x == other.x && y == other.y;
  }

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const SDL_Point& other) const
  {
    return x == other.x && y == other.y;
  }

  /**
   * Check if valid.
   *
   * @returns True if valid state, false otherwise.
   */
  constexpr explicit operator bool() const { return *this != Point{}; }

  /**
   * Get x coordinate
   *
   * @returns x coordinate
   */
  constexpr int GetX() const { return x; }

  /**
   * Set the x coordinate.
   *
   * @param newX the new x coordinate.
   * @returns Reference to self.
   */
  constexpr Point& SetX(int newX)
  {
    x = newX;
    return *this;
  }

  /**
   * Get y coordinate
   *
   * @returns y coordinate
   */
  constexpr int GetY() const { return y; }

  /**
   * Set the y coordinate.
   *
   * @param newY the new y coordinate.
   * @returns Reference to self.
   */
  constexpr Point& SetY(int newY)
  {
    y = newY;
    return *this;
  }

  /**
   * Determine whether a point resides inside a rectangle.
   *
   * A point is considered part of a rectangle if both `p` and `r` are not
   * nullptr, and `p`'s x and y coordinates are >= to the rectangle's top left
   * corner, and < the rectangle's x+w and y+h. So a 1x1 rectangle considers
   * point (0,0) as "inside" and (0,1) as not.
   *
   * Note that this is a forced-inline function in a header, and not a public
   * API function available in the SDL library (which is to say, the code is
   * embedded in the calling program and the linker and dynamic loader will not
   * be able to find this function inside SDL itself).
   *
   * @param r the rectangle to test.
   * @returns true if this is contained by `r`, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool InRect(const SDL_Rect& r) const
  {
    return SDL_PointInRect(this, &r);
  }

  /**
   * Get point's memberwise negation
   *
   * @returns New Point representing memberwise negation
   *
   */
  constexpr Point operator-() const { return Point(-x, -y); }

  /**
   * Get point's memberwise addition with another point
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
   * Get point's memberwise subtraction with another point
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
   * Get point's memberwise division by an integer
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
   * Get point's memberwise division by an integer
   *
   * @param[in] value Divisor
   *
   * @returns New Point representing memberwise division of
   *          point by an integer
   *
   */
  constexpr FPoint operator/(float value) const;

  /**
   * Get point's memberwise division by another point
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
   * Get point's memberwise remainder from division
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
   * Get point's memberwise remainder from division
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
   * Get point's memberwise multiplication by an
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
   * Get point's memberwise multiplication by an
   *        integer
   *
   * @param[in] value Multiplier
   *
   * @returns New Point representing memberwise multiplication
   *          of point by an integer
   *
   */
  constexpr FPoint operator*(float value) const;

  /**
   * Get point's memberwise multiplication by another
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
   * Memberwise add another point
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
   * Memberwise subtract another point
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
   * Memberwise divide by an integer
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
   * Memberwise divide by another point
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
   * Memberwise remainder from division by an integer
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
   * Memberwise remainder from division by another
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
   * Memberwise multiply by an integer
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
   * Memberwise multiply by another point
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
   * Get a point with coordinates modified so it fits
   *        into a given rect
   *
   * @param[in] rect Rectangle to clamp with
   *
   * @returns Clamped point
   *
   */
  constexpr Point GetClamped(const Rect& rect) const;

  /**
   * Clamp point coordinates to make it fit into a
   *        given rect
   *
   * @param[in] rect Rectangle to clamp with
   *
   * @returns Reference to self
   *
   */
  constexpr Point& Clamp(const Rect& rect);

  /**
   * Get a point wrapped within a specified rect
   *
   * @param[in] rect Rectangle to wrap with
   *
   * @returns Wrapped point
   *
   */
  constexpr Point GetWrapped(const Rect& rect) const;

  /**
   * Wrap point coordinates within a specified rect
   *
   * @param[in] rect Rectangle to wrap with
   *
   * @returns Reference to self
   *
   */
  constexpr Point& Wrap(const Rect& rect);

  /**
   * Converts to FPoint
   *
   * @return FPoint
   */
  constexpr operator FPoint() const;
};

/**
 * The structure that defines a point (using floating point values).
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat wrap-extending-struct
 *
 * @sa FRect.GetEnclosingPoints
 * @sa FPoint.InRect
 */
struct FPoint : SDL_FPoint
{
  /**
   * Wraps FPoint.
   *
   * @param p the value to be wrapped
   */
  constexpr FPoint(const SDL_FPoint& p = {})
    : SDL_FPoint(p)
  {
  }

  /**
   * Constructs from its fields.
   *
   * @param x the value for x.
   * @param y the value for y.
   */
  constexpr FPoint(float x, float y)
    : SDL_FPoint{x, y}
  {
  }

  /**
   * Default comparison operator
   */
  constexpr bool operator==(const FPoint& other) const
  {
    return x == other.x && y == other.y;
  }

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const SDL_FPoint& other) const
  {
    return x == other.x && y == other.y;
  }

  /**
   * Check if valid.
   *
   * @returns True if valid state, false otherwise.
   */
  constexpr explicit operator bool() const { return *this != FPoint{}; }

  /**
   * Get the x coordinate.
   *
   * @returns current x value.
   */
  constexpr float GetX() const { return x; }

  /**
   * Set the x coordinate.
   *
   * @param newX the new x coordinate.
   * @returns Reference to self.
   */
  constexpr FPoint& SetX(float newX)
  {
    x = newX;
    return *this;
  }

  /**
   * Get the y coordinate.
   *
   * @returns current y coordinate.
   */
  constexpr float GetY() const { return y; }

  /**
   * Set the y coordinate.
   *
   * @param newY the new y coordinate.
   * @returns Reference to self.
   */
  constexpr FPoint& SetY(float newY)
  {
    y = newY;
    return *this;
  }

  /**
   * Determine whether a point resides inside a floating point rectangle.
   *
   * A point is considered part of a rectangle if both `p` and `r` are not
   * nullptr, and `p`'s x and y coordinates are >= to the rectangle's top left
   * corner, and <= the rectangle's x+w and y+h. So a 1x1 rectangle considers
   * point (0,0) and (0,1) as "inside" and (0,2) as not.
   *
   * Note that this is a forced-inline function in a header, and not a public
   * API function available in the SDL library (which is to say, the code is
   * embedded in the calling program and the linker and dynamic loader will not
   * be able to find this function inside SDL itself).
   *
   * @param r the rectangle to test.
   * @returns true if this is contained by `r`, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool InRect(const SDL_FRect& r) const;

  /**
   * Get point's memberwise negation
   *
   * @returns New Point representing memberwise negation
   *
   */
  constexpr FPoint operator-() const { return FPoint(-x, -y); }

  /**
   * Get point's memberwise addition with another point
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
   * Get point's memberwise subtraction with another point
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
   * Get point's memberwise division by an float
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
   * Get point's memberwise division by another point
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
   * Get point's memberwise multiplication by an
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
   * Get point's memberwise multiplication by another
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
   * Memberwise add another point
   *
   * @param[in] other Point to add to the current one
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& operator+=(const FPoint& other)
  {
    x += other.x;
    y += other.y;
    return *this;
  }

  /**
   * Memberwise subtract another point
   *
   * @param[in] other Point to subtract from the current one
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& operator-=(const FPoint& other)
  {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  /**
   * Memberwise divide by an float
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
   * Memberwise divide by another point
   *
   * @param[in] other Divisor
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& operator/=(const FPoint& other)
  {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  /**
   * Memberwise multiply by an float
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
   * Memberwise multiply by another point
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
   * Get a point with coordinates modified so it fits
   *        into a given rect
   *
   * @param[in] rect Rectangle to clamp with
   *
   * @returns Clamped point
   *
   */
  constexpr FPoint GetClamped(const FRect& rect) const;

  /**
   * Clamp point coordinates to make it fit into a
   *        given rect
   *
   * @param[in] rect Rectangle to clamp with
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& Clamp(const FRect& rect);

  /**
   * Get a point wrapped within a specified rect
   *
   * @param[in] rect Rectangle to wrap with
   *
   * @returns Wrapped point
   *
   */
  constexpr FPoint GetWrapped(const FRect& rect) const;

  /**
   * Wrap point coordinates within a specified rect
   *
   * @param[in] rect Rectangle to wrap with
   *
   * @returns Reference to self
   *
   */
  constexpr FPoint& Wrap(const FRect& rect);
};

/**
 * A rectangle, with the origin at the upper left (using integers).
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat wrap-extending-struct
 *
 * @sa Rect.Empty
 * @sa Rect.Equal
 * @sa Rect.HasIntersection
 * @sa Rect.GetIntersection
 * @sa Rect.GetLineIntersection
 * @sa Rect.GetUnion
 * @sa Rect.GetEnclosingPoints
 */
struct Rect : SDL_Rect
{
  /**
   * Wraps Rect.
   *
   * @param r the value to be wrapped
   */
  constexpr Rect(const SDL_Rect& r = {})
    : SDL_Rect(r)
  {
  }

  /**
   * Constructs from its fields.
   *
   * @param x the left x.
   * @param y the top y.
   * @param w the width.
   * @param h the height.
   */
  constexpr Rect(int x, int y, int w, int h)
    : SDL_Rect{x, y, w, h}
  {
  }

  /**
   * Construct from offset and size
   *
   * @param corner the top-left corner
   * @param size the size
   */
  constexpr Rect(const SDL_Point& corner, const SDL_Point& size)
    : Rect{corner.x, corner.y, size.x, size.y}
  {
  }

  /**
   * Default comparison operator
   */
  constexpr bool operator==(const Rect& other) const { return Equal(other); }

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const SDL_Rect& r) const
  {
    return operator==(Rect(r));
  }

  /**
   * @sa Empty()
   */
  constexpr explicit operator bool() const { return !Empty(); }

  /**
   * Get left x coordinate.
   *
   * @returns coordinate of the left x
   */
  constexpr int GetX() const { return x; }

  /**
   * Set the left x coordinate.
   *
   * @param newX the new left x.
   * @returns Reference to self.
   */
  constexpr Rect& SetX(int newX)
  {
    x = newX;
    return *this;
  }

  /**
   * Get top y coordinate.
   *
   * @returns coordinate of the top y.
   */
  constexpr int GetY() const { return y; }

  /**
   * Set the top y coordinate.
   *
   * @param newY the new top y.
   * @returns Reference to self.
   */
  constexpr Rect& SetY(int newY)
  {
    y = newY;
    return *this;
  }

  /**
   * Get width of the rect
   *
   * @returns Width of the rect
   */
  constexpr int GetW() const { return w; }

  /**
   * Set the width of the rect.
   *
   * @param newW the new width.
   * @returns Reference to self.
   */
  constexpr Rect& SetW(int newW)
  {
    w = newW;
    return *this;
  }

  /**
   * Get height of the rect
   *
   * @returns Height of the rect
   */
  constexpr int GetH() const { return h; }

  /**
   * Set the height of the rect.
   *
   * @param newH the new height.
   * @returns Reference to self.
   */
  constexpr Rect& SetH(int newH)
  {
    h = newH;
    return *this;
  }

  /**
   * Calculate a minimal rectangle enclosing a set of points.
   *
   * If `clip` is not nullptr then only points inside of the clipping rectangle
   * are considered.
   *
   * @param points a span of SDL_Point structures representing points to be
   *               enclosed.
   * @param clip an SDL_Rect used for clipping or std::nullopt to enclose all
   *             points.
   * @returns a SDL_Rect structure filled in with the minimal enclosing
   *          rectangle or an empty rect if all the points were outside of the
   *          clipping rectangle.
   *
   * @since This function is available since SDL 3.2.0.
   */
  static Rect GetEnclosingPoints(
    SpanRef<const SDL_Point> points,
    OptionalRef<const SDL_Rect> clip = std::nullopt)
  {
    Rect result;
    if (SDL_GetRectEnclosingPoints(
          points.data(), points.size(), clip, &result)) {
      return result;
    }
    return {};
  }

  /**
   * Construct the rect from given center coordinates, width and height
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
   * Construct the rect from given center coordinates and size
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
   * Construct the rect from given corners coordinates
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
   * Construct the rect from given centers coordinates
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
   * Get X coordinate of the rect second corner
   *
   * @returns X coordinate of the rect second corner
   *
   */
  constexpr int GetX2() const { return x + w - 1; }

  /**
   * Set X coordinate of the rect second corner
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
   * Get Y coordinate of the rect second corner
   *
   * @returns Y coordinate of the rect second corner
   *
   */
  constexpr int GetY2() const { return y + h - 1; }

  /**
   * Set Y coordinate of the rect second corner
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
   * Get top left corner of the rect
   *
   * @returns Top left corner of the rect
   *
   */
  constexpr Point GetTopLeft() const { return Point(x, y); }

  /**
   * Get top right corner of the rect
   *
   * @returns Top right corner of the rect
   *
   */
  constexpr Point GetTopRight() const { return Point(GetX2(), y); }

  /**
   * Get bottom left corner of the rect
   *
   * @returns bottom left corner of the rect
   *
   */
  constexpr Point GetBottomLeft() const { return Point(x, GetY2()); }

  /**
   * Get bottom right corner of the rect
   *
   * @returns Bottom right corner of the rect
   *
   */
  constexpr Point GetBottomRight() const { return Point(GetX2(), GetY2()); }

  /**
   * Get size of the rect
   *
   * @returns Size of the rect
   *
   */
  constexpr Point GetSize() const { return Point(w, h); }

  /**
   * Get centroid of the rect
   *
   * @returns Centroid of the rect
   *
   */
  constexpr Point GetCentroid() const { return Point(x + w / 2, y + h / 2); }

  /**
   * Calculate the intersection of a rectangle and line segment
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
  bool GetLineIntersection(SDL_Point* p1, SDL_Point* p2) const
  {
    return GetLineIntersection(p1 ? &p1->x : nullptr,
                               p1 ? &p1->y : nullptr,
                               p2 ? &p2->x : nullptr,
                               p2 ? &p2->y : nullptr);
  }

  /**
   * Calculate the intersection of a rectangle and line segment.
   *
   * This function is used to clip a line segment to a rectangle. A line segment
   * contained entirely within the rectangle or that does not intersect will
   * remain unchanged. A line segment that crosses the rectangle at either or
   * both ends will be clipped to the boundary of the rectangle and the new
   * coordinates saved in `X1`, `Y1`, `X2`, and/or `Y2` as necessary.
   *
   * @param X1 a pointer to the starting X-coordinate of the line.
   * @param Y1 a pointer to the starting Y-coordinate of the line.
   * @param X2 a pointer to the ending X-coordinate of the line.
   * @param Y2 a pointer to the ending Y-coordinate of the line.
   * @returns true if there is an intersection, false otherwise.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool GetLineIntersection(int* X1, int* Y1, int* X2, int* Y2) const
  {
    return SDL_GetRectAndLineIntersection(this, X1, Y1, X2, Y2);
  }

  /**
   * Convert an SDL_Rect to SDL_FRect
   *
   * @return A FRect filled in with the floating point representation of
   *              `rect`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr operator SDL_FRect() const
  {
    SDL_FRect r;
    SDL_RectToFRect(this, &r);
    return r;
  }

  /**
   * @sa operator ToFRect()
   */
  constexpr operator FRect() const;

  /**
   * Determine whether a rectangle has no area.
   *
   * A rectangle is considered "empty" for this function if `r` is nullptr, or
   * if `r`'s width and/or height are <= 0.
   *
   * Note that this is a forced-inline function in a header, and not a public
   * API function available in the SDL library (which is to say, the code is
   * embedded in the calling program and the linker and dynamic loader will not
   * be able to find this function inside SDL itself).
   *
   * @returns true if the rectangle is "empty", false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool Empty() const { return SDL_RectEmpty(this); }

  /**
   * Determine whether two rectangles are equal.
   *
   * Rectangles are considered equal if both are not nullptr and each of their
   * x, y, width and height match.
   *
   * Note that this is a forced-inline function in a header, and not a public
   * API function available in the SDL library (which is to say, the code is
   * embedded in the calling program and the linker and dynamic loader will not
   * be able to find this function inside SDL itself).
   *
   * @param other the second rectangle to test.
   * @returns true if the rectangles are equal, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool Equal(const SDL_Rect& other) const
  {
    return SDL_RectsEqual(this, &other);
  }

  /**
   * Check whether the rect contains given point
   *
   * @param p Point to check
   *
   * @returns True if the point is contained in the rect
   *
   */
  constexpr bool Contains(const SDL_Point& p) const
  {
    return SDL_PointInRect(&p, this);
  }

  /**
   * Check whether the rect contains given point
   *
   * @param other Point to check
   *
   * @returns True if the point is contained in the rect
   *
   */
  constexpr bool Contains(const SDL_Rect& other) const
  {
    return GetUnion(other) == *this;
  }

  /**
   * Determine whether two rectangles intersect.
   *
   * @param other an SDL_Rect structure representing the second rectangle.
   * @returns true if there is an intersection, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Rect.GetIntersection
   */
  constexpr bool HasIntersection(const SDL_Rect& other) const
  {
    return SDL_HasRectIntersection(this, &other);
  }

  /**
   * Calculate the intersection of two rectangles.
   *
   * If `result` is nullptr then this function will return false.
   *
   * @param other an SDL_Rect structure representing the second rectangle.
   * @returns an SDL_Rect structure filled in with the intersection of
   *               if there is intersection, std::nullopt otherwise.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Rect.HasIntersection
   */
  constexpr std::optional<Rect> GetIntersection(const SDL_Rect& other) const
  {
    if (Rect result; SDL_GetRectIntersection(this, &other, &result)) {
      return result;
    }
    return std::nullopt;
  }

  /**
   * Calculate the union of two rectangles.
   *
   * @param other an SDL_Rect structure representing the second rectangle.
   * @returns Rect representing union of two rectangles
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr Rect GetUnion(const SDL_Rect& other) const
  {
    Rect result;
    CheckError(SDL_GetRectUnion(this, &other, &result));
    return result;
  }

  /**
   * Get a rect extended by specified amount of pixels
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
   * Get a rect extended by specified amount of pixels
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
   * Extend a rect by specified amount of pixels
   *
   * @param[in] amount Number of pixels to extend by
   *
   * @returns Reference to self
   *
   */
  constexpr Rect& Extend(unsigned int amount) { return Extend(amount, amount); }

  /**
   * Extend a rect by specified amount of pixels
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
   * Get rectangle moved by a given offset
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
   * Get rectangle moved by an opposite of given offset
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
   * Move by then given offset
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
   * Move by an opposite of the given offset
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
};

/**
 * A rectangle, with the origin at the upper left (using floating point
 * values).
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat wrap-extending-struct
 *
 * @sa FRect.Empty
 * @sa FRect.Equal
 * @sa FRect.EqualEpsilon
 * @sa FRect.HasIntersection
 * @sa FRect.GetIntersection
 * @sa FRect.GetLineIntersection
 * @sa FRect.GetUnion
 * @sa FRect.GetEnclosingPoints
 * @sa FPoint.InRect
 */
struct FRect : SDL_FRect
{
  /**
   * Wraps FRect.
   *
   * @param r the value to be wrapped
   */
  constexpr FRect(const SDL_FRect& r = {})
    : SDL_FRect(r)
  {
  }

  /**
   * Constructs from its fields.
   *
   * @param x the left x.
   * @param y the top y.
   * @param w the width.
   * @param h the height.
   */
  constexpr FRect(float x, float y, float w, float h)
    : SDL_FRect{x, y, w, h}
  {
  }

  /**
   * Constructs from top-left corner plus size
   */
  constexpr FRect(const SDL_FPoint& corner, const SDL_FPoint& size)
    : FRect{corner.x, corner.y, size.x, size.y}
  {
  }

  /**
   * Default comparison operator
   */
  constexpr bool operator==(const FRect& other) const { return Equal(other); }

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const SDL_FRect& other) const
  {
    return Equal(FRect(other));
  }

  /**
   * @sa Empty()
   */
  constexpr operator bool() const { return !Empty(); }

  /**
   * Get left x coordinate.
   *
   * @returns coordinate of the left x
   */
  constexpr float GetX() const { return x; }

  /**
   * Set the left x coordinate.
   *
   * @param newX the new left x.
   * @returns Reference to self.
   */
  constexpr FRect& SetX(float newX)
  {
    x = newX;
    return *this;
  }

  /**
   * Get top y coordinate.
   *
   * @returns coordinate of the top y.
   */
  constexpr float GetY() const { return y; }

  /**
   * Set the top y coordinate.
   *
   * @param newY the new top y.
   * @returns Reference to self.
   */
  constexpr FRect& SetY(float newY)
  {
    y = newY;
    return *this;
  }

  /**
   * Get width of the rect
   *
   * @returns Width of the rect
   */
  constexpr float GetW() const { return w; }

  /**
   * Set the width of the rect.
   *
   * @param newW the new width.
   * @returns Reference to self.
   */
  constexpr FRect& SetW(float newW)
  {
    w = newW;
    return *this;
  }

  /**
   * Get height of the rect
   *
   * @returns Height of the rect
   */
  constexpr float GetH() const { return h; }

  /**
   * Set the height of the rect.
   *
   * @param newH the new height.
   * @returns Reference to self.
   */
  constexpr FRect& SetH(float newH)
  {
    h = newH;
    return *this;
  }

  /**
   * Calculate a minimal rectangle enclosing a set of points with float
   * precision.
   *
   * If `clip` is not nullptr then only points inside of the clipping rectangle
   * are considered.
   *
   * @param points a span of SDL_Point structures representing points to be
   *               enclosed.
   * @param clip an SDL_Rect used for clipping or std::nullopt to enclose all
   *             points.
   * @returns a FRect structure filled in with the minimal enclosing
   *          rectangle or an empty FRect if all the points were outside of
   *          the clipping rectangle.
   *
   * @since This function is available since SDL 3.2.0.
   */
  static constexpr FRect GetEnclosingPoints(
    SpanRef<const SDL_FPoint> points,
    OptionalRef<const SDL_FRect> clip = std::nullopt)
  {
    if (FRect result; SDL_GetRectEnclosingPointsFloat(
          points.data(), points.size(), clip, &result)) {
      return result;
    }
    return {};
  }

  /**
   * Construct the rect from given center coordinates, width and height
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
   * Construct the rect from given center coordinates and size
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
   * Construct the rect from given corners coordinates
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
   * Construct the rect from given centers coordinates
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
   * Get X coordinate of the rect second corner
   *
   * @returns X coordinate of the rect second corner
   *
   */
  constexpr float GetX2() const { return x + w - 1; }

  /**
   * Set X coordinate of the rect second corner
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
   * Get Y coordinate of the rect second corner
   *
   * @returns Y coordinate of the rect second corner
   *
   */
  constexpr float GetY2() const { return y + h - 1; }

  /**
   * Set Y coordinate of the rect second corner
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
   * Get top left corner of the rect
   *
   * @returns Top left corner of the rect
   *
   */
  constexpr FPoint GetTopLeft() const { return FPoint(x, y); }

  /**
   * Get top right corner of the rect
   *
   * @returns Top right corner of the rect
   *
   */
  constexpr FPoint GetTopRight() const { return FPoint(GetX2(), y); }

  /**
   * Get bottom left corner of the rect
   *
   * @returns bottom left corner of the rect
   *
   */
  constexpr FPoint GetBottomLeft() const { return FPoint(x, GetY2()); }

  /**
   * Get bottom right corner of the rect
   *
   * @returns Bottom right corner of the rect
   *
   */
  constexpr FPoint GetBottomRight() const { return FPoint(GetX2(), GetY2()); }

  /**
   * Get size of the rect
   *
   * @returns Size of the rect
   *
   */
  constexpr FPoint GetSize() const { return FPoint(w, h); }

  /**
   * Get centroid of the rect
   *
   * @returns Centroid of the rect
   *
   */
  constexpr FPoint GetCentroid() const { return FPoint(x + w / 2, y + h / 2); }

  /**
   * Calculate the intersection of a rectangle and line segment with float
   * precision.
   *
   * This function is used to clip a line segment to a rectangle. A line segment
   * contained entirely within the rectangle or that does not intersect will
   * remain unchanged. A line segment that crosses the rectangle at either or
   * both ends will be clipped to the boundary of the rectangle and the new
   * coordinates saved in `X1`, `Y1`, `X2`, and/or `Y2` as necessary.
   *
   * @param X1 a pointer to the starting X-coordinate of the line.
   * @param Y1 a pointer to the starting Y-coordinate of the line.
   * @param X2 a pointer to the ending X-coordinate of the line.
   * @param Y2 a pointer to the ending Y-coordinate of the line.
   * @returns true if there is an intersection, false otherwise.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool GetLineIntersection(float* X1, float* Y1, float* X2, float* Y2) const
  {
    return SDL_GetRectAndLineIntersectionFloat(this, X1, Y1, X2, Y2);
  }

  /**
   * Calculate the intersection of a rectangle and line segment
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
  bool GetLineIntersection(FPoint* p1, FPoint* p2) const
  {
    return GetLineIntersection(p1 ? &p1->x : nullptr,
                               p1 ? &p1->y : nullptr,
                               p2 ? &p2->x : nullptr,
                               p2 ? &p2->y : nullptr);
  }

  /**
   * Determine whether a rectangle has no area.
   *
   * A rectangle is considered "empty" for this function if `r` is NULL, or if
   * `r`'s width and/or height are <= 0.
   *
   * Note that this is a forced-inline function in a header, and not a public
   * API function available in the SDL library (which is to say, the code is
   * embedded in the calling program and the linker and dynamic loader will not
   * be able to find this function inside SDL itself).
   *
   * @returns true if the rectangle is "empty", false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool Empty() const { return SDL_RectEmptyFloat(this); }

  /**
   * Determine whether two floating point rectangles are equal, within some
   * given epsilon.
   *
   * Rectangles are considered equal if both are not nullptr and each of their
   * x, y, width and height are within `epsilon` of each other. If you don't
   * know what value to use for `epsilon`, you should call the FRect.Equal
   * function instead.
   *
   * Note that this is a forced-inline function in a header, and not a public
   * API function available in the SDL library (which is to say, the code is
   * embedded in the calling program and the linker and dynamic loader will not
   * be able to find this function inside SDL itself).
   *
   * @param other the second rectangle to test.
   * @param epsilon the epsilon value for comparison.
   * @returns true if the rectangles are equal, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa FRect.Equal
   */
  constexpr bool EqualEpsilon(const SDL_FRect& other, const float epsilon) const
  {
    return SDL_RectsEqualEpsilon(this, &other, epsilon);
  }

  /**
   * Determine whether two rectangles are equal.
   *
   * Rectangles are considered equal if both are not nullptr and each of their
   * x, y, width and height are within SDL_FLT_EPSILON of each other. This is
   * often a reasonable way to compare two floating point rectangles and deal
   * with the slight precision variations in floating point calculations that
   * tend to pop up.
   *
   * Note that this is a forced-inline function in a header, and not a public
   * API function available in the SDL library (which is to say, the code is
   * embedded in the calling program and the linker and dynamic loader will not
   * be able to find this function inside SDL itself).
   *
   * @param other the second rectangle to test.
   * @returns true if the rectangles are equal, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa FRect.EqualEpsilon
   */
  constexpr bool Equal(const SDL_FRect& other) const
  {
    return SDL_RectsEqualFloat(this, &other);
  }

  /**
   * Check whether the rect contains given point
   *
   * @param p Point to check
   *
   * @returns True if the point is contained in the rect
   *
   */
  constexpr bool Contains(const SDL_FPoint& p) const
  {
    return SDL_PointInRectFloat(&p, this);
  }

  /**
   * Check whether the rect contains given point
   *
   * @param other Point to check
   *
   * @returns True if the point is contained in the rect
   *
   */
  constexpr bool Contains(const SDL_FRect& other) const
  {
    return GetUnion(other) == *this;
  }

  /**
   * Determine whether two rectangles intersect.
   *
   * @param other an SDL_Rect structure representing the second rectangle.
   * @returns true if there is an intersection, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Rect.GetIntersection
   */
  constexpr bool HasIntersection(const SDL_FRect& other) const
  {
    return SDL_HasRectIntersectionFloat(this, &other);
  }

  /**
   * Calculate the intersection of two rectangles with float precision.
   *
   * If `result` is nullptr then this function will return false.
   *
   * @param other an SDL_Rect structure representing the second rectangle.
   * @returns an SDL_Rect structure filled in with the intersection of
   *          if there is intersection, an empty FRect otherwise.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa FRect.HasIntersection
   */
  constexpr FRect GetIntersection(const SDL_FRect& other) const
  {
    if (FRect result; SDL_GetRectIntersectionFloat(this, &other, &result)) {
      return result;
    }
    return {};
  }

  /**
   * Calculate the union of two rectangles with float precision.
   *
   * @param other an SDL_Rect structure representing the second rectangle.
   * @returns Rect representing union of two rectangles
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr FRect GetUnion(const SDL_FRect& other) const
  {
    FRect result;
    CheckError(SDL_GetRectUnionFloat(this, &other, &result));
    return result;
  }

  /**
   * Get a rect extended by specified amount of pixels
   *
   * @param[in] amount Number of pixels to extend by
   *
   * @returns Extended rect
   *
   */
  constexpr FRect GetExtension(unsigned int amount) const
  {
    FRect r = *this;
    r.Extend(amount);
    return r;
  }

  /**
   * Get a rect extended by specified amount of pixels
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
   * Extend a rect by specified amount of pixels
   *
   * @param[in] amount Number of pixels to extend by
   *
   * @returns Reference to self
   *
   */
  constexpr FRect& Extend(float amount) { return Extend(amount, amount); }

  /**
   * Extend a rect by specified amount of pixels
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
   * Get rectangle moved by a given offset
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
   * Get rectangle moved by an opposite of given offset
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
   * Move by then given offset
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
   * Move by an opposite of the given offset
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
};

/// @}

constexpr Point::operator FPoint() const { return {float(x), float(y)}; }

constexpr FPoint Point::operator/(float value) const
{
  return FPoint(*this) / value;
}
constexpr FPoint Point::operator*(float value) const
{
  return FPoint(*this) * value;
}

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
    x = rect.x + rect.w - 1 - SDL_fmod(rect.x - x + rect.w - 1, rect.w);
  else if (x >= rect.x + rect.w)
    x = rect.x + SDL_fmod(x - rect.x - rect.w, rect.w);

  if (y < rect.y)
    y = rect.y + rect.h - 1 - SDL_fmod(rect.y - y + rect.h - 1, rect.h);
  else if (y >= rect.y + rect.h)
    y = rect.y + SDL_fmod(y - rect.y - rect.h, rect.h);

  return *this;
}

constexpr Rect::operator FRect() const
{
  return {float(x), float(y), float(w), float(h)};
}

} // namespace SDL

#endif /* SDL3PP_RECT_H_ */
