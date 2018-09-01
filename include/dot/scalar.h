// DOT copy-on-write object

#pragma once

#include <dot/object.h>
#include <utility>

namespace dot
{
    template <typename value_type>
    class scalar : public object
    {
    public:
        template <typename... argument_types>
        scalar(argument_types... arguments);

        const value_type& value() const;
        value_type& value();

        typedef object base;
        static const class_id& id();
        virtual const class_id& who() const override;

        class data;

    private:
        data* m_data;
    };

    template <typename value_type>
    class scalar<value_type>::data : public object::data
    {
    public:
        data(const value_type& value);

        template <typename... argument_types>
        data(argument_types... arguments);

        const value_type& value() const;
        value_type& value();

        typedef object::data base;
        static const class_id& id();
        virtual const class_id& who() const override;

    protected:
        virtual object::data* copy_to(void* buffer) const override;
        virtual object::data* move_to(void* buffer) override;
        virtual void write(std::ostream& stream) const override;
        virtual void read(std::istream& stream) override;

    private:
        value_type m_value;
    };

    template <typename value_type>
    template <typename... argument_types>
    scalar<value_type>::scalar(argument_types... arguments)
        : m_data(initialize<data>(arguments...))
    {
    }

    template <typename value_type>
    const value_type& scalar<value_type>::value() const
    {
        return m_data->value();
    }

    template <typename value_type>
    value_type& scalar<value_type>::value()
    {
        return m_data->value();
    }

    template <typename value_type>
    const class_id& scalar<value_type>::id()
    {
        static const class_id scalar_id("scalar");
        return scalar_id;
    }

    template <typename value_type>
    const class_id& scalar<value_type>::who() const
    {
        return scalar<value_type>::id();
    }

    template <typename value_type>
    scalar<value_type>::data::data(const value_type& value)
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
    const value_type& scalar<value_type>::data::value() const
    {
        return m_value;
    }

    template <typename value_type>
    value_type& scalar<value_type>::data::value()
    {
        return m_value;
    }

    template <typename value_type>
    const class_id& scalar<value_type>::data::id()
    {
        static const class_id scalar_data_id("scalar::data");
        return scalar_data_id;
    }

    template <typename value_type>
    const class_id& scalar<value_type>::data::who() const
    {
        return scalar<value_type>::data::id();
    }

    template <typename value_type>
    object::data* scalar<value_type>::data::copy_to(void* buffer) const
    {
        return new(buffer) data(*this);
    }

    template <typename value_type>
    object::data* scalar<value_type>::data::move_to(void* buffer)
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
}

// Unicode signature: Владимир Керимов
