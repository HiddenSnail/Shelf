#include <iostream>

#include "libs/crow_all.h"
#include "service/service.h"
#include "utils/md5.h"

using namespace Shelf;
using namespace crow;

int main() {
    // running port
//    Service::exec(10000);
    // test port
    Service::exec(10001);
    return 0;
}