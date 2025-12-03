![Octoliner Utils Logo](logo.svg)

# Octoliner Utils

**A small, Header Only library that contain usfull bits of codes**



This library is part of the larger **Octoliner** family of tools.

---

## Highlights

- modern C++ 20 functions.
- Expected<V,R> implementation
- Zip<A,B> and Zip_Index<A> for safe and fast loop parsing
- For_Each_Tuple

---

## Quick install (CMake + FetchContent)

Drop this into your top-level `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
    OUtils
    GIT_REPOSITORY https://github.com/leocity207/OUtils.git
    UPDATE_DISCONNECTED ON
)

FetchContent_MakeAvailable(OUtils)
```

## Quick Exemple

easy GeoJSON Parsing

```C++

#include <utils/expected.h>
#include <utils/zip.h>

O::Expected<Value,Error> Do_Weird_Thing(const std::vector<int>& a,const std::vector<int>& b)
{
	for(auto&& [a_elt, b_elt] : O::Zip(a,b))
	{
		if(a_elt == 0 || b_elt == 0)
			return Error::SHOULD_NOT_BE_ZERO;
		if(a_elt > b_elt )
			return Value(a_elt, b_elt);
		if(b_elt > a_elt )
			continue;
	}
}

```