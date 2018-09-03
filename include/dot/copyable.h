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

        const instance_type* operator -> () const noexcept;
        instance_type* operator -> ();

        const instance_type& operator * () const noexcept;
        instance_type& operator * ();

        uint64 ref_count() const noexcept;
        bool unique_ref() const noexcept;

        const instance_type& get() const noexcept;
        instance_type& ref();

        template <typename another_type>
        bool operator == (another_type&& another) const;

        template <typename another_type>
        bool operator != (another_type&& another) const;

        typedef object base;
        static const class_id& id() noexcept;
        virtual const class_id& who() const noexcept override;

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

        virtual ~data() noexcept;

        data(const data& another);
        data& operator = (const data& another);

        data(data&& temporary);
        data& operator = (data&& temporary);

        uint64 ref_count() const noexcept;

        const instance_type& get() const noexcept;
        instance_type& ref();

        typedef object::data base;
        static const class_id& id() noexcept;
        virtual const class_id& who() const noexcept override;

    protected:
        virtual object::data* copy_to(void* buffer) const noexcept override;
        virtual object::data* move_to(void* buffer) noexcept override;
        virtual void write(std::ostream& stream) const override;
        virtual void read(std::istream& stream) override;

    private:
        struct memory_block
        {
            std::atomic<uint64> counter;
            instance_type instance;

            template <typename... argument_types>
            memory_block(argument_types... arguments)
                : counter(1), instance(arguments...) { }

            memory_block* inc_counter()
            {
                ++counter;
                return this;
            }

            void dec_counter()
            {
                if (!--counter)
                    delete this;
            }
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
        : m_data(initialize<data>(std::move(*temporary.m_data)))
    {
    }

    template <typename instance_type>
    copyable<instance_type>& copyable<instance_type>::operator = (copyable&& temporary)
    {
        m_data = initialize<data>(std::move(*temporary.m_data));
        return *this;
    }

    template <typename instance_type>
    const instance_type* copyable<instance_type>::operator -> () const noexcept
    {
        return &m_data->get();
    }

    template <typename instance_type>
    instance_type* copyable<instance_type>::operator -> ()
    {
        return &m_data->ref();
    }

    template <typename instance_type>
    const instance_type& copyable<instance_type>::operator * () const noexcept
    {
        return m_data->get();
    }

    template <typename instance_type>
    instance_type& copyable<instance_type>::operator * ()
    {
        return m_data->ref();
    }

    template <typename instance_type>
    uint64 copyable<instance_type>::ref_count() const noexcept
    {
        return m_data->ref_count();
    }

    template <typename instance_type>
    bool copyable<instance_type>::unique_ref() const noexcept
    {
        return m_data->ref_count() == 1;
    }

    template <typename instance_type>
    const instance_type& copyable<instance_type>::get() const noexcept
    {
        return m_data->get();
    }

    template <typename instance_type>
    instance_type& copyable<instance_type>::ref()
    {
        return m_data->ref();
    }

    template <typename instance_type>
    template <typename another_type>
    bool copyable<instance_type>::operator == (another_type&& another) const
    {
        return m_data->get() == another;
    }

    template <typename instance_type>
    template <typename another_type>
    bool copyable<instance_type>::operator != (another_type&& another) const
    {
        return !(*this == another);
    }

    template <typename instance_type>
    const class_id& copyable<instance_type>::who() const noexcept
    {
        return copyable<instance_type>::id();
    }

    template <typename instance_type>
    template <typename... argument_types>
    copyable<instance_type>::data::data(argument_types... arguments)
        : m_block(new memory_block(arguments...))
    {
    }

    template <typename instance_type>
    copyable<instance_type>::data::~data() noexcept
    {
        m_block->dec_counter();
    }

    template <typename instance_type>
    object::data* copyable<instance_type>::data::copy_to(void* buffer) const noexcept
    {
        return new(buffer) data(*this);
    }

    template <typename instance_type>
    object::data* copyable<instance_type>::data::move_to(void* buffer) noexcept
    {
        return new(buffer) data(std::move(*this));
    }

    template <typename instance_type>
    void copyable<instance_type>::data::write(std::ostream& stream) const
    {
        stream << m_block->instance;
    }

    template <typename instance_type>
    void copyable<instance_type>::data::read(std::istream& stream)
    {
        stream >> m_block->instance;
    }

    template <typename instance_type>
    copyable<instance_type>::data::data(const data& another)
        : m_block(another.m_block->inc_counter())
    {
    }

    template <typename instance_type>
    typename copyable<instance_type>::data&
        copyable<instance_type>::data::operator = (
            const typename copyable<instance_type>::data& another)
    {
        memory_block* old_block = m_block;
        m_block = another.m_block->inc_counter();
        old_block->dec_counter();
        return *this;
    }

    template <typename instance_type>
    copyable<instance_type>::data::data(data&& temporary)
        : m_block(temporary.m_block->inc_counter())
    {
        // to avoid unitialized or nullptr temporary.m_block instead
        // atomic increment and decrement are very light operations
        // instead of heavy comparisons to nullptr for each method
    }

    template <typename instance_type>
    typename copyable<instance_type>::data&
        copyable<instance_type>::data::operator = (
            typename copyable<instance_type>::data&& temporary)
    {
        std::swap(m_block, temporary.m_block);
        return *this;
    }

    template <typename instance_type>
    uint64 copyable<instance_type>::data::ref_count() const noexcept
    {
        return m_block->counter;
    }

    template <typename instance_type>
    const instance_type& copyable<instance_type>::data::get() const noexcept
    {
        return m_block->instance;
    }

    template <typename instance_type>
    instance_type& copyable<instance_type>::data::ref()
    {
        if (m_block->counter > 1)
        {
            memory_block* old_block = m_block;
            m_block = new memory_block(m_block->instance);
            old_block->dec_counter();
        }
        return m_block->instance;
    }

    template <typename instance_type>
    const class_id& copyable<instance_type>::data::who() const noexcept
    {
        return copyable<instance_type>::data::id();
    }
}

// Unicode signature: Владимир Керимов
