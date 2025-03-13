Transformations
===============

We try make the binding as objective as possible, in order to minimize any
semantic deviation from the C interface. 

To do so we define the some rules so each C type or function is almost
mechanically transformed into a C++ type or function.

General rules
-------------

- The C++ header files should have the same name than SDL's C headers, minus 
its prefix.
  - An exception is to some support files that we use for ease the abstraction
  and have no C header equivalent.
- All definitions are inside the `SDL` namespace, definition names lose their
prefix. Eg `SDL_Surface` becomes SDL::Surface;

Function rules
--------------

- All method definitions are inline. We have a header only library here;
  - Forward declarations are permitted whenever is convenient;
- Non method-like functions are represented by simply wrapping them into free
  functions:
  - They are always inside the namespace SDL;
  - The name thus have the "SDL_" prefix removed;
  - Example: SDL_GetCurrentVideoDriver becomes SDL::GetCurrentVideoDriver();
- Method-like functions become class/struct methods accordingly to 
[Type rules](#type-rules);
  - The Type name is removed, eg SDL_GetTextureProperties becomes 
    SDL::Texture::GetProperties();
  - Functions that involve I/O, like `SDL_LoadBMP()`, are considered non-method
    like and thus stay as free functions like: SDL::LoadBMP();
- The parameters of the function have the same meaning and same order than 
  SDL's C API, transformed as the following:
  - If it is a instance function, the first parameter become the `this` pointer
    and thus is omitted.
  - If the parameter was a `const char*`, its becomes a StringWrapper;
  - If the parameter is a SDL type, use the type defined in [Type rules](#type-rules):
    - If it is an [Resource](#resource), use the non owned version, Eg SDL::SurfaceRef;
    - Otherwise use the converted name like SDL::Color;
  - If the parameter is used as const pointer, it is allowed to be transformed to:
    - const reference if mandatory;
    - SDL::OptionalRef value if parameter is optional;
    - The non owning <i>Type</i>Ref, if the type is a [Resource](#resource).
  - If the parameter is the only out parameter, and the return type is the regular 
    boolean for success or failure:
    - Remove the parameter;
    - If the result is a reference type, return one of the wrapped types;
    - Otherwise, wrap the pointed type in a std::optional;
    - Do this only if won't cause ambiguity;
  - If the parameter is a C callback function, the function should have an 
    overload accepting Callback type accordingly to [Callback rules](#callback-rules);
- When there are 4 parameters representing a RGBA value:
  - If they are input parameters, replace them by SDL::Color or SDL::FColor;
  - If they are out parameters, add an overload to return std::optional wrapping
    the SDL::Color or SDL::FColor;
  - If there is both a Uint8 e and float versions, also add an overload taking the
    SDL::Color* and another one taking SDL::FColor*;

Type rules
----------

- Type that aren't opaque and nor have method-like functions are just 
typedef'd into the namespace. Eg: `using InitFlags = SDL_InitFlags`;
- Data types that aren't opaque nor are created and destroyed by SDL functions, 
but have method-like functions are defined as follows:
  - If the type is a struct, we define a struct extending it, eg `struct Rect: SDL_Rect{...};`:
    - A constructor accepting the original type by value or const reference
      must be provided;
    - A constructor with all base class variables should be provided, if there
      are two or more of them;
    - A default constructor should be provided, probably zeroing everything;
    - Example: SDL::Rect;
  - If the type is not a struct (like an enum or an alias for Uint32), then we 
  define a struct containing a single member variable of the original type:
    - A constructor accepting the original type by value or const reference
      must be provided;
    - A conversion operator to the original type must be provided;
    - If there is a default invalid state, this should be the
      default value of the constructor parameter and there must be converting 
      operator to bool;
    - Example SDL::PixelFormat;
  - No further instance variable is added to it;
  - The appropriated method-like functions are wrapped inside its body;
  - A constructor accepting a original type by value or const reference 
  must be provided;
  - No destructor, copy constructor, move constructor or assignment operator
  should be defined;
- [Resource](#resource) types that are created and destroyed by SDL functions
(such `SDL_Create*()` and `SDL_Destroy*()`) are represented as follows:
  - A struct called <i>Type</i>Base, defined as a [CRTP](https://en.cppreference.com/w/cpp/language/crtp)
    is defined, expecting the template parameter to fulfill the SDL::ObjectBox
    concept;
    - This struct will have all wrappers for method-like functions 
      defined in its body;
    - If the original type is a non opaque struct we can define methods to 
      manipulate the most common access patterns directly, when convenient to 
      do so;
    - There must be a constructor for each `SDL_Create*` there; 
    - It also uses all its parent's constructor.
  - The class <i>Type</i> extends <i>Type</i>Base with a SDL::ObjectUnique;
    - This represents an owning reference to the resource and destroy it when
      it goes out of scope, unless we call SDL::ObjectUnique.release() before 
      it;
  - The class <i>Type</i>Ref extends <i>Type</i>Base with a SDL::ObjectRef;
    - This represents a non owning reference to the resource. Use this on 
      parameters and as a sort of weak reference to the previous type;
    - We can still destroy it manually by calling the right `Destroy()`
      on it;

Callback rules
--------------

As we mentioned in [Function rules](#function-rules), the original callback 
functions are always supported. In addition, if has an userdata parameter we 
create an alternative callback as follows:

- The callback name will have the "Function" suffix, replacing a "Callback" 
  suffix if any;
- The callback type is a std::function with the same parameters than the 
  original one, with the following changes: 
  - Remove userdata parameter;
  - Follow any applicable parameter transformation rules as described in 
    [Function rules](#function-rules).
- The function taking the callback (and the one removing it if exists) must
  follow [this guideline](#callback) on how to marshal the std::function object
  into the userdata. 
