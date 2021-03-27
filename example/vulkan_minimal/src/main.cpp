/*
 * Created by Matthias "Nelaty" Gensheimer on 25.03.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#include "HelloTriangle.h"

#include <stdexcept>
#include <iostream>


int main()
{
    HelloTriangleApplication app;

    try{
        app.run();
    } catch(const std::exception& e){
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

