// DOT object base class

#pragma once

#include <dot/type.h>
#include <utility>

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
        object(const object& another);
        object& operator = (const object& another);

        // move object override
        object(object&& temporary);
        object& operator = (object&& temporary);

        // create object by value of another type explicitely
        template <typename another_type>
        explicit object(another_type&& another);

        // assign value of another type to object
        template <typename another_type>
        object& operator = (another_type&& another);

        // cast object to another type
        template <typename another_type>
        explicit operator another_type() const;

        // set object by scalar value
        template <typename another_type>
        void set_as(another_type&& another);

        // get object as derived type reference
        template <typename another_type>
        another_type get_as() const;

        bool operator == (const object& another) const;
        bool operator != (const object& another) const;
        bool operator <= (const object& another) const;
        bool operator >= (const object& another) const;
        bool operator <  (const object& another) const;
        bool operator >  (const object& another) const;

        // base data class for all objects
        class data;

        const data& get_data() const;

        template <typename data_type>
        const data_type& data_as() const;

        // class identification
        DOT_HIERARCHIC(hierarchic);

        // enough for two 64-bit fields and virtual table pointer
        static constexpr size_t data_type_max = 2 * sizeof(int64);
        static constexpr size_t data_buffer_size = data_type_max + sizeof(void*);

    protected:
        // initialize internal object data by derived data type
        template <typename derived_data, typename... argument_types>
        derived_data* initialize(argument_types&&... arguments);

        void assign_to(object& target) const&;
        void assign_to(object& target) &&;

    private:
        // internal object data
        data* m_data;

        // internal object buffer to place data
        byte m_buffer[data_buffer_size];

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

        bool operator == (const data& another) const;
        bool operator != (const data& another) const;
        bool operator <= (const data& another) const;
        bool operator >= (const data& another) const;
        bool operator <  (const data& another) const;
        bool operator >  (const data& another) const;

        // class identification
        DOT_HIERARCHIC(hierarchic);

        // constant for null data string representation
        static const char* const null_string;

    protected:
        // placement into object internal buffer
        virtual data* copy_to(void* buffer) const noexcept = 0;
        virtual data* move_to(void* buffer) noexcept = 0;

        // override if stream input/output operations are required
        virtual void write(std::ostream& stream) const;
        virtual void read(std::istream& stream);

        // override if comparison operations are required
        virtual bool equals(const data& another) const noexcept;
        virtual bool less(const data& another) const noexcept;

        friend class object;

        // data output and input using byte streams
        friend DOT_PUBLIC std::ostream& operator << (std::ostream& stream, const object::data& source);
        friend DOT_PUBLIC std::istream& operator >> (std::istream& stream, object::data& destination);
    };

// -- implmentation --

    template <typename another_type>
    object::object(another_type&& another)
        : m_data(nullptr)
    {
        set_as(std::forward<another_type>(another));
    }

    template <typename another_type>
    object& object::operator = (another_type&& another)
    {
        set_as(std::forward<another_type>(another));
        return *this;
    }

    template <typename another_type>
    object::operator another_type() const
    {
        return get_as<another_type>();
    }

    template <typename another_type>
    void object::set_as(another_type&& another)
    {
        using source_type = std::remove_const_t<std::remove_reference_t<another_type>>;
        if constexpr (std::is_base_of_v<object, source_type>)
        {
            another.assign_to(*this);
        }
        else if constexpr (sizeof(source_type) <= data_type_max)
        {
            using another_data_type = typename scalar_of<source_type>::data;
            initialize<another_data_type>(std::forward<another_type>(another));
        }
        else
        {
            using another_data_type = typename copyable_of<source_type>::data;
            initialize<another_data_type>(std::forward<another_type>(another));
        }
    }

    template <typename another_type>
    another_type object::get_as() const
    {
        using target_type = std::remove_const_t<std::remove_reference_t<another_type>>;
        if constexpr (std::is_base_of_v<object, target_type>)
        {
            return target_type(*this);
        }
        else if constexpr (sizeof(target_type) <= data_type_max)
        {
            return data_as<typename scalar_of<target_type>::data>().get();
        }
        else
        {
            return data_as<typename copyable_of<target_type>::data>().get();
        }
    }

    template <typename data_type>
    const data_type& object::data_as() const
    {
        return get_data().as<data_type>();
    }

    template <typename derived_data, typename... argument_types>
    derived_data* object::initialize(argument_types&&... arguments)
    {
        static_assert(sizeof(derived_data) <= data_buffer_size,
            "Size of derived data type is too big for object data internal buffer.");
        derived_data* result = nullptr;
        reset();
        m_data = result = new(m_buffer) derived_data(std::forward<argument_types>(arguments)...);
        return result;
    }
}

// Unicode signature: Владимир Керимов
