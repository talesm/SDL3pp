#ifndef STRUCTS_RESOURCES_H_
#define STRUCTS_RESOURCES_H_

#include "structs.h"

namespace name {

template<class T>
struct Type1Ref : T
{
  using T::T;

  inline Type1Ref(int var1, const char* var2)
  {
    // return MyCreateType1(var1, var2);
  }

  bool Check() const { return MyType1Check(this); }

  int GetVar1() const { return MyGetType1Var1(this); }

  void SetVar1(int var1) { MySetType1Var1(this, var1); }

  const char* GetVar2() const { return MyType1GetVar2(this); }

  void SetVar2(const char* var2) { MyType1SetVar2(this, var2); }
};

using Byte = MyByte;

struct Type2 : MyType2
{};

} // namespace name

#endif /* STRUCTS_RESOURCES_H_ */
