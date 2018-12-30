// DOT scalar data object

#pragma once

#include <dot/object.h>
#include <utility>
#include <optional>

namespace dot
{
    class DOT_PUBLIC scalar : public object
    {
    public:
        scalar();

        template <typename value_type>
        explicit scalar(value_type&& value);

        DOT_HIERARCHIC(object);
    };

    template <typename value_type>
    class scalar_of : public scalar
    {
    public:
        scalar_of(const object& another);

        template <typename... argument_types>
        explicit scalar_of(argument_types... arguments);

        value_type get() const noexcept;
        const value_type& ref() const noexcept;
        value_type& ref() noexcept;

        bool operator == (const scalar_of<value_type>& another) const;
        bool operator != (const scalar_of<value_type>& another) const;
        bool operator <= (const scalar_of<value_type>& another) const;
        bool operator >= (const scalar_of<value_type>& another) const;
        bool operator <  (const scalar_of<value_type>& another) const;
        bool operator >  (const scalar_of<value_type>& another) const;

        template <typename another_type> bool operator == (const another_type& another) const;
        template <typename another_type> bool operator != (const another_type& another) const;
        template <typename another_type> bool operator <= (const another_type& another) const;
        template <typename another_type> bool operator >= (const another_type& another) const;
        template <typename another_type> bool operator <  (const another_type& another) const;
        template <typename another_type> bool operator >  (const another_type& another) const;

        DOT_HIERARCHIC(scalar);

        class data;

        const data& scalar_data() const noexcept;

    private:
        data* m_data;
    };

    template <typename value_type>
    class scalar_of<value_type>::data : public object::data
    {
    public:
        data(const data& another);

        template <typename... argument_types>
        data(argument_types... arguments);

        value_type get() const noexcept;
        const value_type& ref() const noexcept;
        value_type& ref() noexcept;

        DOT_HIERARCHIC(object::data);

    protected:
        virtual object::data* copy_to(void* buffer) const noexcept override;
        virtual object::data* move_to(void* buffer) noexcept override;

        virtual void write(std::ostream& stream) const override;
        virtual void read(std::istream& stream) override;

        virtual bool equals(const object::data& another) const noexcept override;
        virtual bool less(const object::data& another) const noexcept override;

    private:
        value_type m_value;
    };

    template <typename value_type>
    scalar::scalar(value_type&& value)
        : base(std::forward<value_type>(value))
    {
    }

    template <typename value_type>
    scalar_of<value_type>::scalar_of(const object& another)
        : m_data(initialize<scalar_of<value_type>::data>(
            another.data_as<scalar_of<value_type>::data>()))
    {
    }

    template <typename value_type>
    template <typename... argument_types>
    scalar_of<value_type>::scalar_of(argument_types... arguments)
        : m_data(initialize<data>(std::forward<argument_types>(arguments)...))
    {
    }

    template <typename value_type>
    value_type scalar_of<value_type>::get() const noexcept
    {
        return m_data->get();
    }

    template <typename value_type>
    const value_type& scalar_of<value_type>::ref() const noexcept
    {
        return m_data->ref();
    }

    template <typename value_type>
    value_type& scalar_of<value_type>::ref() noexcept
    {
        return m_data->ref();
    }

    template <typename value_type>
    bool scalar_of<value_type>::operator == (const scalar_of<value_type>& another) const
    {
        if constexpr (is_comparable<value_type>)
        {
            return get() == another.get();
        }
        else
        {
            return m_data->equals(another.get_data());
        }
    }

    template <typename value_type>
    bool scalar_of<value_type>::operator != (const scalar_of<value_type>& another) const
    {
        return !(*this == another);
    }

    template <typename value_type>
    bool scalar_of<value_type>::operator <= (const scalar_of<value_type>& another) const
    {
        return !(*this > another);
    }

    template <typename value_type>
    bool scalar_of<value_type>::operator >= (const scalar_of<value_type>& another) const
    {
        return !(*this < another);
    }

    template <typename value_type>
    bool scalar_of<value_type>::operator < (const scalar_of<value_type>& another) const
    {
        if constexpr (is_orderable<value_type>)
        {
            return get() < another.get();
        }
        else
        {
            return m_data->less(another.get_data());
        }
    }

    template <typename value_type>
    bool scalar_of<value_type>::operator > (const scalar_of<value_type>& another) const
    {
        return another < *this;
    }

    template <typename value_type>
    template <typename another_type>
    bool scalar_of<value_type>::operator == (const another_type& another) const
    {
        if (are_comparable<value_type, another_type>)
        {
            return get() == another;
        }
        else
        {
            return object::operator == (object(another));
        }
    }

    template <typename value_type>
    template <typename another_type>
    bool scalar_of<value_type>::operator != (const another_type& another) const
    {
        return !(*this == another);
    }

    template <typename value_type>
    template <typename another_type>
    bool scalar_of<value_type>::operator <= (const another_type& another) const
    {
        return !(another < *this);
    }

    template <typename value_type>
    template <typename another_type>
    bool scalar_of<value_type>::operator >= (const another_type& another) const
    {
        return !(*this < another);
    }

    template <typename value_type>
    template <typename another_type>
    bool scalar_of<value_type>::operator < (const another_type& another) const
    {
        if (are_orderable<value_type, another_type>)
        {
            return get() < another;
        }
        else
        {
            return object::operator == (object(another));
        }
    }

    template <typename value_type>
    template <typename another_type>
    bool scalar_of<value_type>::operator > (const another_type& another) const
    {
        return another < *this;
    }

    template <typename another_type, typename value_type>
    bool operator == (another_type&& left, scalar_of<value_type>&& right)
    {
        if (are_comparable<another_type, value_type>)
        {
            return left == right.get();
        }
        else
        {
            return operator == (object(left), right);
        }
    }

    template <typename value_type>
    const class_id& scalar_of<value_type>::who() const noexcept
    {
        return scalar_of<value_type>::id();
    }

    template <typename value_type>
    const typename scalar_of<value_type>::data& scalar_of<value_type>::scalar_data() const noexcept
    {
        return *m_data;
    }

    template <typename value_type>
    scalar_of<value_type>::data::data(const data& another)
        : m_value(another.m_value)
    {
    }

    template <typename value_type>
    template <typename... argument_types>
    scalar_of<value_type>::data::data(argument_types... arguments)
        : m_value(arguments...)
    {
    }

    template <typename value_type>
    value_type scalar_of<value_type>::data::get() const noexcept
    {
        return m_value;
    }

    template <typename value_type>
    const value_type& scalar_of<value_type>::data::ref() const noexcept
    {
        return m_value;
    }

    template <typename value_type>
    value_type& scalar_of<value_type>::data::ref() noexcept
    {
        return m_value;
    }

    template <typename value_type>
    const class_id& scalar_of<value_type>::data::who() const noexcept
    {
        return scalar_of<value_type>::data::id();
    }

    template <typename value_type>
    object::data* scalar_of<value_type>::data::copy_to(void* buffer) const noexcept
    {
        return new(buffer) data(*this);
    }

    template <typename value_type>
    object::data* scalar_of<value_type>::data::move_to(void* buffer) noexcept
    {
        return new(buffer) data(std::move(*this));
    }

    template <typename value_type>
    void scalar_of<value_type>::data::write(std::ostream& stream) const
    {
        if constexpr (is_writable<value_type>)
        {
            stream << m_value;
        }
        else
        {
            base::write(stream);
        }
    }

    template <typename value_type>
    void scalar_of<value_type>::data::read(std::istream& stream)
    {
        if constexpr (is_readable<value_type>)
        {
            stream >> m_value;
        }
        else
        {
            base::read(stream);
        }
    }

    template <typename value_type>
    bool scalar_of<value_type>::data::equals(const object::data& another) const noexcept
    {
        if constexpr (is_comparable<value_type>)
        {
            if (another.is<data>())
                return get() == another.as<data>().get();
            else
                return base::equals(another);
        }
        else
        {
            return base::equals(another);
        }
    }

    template <typename value_type>
    bool scalar_of<value_type>::data::less(const object::data& another) const noexcept
    {
        if constexpr (is_orderable<value_type>)
        {
            if (another.is<data>())
                return get() < another.as<data>().get();
            else
                return base::equals(another);
        }
        else
        {
            return base::equals(another);
        }
    }

    template<> DOT_PUBLIC const class_id& scalar_of<long long>::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar_of<long     >::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar_of<int      >::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar_of<short    >::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar_of<char     >::id() noexcept;

    template<> DOT_PUBLIC const class_id& scalar_of<unsigned long long>::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar_of<unsigned long     >::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar_of<unsigned int      >::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar_of<unsigned short    >::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar_of<unsigned char     >::id() noexcept;

    template<> DOT_PUBLIC const class_id& scalar_of<double>::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar_of<float >::id() noexcept;

    template<> DOT_PUBLIC const class_id& scalar_of<bool>::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar_of<char>::id() noexcept;

    template<> DOT_PUBLIC const class_id& scalar_of<long long>::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar_of<long     >::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar_of<int      >::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar_of<short    >::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar_of<char     >::data::id() noexcept;

    template<> DOT_PUBLIC const class_id& scalar_of<unsigned long long>::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar_of<unsigned long     >::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar_of<unsigned int      >::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar_of<unsigned short    >::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar_of<unsigned char     >::data::id() noexcept;

    template<> DOT_PUBLIC const class_id& scalar_of<double>::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar_of<float >::data::id() noexcept;

    template<> DOT_PUBLIC const class_id& scalar_of<bool>::data::id() noexcept;
}

// Unicode signature: Владимир Керимов
