// dot::rope incapsulates Copy-on-Write pattern as rope::cow
// there are implementation of rope and rope::cow methods

#include <dot/rope.h>

namespace dot
{
    rope_based::rope_based()
    {
    }

    const class_id& rope_based::id() noexcept
    {
        static const class_id ropes_id("rope_based");
        return ropes_id;
    }

    const class_id& rope_based::cow_based::id() noexcept
    {
        static const class_id ropes_cows_id("rope_based::cow_based");
        return ropes_cows_id;
    }
}

// Здесь должен быть Unicode
