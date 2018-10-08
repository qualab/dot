// DOT scalar data object

#pragma once

#include <dot/object.h>
#include <utility>

namespace dot
{
    template <typename value_type>
    class scalar : public object
    {
    public:
        scalar(const object& another);

        template <typename... argument_types>
        scalar(argument_types... arguments);

        value_type get() const noexcept;
        value_type& ref();

        typedef object base;
        static const class_id& id() noexcept;
        virtual const class_id& who() const noexcept override;

        class data;

        const data& scalar_data() const noexcept;

    private:
        data* m_data;
    };

    template <typename value_type>
    class scalar<value_type>::data : public object::data
    {
    public:
        data(const data& another);

        template <typename... argument_types>
        data(argument_types... arguments);

        value_type get() const noexcept;
        value_type& ref() noexcept;

        typedef object::data base;
        static const class_id& id() noexcept;
        virtual const class_id& who() const noexcept override;

    protected:
        virtual object::data* copy_to(void* buffer) const noexcept override;
        virtual object::data* move_to(void* buffer) noexcept override;
        virtual void write(std::ostream& stream) const override;
        virtual void read(std::istream& stream) override;

    private:
        value_type m_value;
    };

    template <typename value_type>
    scalar<value_type>::scalar(const object& another)
        : m_data(initialize<scalar<value_type>::data>(
            another.data_as<scalar<value_type>::data>()))
    {
    }

    template <typename value_type>
    template <typename... argument_types>
    scalar<value_type>::scalar(argument_types... arguments)
        : m_data(initialize<data>(arguments...))
    {
    }

    template <typename value_type>
    value_type scalar<value_type>::get() const noexcept
    {
        return m_data->get();
    }

    template <typename value_type>
    value_type& scalar<value_type>::ref()
    {
        return m_data->ref();
    }

    template <typename value_type>
    const class_id& scalar<value_type>::who() const noexcept
    {
        return scalar<value_type>::id();
    }

    template <typename value_type>
    const typename scalar<value_type>::data& scalar<value_type>::scalar_data() const noexcept
    {
        return *m_data;
    }

    template <typename value_type>
    scalar<value_type>::data::data(const data& another)
        : m_value(another.m_value)
    {
    }

    template <typename value_type>
    template <typename... argument_types>
    scalar<value_type>::data::data(argument_types... arguments)
        : m_value(arguments...)
    {
    }

    template <typename value_type>
    value_type scalar<value_type>::data::get() const noexcept
    {
        return m_value;
    }

    template <typename value_type>
    value_type& scalar<value_type>::data::ref() noexcept
    {
        return m_value;
    }

    template <typename value_type>
    const class_id& scalar<value_type>::data::who() const noexcept
    {
        return scalar<value_type>::data::id();
    }

    template <typename value_type>
    object::data* scalar<value_type>::data::copy_to(void* buffer) const noexcept
    {
        return new(buffer) data(*this);
    }

    template <typename value_type>
    object::data* scalar<value_type>::data::move_to(void* buffer) noexcept
    {
        return new(buffer) data(std::move(*this));
    }

    template <typename value_type>
    void scalar<value_type>::data::write(std::ostream& stream) const
    {
        stream << m_value;
    }

    template <typename value_type>
    void scalar<value_type>::data::read(std::istream& stream)
    {
        stream >> m_value;
    }

    template<> DOT_PUBLIC const class_id& scalar<long long>::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<long     >::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<int      >::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<short    >::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<char     >::id() noexcept;

    template<> DOT_PUBLIC const class_id& scalar<unsigned long long>::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<unsigned long     >::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<unsigned int      >::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<unsigned short    >::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<unsigned char     >::id() noexcept;

    template<> DOT_PUBLIC const class_id& scalar<double>::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<float >::id() noexcept;

    template<> DOT_PUBLIC const class_id& scalar<bool>::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<char>::id() noexcept;

    template<> DOT_PUBLIC const class_id& scalar<long long>::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<long     >::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<int      >::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<short    >::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<char     >::data::id() noexcept;

    template<> DOT_PUBLIC const class_id& scalar<unsigned long long>::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<unsigned long     >::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<unsigned int      >::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<unsigned short    >::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<unsigned char     >::data::id() noexcept;

    template<> DOT_PUBLIC const class_id& scalar<double>::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<float >::data::id() noexcept;

    template<> DOT_PUBLIC const class_id& scalar<bool>::data::id() noexcept;
}

// Unicode signature: Владимир Керимов
