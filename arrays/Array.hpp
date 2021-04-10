#ifndef ARRAY_H
#define ARRAY_H

namespace arrays
{
    template <class T, size_t block = 16>
    class Array
    {
    public:
        using Checker = bool (*)(const Array<T, block> &, size_t, const T &);
        template<class AT>
        using Convertor = AT (*)(const Array<T, block> &, size_t, const T &);
        using Comparator = int (*)(const T *, const T *);

        typedef std::initializer_list<T> Values;

    private:
        size_t offset = 0;
        size_t length = 0;

        size_t size = 0;
        T *cache = nullptr;

        void move(size_t index, int offset);
        void resize(size_t length);

    public:
        Array(const T *array, size_t length);
        template<size_t ablock>
        Array(const Array<T, ablock> &array);
        Array(const Array<T, block> &array);
        Array(Values values);
        Array();

        ~Array();

        [[nodiscard]] inline bool empty() const;
        [[nodiscard]] inline bool reaches(size_t length) const;

        [[nodiscard]] bool has(size_t index, size_t length, const T &instance) const;
        [[nodiscard]] bool has(size_t index, const T &instance) const;

        [[nodiscard]] bool some(size_t index, size_t length, Checker checker) const;
        [[nodiscard]] bool some(size_t index, Checker checker) const;

        [[nodiscard]] T *find(size_t index, size_t length, Checker checker) const;
        [[nodiscard]] T *find(size_t index, Checker checker) const;

        template<size_t rblock = block>
        [[nodiscard]] Array<T, rblock> filter(size_t index, size_t length, Checker checker) const;
        template<size_t rblock = block>
        [[nodiscard]] Array<T, rblock> filter(size_t index, Checker checker) const;

        template<class AT, size_t rblock = block>
        [[nodiscard]] Array<AT, rblock> map(size_t index, size_t length, Convertor<AT> convertor) const;
        template<class AT, size_t rblock = block>
        [[nodiscard]] Array<AT, rblock> map(size_t index, Convertor<AT> convertor) const;

        template<size_t rblock = block>
        [[nodiscard]] Array<T, rblock> slice(size_t index, size_t length) const;

        [[nodiscard]] T &get(size_t index) const;
        void set(size_t index, const T &value);

        [[nodiscard]] const T *begin() const;
        [[nodiscard]] const T *end() const;

        [[nodiscard]] T *begin();
        [[nodiscard]] T *end();

        [[nodiscard]] size_t firstIndex(size_t offset, const T &instance) const;
        [[nodiscard]] size_t lastIndex(size_t offset, const T &instance) const;

        [[nodiscard]] size_t count() const;

        void put(size_t index, const T *array, size_t length);
        template<size_t ablock>
        void put(size_t index, const Array<T, ablock> &array);
        void put(size_t index, Values values);
        void put(size_t index, const T &instance);

        void push(const T *array, size_t length);
        template<size_t ablock>
        void push(const Array<T, ablock> &array);
        void push(Values values);
        void push(const T &instance);

        void unshift(const T *array, size_t length);
        template<size_t ablock>
        void unshift(const Array<T, ablock> &array);
        void unshift(Values values);
        void unshift(const T &instance);

        template<size_t rblock = block>
        [[nodiscard]] Array<T, rblock> pull(size_t index, size_t length);
        [[nodiscard]] T pull(size_t index);

        template<size_t rblock = block>
        [[nodiscard]] Array<T, rblock> pop(size_t length);
        [[nodiscard]] T pop();

        template<size_t rblock = block>
        [[nodiscard]] Array<T, rblock> shift(size_t length);
        [[nodiscard]] T shift();

        void remove(size_t index, size_t length);
        void remove(size_t index);

        void flip();
        void sort(Comparator comparator);

        void clear();

        template<class AT, size_t ablock>
        [[nodiscard]] bool operator==(const Array<AT, ablock> &another) const;

        template<class AT, size_t ablock>
        [[nodiscard]] bool operator!=(const Array<AT, ablock> &another) const;

        [[nodiscard]] inline T &operator[](size_t index);
        [[nodiscard]] inline const T &operator[](size_t index) const;

        template<size_t ablock>
        inline Array<T, block> &operator<<(const Array<T, ablock> &another);
        inline Array<T, block> &operator<<(Values values);
        inline Array<T, block> &operator<<(const T &instance);

        [[nodiscard]] inline T operator--();
        [[nodiscard]] inline T operator--(int);

        template<class AT, size_t rblock = block>
        [[nodiscard]] inline explicit operator Array<AT, rblock>() const;
        [[nodiscard]] inline explicit operator bool() const;
    };

    template<class T, size_t block, size_t ablock>
    inline Array<T, block> &operator>>(const Array<T, ablock> &another, Array<T, block> &array);

    template<class T, size_t block>
    inline Array<T, block> &operator>>(class Array<T, block>::Values values, Array<T, block> &array);

    template<class T, size_t block>
    inline Array<T, block> &operator>>(const T &instance, Array<T, block> &array);
}

#include "Array.inl"

#endif //ARRAY_H
