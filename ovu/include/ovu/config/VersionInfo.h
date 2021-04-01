/* 
 * Created by Matthias "Nelaty" Gensheimer on 01.04.21.
 * Copyright (c) 2021 Matthias Gensheimer. All rights reserved.
 */

#pragma once
#include "fmt/format.h"

#include <string>
#include <cstdint>

namespace ovu
{
    struct VersionInfo {
        VersionInfo(uint32_t major, uint32_t minor,
                    uint32_t patch, uint32_t revision)
            : m_major{major},
              m_minor{minor},
              m_patch{patch},
              m_revision{revision}{
        }

        uint32_t toVulkanVersion() const {
            return VK_MAKE_VERSION(m_major, m_minor, m_patch);
        }

        std::string toString() const {
            return fmt::format("{}.{}.{}.{}", m_major, m_minor, m_patch, m_revision);
        }

        uint32_t m_major;
        uint32_t m_minor;
        uint32_t m_patch;
        uint32_t m_revision;
    };
}