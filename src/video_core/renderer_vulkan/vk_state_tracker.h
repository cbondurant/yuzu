// Copyright 2020 yuzu Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <type_traits> // REMOVE ME
#include <utility>

#include "common/common_types.h"
#include "core/core.h"
#include "video_core/dirty_flags.h"
#include "video_core/engines/maxwell_3d.h"

namespace Vulkan {

namespace Dirty {

enum : u8 {
    First = VideoCommon::Dirty::LastCommonEntry,

    Viewports,
    Scissors,
    DepthBias,
};

} // namespace Dirty

class StateTracker {
public:
    explicit StateTracker(Core::System& system);

    void Initialize();

    void InvalidateCommandBufferState();

    bool TouchViewports() {
        return Exchange(Dirty::Viewports, false);
    }

    bool TouchScissors() {
        return Exchange(Dirty::Scissors, false);
    }

    bool TouchDepthBias() {
        return Exchange(Dirty::DepthBias, false);
    }

private:
    using Flags = std::remove_reference_t<decltype(Tegra::Engines::Maxwell3D::dirty.flags)>;

    bool Exchange(std::size_t id, bool new_value) const noexcept {
        auto& flags = system.GPU().Maxwell3D().dirty.flags;
        const bool is_dirty = flags[id];
        flags[id] = new_value;
        return is_dirty;
    }

    Core::System& system;
    Flags invalidation_flags;
};

} // namespace Vulkan
