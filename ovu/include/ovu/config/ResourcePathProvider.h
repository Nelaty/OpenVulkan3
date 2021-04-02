/* 
 * Created by Matthias "Nelaty" Gensheimer on 02.04.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once
#include <filesystem>
#include <iostream>

class ResourcePathProvider
{
public:
    static std::filesystem::path getRoot()
    {
        // TODO: copy resources after build to binary location
        static constexpr const char* tempProjectRoot = "../../../";

        std::filesystem::path result = tempProjectRoot;
        result /= s_libraryDirName;
        result /= s_resourceDirName;
        return result;
    }

    static std::filesystem::path getShaderRoot()
    {
        return getRoot() / s_shaderDirName;
    }

    static std::filesystem::path getShaderPath(std::string_view fileName)
    {
        return getShaderRoot() / fileName;
    }

private:
    static constexpr const char* s_libraryDirName = "ovu";
    static constexpr const char* s_resourceDirName = "res";
    static constexpr const char* s_shaderDirName = "shader";
};