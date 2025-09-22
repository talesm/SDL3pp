#ifndef SDL3PP_RECT_H_
#define SDL3PP_RECT_H_

#include <SDL3/SDL_rect.h>
#include "SDL3pp_error.h"
#include "SDL3pp_optionalRef.h"
#include "SDL3pp_spanRef.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryRect Category Rect
 *
 * Some helper functions for managing rectangles and 2D points, in both
 * integer and floating point versions.
 *
 * @{
 */

using PointRaw = SDL_Point;

using FPointRaw = SDL_FPoint;

using RectRaw = SDL_Rect;

using FRectRaw = SDL_FRect;

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
struct Point : PointRaw
{
  /**
   * Wraps Point.
   *
   * @param p the value to be wrapped
   */
  constexpr Point(const PointRaw& p = {})
    : PointRaw(p)
  {
  }

  /**
   * Constructs from its fields.
   *
   * @param x the value for x.
   * @param y the value for y.
   */
  constexpr Point(int x, int y)
    : PointRaw{x, y}
  {
  }

  /**
   * Wraps Point.
   *
   * @param p the value to be wrapped
   */
  constexpr explicit Point(const FPointRaw& p) {}

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const PointRaw& other) const
  {
    return x == other.x && y == other.y;
  }

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const Point& other) const
  {
    return *this == (const PointRaw&)(other);
  }

  /**
   * Check if valid.
   *
   * @returns True if valid state, false otherwise.
   */
  constexpr explicit operator bool() const { return *this != PointRaw{}; }

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
  constexpr bool InRect(const RectRaw& r) const
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
struct FPoint : FPointRaw
{
  /**
   * Wraps FPoint.
   *
   * @param p the value to be wrapped
   */
  constexpr FPoint(const FPointRaw& p = {})
    : FPointRaw(p)
  {
  }

  /**
   * Constructs from its fields.
   *
   * @param x the value for x.
   * @param y the value for y.
   */
  constexpr FPoint(float x, float y)
    : FPointRaw{x, y}
  {
  }

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const FPointRaw& other) const
  {
    return x == other.x && y == other.y;
  }

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const FPoint& other) const
  {
    return *this == (const FPointRaw&)(other);
  }

  /**
   * Check if valid.
   *
   * @returns True if valid state, false otherwise.
   */
  constexpr explicit operator bool() const { return *this != FPointRaw{}; }

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
  constexpr bool InRect(const FRectRaw& r) const
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
struct Rect : RectRaw
{
  /**
   * Wraps Rect.
   *
   * @param r the value to be wrapped
   */
  constexpr Rect(const RectRaw& r = {})
    : RectRaw(r)
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
    : RectRaw{x, y, w, h}
  {
  }

  /**
   * Wraps Rect.
   *
   * @param r the value to be wrapped
   */
  Rect(const PointRaw& corner, const PointRaw& size) {}

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const RectRaw& other) const
  {
    return x == other.x && y == other.y && w == other.w && h == other.h;
  }

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const Rect& other) const
  {
    return *this == (const RectRaw&)(other);
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
  static Rect GetEnclosingPoints(SpanRef<const PointRaw> points,
                                 OptionalRef<const RectRaw> clip = std::nullopt)
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

  static constexpr Rect FromCorners(const PointRaw& p1, const PointRaw& p2)
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

  bool GetLineIntersection(PointRaw* p1, PointRaw* p2)
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
  constexpr operator SDL_FRect() const : RectRaw(SDL_RectToFRect()) {}

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
  constexpr bool Equal(const RectRaw& other) const
  {
    return SDL_RectsEqual(this, other);
  }

  constexpr bool Contains(const PointRaw& p) const
  {
    static_assert(false, "Not implemented");
  }

  constexpr bool Contains(const RectRaw& other) const
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
  constexpr bool HasIntersection(const RectRaw& other) const
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
  constexpr std::optional<Rect> GetIntersection(const RectRaw& other) const
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
  constexpr Rect GetUnion(const RectRaw& other) const
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
struct FRect : FRectRaw
{
  /**
   * Wraps FRect.
   *
   * @param r the value to be wrapped
   */
  constexpr FRect(const FRectRaw& r = {})
    : FRectRaw(r)
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
    : FRectRaw{x, y, w, h}
  {
  }

  /**
   * Wraps FRect.
   *
   * @param r the value to be wrapped
   */
  constexpr FRect(const FPointRaw& corner, const FPointRaw& size) {}

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const FRectRaw& other) const
  {
    return x == other.x && y == other.y && w == other.w && h == other.h;
  }

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(const FRect& other) const
  {
    return *this == (const FRectRaw&)(other);
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
    SpanRef<const FPointRaw> points,
    OptionalRef<const FRectRaw> clip = std::nullopt)
  {
    return SDL_GetRectEnclosingPointsFloat(points, clip);
  }

  static constexpr FRect FromCenter(float cx, float cy, float w, float h)
  {
    static_assert(false, "Not implemented");
  }

  static constexpr FRect FromCenter(const FPointRaw& center,
                                    const FPointRaw& size)
  {
    static_assert(false, "Not implemented");
  }

  static constexpr FRect FromCorners(float x1, float y1, float x2, float y2)
  {
    static_assert(false, "Not implemented");
  }

  static constexpr FRect FromCorners(const FPointRaw& p1, const FPointRaw& p2)
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
  constexpr bool EqualEpsilon(const FRectRaw& other, const float epsilon) const
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
  constexpr bool Equal(const FRectRaw& other) const
  {
    return SDL_RectsEqualFloat(this, other);
  }

  constexpr bool Contains(const FPointRaw& p) const
  {
    static_assert(false, "Not implemented");
  }

  constexpr bool Contains(const FRectRaw& other) const
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
  constexpr bool HasIntersection(const FRectRaw& other) const
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
  constexpr FRect GetIntersection(const FRectRaw& other) const
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
  constexpr FRect GetUnion(const FRectRaw& other) const
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
constexpr Rect RectToFRect(const RectRaw& rect)
{
  return SDL_RectToFRect(rect);
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
constexpr bool PointInRect(const PointRaw& self, const RectRaw& r)
{
  return SDL_PointInRect(self, r);
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
constexpr bool RectEmpty(const RectRaw& r) { return SDL_RectEmpty(r); }

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
constexpr bool RectsEqual(const RectRaw& self, const RectRaw& other)
{
  return SDL_RectsEqual(self, other);
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
constexpr bool HasRectIntersection(const RectRaw& self, const RectRaw& other)
{
  return SDL_HasRectIntersection(self, other);
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
constexpr std::optional<Rect> GetRectIntersection(const RectRaw& self,
                                                  const RectRaw& other)
{
  return SDL_GetRectIntersection(self, other);
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
constexpr Rect GetRectUnion(const RectRaw& self, const RectRaw& other)
{
  return CheckError(SDL_GetRectUnion(self, other));
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
static inline bool GetRectEnclosingPoints(SpanRef<const PointRaw> points,
                                          OptionalRef<const RectRaw> clip,
                                          RectRaw* result)
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
inline bool GetRectAndLineIntersection(const RectRaw& rect,
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
constexpr bool PointInRectFloat(const FPointRaw& self, const FRectRaw& r)
{
  return SDL_PointInRectFloat(self, r);
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
constexpr bool RectEmptyFloat(const FRectRaw& r)
{
  return SDL_RectEmptyFloat(r);
}

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
constexpr bool RectsEqualEpsilon(const FRectRaw& self,
                                 const FRectRaw& other,
                                 const float epsilon)
{
  return SDL_RectsEqualEpsilon(self, other, epsilon);
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
constexpr bool RectsEqualFloat(const FRectRaw& self, const FRectRaw& other)
{
  return SDL_RectsEqualFloat(self, other);
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
constexpr bool HasRectIntersectionFloat(const FRectRaw& self,
                                        const FRectRaw& other)
{
  return SDL_HasRectIntersectionFloat(self, other);
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
constexpr FRect GetRectIntersectionFloat(const FRectRaw& self,
                                         const FRectRaw& other)
{
  return SDL_GetRectIntersectionFloat(self, other);
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
constexpr FRect GetRectUnionFloat(const FRectRaw& self, const FRectRaw& other)
{
  return CheckError(SDL_GetRectUnionFloat(self, other));
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
static inline bool GetRectEnclosingPointsFloat(SpanRef<const FPointRaw> points,
                                               OptionalRef<const FRectRaw> clip,
                                               FRectRaw* result)
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
inline bool GetRectAndLineIntersectionFloat(const FRectRaw& rect,
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
