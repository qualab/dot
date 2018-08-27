// DOT object base class

#pragma once

#include <dot/type.h>
#include <iosfwd>

namespace dot
{
    // base class for any object with dynamic typification
    class DOT_PUBLIC object : public hierarchic
    {
    public:
        object();
        virtual ~object();

        // work with null object
        virtual void reset();
        virtual bool is_null() const;
        virtual bool is_not_null() const;

        // copy object override
        object(const object& another);
        object& operator = (const object& another);

        // move object override
        object(object&& temporary);
        object& operator = (object&& temporary);

        // base data class for all objects
        class data;

        // enough for two 64-bit fields and virtual table pointer
        static const size_t max_data_size = 3 * sizeof(int64);

        // class identification
        typedef hierarchic base;
        static const class_id& id();
        virtual const class_id& who() const override;

    protected:
        template <typename derived_data, typename... argument_types>
        derived_data* initialize(argument_types... arguments);

    private:
        data* m_data;
        uint8 m_buffer[max_data_size];

        friend DOT_PUBLIC std::ostream& operator << (std::ostream& stream, const object& value);
        friend DOT_PUBLIC std::istream& operator >> (std::istream& stream, object& value);
    };

    // base class for any object data which is dynamically typified
    class DOT_PUBLIC object::data : public hierarchic
    {
    public:
        data();
        virtual ~data();

        // class identification
        typedef hierarchic base;
        static const class_id& id();
        virtual const class_id& who() const override;

    protected:
        // placement into object internal buffer
        virtual data* copy_to(void* buffer) const = 0;
        virtual data* move_to(void* buffer) = 0;

        // stream input and output
        virtual void write(std::ostream& stream) const = 0;
        virtual void read(std::istream& stream) = 0;

        friend class object;
        friend DOT_PUBLIC std::ostream& operator << (std::ostream& stream, const object::data& value);
        friend DOT_PUBLIC std::istream& operator >> (std::istream& stream, object::data& value);
    };

    template <typename derived_data, typename... argument_types>
    derived_data* object::initialize(argument_types... arguments)
    {
        static_assert(sizeof(derived_data) <= object::max_data_size,
            "Size of derived data type is too big for object data internal buffer.");
        derived_data* result = nullptr;
        reset();
        m_data = result = new(m_buffer) derived_data(arguments...);
        return result;
    }
}

// Unicode signature: Владимир Керимов
