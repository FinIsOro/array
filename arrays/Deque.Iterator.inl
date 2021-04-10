namespace arrays
{
    template<class T, size_t block>
    Deque<T, block>::Iterator::Iterator(Deque::Block *start, Deque::Offset offset, size_t index) :
            index(index),
            current(start),
            offset(offset)
    { }

    template<class T, size_t block>
    Deque<T, block>::Iterator::Iterator(Deque::Block *start, Deque::Offset offset) :
            index(offset.left + 1),
            current(start),
            offset(offset)
    { }

    template<class T, size_t block>
    Deque<T, block>::Iterator::Iterator(const Deque::Iterator &iterator) :
            index(iterator.index),
            current(iterator.current),
            offset(iterator.offset)
    { }

    template<class T, size_t block>
    void Deque<T, block>::Iterator::next(bool right)
    {
        if (current->right != nullptr)
        {
            current = current->right;

            reset(right);
        }
        else
            index = offset.right;

    }

    template<class T, size_t block>
    void Deque<T, block>::Iterator::prev(bool right)
    {
        if (current->left != nullptr)
        {
            current = current->left;

            reset(right);
        }
        else
            index = offset.left;
    }

    template<class T, size_t block>
    size_t Deque<T, block>::Iterator::rest(bool right)
    {
        if (right)
        {
            if (current->right == nullptr)
                return offset.right - index;

            return block - index;
        }

        if (current->left == nullptr)
            return index + 1 - offset.left;

        return index + 1;
    }

    template<class T, size_t block>
    void Deque<T, block>::Iterator::reset(bool right)
    {
        if (right)
            if (current->right == nullptr)
                index = offset.right - 1;
            else
                index = block - 1;
        else if (current->left == nullptr)
            index = offset.left + 1;
        else
            index = 0;
    }

    template<class T, size_t block>
    typename Deque<T, block>::Iterator::pointer Deque<T, block>::Iterator::address()
    {
        return current->cache + index;
    }

    template<class T, size_t block>
    typename Deque<T, block>::Iterator::reference Deque<T, block>::Iterator::operator*()
    {
        return *(current->cache + index);
    }

    template<class T, size_t block>
    typename Deque<T, block>::Iterator::pointer Deque<T, block>::Iterator::operator->()
    {
        return current->cache + index;
    }

    template<class T, size_t block>
    typename Deque<T, block>::Iterator &Deque<T, block>::Iterator::operator++()
    {
        if (current->right != nullptr || current->right == nullptr && index < offset.right)
            index++;

        if (index >= block && current->right != nullptr)
        {
            index = 0;

            current = current->right;
        }

        return *this;
    }

    template<class T, size_t block>
    typename Deque<T, block>::Iterator Deque<T, block>::Iterator::operator++(int)
    {
        Iterator temp = *this;

        ++(*this);

        return temp;
    }

    template<class T, size_t block>
    typename Deque<T, block>::Iterator &Deque<T, block>::Iterator::operator+=(int offset)
    {
        size_t length;

        if (offset > 0)
            while ((length = rest()) != 0 && offset > 0)
            {
                offset -= length;

                if (length != block)
                    index += length;

                if (offset < 0)
                    index += offset;
                else
                    next();
            }
        else
            while ((length = rest(false)) != 0 && offset < 0)
            {
                offset += length;

                if (length != block)
                    index -= length;

                if (offset > 0)
                    index += offset;
                else
                    prev();
            }

        return *this;
    }

    template<class T, size_t block>
    typename Deque<T, block>::Iterator Deque<T, block>::Iterator::operator+(int offset)
    {
        Iterator result = *this;

        result += offset;

        return result;
    }

    template<class T, size_t block>
    typename Deque<T, block>::Iterator &Deque<T, block>::Iterator::operator--()
    {
        if (current->left != nullptr || current->left == nullptr && index > offset.left)
            index--;

        if (index == SIZE_MAX && current->left != nullptr)
        {
            index = block - 1;

            current = current->left;
        }

        return *this;
    }

    template<class T, size_t block>
    typename Deque<T, block>::Iterator Deque<T, block>::Iterator::operator--(int)
    {
        Iterator temp = *this;

        --(*this);

        return temp;
    }

    template<class T, size_t block>
    typename Deque<T, block>::Iterator &Deque<T, block>::Iterator::operator-=(int offset)
    {
        size_t length;

        if (offset > 0)
            while ((length = rest(false)) != 0 && offset > 0)
            {
                offset -= length;

                if (length != block)
                    index -= length;

                if (offset < 0)
                    index -= offset;
                else
                    prev();
            }
        else
            while ((length = rest()) != 0 && offset < 0)
            {
                offset += length;

                if (length != block)
                    index += length;

                if (offset > 0)
                    index -= offset;
                else
                    next();
            }

        return *this;
    }

    template<class T, size_t block>
    typename Deque<T, block>::Iterator Deque<T, block>::Iterator::operator-(int offset)
    {
        Iterator result = *this;

        result -= offset;

        return result;
    }

    template<class T, size_t block>
    bool Deque<T, block>::Iterator::operator==(const Deque::Iterator &another) const
    {
        return current == another.current && index == another.index;
    }

    template<class T, size_t block>
    bool Deque<T, block>::Iterator::operator!=(const Deque::Iterator &another) const
    {
        return current != another.current || index != another.index;
    }
}