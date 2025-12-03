#ifndef ENUM_ALIASES_H_
#define ENUM_ALIASES_H_

#include "enums.h"

namespace name {

/**
 * @brief My enum one
 *
 */
using Enum = MyEnum;

constexpr Enum ENUM_VALUE0 = MyENUM_VALUE0; ///< ENUM_VALUE0

constexpr Enum ENUM_VALUE1 = MyENUM_VALUE1; ///< ENUM_VALUE1

constexpr Enum ENUM_VALUE_N = MyENUM_VALUE_N; ///< ENUM_VALUE_N

/**
 * @brief My enum 2
 *
 */
using EnumBit = MyEnumBit;

constexpr EnumBit BIT_VALUE0 = MyBIT_VALUE0; ///< BIT_VALUE0

constexpr EnumBit BIT_VALUE1 = MyBIT_VALUE1; ///< BIT_VALUE1

constexpr EnumBit BIT_VALUE2 = MyBIT_VALUE2; ///< BIT_VALUE2

} // namespace name

#endif /* ENUM_ALIASES_H_ */
