// DOT public declaration

#pragma once

#include <dot/object.h>
#include <utility>

namespace dot
{
    class_name_type object::class_name = "object";
    class_name_type object::data::class_name = "object::data";

    object::object()
        : m_data(nullptr)
    {
    }

    object::~object()
    {
        reset();
    }

    void object::reset()
    {
        if (m_data)
        {
            m_data->~data();
            m_data = nullptr;
        }
    }

    bool object::is_null() const
    {
        return m_data == nullptr;
    }

    bool object::is_not_null() const
    {
        return !is_null();
    }

    object::object(const object& another)
        : m_data(nullptr)
    {
        *this = another;
    }

    object& object::operator = (const object& another)
    {
        reset();
        if (another.m_data)
            m_data = another.m_data->copy_to(m_buffer);
        return *this;
    }

    object::object(object&& another)
        : m_data(nullptr)
    {
        *this = std::move(another);
    }

    object& object::operator = (object&& another)
    {
        reset();
        if (another.m_data)
            m_data = another.m_data->move_to(m_buffer);
        return *this;
    }

    class_name_type object::get_class_name() const
    {
        return object::class_name;
    }

    object::data::data()
    {
    }

    object::data::~data()
    {
    }

    class_name_type object::data::get_class_name() const
    {
        return object::data::class_name;
    }
}

// Unicode signature: Владимир Керимов
