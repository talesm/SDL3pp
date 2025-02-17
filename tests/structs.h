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

using MyByte = unsigned char;

typedef struct MyType2
{
  MyType1 var1;
  int* var2;
} MyType2;

} // namespace name

#endif /* STRUCTS_H_ */
