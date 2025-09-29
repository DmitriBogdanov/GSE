# Version

<div class="grid cards" markdown>

- Implementation
- [`#include "GSE/core/version.hpp"`](../../../include/GSE/core/version.hpp)
- Namespace
- Global
- Content
- Library version macros

</div>

## Definitions

```cpp
#define GSE_VERSION_MAJOR /* integer value */
#define GSE_VERSION_MINOR /* integer value */
#define GSE_VERSION_PATCH /* integer value */
```

## API

> ```cpp
> #define GSE_VERSION_MAJOR /* integer value */
> #define GSE_VERSION_MINOR /* integer value */
> #define GSE_VERSION_PATCH /* integer value */
> ```

Current version of the library according to [semver 2.0.0](https://semver.org/) convention.

Such macros are frequently used to provide conditional compilation based on the version of the library.

