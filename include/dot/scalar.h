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
        scalar();
        scalar(value_type value);

        template <typename... argument_types>
        scalar(argument_types... arguments);

        value_type get() const noexcept;
        value_type& ref();

        typedef object base;
        static const class_id& id() noexcept;
        virtual const class_id& who() const noexcept override;

        class data;

    private:
        data* m_data;
    };

    template <typename value_type>
    class scalar<value_type>::data : public object::data
    {
    public:
        data();
        data(value_type value);

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
    scalar<value_type>::scalar()
        : m_data(initialize<data>())
    {
    }

    template <typename value_type>
    scalar<value_type>::scalar(value_type value)
        : m_data(initialize<data>(value))
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
    scalar<value_type>::data::data()
        : m_value()
    {
    }

    template <typename value_type>
    scalar<value_type>::data::data(value_type value)
        : m_value(value)
    {
    }

    template <typename value_type>
    template <typename... argument_types>
    scalar<value_type>::data::data(argument_types... arguments)
        : m_value(arguments....)
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

    template<> DOT_PUBLIC const class_id& scalar<int64>::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<int32>::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<int16>::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<int8 >::id() noexcept;

    template<> DOT_PUBLIC const class_id& scalar<uint64>::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<uint32>::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<uint16>::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<uint8 >::id() noexcept;

    template<> DOT_PUBLIC const class_id& scalar<double>::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<float >::id() noexcept;

    template<> DOT_PUBLIC const class_id& scalar<bool>::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<char>::id() noexcept;

    template<> DOT_PUBLIC const class_id& scalar<int64>::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<int32>::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<int16>::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<int8 >::data::id() noexcept;

    template<> DOT_PUBLIC const class_id& scalar<uint64>::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<uint32>::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<uint16>::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<uint8 >::data::id() noexcept;

    template<> DOT_PUBLIC const class_id& scalar<double>::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<float >::data::id() noexcept;

    template<> DOT_PUBLIC const class_id& scalar<bool>::data::id() noexcept;
    template<> DOT_PUBLIC const class_id& scalar<char>::data::id() noexcept;
}

// Unicode signature: Владимир Керимов
