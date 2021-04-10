#ifndef DEQUE_HPP
#define DEQUE_HPP

#include "utils/calculations.hpp"

namespace arrays
{
    template<class T, size_t block = utils::blockSize<T>()>
    class Deque
    {
    public:
        typedef std::initializer_list<T> Values;

        struct Offset
        {
            size_t left = SIZE_MAX;
            size_t right = 0;
        };

        struct Block
        {
            T *cache = new T[block];

            Block *left = nullptr;
            Block *right = nullptr;

            explicit Block(Block *left, Block *right = nullptr);

            Block();

            ~Block();
        };

        struct Iterator
        {
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T *;
            using reference = T &;

            Iterator(Block *start, Offset offset, size_t index);
            Iterator(Block *start, Offset offset);
            Iterator(const Iterator &iterator);

            void next(bool right = false);
            void prev(bool right = true);
            size_t rest(bool right = true);
            void reset(bool right = false);

            inline pointer address();

            inline reference operator*();
            inline pointer operator->();

            Iterator &operator++();
            [[nodiscard]] Iterator operator++(int);

            Iterator &operator+=(int offset);
            [[nodiscard]] Iterator operator+(int offset);

            Iterator &operator--();
            [[nodiscard]] Iterator operator--(int);

            Iterator &operator-=(int offset);
            [[nodiscard]] Iterator operator-(int offset);

            inline bool operator==(const Iterator &another) const;
            inline bool operator!=(const Iterator &another) const;

        private:
            size_t index;

            Block *current;
            Offset offset;
        };

        Deque(T *array, size_t length);

        template<size_t ablock>
        Deque(const Deque<T, ablock> &deque);

        Deque(const Deque<T, block> &deque);

        Deque(Values values);

        Deque();

        ~Deque();

        [[nodiscard]] inline bool empty() const;

        [[nodiscard]] inline bool reaches(size_t length) const;

        [[nodiscard]] size_t count() const;

        [[nodiscard]] Iterator begin() const;

        [[nodiscard]] Iterator end() const;

        [[nodiscard]] Iterator leftStart() const;

        [[nodiscard]] Iterator rightStart() const;

        [[nodiscard]] Iterator leftStop() const;

        [[nodiscard]] Iterator rightStop() const;

        [[nodiscard]] inline T &get(size_t index) const;

        inline void set(size_t index, const T &value);

        void pushRight(const T *array, size_t length);

        template<size_t ablock>
        void pushRight(const Deque<T, ablock> &deque);

        void pushRight(const Deque<T, block> &deque);

        void pushRight(Values values);

        void pushRight(const T &instance);

        void pushLeft(const T *array, size_t length);

        template<size_t ablock>
        void pushLeft(const Deque<T, ablock> &deque);

        void pushLeft(const Deque<T, block> &deque);

        void pushLeft(Values values);

        void pushLeft(const T &instance);

        template<size_t rblock = block>
        [[nodiscard]] Deque<T, rblock> popRight(size_t length);

        [[nodiscard]] T popRight();

        template<size_t rblock = block>
        [[nodiscard]] Deque<T, rblock> popLeft(size_t length);

        [[nodiscard]] T popLeft();

        [[nodiscard]] inline T &operator[](size_t index);

        [[nodiscard]] inline const T &operator[](size_t index) const;

        template<class AT, size_t ablock>
        [[nodiscard]] bool operator==(const Deque<AT, ablock> &another) const;

        template<size_t ablock>
        [[nodiscard]] bool operator==(const Deque<T, ablock> &another) const;

        template<class AT, size_t ablock>
        [[nodiscard]] bool operator!=(const Deque<AT, ablock> &another) const;

        template<size_t ablock>
        [[nodiscard]] bool operator!=(const Deque<T, ablock> &another) const;

        template<size_t ablock>
        inline Deque<T, block> &operator<<(const Deque<T, ablock> &another);

        inline Deque<T, block> &operator<<(const T &instance);

        [[nodiscard]] inline T operator--();

        [[nodiscard]] inline T operator--(int);

        template<class AT, size_t rblock = block>
        [[nodiscard]] inline explicit operator Deque<AT, rblock>() const;

        template<size_t rblock = block>
        [[nodiscard]] inline explicit operator Deque<T, rblock>() const;

        [[nodiscard]] inline explicit operator bool() const;

    private:
        Offset offset;

        size_t length = 0;
        size_t blocks = 1;
        size_t size = block;

        Block *left = new Block();
        Block *right = left;

        inline void expandRight();

        inline void expandLeft();

        inline void shrinkRight();

        inline void shrinkLeft();

        T *getElement(size_t index) const;
    };

    template<class T, size_t block, size_t ablock>
    inline Deque<T, block> &operator>>(const Deque<T, ablock> &another, Deque<T, block> &deque);

    template<class T, size_t block>
    inline Deque<T, block> &operator>>(const T &instance, Deque<T, block> &deque);
}

#include "Deque.Block.inl"
#include "Deque.Iterator.inl"
#include "Deque.inl"

#endif