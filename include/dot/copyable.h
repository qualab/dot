// DOT copy-on-write object

#pragma once

#include <dot/object.h>
#include <utility>
#include <atomic>

namespace dot
{
    class DOT_PUBLIC copyable : public object
    {
    public:
        copyable();

        template <typename instance_type>
        copyable(instance_type&& instance);

        DOT_HIERARCHIC(object);

        class data;
    };

    template <typename instance_type>
    class copyable_of : public copyable
    {
    public:
        template <typename... argument_types>
        copyable_of(argument_types... arguments);

        copyable_of(const copyable_of& another);
        copyable_of& operator = (const copyable_of& another);

        copyable_of(copyable_of&& temporary);
        copyable_of& operator = (copyable_of&& temporary);

        copyable_of(const object& another);
        copyable_of& operator = (const object& another);

        const instance_type* operator -> () const noexcept;
        instance_type* operator -> ();

        const instance_type& operator * () const noexcept;
        instance_type& operator * ();

        uint64 ref_count() const noexcept;
        bool unique_ref() const noexcept;

        const instance_type& get() const noexcept;
        instance_type& ref();

        DOT_HIERARCHIC(copyable);

        class data;

    private:
        data* my_data;
    };

    class DOT_PUBLIC copyable::data : public object::data
    {
    public:
        DOT_HIERARCHIC(object::data);
    };

    template <typename instance_type>
    class copyable_of<instance_type>::data : public copyable::data
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

        DOT_HIERARCHIC(copyable::data);

    protected:
        virtual object::data* copy_to(void* buffer) const noexcept override;
        virtual object::data* move_to(void* buffer) noexcept override;

        virtual void write(std::ostream& stream) const override;
        virtual void read(std::istream& stream) override;

        virtual bool equals(const object::data& another) const noexcept override;
        virtual bool less(const object::data& another) const noexcept override;

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

        memory_block* my_block;
    };

    template <typename instance_type>
    copyable::copyable(instance_type&& instance)
        : base(std::forward<instance_type>(instance))
    {
    }

    template <typename instance_type>
    template <typename... argument_types>
    copyable_of<instance_type>::copyable_of(argument_types... arguments)
        : my_data(initialize<data>(arguments...))
    {
    }

    template <typename instance_type>
    copyable_of<instance_type>::copyable_of(const copyable_of& another)
        : my_data(initialize<data>(*another.my_data))
    {
    }

    template <typename instance_type>
    copyable_of<instance_type>& copyable_of<instance_type>::operator = (const copyable_of& another)
    {
        my_data = initialize<data>(*another.my_data);
        return *this;
    }

    template <typename instance_type>
    copyable_of<instance_type>::copyable_of(copyable_of&& temporary)
        : my_data(initialize<data>(std::move(*temporary.my_data)))
    {
    }

    template <typename instance_type>
    copyable_of<instance_type>& copyable_of<instance_type>::operator = (copyable_of&& temporary)
    {
        my_data = initialize<data>(std::move(*temporary.my_data));
        return *this;
    }

    template <typename instance_type>
    copyable_of<instance_type>::copyable_of(const object& another)
        : my_data(initialize<data>(another.data_as<data>()))
    {
    }

    template <typename instance_type>
    copyable_of<instance_type>& copyable_of<instance_type>::operator = (const object& another)
    {
        my_data = initialize<data>(another.data_as<data>());
        return *this;
    }

    template <typename instance_type>
    const instance_type* copyable_of<instance_type>::operator -> () const noexcept
    {
        return &my_data->get();
    }

    template <typename instance_type>
    instance_type* copyable_of<instance_type>::operator -> ()
    {
        return &my_data->ref();
    }

    template <typename instance_type>
    const instance_type& copyable_of<instance_type>::operator * () const noexcept
    {
        return my_data->get();
    }

    template <typename instance_type>
    instance_type& copyable_of<instance_type>::operator * ()
    {
        return my_data->ref();
    }

    template <typename instance_type>
    uint64 copyable_of<instance_type>::ref_count() const noexcept
    {
        return my_data->ref_count();
    }

    template <typename instance_type>
    bool copyable_of<instance_type>::unique_ref() const noexcept
    {
        return my_data->ref_count() == 1;
    }

    template <typename instance_type>
    const instance_type& copyable_of<instance_type>::get() const noexcept
    {
        return my_data->get();
    }

    template <typename instance_type>
    instance_type& copyable_of<instance_type>::ref()
    {
        return my_data->ref();
    }

    template <typename instance_type>
    template <typename... argument_types>
    copyable_of<instance_type>::data::data(argument_types... arguments)
        : my_block(new memory_block(arguments...))
    {
    }

    template <typename instance_type>
    copyable_of<instance_type>::data::~data() noexcept
    {
        my_block->dec_counter();
    }

    template <typename instance_type>
    object::data* copyable_of<instance_type>::data::copy_to(void* buffer) const noexcept
    {
        return new(buffer) data(*this);
    }

    template <typename instance_type>
    object::data* copyable_of<instance_type>::data::move_to(void* buffer) noexcept
    {
        return new(buffer) data(std::move(*this));
    }

    template <typename instance_type>
    void copyable_of<instance_type>::data::write(std::ostream& stream) const
    {
        if constexpr (is_writable<instance_type>)
        {
            stream << get();
        }
        else
        {
            base::write(stream);
        }
    }

    template <typename instance_type>
    void copyable_of<instance_type>::data::read(std::istream& stream)
    {
        if constexpr (is_readable<instance_type>)
        {
            stream >> ref();
        }
        else
        {
            base::read(stream);
        }
    }

    template <typename instance_type>
    copyable_of<instance_type>::data::data(const data& another)
        : my_block(another.my_block->inc_counter())
    {
    }

    template <typename instance_type>
    typename copyable_of<instance_type>::data&
        copyable_of<instance_type>::data::operator = (
            const typename copyable_of<instance_type>::data& another)
    {
        memory_block* old_block = my_block;
        my_block = another.my_block->inc_counter();
        old_block->dec_counter();
        return *this;
    }

    template <typename instance_type>
    copyable_of<instance_type>::data::data(data&& temporary)
        : my_block(temporary.my_block->inc_counter())
    {
        // to avoid unitialized or nullptr temporary.m_block instead
        // atomic increment and decrement are very light operations
        // instead of heavy comparisons to nullptr for each method
    }

    template <typename instance_type>
    typename copyable_of<instance_type>::data&
        copyable_of<instance_type>::data::operator = (
            typename copyable_of<instance_type>::data&& temporary)
    {
        std::swap(my_block, temporary.my_block);
        return *this;
    }

    template <typename instance_type>
    uint64 copyable_of<instance_type>::data::ref_count() const noexcept
    {
        return my_block->counter;
    }

    template <typename instance_type>
    const instance_type& copyable_of<instance_type>::data::get() const noexcept
    {
        return my_block->instance;
    }

    template <typename instance_type>
    instance_type& copyable_of<instance_type>::data::ref()
    {
        if (my_block->counter > 1)
        {
            memory_block* old_block = my_block;
            my_block = new memory_block(my_block->instance);
            old_block->dec_counter();
        }
        return my_block->instance;
    }

    template <typename instance_type>
    bool copyable_of<instance_type>::data::equals(const object::data& another) const noexcept
    {
        if constexpr (is_comparable<instance_type>)
        {
            if (another.is<data>())
                return get() == another.as<data>().get();
            else
                return base::equals(another);
        }
        else
        {
            return base::equals(another);
        }
    }

    template <typename instance_type>
    bool copyable_of<instance_type>::data::less(const object::data& another) const noexcept
    {
        if constexpr (is_orderable<instance_type>)
        {
            if (another.is<data>())
                return get() < another.as<data>().get();
            else
                return base::less(another);
        }
        else
        {
            return base::less(another);
        }
    }
}

// Здесь должен быть Unicode
