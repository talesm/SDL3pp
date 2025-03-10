#ifndef ENUM_H_
#define ENUM_H_

namespace name {

/**
 * @brief My enum one
 *
 */
typedef enum MyEnum
{
  MyENUM_VALUE0,
  MyENUM_VALUE1 = 123,
  MyENUM_VALUE_N = 9999,
} MyEnum;

/**
 * @brief My enum 2
 *
 */
typedef enum MyEnumBit
{
  MyBIT_VALUE0,
  MyBIT_VALUE1 = 1,
  MyBIT_VALUE2 = 2,
} MyEnumBit;

} // namespace name

#endif /* ENUM_H_ */
