// dot::copyable copy-on-write reference object
// multiple object may refer to single fat
// any const method must not change common fat
// any non-const method create its own copy if
// more than one object refers same fat

#pragma once

#include <dot/object.h>
#include <utility>
#include <atomic>

namespace dot
{
    // base class for any rope which is bound to the cow
    class DOT_PUBLIC ropes : public object
    {
    public:
        ropes();

        template <class fat>
        ropes(fat&& inst);

        DOT_HIERARCHIC(object);

        class cows;
    };

    // rope is bound to some cow
    // such link may be not unique for cow
    // any non-constant operation will copy the bound cow
    // to make any edit operation on its own cow
    // Copy-on-Write pattern is CoW another words
    template <class fat>
    class rope : public ropes
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

        DOT_HIERARCHIC(ropes);

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

    // base class for any data of copyable_of<fat>
    class DOT_PUBLIC ropes::cows : public object::data
    {
    public:
        DOT_HIERARCHIC(object::data);
    };

    // data contains reference to a copyable fat as pointer to memory block
    // which contain fat and reference counter (standard atomic)
    // any non-const method on fat when reference count is above one
    // follows copy own memory block to refer it without change of original
    template <class fat>
    class rope<fat>::cow : public ropes::cows
    {
    public:
        template <class... arguments>
        cow(arguments... args);

        virtual ~cow() noexcept;

        cow(const cow& another);
        cow& operator = (const cow& another);

        cow(cow&& temp);
        cow& operator = (cow&& temp);

        uint64 bound() const noexcept;

        const fat& look() const noexcept;
        fat& touch();

        DOT_HIERARCHIC(ropes::cows);

    protected:
        virtual object::data* copy_to(void* buffer) const noexcept override;
        virtual object::data* move_to(void* buffer) noexcept override;

        virtual void write(std::ostream& stream) const override;
        virtual void read(std::istream& stream) override;

        virtual bool equals(const object::data& another) const noexcept override;
        virtual bool less(const object::data& another) const noexcept override;

    private:
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

    // -- methods implementation of copyable --

    template <class fat>
    ropes::ropes(fat&& inst)
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
            throw fail::non_comparable("Unable to compare copyables of non comparable types.");
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
            throw fail::non_orderable("Unable to order copyables of non orderable types.");
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
            another;
            throw fail::non_comparable("Unable to compare ropes with non comparable type.");
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
            another;
            throw fail::non_orderable("Unable to order ropes with non orderable type.");
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
            another;
            throw fail::non_orderable("Unable to order ropes with non orderable type.");
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

    // -- methods implementation of copyable data --

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
    rope<fat>::cow::cow(cow&& temp)
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
