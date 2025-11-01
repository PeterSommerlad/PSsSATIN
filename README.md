# PSsSATIN

simple SATurating Integral Numbers

I provide two implementations of saturating integer arithmetic for signed and unsigned integers of sizes 8, 16, 32, 64 bits. 

* [psssatin.h](include/psssatin.h) is using `enum class` types and corresponding operator overloading etc.
* [satins.h](include/satins.h) is using a class template for the integers with fixed sizes. 

Note that according to MISRA C++ guidelines arithmetic on character types (i.e., `char16_t`) is not supported.

All operations are supported at compile time and this is checked by the tests.

Invalid operations, such as combining too large types in arithmetic with smaller types or mixing signed and unsigned values in operations are prevented to compile.

No operation will generate a run-time error or have undefined behavior, but instead the correspinding `std::numeric_limits<>` minimum or maximum value for the given type.

Shifting by too many bits will give corresponding zero value.

