// dot::rope implements copy-on-write reference
// multiple object may refer to single fat cow
// any const method must not change common cow
// any non-const method create its own clone
// if more than one rope refers to the same cow

#pragma once

#include <dot/object.h>
#include <utility>
#include <atomic>

namespace dot
{
    // base class for any rope which is bound to the cow
    class DOT_PUBLIC rope_based : public object
    {
    public:
        rope_based() = default;
        rope_based(const rope_based&) = default;
        rope_based(rope_based&&) = default;
        rope_based(rope_based&);

        template <class fat>
        rope_based(fat&& inst);

        DOT_HIERARCHIC(object);

        class cow_based;
    };

    // rope is bound to some cow
    // such link may be not unique for cow
    // any non-constant operation will copy the bound cow
    // to make any edit operation on its own cow
    // Copy-on-Write pattern is CoW another words
    template <class fat>
    class rope : public rope_based
    {
    public:
        template <class... arguments>
        rope(arguments... args);

        rope(const rope& another);
        rope& operator = (const rope& another);

        rope(rope&& temp);
        rope& operator = (rope&& temp);

        rope(const object& another);
        rope& operator = (const object& another);

        const fat* operator -> () const noexcept;
        fat* operator -> ();

        const fat& operator * () const noexcept;
        fat& operator * ();

        // number of ropes bound to the same cow
        uint64 bound() const noexcept;

        // is the bound cow has unique rope
        bool unique() const noexcept;

        const fat& look() const noexcept;
        fat& touch();

        template <typename other> bool operator == (const rope<other>& another) const;
        template <typename other> bool operator != (const rope<other>& another) const;
        template <typename other> bool operator <= (const rope<other>& another) const;
        template <typename other> bool operator >= (const rope<other>& another) const;
        template <typename other> bool operator <  (const rope<other>& another) const;
        template <typename other> bool operator >  (const rope<other>& another) const;

        template <typename other> bool operator == (const other& another) const;
        template <typename other> bool operator != (const other& another) const;
        template <typename other> bool operator <= (const other& another) const;
        template <typename other> bool operator >= (const other& another) const;
        template <typename other> bool operator <  (const other& another) const;
        template <typename other> bool operator >  (const other& another) const;

        DOT_HIERARCHIC(rope_based);

        class cow;

    private:
        cow* my_cow;
    };

    template <typename left, typename right> bool operator == (const left& x, const rope<right>& y);
    template <typename left, typename right> bool operator != (const left& x, const rope<right>& y);
    template <typename left, typename right> bool operator <= (const left& x, const rope<right>& y);
    template <typename left, typename right> bool operator >= (const left& x, const rope<right>& y);
    template <typename left, typename right> bool operator <  (const left& x, const rope<right>& y);
    template <typename left, typename right> bool operator >  (const left& x, const rope<right>& y);

    // base class for any cow which implements copy-on-write pattern
    class DOT_PUBLIC rope_based::cow_based : public object::data
    {
    public:
        DOT_HIERARCHIC(object::data);
    };

    // Copy-on-Write pattern or another words C-o-W
    // contains fat type value and reference counter
    // any non-const method when more than one rope is bound
    // follows clone of the cow with copy of fat type value
    template <class fat>
    class rope<fat>::cow : public rope_based::cow_based
    {
    public:
        template <class... arguments>
        cow(arguments... args);

        virtual ~cow() noexcept;

        cow(const cow& another);
        cow& operator = (const cow& another);

        cow(cow&& temp) noexcept;
        cow& operator = (cow&& temp);

        uint64 bound() const noexcept;

        const fat& look() const noexcept;
        fat& touch();

        DOT_HIERARCHIC(rope_based::cow_based);

    protected:
        virtual object::data* copy_to(void* buffer) const noexcept override;
        virtual object::data* move_to(void* buffer) noexcept override;

        virtual void write(std::ostream& stream) const override;
        virtual void read(std::istream& stream) override;

        virtual bool equals(const object::data& another) const noexcept override;
        virtual bool less(const object::data& another) const noexcept override;

    private:
        // cow's neck is the place where bound multiple ropes
        // the moment when no rope is bound to the cow's neck
        // cow runs out and goes to be deleted from system memory
        struct neck
        {
            std::atomic<uint64> bound;
            fat value;

            template <typename... arguments>
            neck(arguments... args)
                : bound(1),
                value(std::forward<arguments>(args)...)
            {
            }

            neck* add_rope()
            {
                ++bound;
                return this;
            }

            void remove_rope()
            {
                if (!--bound)
                    delete this;
            }
        };

        neck* my_neck;
    };

// -- implementation of the rope methods --

    template <class fat>
    rope_based::rope_based(fat&& inst)
        : base(std::forward<fat>(inst))
    {
    }

    template <class fat>
    template <class... arguments>
    rope<fat>::rope(arguments... args)
        : my_cow(initialize<cow>(args...))
    {
    }

    template <class fat>
    rope<fat>::rope(const rope& another)
        : my_cow(initialize<cow>(*another.my_cow))
    {
    }

    template <class fat>
    rope<fat>& rope<fat>::operator = (const rope& another)
    {
        my_cow = initialize<cow>(*another.my_cow);
        return *this;
    }

    template <class fat>
    rope<fat>::rope(rope&& temp)
        : my_cow(initialize<cow>(std::move(*temp.my_cow)))
    {
    }

    template <class fat>
    rope<fat>& rope<fat>::operator = (rope&& temp)
    {
        my_cow = initialize<cow>(std::move(*temp.my_cow));
        return *this;
    }

    template <class fat>
    rope<fat>::rope(const object& another)
        : my_cow(initialize<cow>(another.data_as<cow>()))
    {
    }

    template <class fat>
    rope<fat>& rope<fat>::operator = (const object& another)
    {
        my_cow = initialize<cow>(another.data_as<cow>());
        return *this;
    }

    template <class fat>
    const fat* rope<fat>::operator -> () const noexcept
    {
        return &my_cow->look();
    }

    template <class fat>
    fat* rope<fat>::operator -> ()
    {
        return &my_cow->touch();
    }

    template <class fat>
    const fat& rope<fat>::operator * () const noexcept
    {
        return my_cow->look();
    }

    template <class fat>
    fat& rope<fat>::operator * ()
    {
        return my_cow->touch();
    }

    template <class fat>
    uint64 rope<fat>::bound() const noexcept
    {
        return my_cow->bound();
    }

    template <class fat>
    bool rope<fat>::unique() const noexcept
    {
        return my_cow->bound() == 1;
    }

    template <class fat>
    const fat& rope<fat>::look() const noexcept
    {
        return my_cow->look();
    }

    template <class fat>
    fat& rope<fat>::touch()
    {
        return my_cow->touch();
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator == (const rope<other>& another) const
    {
        if constexpr (are_comparable<fat, other>)
        {
            return look() == another.look();
        }
        else
        {
            static_assert(false, "Unable to compare rope_based of non comparable types.");
        }
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator != (const rope<other>& another) const
    {
        return !(*this == another);
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator <= (const rope<other>& another) const
    {
        return !(another < *this);
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator >= (const rope<other>& another) const
    {
        return !(*this < another);
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator < (const rope<other>& another) const
    {
        if constexpr (are_orderable<fat, other>)
        {
            return look() < another.look();
        }
        else
        {
            static_assert(false, "Unable to order rope_based of non orderable types.");
        }
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator > (const rope<other>& another) const
    {
        return another < *this;
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator == (const other& another) const
    {
        if constexpr (are_comparable<fat, other>)
        {
            return look() == another;
        }
        else if constexpr (std::is_base_of_v<object, other>)
        {
            return base::operator == (another);
        }
        else
        {
            static_assert(false, "Unable to compare rope_based with non comparable type.");
        }
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator != (const other& another) const
    {
        return !(*this == another);
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator <= (const other& another) const
    {
        return !(another < *this);
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator >= (const other& another) const
    {
        return !(*this < another);
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator < (const other& another) const
    {
        if constexpr (are_orderable<fat, other>)
        {
            return look() < another;
        }
        else if constexpr (std::is_base_of_v<object, other>)
        {
            return base::operator < (another);
        }
        else
        {
            static_assert(false, "Unable to order rope_based with non orderable type.");
        }
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator > (const other& another) const
    {
        if constexpr (are_orderable<other, fat>)
        {
            return another < look();
        }
        else if constexpr (std::is_base_of_v<object, other>)
        {
            return base::operator > (another);
        }
        else
        {
            static_assert(false, "Unable to order rope_based with non orderable type.");
        }
    }

    template <typename left, typename right>
    bool operator == (const left& x, const rope<right>& y)
    {
        return y == x;
    }

    template <typename left, typename right>
    bool operator != (const left& x, const rope<right>& y)
    {
        return y != x;
    }

    template <typename left, typename right>
    bool operator <= (const left& x, const rope<right>& y)
    {
        return y >= x;
    }

    template <typename left, typename right>
    bool operator >= (const left& x, const rope<right>& y)
    {
        return y <= x;
    }

    template <typename left, typename right>
    bool operator < (const left& x, const rope<right>& y)
    {
        return y > x;
    }

    template <typename left, typename right>
    bool operator > (const left& x, const rope<right>& y)
    {
        return y < x;
    }

    // -- implementation of the cow bound by the rope methods --

    template <class fat>
    template <class... arguments>
    rope<fat>::cow::cow(arguments... args)
        : my_neck(new neck(args...))
    {
    }

    template <class fat>
    rope<fat>::cow::~cow() noexcept
    {
        my_neck->remove_rope();
    }

    template <class fat>
    object::data* rope<fat>::cow::copy_to(void* buffer) const noexcept
    {
        return new(buffer) cow(*this);
    }

    template <class fat>
    object::data* rope<fat>::cow::move_to(void* buffer) noexcept
    {
        return new(buffer) cow(std::move(*this));
    }

    template <class fat>
    void rope<fat>::cow::write(std::ostream& stream) const
    {
        if constexpr (is_writable<fat>)
        {
            stream << look();
        }
        else
        {
            base::write(stream);
        }
    }

    template <class fat>
    void rope<fat>::cow::read(std::istream& stream)
    {
        if constexpr (is_readable<fat>)
        {
            stream >> touch();
        }
        else
        {
            base::read(stream);
        }
    }

    template <class fat>
    rope<fat>::cow::cow(const cow& another)
        : my_neck(another.my_neck->add_rope())
    {
    }

    template <class fat>
    typename rope<fat>::cow&
        rope<fat>::cow::operator = (
            const typename rope<fat>::cow& another)
    {
        neck* old_block = my_neck;
        my_neck = another.my_neck->add_rope();
        old_block->remove_rope();
        return *this;
    }

    template <class fat>
    rope<fat>::cow::cow(cow&& temp) noexcept
        : my_neck(temp.my_neck->add_rope())
    {
        // to avoid unitialized or nullptr temp.m_block instead
        // atomic increment and decrement are very light operations
        // instead of heavy comparisons to nullptr for each method
    }

    template <class fat>
    typename rope<fat>::cow&
        rope<fat>::cow::operator = (
            typename rope<fat>::cow&& temp)
    {
        std::swap(my_neck, temp.my_neck);
        return *this;
    }

    template <class fat>
    uint64 rope<fat>::cow::bound() const noexcept
    {
        return my_neck->bound;
    }

    template <class fat>
    const fat& rope<fat>::cow::look() const noexcept
    {
        return my_neck->value;
    }

    template <class fat>
    fat& rope<fat>::cow::touch()
    {
        if (my_neck->bound > 1)
        {
            neck* old_block = my_neck;
            my_neck = new neck(my_neck->value);
            old_block->remove_rope();
        }
        return my_neck->value;
    }

    template <class fat>
    bool rope<fat>::cow::equals(const object::data& another) const noexcept
    {
        if constexpr (is_comparable<fat>)
        {
            if (another.is<cow>())
                return look() == another.as<cow>().look();
            else
                return base::equals(another);
        }
        else
        {
            return base::equals(another);
        }
    }

    template <class fat>
    bool rope<fat>::cow::less(const object::data& another) const noexcept
    {
        if constexpr (is_orderable<fat>)
        {
            if (another.is<cow>())
                return look() < another.as<cow>().look();
            else
                return base::less(another);
        }
        else
        {
            return base::less(another);
        }
    }
}

// Здесь должен быть Unicode
