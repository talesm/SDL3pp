#ifndef SDL3PP_RECT_H_
#define SDL3PP_RECT_H_

#include <SDL3/SDL_rect.h>
#include "SDL3pp_error.h"
#include "SDL3pp_optionalRef.h"
#include "SDL3pp_spanRef.h"

namespace SDL {

/**
 * @defgroup CategoryRect Category Rect
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
 * @since This struct is available since SDL 3.2.0.
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
  constexpr explicit Point(const SDL_FPoint& p) {}

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const SDL_Point& other) const
  {
    return x == other.x && y == other.y;
  }

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const Point& other) const
  {
    return *this == (const SDL_Point&)(other);
  }

  /**
   * Check if valid.
   *
   * @returns True if valid state, false otherwise.
   */
  constexpr explicit operator bool() const { return *this != SDL_Point{}; }

  /**
   * Get the x.
   *
   * @returns current x value.
   */
  constexpr int GetX() const { return x; }

  /**
   * Set the x.
   *
   * @param newX the new x value.
   * @returns Reference to self.
   */
  constexpr Point& SetX(int newX)
  {
    x = newX;
    return *this;
  }

  /**
   * Get the y.
   *
   * @returns current y value.
   */
  constexpr int GetY() const { return y; }

  /**
   * Set the y.
   *
   * @param newY the new y value.
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
   * @returns true if `p` is contained by `r`, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool InRect(const SDL_Rect& r) const
  {
    return SDL_PointInRect(this, r);
  }
};

/**
 * The structure that defines a point (using floating point values).
 *
 * @since This struct is available since SDL 3.2.0.
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
   * Compares with the underlying type
   */
  constexpr bool operator==(const SDL_FPoint& other) const
  {
    return x == other.x && y == other.y;
  }

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const FPoint& other) const
  {
    return *this == (const SDL_FPoint&)(other);
  }

  /**
   * Check if valid.
   *
   * @returns True if valid state, false otherwise.
   */
  constexpr explicit operator bool() const { return *this != SDL_FPoint{}; }

  /**
   * Get the x.
   *
   * @returns current x value.
   */
  constexpr float GetX() const { return x; }

  /**
   * Set the x.
   *
   * @param newX the new x value.
   * @returns Reference to self.
   */
  constexpr FPoint& SetX(float newX)
  {
    x = newX;
    return *this;
  }

  /**
   * Get the y.
   *
   * @returns current y value.
   */
  constexpr float GetY() const { return y; }

  /**
   * Set the y.
   *
   * @param newY the new y value.
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
   * @returns true if `p` is contained by `r`, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool InRect(const SDL_FRect& r) const
  {
    return SDL_PointInRectFloat(this, r);
  }
};

/**
 * A rectangle, with the origin at the upper left (using integers).
 *
 * @since This struct is available since SDL 3.2.0.
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
   * @param x the value for x.
   * @param y the value for y.
   * @param w the value for w.
   * @param h the value for h.
   */
  constexpr Rect(int x, int y, int w, int h)
    : SDL_Rect{x, y, w, h}
  {
  }

  /**
   * Wraps Rect.
   *
   * @param r the value to be wrapped
   */
  Rect(const SDL_Point& corner, const SDL_Point& size) {}

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const SDL_Rect& other) const
  {
    return x == other.x && y == other.y && w == other.w && h == other.h;
  }

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const Rect& other) const
  {
    return *this == (const SDL_Rect&)(other);
  }

  /**
   * @sa Empty()
   */
  constexpr explicit operator bool() const
  {
    static_assert(false, "Not implemented");
  }

  /**
   * Get the x.
   *
   * @returns current x value.
   */
  constexpr int GetX() const { return x; }

  /**
   * Set the x.
   *
   * @param newX the new x value.
   * @returns Reference to self.
   */
  constexpr Rect& SetX(int newX)
  {
    x = newX;
    return *this;
  }

  /**
   * Get the y.
   *
   * @returns current y value.
   */
  constexpr int GetY() const { return y; }

  /**
   * Set the y.
   *
   * @param newY the new y value.
   * @returns Reference to self.
   */
  constexpr Rect& SetY(int newY)
  {
    y = newY;
    return *this;
  }

  /**
   * Get the w.
   *
   * @returns current w value.
   */
  constexpr int GetW() const { return w; }

  /**
   * Set the w.
   *
   * @param newW the new w value.
   * @returns Reference to self.
   */
  constexpr Rect& SetW(int newW)
  {
    w = newW;
    return *this;
  }

  /**
   * Get the h.
   *
   * @returns current h value.
   */
  constexpr int GetH() const { return h; }

  /**
   * Set the h.
   *
   * @param newH the new h value.
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
   * @param points an array of Point structures representing points to be
   *               enclosed.
   * @param count the number of structures in the `points` array.
   * @param clip an Rect used for clipping or nullptr to enclose all points.
   * @param result an Rect structure filled in with the minimal enclosing
   *               rectangle.
   * @returns true if any points were enclosed or false if all the points were
   *          outside of the clipping rectangle.
   *
   * @since This function is available since SDL 3.2.0.
   */
  static Rect GetEnclosingPoints(
    SpanRef<const SDL_Point> points,
    OptionalRef<const SDL_Rect> clip = std::nullopt)
  {
    return SDL_GetRectEnclosingPoints(points, clip);
  }

  static constexpr Rect FromCenter(int cx, int cy, int w, int h)
  {
    static_assert(false, "Not implemented");
  }

  static constexpr Rect FromCenter(const Point& center, const Point& size)
  {
    static_assert(false, "Not implemented");
  }

  static constexpr Rect FromCorners(int x1, int y1, int x2, int y2)
  {
    static_assert(false, "Not implemented");
  }

  static constexpr Rect FromCorners(const SDL_Point& p1, const SDL_Point& p2)
  {
    static_assert(false, "Not implemented");
  }

  auto GetX2() { static_assert(false, "Not implemented"); }

  auto SetX2() { static_assert(false, "Not implemented"); }

  auto GetY2() { static_assert(false, "Not implemented"); }

  auto SetY2() { static_assert(false, "Not implemented"); }

  auto GetTopLeft() { static_assert(false, "Not implemented"); }

  auto GetTopRight() { static_assert(false, "Not implemented"); }

  auto GetBottomLeft() { static_assert(false, "Not implemented"); }

  auto GetBottomRight() { static_assert(false, "Not implemented"); }

  auto GetSize() { static_assert(false, "Not implemented"); }

  auto GetCentroid() { static_assert(false, "Not implemented"); }

  bool GetLineIntersection(SDL_Point* p1, SDL_Point* p2)
  {
    static_assert(false, "Not implemented");
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
   * Convert an Rect to FRect
   *
   * @param rect a pointer to an Rect.
   * @param frect a pointer filled in with the floating point representation of
   *              `rect`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr operator SDL_FRect() const : SDL_Rect(SDL_RectToFRect()) {}

  /**
   * @sa operator ToFRect()
   */
#error "operator FRect (undefined)"

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
   * @param b the second rectangle to test.
   * @returns true if the rectangles are equal, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool Equal(const SDL_Rect& other) const
  {
    return SDL_RectsEqual(this, other);
  }

  constexpr bool Contains(const SDL_Point& p) const
  {
    static_assert(false, "Not implemented");
  }

  constexpr bool Contains(const SDL_Rect& other) const
  {
    static_assert(false, "Not implemented");
  }

  /**
   * Determine whether two rectangles intersect.
   *
   * If either pointer is nullptr the function will return false.
   *
   * @param B an Rect structure representing the second rectangle.
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
    return SDL_HasRectIntersection(this, other);
  }

  /**
   * Calculate the intersection of two rectangles.
   *
   * If `result` is nullptr then this function will return false.
   *
   * @param B an Rect structure representing the second rectangle.
   * @param result an Rect structure filled in with the intersection of
   *               rectangles `A` and `B`.
   * @returns true if there is an intersection, false otherwise.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Rect.HasIntersection
   */
  constexpr std::optional<Rect> GetIntersection(const SDL_Rect& other) const
  {
    return SDL_GetRectIntersection(this, other);
  }

  /**
   * Calculate the union of two rectangles.
   *
   * @param B an Rect structure representing the second rectangle.
   * @param result an Rect structure filled in with the union of rectangles
   *               `A` and `B`.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr Rect GetUnion(const SDL_Rect& other) const
  {
    return CheckError(SDL_GetRectUnion(this, other));
  }
};

/**
 * A rectangle, with the origin at the upper left (using floating point
 * values).
 *
 * @since This struct is available since SDL 3.2.0.
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
   * @param x the value for x.
   * @param y the value for y.
   * @param w the value for w.
   * @param h the value for h.
   */
  constexpr FRect(float x, float y, float w, float h)
    : SDL_FRect{x, y, w, h}
  {
  }

  /**
   * Wraps FRect.
   *
   * @param r the value to be wrapped
   */
  constexpr FRect(const SDL_FPoint& corner, const SDL_FPoint& size) {}

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const SDL_FRect& other) const
  {
    return x == other.x && y == other.y && w == other.w && h == other.h;
  }

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const FRect& other) const
  {
    return *this == (const SDL_FRect&)(other);
  }

  /**
   * @sa Empty()
   */
  constexpr explicit operator bool() const
  {
    static_assert(false, "Not implemented");
  }

  /**
   * Get the x.
   *
   * @returns current x value.
   */
  constexpr float GetX() const { return x; }

  /**
   * Set the x.
   *
   * @param newX the new x value.
   * @returns Reference to self.
   */
  constexpr FRect& SetX(float newX)
  {
    x = newX;
    return *this;
  }

  /**
   * Get the y.
   *
   * @returns current y value.
   */
  constexpr float GetY() const { return y; }

  /**
   * Set the y.
   *
   * @param newY the new y value.
   * @returns Reference to self.
   */
  constexpr FRect& SetY(float newY)
  {
    y = newY;
    return *this;
  }

  /**
   * Get the w.
   *
   * @returns current w value.
   */
  constexpr float GetW() const { return w; }

  /**
   * Set the w.
   *
   * @param newW the new w value.
   * @returns Reference to self.
   */
  constexpr FRect& SetW(float newW)
  {
    w = newW;
    return *this;
  }

  /**
   * Get the h.
   *
   * @returns current h value.
   */
  constexpr float GetH() const { return h; }

  /**
   * Set the h.
   *
   * @param newH the new h value.
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
   * @param points an array of FPoint structures representing points to be
   *               enclosed.
   * @param count the number of structures in the `points` array.
   * @param clip an FRect used for clipping or nullptr to enclose all points.
   * @param result an FRect structure filled in with the minimal enclosing
   *               rectangle.
   * @returns true if any points were enclosed or false if all the points were
   *          outside of the clipping rectangle.
   *
   * @since This function is available since SDL 3.2.0.
   */
  static constexpr FRect GetEnclosingPoints(
    SpanRef<const SDL_FPoint> points,
    OptionalRef<const SDL_FRect> clip = std::nullopt)
  {
    return SDL_GetRectEnclosingPointsFloat(points, clip);
  }

  static constexpr FRect FromCenter(float cx, float cy, float w, float h)
  {
    static_assert(false, "Not implemented");
  }

  static constexpr FRect FromCenter(const SDL_FPoint& center,
                                    const SDL_FPoint& size)
  {
    static_assert(false, "Not implemented");
  }

  static constexpr FRect FromCorners(float x1, float y1, float x2, float y2)
  {
    static_assert(false, "Not implemented");
  }

  static constexpr FRect FromCorners(const SDL_FPoint& p1, const SDL_FPoint& p2)
  {
    static_assert(false, "Not implemented");
  }

  auto GetX2() { static_assert(false, "Not implemented"); }

  auto SetX2() { static_assert(false, "Not implemented"); }

  auto GetY2() { static_assert(false, "Not implemented"); }

  auto SetY2() { static_assert(false, "Not implemented"); }

  auto GetTopLeft() { static_assert(false, "Not implemented"); }

  auto GetTopRight() { static_assert(false, "Not implemented"); }

  auto GetBottomLeft() { static_assert(false, "Not implemented"); }

  auto GetBottomRight() { static_assert(false, "Not implemented"); }

  auto GetSize() { static_assert(false, "Not implemented"); }

  auto GetCentroid() { static_assert(false, "Not implemented"); }

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
   * Determine whether a floating point rectangle can contain any point.
   *
   * A rectangle is considered "empty" for this function if `r` is nullptr, or
   * if `r`'s width and/or height are < 0.0f.
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
   * @param b the second rectangle to test.
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
    return SDL_RectsEqualEpsilon(this, other, epsilon);
  }

  /**
   * Determine whether two floating point rectangles are equal, within a default
   * epsilon.
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
   * @param b the second rectangle to test.
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
    return SDL_RectsEqualFloat(this, other);
  }

  constexpr bool Contains(const SDL_FPoint& p) const
  {
    static_assert(false, "Not implemented");
  }

  constexpr bool Contains(const SDL_FRect& other) const
  {
    static_assert(false, "Not implemented");
  }

  /**
   * Determine whether two rectangles intersect with float precision.
   *
   * If either pointer is nullptr the function will return false.
   *
   * @param B an FRect structure representing the second rectangle.
   * @returns true if there is an intersection, false otherwise.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Rect.GetIntersection
   */
  constexpr bool HasIntersection(const SDL_FRect& other) const
  {
    return SDL_HasRectIntersectionFloat(this, other);
  }

  /**
   * Calculate the intersection of two rectangles with float precision.
   *
   * If `result` is nullptr then this function will return false.
   *
   * @param B an FRect structure representing the second rectangle.
   * @param result an FRect structure filled in with the intersection of
   *               rectangles `A` and `B`.
   * @returns true if there is an intersection, false otherwise.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa FRect.HasIntersection
   */
  constexpr FRect GetIntersection(const SDL_FRect& other) const
  {
    return SDL_GetRectIntersectionFloat(this, other);
  }

  /**
   * Calculate the union of two rectangles with float precision.
   *
   * @param B an FRect structure representing the second rectangle.
   * @param result an FRect structure filled in with the union of rectangles
   *               `A` and `B`.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr FRect GetUnion(const SDL_FRect& other) const
  {
    return CheckError(SDL_GetRectUnionFloat(this, other));
  }
};

/**
 * Convert an Rect to FRect
 *
 * @param rect a pointer to an Rect.
 * @param frect a pointer filled in with the floating point representation of
 *              `rect`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void RectToFRect(const SDL_Rect& rect, SDL_FRect* frect)
{
  SDL_RectToFRect(rect, frect);
}

/**
 * Determine whether a point resides inside a rectangle.
 *
 * A point is considered part of a rectangle if both `p` and `r` are not
 * nullptr, and `p`'s x and y coordinates are >= to the rectangle's top left
 * corner, and < the rectangle's x+w and y+h. So a 1x1 rectangle considers point
 * (0,0) as "inside" and (0,1) as not.
 *
 * Note that this is a forced-inline function in a header, and not a public
 * API function available in the SDL library (which is to say, the code is
 * embedded in the calling program and the linker and dynamic loader will not
 * be able to find this function inside SDL itself).
 *
 * @param p the point to test.
 * @param r the rectangle to test.
 * @returns true if `p` is contained by `r`, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool PointInRect(const SDL_Point& p, const SDL_Rect& r)
{
  return SDL_PointInRect(p, r);
}

/**
 * Determine whether a rectangle has no area.
 *
 * A rectangle is considered "empty" for this function if `r` is nullptr, or if
 * `r`'s width and/or height are <= 0.
 *
 * Note that this is a forced-inline function in a header, and not a public
 * API function available in the SDL library (which is to say, the code is
 * embedded in the calling program and the linker and dynamic loader will not
 * be able to find this function inside SDL itself).
 *
 * @param r the rectangle to test.
 * @returns true if the rectangle is "empty", false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool RectEmpty(const SDL_Rect& r) { return SDL_RectEmpty(r); }

/**
 * Determine whether two rectangles are equal.
 *
 * Rectangles are considered equal if both are not nullptr and each of their x,
 * y, width and height match.
 *
 * Note that this is a forced-inline function in a header, and not a public
 * API function available in the SDL library (which is to say, the code is
 * embedded in the calling program and the linker and dynamic loader will not
 * be able to find this function inside SDL itself).
 *
 * @param a the first rectangle to test.
 * @param b the second rectangle to test.
 * @returns true if the rectangles are equal, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool RectsEqual(const SDL_Rect& a, const SDL_Rect& b)
{
  return SDL_RectsEqual(a, b);
}

/**
 * Determine whether two rectangles intersect.
 *
 * If either pointer is nullptr the function will return false.
 *
 * @param A an Rect structure representing the first rectangle.
 * @param B an Rect structure representing the second rectangle.
 * @returns true if there is an intersection, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Rect.GetIntersection
 */
inline bool HasRectIntersection(const SDL_Rect& A, const SDL_Rect& B)
{
  return SDL_HasRectIntersection(A, B);
}

/**
 * Calculate the intersection of two rectangles.
 *
 * If `result` is nullptr then this function will return false.
 *
 * @param A an Rect structure representing the first rectangle.
 * @param B an Rect structure representing the second rectangle.
 * @param result an Rect structure filled in with the intersection of
 *               rectangles `A` and `B`.
 * @returns true if there is an intersection, false otherwise.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Rect.HasIntersection
 */
inline bool GetRectIntersection(const SDL_Rect& A,
                                const SDL_Rect& B,
                                SDL_Rect* result)
{
  return SDL_GetRectIntersection(A, B, result);
}

/**
 * Calculate the union of two rectangles.
 *
 * @param A an Rect structure representing the first rectangle.
 * @param B an Rect structure representing the second rectangle.
 * @param result an Rect structure filled in with the union of rectangles
 *               `A` and `B`.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void GetRectUnion(const SDL_Rect& A, const SDL_Rect& B, SDL_Rect* result)
{
  CheckError(SDL_GetRectUnion(A, B, result));
}

/**
 * Calculate a minimal rectangle enclosing a set of points.
 *
 * If `clip` is not nullptr then only points inside of the clipping rectangle
 * are considered.
 *
 * @param points an array of Point structures representing points to be
 *               enclosed.
 * @param count the number of structures in the `points` array.
 * @param clip an Rect used for clipping or nullptr to enclose all points.
 * @param result an Rect structure filled in with the minimal enclosing
 *               rectangle.
 * @returns true if any points were enclosed or false if all the points were
 *          outside of the clipping rectangle.
 *
 * @since This function is available since SDL 3.2.0.
 */
static inline bool GetRectEnclosingPoints(SpanRef<const SDL_Point> points,
                                          OptionalRef<const SDL_Rect> clip,
                                          SDL_Rect* result)
{
  return SDL_GetRectEnclosingPoints(points, clip, result);
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
 * @param rect an Rect structure representing the rectangle to intersect.
 * @param X1 a pointer to the starting X-coordinate of the line.
 * @param Y1 a pointer to the starting Y-coordinate of the line.
 * @param X2 a pointer to the ending X-coordinate of the line.
 * @param Y2 a pointer to the ending Y-coordinate of the line.
 * @returns true if there is an intersection, false otherwise.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool GetRectAndLineIntersection(const SDL_Rect& rect,
                                       int* X1,
                                       int* Y1,
                                       int* X2,
                                       int* Y2)
{
  return SDL_GetRectAndLineIntersection(rect, X1, Y1, X2, Y2);
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
 * @param p the point to test.
 * @param r the rectangle to test.
 * @returns true if `p` is contained by `r`, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool PointInRectFloat(const SDL_FPoint& p, const SDL_FRect& r)
{
  return SDL_PointInRectFloat(p, r);
}

/**
 * Determine whether a floating point rectangle can contain any point.
 *
 * A rectangle is considered "empty" for this function if `r` is nullptr, or if
 * `r`'s width and/or height are < 0.0f.
 *
 * Note that this is a forced-inline function in a header, and not a public
 * API function available in the SDL library (which is to say, the code is
 * embedded in the calling program and the linker and dynamic loader will not
 * be able to find this function inside SDL itself).
 *
 * @param r the rectangle to test.
 * @returns true if the rectangle is "empty", false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool RectEmptyFloat(const SDL_FRect& r) { return SDL_RectEmptyFloat(r); }

/**
 * Determine whether two floating point rectangles are equal, within some
 * given epsilon.
 *
 * Rectangles are considered equal if both are not nullptr and each of their x,
 * y, width and height are within `epsilon` of each other. If you don't know
 * what value to use for `epsilon`, you should call the FRect.Equal
 * function instead.
 *
 * Note that this is a forced-inline function in a header, and not a public
 * API function available in the SDL library (which is to say, the code is
 * embedded in the calling program and the linker and dynamic loader will not
 * be able to find this function inside SDL itself).
 *
 * @param a the first rectangle to test.
 * @param b the second rectangle to test.
 * @param epsilon the epsilon value for comparison.
 * @returns true if the rectangles are equal, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa FRect.Equal
 */
inline bool RectsEqualEpsilon(const SDL_FRect& a,
                              const SDL_FRect& b,
                              float epsilon)
{
  return SDL_RectsEqualEpsilon(a, b, epsilon);
}

/**
 * Determine whether two floating point rectangles are equal, within a default
 * epsilon.
 *
 * Rectangles are considered equal if both are not nullptr and each of their x,
 * y, width and height are within SDL_FLT_EPSILON of each other. This is often
 * a reasonable way to compare two floating point rectangles and deal with the
 * slight precision variations in floating point calculations that tend to pop
 * up.
 *
 * Note that this is a forced-inline function in a header, and not a public
 * API function available in the SDL library (which is to say, the code is
 * embedded in the calling program and the linker and dynamic loader will not
 * be able to find this function inside SDL itself).
 *
 * @param a the first rectangle to test.
 * @param b the second rectangle to test.
 * @returns true if the rectangles are equal, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa FRect.EqualEpsilon
 */
inline bool RectsEqualFloat(const SDL_FRect& a, const SDL_FRect& b)
{
  return SDL_RectsEqualFloat(a, b);
}

/**
 * Determine whether two rectangles intersect with float precision.
 *
 * If either pointer is nullptr the function will return false.
 *
 * @param A an FRect structure representing the first rectangle.
 * @param B an FRect structure representing the second rectangle.
 * @returns true if there is an intersection, false otherwise.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Rect.GetIntersection
 */
inline bool HasRectIntersectionFloat(const SDL_FRect& A, const SDL_FRect& B)
{
  return SDL_HasRectIntersectionFloat(A, B);
}

/**
 * Calculate the intersection of two rectangles with float precision.
 *
 * If `result` is nullptr then this function will return false.
 *
 * @param A an FRect structure representing the first rectangle.
 * @param B an FRect structure representing the second rectangle.
 * @param result an FRect structure filled in with the intersection of
 *               rectangles `A` and `B`.
 * @returns true if there is an intersection, false otherwise.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa FRect.HasIntersection
 */
inline bool GetRectIntersectionFloat(const SDL_FRect& A,
                                     const SDL_FRect& B,
                                     SDL_FRect* result)
{
  return SDL_GetRectIntersectionFloat(A, B, result);
}

/**
 * Calculate the union of two rectangles with float precision.
 *
 * @param A an FRect structure representing the first rectangle.
 * @param B an FRect structure representing the second rectangle.
 * @param result an FRect structure filled in with the union of rectangles
 *               `A` and `B`.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void GetRectUnionFloat(const SDL_FRect& A,
                              const SDL_FRect& B,
                              SDL_FRect* result)
{
  CheckError(SDL_GetRectUnionFloat(A, B, result));
}

/**
 * Calculate a minimal rectangle enclosing a set of points with float
 * precision.
 *
 * If `clip` is not nullptr then only points inside of the clipping rectangle
 * are considered.
 *
 * @param points an array of FPoint structures representing points to be
 *               enclosed.
 * @param count the number of structures in the `points` array.
 * @param clip an FRect used for clipping or nullptr to enclose all points.
 * @param result an FRect structure filled in with the minimal enclosing
 *               rectangle.
 * @returns true if any points were enclosed or false if all the points were
 *          outside of the clipping rectangle.
 *
 * @since This function is available since SDL 3.2.0.
 */
static inline bool GetRectEnclosingPointsFloat(
  SpanRef<const SDL_FPoint> points,
  OptionalRef<const SDL_FRect> clip,
  SDL_FRect* result)
{
  return SDL_GetRectEnclosingPointsFloat(points, clip, result);
}

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
 * @param rect an FRect structure representing the rectangle to intersect.
 * @param X1 a pointer to the starting X-coordinate of the line.
 * @param Y1 a pointer to the starting Y-coordinate of the line.
 * @param X2 a pointer to the ending X-coordinate of the line.
 * @param Y2 a pointer to the ending Y-coordinate of the line.
 * @returns true if there is an intersection, false otherwise.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool GetRectAndLineIntersectionFloat(const SDL_FRect& rect,
                                            float* X1,
                                            float* Y1,
                                            float* X2,
                                            float* Y2)
{
  return SDL_GetRectAndLineIntersectionFloat(rect, X1, Y1, X2, Y2);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_RECT_H_ */
