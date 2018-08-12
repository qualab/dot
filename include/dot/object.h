// DOT object base class

#pragma once

#include <dot/type.h>

namespace dot
{
    // base class for any object with dynamic typification
    class DOT_PUBLIC object : public basement
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
        static constexpr size_t max_data_size = 3 * sizeof(int64);

        // class identification
        typedef basement base;
        static class_name_type class_name;
        virtual class_name_type get_class_name() const override;
        virtual class_id&& get_class_id() const override;

    protected:
        template <typename derived_data, typename... argument_types>
        derived_data* initialize(argument_types... arguments)
        {
            static_cast(sizeof(derived_type) < max_data_size,
                "Size of derived data type is too big for object data internal buffer.");
            derived_data* result = nullptr;
            reset();
            m_data = result = new(m_buffer) derived_data(arguments...);
            return result;
        }

    private:
        data* m_data;
        uint8 m_buffer[max_data_size];
    };

    // base class for any object data which is dynamically typified
    class DOT_PUBLIC object::data : public basement
    {
    public:
        data();
        virtual ~data();

        // placement into object internal buffer
        virtual data* move_to(void* buffer) = 0;
        virtual data* copy_to(void* buffer) = 0;

        // class identification
        typedef basement base;
        static class_name_type class_name;
        virtual class_name_type get_class_name() const override;
        virtual class_id&& get_class_id() const override;
    };
}

// Unicode signature: Владимир Керимов
