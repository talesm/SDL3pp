# Transformations

We try make the binding as objective as possible, in order to minimize any
semantic deviation from the C interface. 

To do so we define the some rules so each C type or function is almost
mechanically transformed into a C++ type or function.

## General rules

- The C++ header files should have the same name than SDL's C headers, minus 
its prefix.
  - An exception is to some support files that we use for ease the abstraction
  and have no C header equivalent.
- All definitions are inside the `SDL` namespace, definition names lose their
prefix. Eg `SDL_Surface` becomes `SDL::Surface`;

## Function rules
- All method definitions are inline. We have a header only library here;
  - Forward declarations are permitted whenever is convenient;
- Non method-like functions are represented by simply wrapping them into free
functions.
- Method-like functions become class/struct methods accordingly to 
[Type rules](#type-rules);
  - Functions that involve I/O, like `SDL_LoadBMP()`, are considered non-method
  like and thus stay as free functions (`LoadBMP()`);
- The parameters of the function have the same meaning and same order than 
SDL's C API, transformed as the following:
  - If it is a instance function, the first parameter become the `this` pointer
  and thus is omitted.
  - If the parameter was a `const char*`, its becomes a StringWrapper;
  - If the parameter is a SDL type, use the type defined in [Type rules](#type-rules);
  - If the parameter is used as const pointer, it is allowed to be transformed to:
    - const reference;
    - std::optional value;
    - direct value;
    - Stay as const pointer;
    - Have a combination of any of those as different overloads;
  - If the parameter is a C callback function, the function should have an 
  overload accepting Callback type accordingly to [Callback rules](#callback-rules);

## Type rules
- Type that aren't opaque and nor have method-like functions are just 
typedef'd into the namespace. Eg: `using InitFlags = SDL_InitFlags`;
- Typedef that aren't opaque nor are created and destroyed by SDL functions, 
but have method-like functions are defined as follows:
  - We define a struct extending the original type for it, eg `struct Rect: SDL_Rect{...};`;
  - No further instance variable is added to it;
  - The appropriated method-like functions are wrapped inside its body;
  - A default constructor should be provided;
  - A constructor with all base class variables should be provided, if there
  are two or more of them;
  - A constructor accepting a base class instance by value or const reference 
  must be provided;
  - No destructor, copy constructor, move constructor or assignment operator
  should be defined;
- Typedef that create and destroyed by SDL functions (such `SDL_Create*()` and
`SDL_Destroy*()`) are represented as follows:
  - A class or struct called "*Type*Base", defined as a [CRTP](https://en.cppreference.com/w/cpp/language/crtp)
  is defined, expecting the template parameter to have Get() function;
    - If the typedef is opaque (eg SDL_Renderer), we use a class, otherwise 
    we use a struct (eg SDL_Surface);
    - This class or struct will have all wrappers for method-like functions 
    defined in its body;
    - protected static functions implement its `SDL_Create*()` and `SDL_Destroy*()` functions;
  - The class *Type*Unique extends *Type*Base and defines it as a std::unique_ptr to 
  the underling type;
    - Inheritance is public;
    - The constructors use the *Type*Base's `Create*()` methods;
    - It sets the unique_ptr's deleter to call SDL_Destroy(), so we don't need
    to deal with the C++ rule of five;
    - A typedef to its type without the "Unique" suffix is also provided;
  - The classes *Type*Shared and *Type*SharedWeak do the equivalent of \*Unique
  but using std::shared_ptr and std::weak_ptr;
  - The classes *Type*Wrapper uses a raw pointer internally. It does not 
  provide any way to destroy it and is intended to use by function parameters or
  some sort of very weak reference;

## Callback rules
- todo
