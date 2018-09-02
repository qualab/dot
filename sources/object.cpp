// DOT public declaration

#include <dot/object.h>
#include <dot/scalar.h>
#include <dot/copyable.h>
#include <dot/fail.h>
#include <iostream>
#include <utility>
#include <string>

namespace dot
{
    object::object() noexcept
        : m_data(nullptr)
    {
    }

    object::~object() noexcept
    {
        reset();
    }

    void object::reset() noexcept
    {
        if (m_data)
        {
            m_data->~data();
            m_data = nullptr;
        }
    }

    bool object::is_null() const noexcept
    {
        return m_data == nullptr;
    }

    bool object::is_not_null() const noexcept
    {
        return !is_null();
    }

    object::object(const object& another) noexcept
        : m_data(another.m_data ? another.m_data->copy_to(m_buffer) : nullptr)
    {
    }

    object& object::operator = (const object& another) noexcept
    {
        reset();
        if (another.m_data)
            m_data = another.m_data->copy_to(m_buffer); 
        return *this;
    }

    object::object(object&& another) noexcept
        : m_data(another.m_data ? another.m_data->move_to(m_buffer) : nullptr)
    {
    }

    object& object::operator = (object&& another) noexcept
    {
        reset();
        if (another.m_data)
            m_data = another.m_data->move_to(m_buffer);
        return *this;
    }

    const object::data& object::get_data() const
    {
        if (!m_data)
            throw fail::null_reference("Object has no data but data reference occured.");
        return *m_data;
    }

    const class_id& object::id() noexcept
    {
        static const class_id object_id("object");
        return object_id;
    }

    const class_id& object::who() const noexcept
    {
        return object::id();
    }

    std::ostream& operator << (std::ostream& stream, const object& source)
    {
        stream << source.who().name() << " = { ";
        if (source.m_data)
        {
            stream << source.m_data->who().name() << " = " << *source.m_data;
        }
        else
        {
            stream << "null";
        }
        return stream << " }";
    }

    std::istream& operator >> (std::istream& stream, object& /*destination*/)
    {
        // TODO: read object with data
        return stream;
    }

    object::data::data() noexcept
    {
    }

    object::data::~data() noexcept
    {
    }

    const class_id& object::data::id() noexcept
    {
        static const class_id object_data_id("object::data");
        return object_data_id;
    }

    const class_id& object::data::who() const noexcept
    {
        return object::data::id();
    }

    std::ostream& operator << (std::ostream& stream, const object::data& value)
    {
        value.write(stream);
        return stream;
    }

    std::istream& operator >> (std::istream& stream, object::data& value)
    {
        value.read(stream);
        return stream;
    }

    template<> void object::set_as(int64 value) { initialize<scalar<int64>::data>(value); }
    template<> void object::set_as(int32 value) { initialize<scalar<int32>::data>(value); }
    template<> void object::set_as(int16 value) { initialize<scalar<int16>::data>(value); }
    template<> void object::set_as(int8  value) { initialize<scalar<int8 >::data>(value); }

    template<> void object::set_as(uint64 value) { initialize<scalar<uint64>::data>(value); }
    template<> void object::set_as(uint32 value) { initialize<scalar<uint32>::data>(value); }
    template<> void object::set_as(uint16 value) { initialize<scalar<uint16>::data>(value); }
    template<> void object::set_as(uint8  value) { initialize<scalar<uint8 >::data>(value); }

    template<> void object::set_as(double value) { initialize<scalar<double>::data>(value); }
    template<> void object::set_as(float  value) { initialize<scalar<float >::data>(value); }

    template<> void object::set_as(bool value) { initialize<scalar<bool>::data>(value); }
    template<> void object::set_as(char value) { initialize<scalar<char>::data>(value); }

    template<> void object::set_as(const char* value) { initialize<copyable<std::string>::data>(value); }
    template<> void object::set_as(std::string value) { initialize<copyable<std::string>::data>(std::move(value)); }

    template<> void object::set_as(std::nullptr_t) { reset(); }

    template<> int64 object::get_as() const { return data_as<scalar<int64>::data>().get(); }
    template<> int32 object::get_as() const { return data_as<scalar<int32>::data>().get(); }
    template<> int16 object::get_as() const { return data_as<scalar<int16>::data>().get(); }
    template<> int8  object::get_as() const { return data_as<scalar<int8 >::data>().get(); }

    template<> uint64 object::get_as() const { return data_as<scalar<uint64>::data>().get(); }
    template<> uint32 object::get_as() const { return data_as<scalar<uint32>::data>().get(); }
    template<> uint16 object::get_as() const { return data_as<scalar<uint16>::data>().get(); }
    template<> uint8  object::get_as() const { return data_as<scalar<uint8 >::data>().get(); }

    template<> double object::get_as() const { return data_as<scalar<double>::data>().get(); }
    template<> float  object::get_as() const { return data_as<scalar<float >::data>().get(); }

    template<> bool object::get_as() const { return data_as<scalar<bool>::data>().get(); }
    template<> char object::get_as() const { return data_as<scalar<char>::data>().get(); }

    template<> const char* object::get_as() const { return data_as<copyable<std::string>::data>().get().c_str(); }
    template<> std::string object::get_as() const { return data_as<copyable<std::string>::data>().get(); }
}

// Unicode signature: Владимир Керимов
