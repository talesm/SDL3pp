#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <iostream>
#include <optional>

namespace name {

struct MyType1
{
  int var1;
  const char* var2;
};

bool MyType1Check(const MyType1* value);

int MyGetType1Var1(const MyType1* value);

void MySetType1Var1(MyType1* value, int var1);

const char* MyType1GetVar2(const MyType1* value);

void MyType1SetVar2(MyType1* value, const char* var2);

using MyByte = unsigned char;

typedef struct MyType2
{
  MyType1 var1;
  int* var2;
} MyType2;

} // namespace name

#endif /* STRUCTS_H_ */
