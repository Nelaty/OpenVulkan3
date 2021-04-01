/* 
 * Created by Matthias "Nelaty" Gensheimer on 19.03.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once
#include "glm/glm.hpp"

namespace ov
{
    /**
     * \brief The base window class
     */
    class WindowBase
    {
    public:
        virtual bool init() = 0;
        virtual void render() = 0;
        virtual void cleanup() = 0;

        virtual void show(){}
        virtual void hide(){}
    };
}