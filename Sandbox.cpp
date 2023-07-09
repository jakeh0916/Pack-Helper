#include "include/Pack.h"
#include "include/PackAlgorithms.h"

int main()
{
    using namespace packhelp;
    using P = Pack<int, const char*>;

    constexpr int b = HasAt<P, float>::Value;
}
