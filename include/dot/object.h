// DOT object base class

#pragma once

#include <dot/type.h>
#include <dot/stdfwd.h>

namespace dot
{
    // base class for any object with dynamic typification
    class DOT_PUBLIC object : public hierarchic
    {
    public:
        object() noexcept;
        virtual ~object() noexcept;

        // work with null object
        virtual void reset() noexcept;
        virtual bool is_null() const noexcept;
        virtual bool is_not_null() const noexcept;

        // copy object override
        object(const object& another) noexcept;
        object& operator = (const object& another) noexcept;

        // move object override
        object(object&& temporary) noexcept;
        object& operator = (object&& temporary) noexcept;

        // create object with value
        template <typename value_type>
        object(value_type value);

        // assign value to object
        template <typename value_type>
        object& operator = (value_type value);

        // cast object to value type
        template <typename value_type>
        explicit operator value_type() const;

        // set value to object
        template <typename value_type>
        void set_as(value_type value);

        // get value of object
        template <typename value_type>
        value_type get_as() const;

        // base data class for all objects
        class data;

        const data& get_data() const;

        template <typename data_type>
        const data_type& data_as() const;

        // enough for two 64-bit fields and virtual table pointer
        static const size_t max_data_size = 3 * sizeof(int64);

        // class identification
        typedef hierarchic base;
        static const class_id& id() noexcept;
        virtual const class_id& who() const noexcept override;

    protected:
        // initialize internal object data by derived data type
        template <typename derived_data, typename... argument_types>
        derived_data* initialize(argument_types... arguments);

    private:
        // internal object data
        data* m_data;

        // internal object buffer to place data
        byte m_buffer[max_data_size];

        // object output and input using byte characters
        friend DOT_PUBLIC std::ostream& operator << (std::ostream& stream, const object& source);
        friend DOT_PUBLIC std::istream& operator >> (std::istream& stream, object& destination);
    };

    // base class for any object data which is dynamically typified
    class DOT_PUBLIC object::data : public hierarchic
    {
    public:
        data() noexcept;
        virtual ~data() noexcept;

        // class identification
        typedef hierarchic base;
        static const class_id& id() noexcept;
        virtual const class_id& who() const noexcept override;

    protected:
        // placement into object internal buffer
        virtual data* copy_to(void* buffer) const noexcept = 0;
        virtual data* move_to(void* buffer) noexcept = 0;

        // stream input and output
        virtual void write(std::ostream& stream) const = 0;
        virtual void read(std::istream& stream) = 0;

        friend class object;

        // data output and input using byte characters
        friend DOT_PUBLIC std::ostream& operator << (std::ostream& stream, const object::data& source);
        friend DOT_PUBLIC std::istream& operator >> (std::istream& stream, object::data& destination);
    };

    template <typename value_type>
    object::object(value_type value)
        : m_data(nullptr)
    {
        set_as(value);
    }

    template <typename value_type>
    object& object::operator = (value_type value)
    {
        set_as(value);
        return *this;
    }

    template <typename value_type>
    object::operator value_type() const
    {
        return get_as<value_type>();
    }

    template <typename value_type>
    void object::set_as(value_type value)
    {
        static_assert(false,
            "Template object::set_as<type> is not specialized for this type.");
    }

    template <typename value_type>
    value_type object::get_as() const
    {
        static_assert(false,
            "Template object::get_as<type> is not specialized for this type.");
    }

    template <typename data_type>
    const data_type& object::data_as() const
    {
        return get_data().as<data_type>();
    }

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

    template<> DOT_PUBLIC void object::set_as(long long value);
    template<> DOT_PUBLIC void object::set_as(long value);
    template<> DOT_PUBLIC void object::set_as(int value);
    template<> DOT_PUBLIC void object::set_as(short value);
    template<> DOT_PUBLIC void object::set_as(char value);

    template<> DOT_PUBLIC void object::set_as(unsigned long long value);
    template<> DOT_PUBLIC void object::set_as(unsigned long value);
    template<> DOT_PUBLIC void object::set_as(unsigned int value);
    template<> DOT_PUBLIC void object::set_as(unsigned short value);
    template<> DOT_PUBLIC void object::set_as(unsigned char value);

    template<> DOT_PUBLIC void object::set_as(double value);
    template<> DOT_PUBLIC void object::set_as(float  value);

    template<> DOT_PUBLIC void object::set_as(bool value);

    template<> DOT_PUBLIC void object::set_as(const char* value);
    template<> DOT_PUBLIC void object::set_as(std::string value);

    template<> DOT_PUBLIC void object::set_as(std::nullptr_t);

    template<> DOT_PUBLIC long long object::get_as() const;
    template<> DOT_PUBLIC long      object::get_as() const;
    template<> DOT_PUBLIC int       object::get_as() const;
    template<> DOT_PUBLIC short     object::get_as() const;
    template<> DOT_PUBLIC char      object::get_as() const;

    template<> DOT_PUBLIC unsigned long long object::get_as() const;
    template<> DOT_PUBLIC unsigned long      object::get_as() const;
    template<> DOT_PUBLIC unsigned int       object::get_as() const;
    template<> DOT_PUBLIC unsigned short     object::get_as() const;
    template<> DOT_PUBLIC unsigned char      object::get_as() const;

    template<> DOT_PUBLIC double object::get_as() const;
    template<> DOT_PUBLIC float  object::get_as() const;

    template<> DOT_PUBLIC bool object::get_as() const;

    template<> DOT_PUBLIC const char* object::get_as() const;
    template<> DOT_PUBLIC std::string object::get_as() const;
}

// Unicode signature: Владимир Керимов
