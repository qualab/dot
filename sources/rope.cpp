// dot::rope incapsulates Copy-on-Write pattern as rope::cow
// there are implementation of rope and rope::cow methods

#include <dot/rope.h>

namespace dot
{
    ropes::ropes()
    {
    }

    const class_id& ropes::id() noexcept
    {
        static const class_id ropes_id("ropes");
        return ropes_id;
    }

    const class_id& ropes::cows::id() noexcept
    {
        static const class_id ropes_cows_id("ropes::cows");
        return ropes_cows_id;
    }
}

// Здесь должен быть Unicode
