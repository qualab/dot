// DOT copy-on-write object

#pragma once

#include <dot/object.h>
#include <utility>
#include <atomic>

namespace dot
{
    template <typename instance_type>
    class copyable : public object
    {
    public:
        template <typename... argument_types>
        copyable(argument_types... arguments);

        copyable(const copyable& another);
        copyable& operator = (const copyable& another);

        copyable(copyable&& temporary);
        copyable& operator = (copyable&& temporary);

        const instance_type* operator -> () const;
        instance_type* operator -> ();

        const instance_type& operator * () const;
        instance_type& operator * ();

        typedef object base;
        static class_name_type class_name;
        virtual class_name_type who() const override;

        class data;

    private:
        data* m_data;
    };

    template <typename instance_type>
    class copyable<instance_type>::data : public object::data
    {
    public:
        template <typename... argument_types>
        data(argument_types... arguments);

        virtual ~data();

        virtual object::data* copy_to(void* buffer) const override;
        virtual object::data* move_to(void* buffer) override;

        data(const data& another);
        data& operator = (const data& another);

        data(data&& temporary);
        data& operator = (data&& temporary);

        uint64 ref_counter() const { return m_block->counter; }

        const instance_type& shared_instance() const { return *m_instance; }
        instance_type& unique_instance();

        typedef object::data base;
        static class_name_type class_name;
        virtual class_name_type who() const override;

    private:
        struct memory_block
        {
            std::atomic<uint64> counter;
            instance_type instance;

            template <typename... argument_types>
            memory_block(argument_types... arguments)
                : counter(1), instance(arguments...) { }
        };

        memory_block* m_block;
    };

    template <typename instance_type>
    template <typename... argument_types>
    copyable<instance_type>::copyable(argument_types... arguments)
        : m_data(initialize<data>(arguments...))
    {
    }

    template <typename instance_type>
    copyable<instance_type>::copyable(const copyable& another)
        : m_data(initialize<data>(*another.m_data))
    {
    }

    template <typename instance_type>
    copyable<instance_type>& copyable<instance_type>::operator = (const copyable& another)
    {
        m_data = initialize<data>(*another.m_data);
        return *this;
    }

    template <typename instance_type>
    copyable<instance_type>::copyable(copyable&& temporary)
        : m_data(initialize<data>(std::forward(temporary)))
    {
    }

    template <typename instance_type>
    copyable<instance_type>& copyable<instance_type>::operator = (copyable&& temporary)
    {
        return *this;
    }

    template <typename instance_type>
    const instance_type* copyable<instance_type>::operator -> () const
    {
        return &m_data->shared_instance();
    }

    template <typename instance_type>
    instance_type* copyable<instance_type>::operator -> ()
    {
        return &m_data->unique_instance();
    }

    template <typename instance_type>
    const instance_type& copyable<instance_type>::operator * () const
    {
        return m_data->shared_instance();
    }

    template <typename instance_type>
    instance_type& copyable<instance_type>::operator * ()
    {
        return m_data->unique_instance();
    }

    template <typename instance_type>
    inline class_name_type copyable<instance_type>::who() const
    {
        return copyable<instance_type>::class_name;
    }

    template <typename instance_type>
    template <typename... argument_types>
    copyable<instance_type>::data::data(argument_types... arguments)
        : m_block(new memory_block(arguments...))
    {
    }

    template <typename instance_type>
    copyable<instance_type>::data::~data()
    {
        if (--m_block->counter)
            delete m_block;
    }

    template <typename instance_type>
    object::data* copyable<instance_type>::data::copy_to(void* buffer) const
    {
        return new(buffer) data(*this);
    }

    template <typename instance_type>
    object::data* copyable<instance_type>::data::move_to(void* buffer)
    {
        return new(buffer) data(std::move(*this));
    }

    template <typename instance_type>
    copyable<instance_type>::data::data(const data& another)
        : m_block(another.m_block)
    {
        ++m_block->counter;
    }

    template <typename instance_type>
    typename copyable<instance_type>::data&
        copyable<instance_type>::data::operator = (
            const copyable<instance_type>::data& another)
    {
        memory_block* old_block = m_block;
        m_block = another.m_block;
        ++m_block->counter;
        --old_block->counter;
        return *this;
    }

    template <typename instance_type>
    copyable<instance_type>::data::data(data&& temporary)
        : m_block(temporary.m_block)
    {
        // to avoid unitialized temporary.m_block instead
        ++m_block->counter;
    }

    template <typename instance_type>
    typename copyable<instance_type>::data&
        copyable<instance_type>::data::operator = (
            copyable<instance_type>::data&& temporary)
    {
        std::swap(m_block, temporary.m_block);
        return *this;
    }

    template <typename instance_type>
    instance_type& copyable<instance_type>::data::unique_instance()
    {
        if (m_block->counter > 1)
        {
            m_block = new memory_block(m_block->instance);
        }
        return m_block->instance;
    }

    template <typename instance_type>
    class_name_type copyable<instance_type>::data::who() const
    {
        return copyable<instance_type>::data::class_name;
    }
}

// Unicode signature: Владимир Керимов
