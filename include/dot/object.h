// Базовый объект для любого класса в иерархии типов
// может принимать любой тип данных, всё что нужно
// это унаследоваться от объекта и создать свои данные

#pragma once

#include <dot/type.h>
#include <utility>

namespace dot
{
    // объект может хранить произвольные данные
    class DOT_PUBLIC object : public hierarchic
    {
    public:
        object() = default;
        virtual ~object() noexcept;

        // сброс и отсутствие данных
        virtual void reset() noexcept;
        virtual bool is_null() const noexcept;
        virtual bool is_not_null() const noexcept;

        // копирование данных из другого объекта
        object(const object& another);
        object& operator = (const object& another);

        // перенос данных из другого объекта
        object(object&& temporary) noexcept;
        object& operator = (object&& temporary) noexcept;

        // создание объекта по произвольному типу
        template <class other>
        explicit object(other&& another);

        // создание данных объекта по произвольному типу
        template <class other>
        object& operator = (other&& another);

        // преобразование к произвольному типу
        template <class other>
        explicit operator other() const;

        // установка данных объекта произвольного типа
        template <class other>
        void set_as(other&& another);

        // приведение данных объекта к произвольному типу
        template <class other>
        other get_as() const;

        // сравнения объектов
        bool operator == (const object& another) const;
        bool operator != (const object& another) const;
        bool operator <= (const object& another) const;
        bool operator >= (const object& another) const;
        bool operator <  (const object& another) const;
        bool operator >  (const object& another) const;

        // базовый класс для любых данных объекта
        class data;

        // получение ссылки на данные объекта
        const data& get_data() const;

        // приведение данных объекта к типу наследника
        template <typename data_type>
        const data_type& data_as() const;

        // базовый класс иерархии
        DOT_HIERARCHIC(hierarchic);

        // данных достаточно для хранения 2х int64 либо 4х float
        static constexpr size_t data_type_max = 2 * sizeof(int64);
        static constexpr size_t data_buffer_size = data_type_max + sizeof(void*);

    protected:
        // инициализация данных по произвольному вызову конструктора наследника
        template <class derived, class... arguments>
        derived* initialize(arguments&&... args);

        // служебные методы для копирования и переноса данных
        void copy_to(object& target) const&;
        void move_to(object& target) &&;

    private:
        // указатель на данные
        data* my_data = nullptr;

        // внутренний буфер для хранения данных
        byte my_buffer[data_buffer_size] = {};

        // работа с потоками ввода-вывода
        friend DOT_PUBLIC std::ostream& operator << (std::ostream& stream, const object& source);
        friend DOT_PUBLIC std::istream& operator >> (std::istream& stream, object& destination);
    };

    // базовый класс для любых данных объекта
    class DOT_PUBLIC object::data : public hierarchic
    {
    public:
        data() noexcept = default;
        virtual ~data() noexcept;

        // сравнение данных объекта
        bool operator == (const data& another) const;
        bool operator != (const data& another) const;
        bool operator <= (const data& another) const;
        bool operator >= (const data& another) const;
        bool operator <  (const data& another) const;
        bool operator >  (const data& another) const;

        // базовый класс иерархии
        DOT_HIERARCHIC(hierarchic);

        // константа для null объекта без данных
        static const char* const null_string;

    protected:
        // копирование и перенос данных в буфер другого объекта
        virtual data* copy_to(void* buffer) const noexcept = 0;
        virtual data* move_to(void* buffer) noexcept = 0;

        // работа с потоками ввода и вывода
        virtual void write(std::ostream& stream) const;
        virtual void read(std::istream& stream);

        // работа со сравнениями типов хранящихся в данных
        virtual bool equals(const data& another) const noexcept;
        virtual bool less(const data& another) const noexcept;

        // доступ к данным
        friend class object;

        // ввод и вывод в стандартные потоки
        friend DOT_PUBLIC std::ostream& operator << (std::ostream& stream, const object::data& source);
        friend DOT_PUBLIC std::istream& operator >> (std::istream& stream, object::data& destination);
    };

    // -- шаблонные методы --

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
            // данные поместятся во внутренний буфер
            initialize<typename box<source_type>::cat>(std::forward<other>(another));
        }
        else
        {
            // данные придётся хранить в динамически выделенной памяти
            initialize<typename rope<source_type>::cow>(std::forward<other>(another));
        }
    }

    template <class other>
    other object::get_as() const
    {
        using target_type = std::remove_const_t<std::remove_reference_t<other>>;
        if constexpr (sizeof(target_type) <= data_type_max)
        {
            // доступ к данным по значению в буфере
            return data_as<typename box<target_type>::cat>().look();
        }
        else
        {
            // доступ к данным по ссылке не приведёт к копированию
            return data_as<typename rope<target_type>::cow>().look();
        }
    }

    template <typename data_type>
    const data_type& object::data_as() const
    {
        // приведение к типу данных наследника
        return get_data().as<data_type>();
    }

    template <typename derived, typename... arguments>
    derived* object::initialize(arguments&&... args)
    {
        // проверка на размер типа данных для помещения в буфер
        static_assert(sizeof(derived) <= data_buffer_size,
            "Size of derived data type is too big for object data internal buffer.");
        // ссылка на данные нужного типа
        derived* result;
        reset();
        // инициализация данных в буфере объекта произвольным набором аргументов
        my_data = result = new(my_buffer) derived(std::forward<arguments>(args)...);
        return result;
    }
}

// Здесь должен быть Unicode
