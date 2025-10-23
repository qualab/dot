// Данные объектов слишком велики для внутреннего буфера
// в буфере расположена ссылка на данные объекта
// копирование которого происходит только при попытке
// изменять значения в данных неконстантным доступом

#pragma once

#include <dot/object.h>
#include <utility>
#include <atomic>

namespace dot
{
    // базовый класс для всех объектов-"верёвок"
    class DOT_PUBLIC rope_based : public object
    {
    public:
        DOT_HIERARCHIC(object);

        class cow_based;
    };
    
    // объектом-"верёвкой" мы привязываем "толстые" данные-"корову"
    template <class fat>
    class rope : public rope_based
    {
    public:
        // создание новых данных по произвольному набору аргументов
        template <class... arguments>
        rope(arguments... args);

        // создаём новую ссылку на существующие данные без копирования
        rope(const rope& another);
        rope& operator = (const rope& another);

        // забираем ссылку на существующие данные не меняя счётчик ссылок
        rope(rope&& temp);
        rope& operator = (rope&& temp);

        // инициализируем данные по данным произвольного объекта
        rope(const object& another);
        rope& operator = (const object& another);

        // доступ к методам "толстого" типа значения без копирования
        const fat* operator -> () const noexcept;

        // доступ к методам значения с созданием своего экземпляра
        fat* operator -> ();

        // ссылка на "толстый" тип значения без копирования
        const fat& operator * () const noexcept;

        // ссылка на значение с созданием своего экземпляра
        fat& operator * ();

        // счётчик ссылок-"верёвок" для общих данных
        uint64 bound() const noexcept;

        // проверка уникальная ли ссылка на данные которые можно менять
        bool unique() const noexcept;

        // константный доступ к значению внутри данных
        const fat& look() const noexcept;

        // неконстантный доступ приведёт к созданию своего экземпляра
        fat& touch();

        // сравнения с другими типами объектов-"верёвок"
        template <typename other> bool operator == (const rope<other>& another) const;
        template <typename other> bool operator != (const rope<other>& another) const;
        template <typename other> bool operator <= (const rope<other>& another) const;
        template <typename other> bool operator >= (const rope<other>& another) const;
        template <typename other> bool operator <  (const rope<other>& another) const;
        template <typename other> bool operator >  (const rope<other>& another) const;

        // сравнения с произвольными типами
        template <typename other> bool operator == (const other& another) const;
        template <typename other> bool operator != (const other& another) const;
        template <typename other> bool operator <= (const other& another) const;
        template <typename other> bool operator >= (const other& another) const;
        template <typename other> bool operator <  (const other& another) const;
        template <typename other> bool operator >  (const other& another) const;

        DOT_HIERARCHIC(rope_based);

        class cow;

    private:
        cow* my_cow;
    };

    // сравнения произвольных типов слева с объектами-"верёвками"
    template <typename left, typename right> bool operator == (const left& x, const rope<right>& y);
    template <typename left, typename right> bool operator != (const left& x, const rope<right>& y);
    template <typename left, typename right> bool operator <= (const left& x, const rope<right>& y);
    template <typename left, typename right> bool operator >= (const left& x, const rope<right>& y);
    template <typename left, typename right> bool operator <  (const left& x, const rope<right>& y);
    template <typename left, typename right> bool operator >  (const left& x, const rope<right>& y);

    // базовый тип для всех данных-"коров"
    class DOT_PUBLIC rope_based::cow_based : public object::data
    {
    public:
        DOT_HIERARCHIC(object::data);
    };

    // данные-"коровы" хранящие "толстые" значения
    // по ссылке в динамически выделенной памяти
    // создают свой уникальный экземпляр при изменении
    template <class fat>
    class rope<fat>::cow : public rope_based::cow_based
    {
    public:
        // создания нового значения в динамической памяти
        // по аргументам конструктора "толстого" типа
        template <class... arguments>
        cow(arguments... args);

        // удаление ссылки на данные-"корову"
        // удаление данных если ссылка была последней
        virtual ~cow() noexcept;

        // новая ссылка на существующую "корову"
        cow(const cow& another);
        cow& operator = (const cow& another);

        // перенос ссылки в новые данные
        cow(cow&& temp) noexcept;
        cow& operator = (cow&& temp);

        // счётчик ссылок на общие данные
        uint64 bound() const noexcept;

        // доступ к значению "толстого" типа без копирования
        const fat& look() const noexcept;

        // доступ для изменения ведёт к созданию своего экземпляра
        fat& touch();

        DOT_HIERARCHIC(rope_based::cow_based);

    protected:
        // копирование и перенос ссылки в буфер другого объекта
        virtual object::data* copy_to(void* buffer) const noexcept override;
        virtual object::data* move_to(void* buffer) noexcept override;

        // работа со стандартными потоками ввода и вывода
        virtual void write(std::ostream& stream) const override;
        virtual void read(std::istream& stream) override;

        // сравнения с данными других классов
        virtual bool equals(const object::data& another) const noexcept override;
        virtual bool less(const object::data& another) const noexcept override;

    private:
        // вспомогательная структура "шея" "коровы"
        // хранит счётчик ссылок и значение толстого типа
        // именно к "шее" привязаны объекты-"верёвки"
        struct neck
        {
            std::atomic<uint64> bound;
            fat value;

            // создание уникального значения
            template <typename... arguments>
            neck(arguments... args)
                : bound(1),
                  value(std::forward<arguments>(args)...)
            {
            }

            // привязать к "шее" новую ссылку-"верёвку"
            neck* add_rope()
            {
                ++bound;
                return this;
            }

            // отвязать от "шеи" одну ссылку-"верёвку"
            void remove_rope()
            {
                if (!--bound)
                    delete this;
            }
        };

        neck* my_neck;
    };

// -- шаблонные методы --

    template <class fat>
    template <class... arguments>
    rope<fat>::rope(arguments... args)
        : my_cow(initialize<cow>(args...))
    {
    }

    template <class fat>
    rope<fat>::rope(const rope& another)
        : my_cow(initialize<cow>(*another.my_cow))
    {
    }

    template <class fat>
    rope<fat>& rope<fat>::operator = (const rope& another)
    {
        my_cow = initialize<cow>(*another.my_cow);
        return *this;
    }

    template <class fat>
    rope<fat>::rope(rope&& temp)
        : my_cow(initialize<cow>(std::move(*temp.my_cow)))
    {
    }

    template <class fat>
    rope<fat>& rope<fat>::operator = (rope&& temp)
    {
        my_cow = initialize<cow>(std::move(*temp.my_cow));
        return *this;
    }

    template <class fat>
    rope<fat>::rope(const object& another)
        : my_cow(initialize<cow>(another.data_as<cow>()))
    {
    }

    template <class fat>
    rope<fat>& rope<fat>::operator = (const object& another)
    {
        my_cow = initialize<cow>(another.data_as<cow>());
        return *this;
    }

    template <class fat>
    const fat* rope<fat>::operator -> () const noexcept
    {
        return &my_cow->look();
    }

    template <class fat>
    fat* rope<fat>::operator -> ()
    {
        return &my_cow->touch();
    }

    template <class fat>
    const fat& rope<fat>::operator * () const noexcept
    {
        return my_cow->look();
    }

    template <class fat>
    fat& rope<fat>::operator * ()
    {
        return my_cow->touch();
    }

    template <class fat>
    uint64 rope<fat>::bound() const noexcept
    {
        return my_cow->bound();
    }

    template <class fat>
    bool rope<fat>::unique() const noexcept
    {
        return my_cow->bound() == 1;
    }

    template <class fat>
    const fat& rope<fat>::look() const noexcept
    {
        return my_cow->look();
    }

    template <class fat>
    fat& rope<fat>::touch()
    {
        return my_cow->touch();
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator == (const rope<other>& another) const
    {
        if constexpr (are_comparable<fat, other>)
        {
            return look() == another.look();
        }
        else
        {
            static_assert(false, "Unable to compare rope_based of non comparable types.");
        }
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator != (const rope<other>& another) const
    {
        return !(*this == another);
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator <= (const rope<other>& another) const
    {
        return !(another < *this);
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator >= (const rope<other>& another) const
    {
        return !(*this < another);
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator < (const rope<other>& another) const
    {
        if constexpr (are_orderable<fat, other>)
        {
            return look() < another.look();
        }
        else
        {
            static_assert(false, "Unable to order rope_based of non orderable types.");
        }
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator > (const rope<other>& another) const
    {
        return another < *this;
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator == (const other& another) const
    {
        if constexpr (are_comparable<fat, other>)
        {
            return look() == another;
        }
        else if constexpr (std::is_base_of_v<object, other>)
        {
            return base::operator == (another);
        }
        else
        {
            static_assert(false, "Unable to compare rope_based with non comparable type.");
        }
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator != (const other& another) const
    {
        return !(*this == another);
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator <= (const other& another) const
    {
        return !(another < *this);
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator >= (const other& another) const
    {
        return !(*this < another);
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator < (const other& another) const
    {
        if constexpr (are_orderable<fat, other>)
        {
            return look() < another;
        }
        else if constexpr (std::is_base_of_v<object, other>)
        {
            return base::operator < (another);
        }
        else
        {
            static_assert(false, "Unable to order rope_based with non orderable type.");
        }
    }

    template <class fat>
    template <class other>
    bool rope<fat>::operator > (const other& another) const
    {
        if constexpr (are_orderable<other, fat>)
        {
            return another < look();
        }
        else if constexpr (std::is_base_of_v<object, other>)
        {
            return base::operator > (another);
        }
        else
        {
            static_assert(false, "Unable to order rope_based with non orderable type.");
        }
    }

    template <typename left, typename right>
    bool operator == (const left& x, const rope<right>& y)
    {
        return y == x;
    }

    template <typename left, typename right>
    bool operator != (const left& x, const rope<right>& y)
    {
        return y != x;
    }

    template <typename left, typename right>
    bool operator <= (const left& x, const rope<right>& y)
    {
        return y >= x;
    }

    template <typename left, typename right>
    bool operator >= (const left& x, const rope<right>& y)
    {
        return y <= x;
    }

    template <typename left, typename right>
    bool operator < (const left& x, const rope<right>& y)
    {
        return y > x;
    }

    template <typename left, typename right>
    bool operator > (const left& x, const rope<right>& y)
    {
        return y < x;
    }

// -- шаблонные методы данных --

    template <class fat>
    template <class... arguments>
    rope<fat>::cow::cow(arguments... args)
        : my_neck(new neck(args...))
    {
    }

    template <class fat>
    rope<fat>::cow::~cow() noexcept
    {
        my_neck->remove_rope();
    }

    template <class fat>
    object::data* rope<fat>::cow::copy_to(void* buffer) const noexcept
    {
        return new(buffer) cow(*this);
    }

    template <class fat>
    object::data* rope<fat>::cow::move_to(void* buffer) noexcept
    {
        return new(buffer) cow(std::move(*this));
    }

    template <class fat>
    void rope<fat>::cow::write(std::ostream& stream) const
    {
        if constexpr (is_writable<fat>)
        {
            stream << look();
        }
        else
        {
            base::write(stream);
        }
    }

    template <class fat>
    void rope<fat>::cow::read(std::istream& stream)
    {
        if constexpr (is_readable<fat>)
        {
            stream >> touch();
        }
        else
        {
            base::read(stream);
        }
    }

    template <class fat>
    rope<fat>::cow::cow(const cow& another)
        : my_neck(another.my_neck->add_rope())
    {
    }

    template <class fat>
    typename rope<fat>::cow&
        rope<fat>::cow::operator = (
            const typename rope<fat>::cow& another)
    {
        neck* old_block = my_neck;
        my_neck = another.my_neck->add_rope();
        old_block->remove_rope();
        return *this;
    }

    template <class fat>
    rope<fat>::cow::cow(cow&& temp) noexcept
        : my_neck(temp.my_neck->add_rope())
    {
        // во избежание нулевых или неинициализированных данных
    }

    template <class fat>
    typename rope<fat>::cow&
        rope<fat>::cow::operator = (
            typename rope<fat>::cow&& temp)
    {
        std::swap(my_neck, temp.my_neck);
        return *this;
    }

    template <class fat>
    uint64 rope<fat>::cow::bound() const noexcept
    {
        return my_neck->bound;
    }

    template <class fat>
    const fat& rope<fat>::cow::look() const noexcept
    {
        return my_neck->value;
    }

    template <class fat>
    fat& rope<fat>::cow::touch()
    {
        if (my_neck->bound > 1)
        {
            neck* old_block = my_neck;
            my_neck = new neck(my_neck->value);
            old_block->remove_rope();
        }
        return my_neck->value;
    }

    template <class fat>
    bool rope<fat>::cow::equals(const object::data& another) const noexcept
    {
        if constexpr (is_comparable<fat>)
        {
            if (another.is<cow>())
                return look() == another.as<cow>().look();
            else
                return base::equals(another);
        }
        else
        {
            return base::equals(another);
        }
    }

    template <class fat>
    bool rope<fat>::cow::less(const object::data& another) const noexcept
    {
        if constexpr (is_orderable<fat>)
        {
            if (another.is<cow>())
                return look() < another.as<cow>().look();
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
