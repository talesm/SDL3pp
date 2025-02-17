#ifndef STRUCTS_ALIASES_H_
#define STRUCTS_ALIASES_H_

#include "structs.h"

namespace name {

using Type1 = MyType1;

bool Type1Check(const Type1* value) { return MyType1Check(value); }

int GetType1Var1(const Type1* value) { return MyGetType1Var1(value); }

void SetType1Var1(Type1* value, int var1) { MySetType1Var1(value, var1); }

const char* Type1GetVar2(const Type1* value) { return MyType1GetVar2(value); }

void Type1SetVar2(Type1* value, const char* var2)
{
  MyType1SetVar2(value, var2);
}

using Byte = MyByte;

using Type2 = MyType2;

} // namespace name

#endif /* STRUCTS_ALIASES_H_ */
