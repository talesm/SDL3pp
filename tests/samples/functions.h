/**
 * @file functions.h
 *
 * Documentation
 */
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <iostream>

namespace name {

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
int funcInt(int value);

/**
 * Documentation
 */
const char* funcIntReturnsCStr(int value, int otherValue = 0);

/**
 * @brief Documentation
 *
 */
constexpr int functionConstexpr(int value, int otherValue)
{
  return value + otherValue;
}

} // namespace name

#endif // FUNCTIONS_H_
