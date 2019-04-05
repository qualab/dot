// dot::box<slim> contains value of slim type
// which size allows ot store it in-place (by value)
// this is suitable for simple values and small structures
// data class box<slim>::cat is simply contain its value

#pragma once

#include <dot/object.h>
#include <utility>

namespace dot
{
    // base class for any other box
    class DOT_PUBLIC box_based : public object
    {
    public:
        box_based();

        template <class slim>
        explicit box_based(slim&& value);

        DOT_HIERARCHIC(object);

        class cat_based;
    };

    // box contains slim cat which is liquid enough
    // to be suitable for internal object's buffer
    template <class slim>
    class box : public box_based
    {
    public:
        box(const object& another);

        template <class... arguments>
        explicit box(arguments... args);

        const slim& look() const noexcept;
        slim& touch() noexcept;

        template <class other> bool operator == (const box<other>& another) const;
        template <class other> bool operator != (const box<other>& another) const;
        template <class other> bool operator <= (const box<other>& another) const;
        template <class other> bool operator >= (const box<other>& another) const;
        template <class other> bool operator <  (const box<other>& another) const;
        template <class other> bool operator >  (const box<other>& another) const;

        template <class other> bool operator == (const other& another) const;
        template <class other> bool operator != (const other& another) const;
        template <class other> bool operator <= (const other& another) const;
        template <class other> bool operator >= (const other& another) const;
        template <class other> bool operator <  (const other& another) const;
        template <class other> bool operator >  (const other& another) const;

        DOT_HIERARCHIC(box_based);

        class cat;

    private:
        cat* my_cat;
    };

    template <typename left, typename right> bool operator == (const left& x, const box<right>& y);
    template <typename left, typename right> bool operator != (const left& x, const box<right>& y);
    template <typename left, typename right> bool operator <= (const left& x, const box<right>& y);
    template <typename left, typename right> bool operator >= (const left& x, const box<right>& y);
    template <typename left, typename right> bool operator <  (const left& x, const box<right>& y);
    template <typename left, typename right> bool operator >  (const left& x, const box<right>& y);

    // base class for any cat in the box
    class DOT_PUBLIC box_based::cat_based : public object::data
    {
    public:
        DOT_HIERARCHIC(object::data);
    };

    // data contains value in-placed as a field
    template <class slim>
    class box<slim>::cat : public box_based::cat_based
    {
    public:
        cat(const cat& another);

        template <class... arguments>
        cat(arguments... args);

        const slim& look() const noexcept;
        slim& touch() noexcept;

        DOT_HIERARCHIC(box_based::cat_based);

    protected:
        virtual object::data* copy_to(void* buffer) const noexcept override;
        virtual object::data* move_to(void* buffer) noexcept override;

        virtual void write(std::ostream& stream) const override;
        virtual void read(std::istream& stream) override;

        virtual bool equals(const object::data& another) const noexcept override;
        virtual bool less(const object::data& another) const noexcept override;

    private:
        slim my_value;
    };

    // -- implementation of the box methods --

    template <class slim>
    box_based::box_based(slim&& value)
        : base(std::forward<slim>(value))
    {
    }

    template <class slim>
    box<slim>::box(const object& another)
        : my_cat(initialize<box<slim>::cat>(
            another.data_as<box<slim>::cat>()))
    {
    }

    template <class slim>
    template <class... arguments>
    box<slim>::box(arguments... args)
        : my_cat(initialize<cat>(std::forward<arguments>(args)...))
    {
    }

    template <class slim>
    const slim& box<slim>::look() const noexcept
    {
        return my_cat->look();
    }

    template <class slim>
    slim& box<slim>::touch() noexcept
    {
        return my_cat->touch();
    }

    template <class slim>
    template <class other>
    bool box<slim>::operator == (const box<other>& another) const
    {
        if constexpr (are_comparable<slim, other>)
        {
            return look() == another.look();
        }
        else
        {
            return my_cat->equals(another.get_data());
        }
    }

    template <class slim>
    template <class other>
    bool box<slim>::operator != (const box<other>& another) const
    {
        return !(*this == another);
    }

    template <class slim>
    template <class other>
    bool box<slim>::operator <= (const box<other>& another) const
    {
        return !(another < *this);
    }

    template <class slim>
    template <class other>
    bool box<slim>::operator >= (const box<other>& another) const
    {
        return !(*this < another);
    }

    template <class slim>
    template <class other>
    bool box<slim>::operator < (const box<other>& another) const
    {
        if constexpr (are_orderable<slim, other>)
        {
            return look() < another.look();
        }
        else
        {
            return my_cat->less(another.get_data());
        }
    }

    template <class slim>
    template <class other>
    bool box<slim>::operator > (const box<other>& another) const
    {
        return another < *this;
    }

    template <class slim>
    template <class other>
    bool box<slim>::operator == (const other& another) const
    {
        if constexpr (are_comparable<slim, other>)
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
            throw fail::non_comparable("Unable to compare box_based with non comparable type.");
        }
    }

    template <class slim>
    template <class other>
    bool box<slim>::operator != (const other& another) const
    {
        return !(*this == another);
    }

    template <class slim>
    template <class other>
    bool box<slim>::operator <= (const other& another) const
    {
        return !(*this > another);
    }

    template <class slim>
    template <class other>
    bool box<slim>::operator >= (const other& another) const
    {
        return !(*this < another);
    }

    template <class slim>
    template <class other>
    bool box<slim>::operator < (const other& another) const
    {
        if (are_orderable<slim, other>)
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
            throw fail::non_orderable("Unable to order box_based with non orderable type.");
        }
    }

    template <class slim>
    template <class other>
    bool box<slim>::operator > (const other& another) const
    {
        if (are_orderable<other, slim>)
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
            throw fail::non_orderable("Unable to order box_based with non orderable type.");
        }
    }

    template <typename left, typename right>
    bool operator == (const left& x, const box<right>& y)
    {
        return y == x;
    }

    template <typename left, typename right>
    bool operator != (const left& x, const box<right>& y)
    {
        return y != x;
    }

    template <typename left, typename right>
    bool operator <= (const left& x, const box<right>& y)
    {
        return y >= x;
    }

    template <typename left, typename right>
    bool operator >= (const left& x, const box<right>& y)
    {
        return y <= x;
    }

    template <typename left, typename right>
    bool operator < (const left& x, const box<right>& y)
    {
        return y > x;
    }

    template <typename left, typename right>
    bool operator > (const left& x, const box<right>& y)
    {
        return y < x;
    }

    // -- implementation of the cat in the box methods --

    template <class slim>
    box<slim>::cat::cat(const cat& another)
        : my_value(another.my_value)
    {
    }

    template <class slim>
    template <class... arguments>
    box<slim>::cat::cat(arguments... args)
        : my_value(std::forward<arguments>(args)...)
    {
    }

    template <class slim>
    const slim& box<slim>::cat::look() const noexcept
    {
        return my_value;
    }

    template <class slim>
    slim& box<slim>::cat::touch() noexcept
    {
        return my_value;
    }

    template <class slim>
    object::data* box<slim>::cat::copy_to(void* buffer) const noexcept
    {
        return new(buffer) cat(*this);
    }

    template <class slim>
    object::data* box<slim>::cat::move_to(void* buffer) noexcept
    {
        return new(buffer) cat(std::move(*this));
    }

    template <class slim>
    void box<slim>::cat::write(std::ostream& stream) const
    {
        if constexpr (is_writable<slim>)
        {
            stream << look();
        }
        else
        {
            base::write(stream);
        }
    }

    template <class slim>
    void box<slim>::cat::read(std::istream& stream)
    {
        if constexpr (is_readable<slim>)
        {
            stream >> touch();
        }
        else
        {
            base::read(stream);
        }
    }

    template <class slim>
    bool box<slim>::cat::equals(const object::data& another) const noexcept
    {
        if constexpr (is_comparable<slim>)
        {
            if (another.is<cat>())
                return look() == another.as<cat>().look();
            else
                return base::equals(another);
        }
        else
        {
            return base::equals(another);
        }
    }

    template <class slim>
    bool box<slim>::cat::less(const object::data& another) const noexcept
    {
        if constexpr (is_orderable<slim>)
        {
            if (another.is<cat>())
                return look() < another.as<cat>().look();
            else
                return base::equals(another);
        }
        else
        {
            return base::equals(another);
        }
    }

    // -- definition of the identifiers for the boxes of native types --

    template<> DOT_PUBLIC const class_id& box<long long>::id() noexcept;
    template<> DOT_PUBLIC const class_id& box<long     >::id() noexcept;
    template<> DOT_PUBLIC const class_id& box<int      >::id() noexcept;
    template<> DOT_PUBLIC const class_id& box<short    >::id() noexcept;
    template<> DOT_PUBLIC const class_id& box<char     >::id() noexcept;

    template<> DOT_PUBLIC const class_id& box<unsigned long long>::id() noexcept;
    template<> DOT_PUBLIC const class_id& box<unsigned long     >::id() noexcept;
    template<> DOT_PUBLIC const class_id& box<unsigned int      >::id() noexcept;
    template<> DOT_PUBLIC const class_id& box<unsigned short    >::id() noexcept;
    template<> DOT_PUBLIC const class_id& box<unsigned char     >::id() noexcept;

    template<> DOT_PUBLIC const class_id& box<double>::id() noexcept;
    template<> DOT_PUBLIC const class_id& box<float >::id() noexcept;

    template<> DOT_PUBLIC const class_id& box<bool>::id() noexcept;
    template<> DOT_PUBLIC const class_id& box<char>::id() noexcept;

    // -- definition of the identifiers for the cats in the box of native types --

    template<> DOT_PUBLIC const class_id& box<long long>::cat::id() noexcept;
    template<> DOT_PUBLIC const class_id& box<long     >::cat::id() noexcept;
    template<> DOT_PUBLIC const class_id& box<int      >::cat::id() noexcept;
    template<> DOT_PUBLIC const class_id& box<short    >::cat::id() noexcept;
    template<> DOT_PUBLIC const class_id& box<char     >::cat::id() noexcept;

    template<> DOT_PUBLIC const class_id& box<unsigned long long>::cat::id() noexcept;
    template<> DOT_PUBLIC const class_id& box<unsigned long     >::cat::id() noexcept;
    template<> DOT_PUBLIC const class_id& box<unsigned int      >::cat::id() noexcept;
    template<> DOT_PUBLIC const class_id& box<unsigned short    >::cat::id() noexcept;
    template<> DOT_PUBLIC const class_id& box<unsigned char     >::cat::id() noexcept;

    template<> DOT_PUBLIC const class_id& box<double>::cat::id() noexcept;
    template<> DOT_PUBLIC const class_id& box<float >::cat::id() noexcept;

    template<> DOT_PUBLIC const class_id& box<bool>::cat::id() noexcept;
}

// Здесь должен быть Unicode
