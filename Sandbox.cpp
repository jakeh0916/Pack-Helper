#include "include/Pack.h"
#include "include/PackAlgorithms.h"

#include <string>

int main()
{
    using namespace packhelp;
    using P = Pack<int, const char*, double, double, double, std::string, std::string&&, const char*, const char*&>;

    constexpr size_t x = Find<double, P>;

    constexpr size_t f = Find_native<std::string, int, const char*, double, double, double, std::string, std::string&&, const char*, const char*&>;

    constexpr bool u = IsUnique<P>;
}
