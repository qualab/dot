// dot::object is base class for any other class
// with dynamic typification by parallel hierarchy
// of object classes and their data classes
// object::data is base class for any data class
// object contains buffer of limited size to contain value
// stored in data in-place or by-reference depends of size

#pragma once

#include <dot/type.h>
#include <utility>

namespace dot
{
    // base class for any object with dynamic typification
    class DOT_PUBLIC object : public hierarchic
    {
    public:
        object() = default;
        virtual ~object() noexcept;

        // work with null object
        virtual void reset() noexcept;
        virtual bool is_null() const noexcept;
        virtual bool is_not_null() const noexcept;

        // copy object override
        object(const object& another);
        object& operator = (const object& another);

        // move object override
        object(object&& temporary) noexcept;
        object& operator = (object&& temporary) noexcept;

        // create object by value of another type explicitely
        template <class other>
        explicit object(other&& another);

        // assign value of another type to object
        template <class other>
        object& operator = (other&& another);

        // cast object to another type
        template <class other>
        explicit operator other() const;

        // set object by value of any other type
        template <class other>
        void set_as(other&& another);

        // get object as derived type reference
        template <class other>
        other get_as() const;

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
        template <class derived, class... arguments>
        derived* initialize(arguments&&... args);

        void copy_to(object& target) const&;
        void move_to(object& target) &&;

    private:
        // internal object data
        data* my_data = nullptr;

        // internal object buffer to place data
        byte my_buffer[data_buffer_size] = {};

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

    template <class other>
    object::object(other&& another)
        : my_data(nullptr)
    {
        set_as(std::forward<other>(another));
    }

    template <class other>
    object& object::operator = (other&& another)
    {
        set_as(std::forward<other>(another));
        return *this;
    }

    template <class other>
    object::operator other() const
    {
        return get_as<other>();
    }

    template <class other>
    void object::set_as(other&& another)
    {
        using source_type = std::remove_const_t<std::remove_reference_t<other>>;
        if constexpr (sizeof(source_type) <= data_type_max)
        {
            initialize<typename box<source_type>::cat>(std::forward<other>(another));
        }
        else
        {
            initialize<typename rope<source_type>::cow>(std::forward<other>(another));
        }
    }

    template <class other>
    other object::get_as() const
    {
        using target_type = std::remove_const_t<std::remove_reference_t<other>>;
        if constexpr (sizeof(target_type) <= data_type_max)
        {
            return data_as<typename box<target_type>::cat>().look();
        }
        else
        {
            return data_as<typename rope<target_type>::cow>().look();
        }
    }

    template <typename data_type>
    const data_type& object::data_as() const
    {
        return get_data().as<data_type>();
    }

    template <typename derived, typename... arguments>
    derived* object::initialize(arguments&&... args)
    {
        static_assert(sizeof(derived) <= data_buffer_size,
            "Size of derived data type is too big for object data internal buffer.");
        derived* result = nullptr;
        reset();
        my_data = result = new(my_buffer) derived(std::forward<arguments>(args)...);
        return result;
    }
}

// Здесь должен быть Unicode
