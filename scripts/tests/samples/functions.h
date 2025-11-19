/**
 * @file functions.h
 *
 * Documentation
 */
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <iostream>

/**
 * Documentation
 */
int funcCStr(const char* value);

/**
 * Documentation
 */
int funcVoid(void);

/**
 * Documentation
 */
int funcEmpty();

/**
 * Documentation
 */
int funcInt(int value, int otherValue);

/**
 * Documentation
 */
const char* funcIntReturnsCStr(int value, int otherValue);

/**
 * @brief Documentation
 *
 */
SDL_FORCE_INLINE int functionConstexpr(int value, int otherValue)
{
  return value + otherValue;
}

#endif // FUNCTIONS_H_
