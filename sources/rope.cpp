// dot::rope incapsulates Copy-on-Write pattern as rope::cow
// there are implementation of rope and rope::cow methods

#include <dot/rope.h>

namespace dot
{
    rope_based::rope_based()
    {
    }

    DOT_CLASS_ID(rope_based)
    DOT_CLASS_ID(rope_based::cow_based)

    DOT_CLASS_ID(rope<rope_based>)
    DOT_CLASS_ID(rope<rope_based>::cow)
}

// Здесь должен быть Unicode
