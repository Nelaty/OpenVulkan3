/* 
 * Created by Matthias "Nelaty" Gensheimer on 19.03.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */
#define CATCH_CONFIG_RUNNER
#include "test/catch.hpp"

int main(int argc, char** argv)
{
    int result = Catch::Session().run(argc, argv);
    return result;
}