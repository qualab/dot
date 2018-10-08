// DOT object base class

#pragma once

#include <dot/type.h>
#include <dot/stdfwd.h>
#include <type_traits>
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

        // create object by value of another type
        template <typename another_type>
        object(another_type&& another);

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
        derived_data* initialize(argument_types&&... arguments);

        void assign_to(object& target) const&;
        void assign_to(object& target) &&;

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
        using source_type = std::remove_reference_t<another_type>;
        if constexpr (std::is_same_v<object, source_type>)
        {
            another.assign_to(*this);
        }
        else if constexpr (std::is_base_of_v<object, source_type>)
        {
            using another_data_type = typename source_type::data;
            const another_data_type& another_data = another.data_as<another_data_type>();
            initialize<another_data_type>(another_data);
        }
        else if constexpr (std::is_arithmetic_v<source_type>)
        {
            using another_data_type = typename scalar<source_type>::data;
            initialize<another_data_type>(std::forward<another_type>(another));
        }
        else
        {
            static_assert(false, "Required specialization of object::set_as<> method for this type.");
        }
    }

    template <typename another_type>
    another_type object::get_as() const
    {
        if constexpr (std::is_base_of_v<object, another_type>)
        {
            return another_type(*this);
        }
        else if constexpr (std::is_arithmetic_v<another_type>)
        {
            return data_as<typename scalar<another_type>::data>().get();
        }
        else
        {
            static_assert(false, "Required specialization of object::get_as<> method for this type.");
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
        static_assert(sizeof(derived_data) <= object::max_data_size,
            "Size of derived data type is too big for object data internal buffer.");
        derived_data* result = nullptr;
        reset();
        m_data = result = new(m_buffer) derived_data(std::forward<argument_types>(arguments)...);
        return result;
    }
}

// Unicode signature: Владимир Керимов
