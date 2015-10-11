# A `constexpr` Murmur3A hash

This is a constexpr (C++14) implementation or MurMur3A which can be compiled
with clang or gcc. For more details, see my
[blog post](http://szelei.me/constexpr-murmurhash) about this library.

# Usage

This is a header-only library. Include the header `constexpr_murmur3.h` and call
the function `ce_mm3::mm3_x86_32`.

# Is the value returned really a compile-time constant?

Yes. The following code would not compile otherwise:

```cpp
using ce_mm3::mm3_x86_32;

template<int N>
struct printer
{
    printer() { std::cout << N << std::endl; }
};

printer<mm3_x86_32("Hello, world!", 0x9747b28c)> p;
```

# Is this code well-formed according to the standard?

To be honest, I'm not entirely sure. I am not using any `reinterpret_cast`s which
would be the main culprit in implementing hash functions that deal operate on
blocks of data. Feel free to comment on my
[post](http://szelei.me/constexpr-murmurhash) if you have any input on the issue.
