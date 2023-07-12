#include "include/Pack.h"
#include "include/PackAlgorithms.h"

#include <string>

int main()
{
    using namespace packhelp;
    using P = Pack<int, const char*, double, double, double, std::string, std::string&&, const char*, const char*&>;

    using T = packhelp::At<0, P>;
}
