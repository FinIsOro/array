namespace arrays
{
    template<class T, size_t block>
    Deque<T, block>::Deque(T *array, size_t length)
    {
        pushRight(array, length);
    }

    template<class T, size_t block>
    template<size_t ablock>
    Deque<T, block>::Deque(const Deque <T, ablock> &deque)
    {
        pushRight(deque);
    }

    template<class T, size_t block>
    Deque<T, block>::Deque(const Deque <T, block> &deque)
    {
        pushRight(deque);
    }

    template<class T, size_t block>
    Deque<T, block>::Deque(Deque::Values values)
    {
        pushRight(values);
    }

    template<class T, size_t block>
    Deque<T, block>::Deque()
    = default;

    template<class T, size_t block>
    Deque<T, block>::~Deque()
    {
        Block *current = left;

        for (size_t index = 0; index < blocks; index++, current = current->right)
            delete current;
    }

    template<class T, size_t block>
    bool Deque<T, block>::empty() const
    {
        return length == 0;
    }

    template<class T, size_t block>
    bool Deque<T, block>::reaches(size_t length) const
    {
        return this->length >= length;
    }

    template<class T, size_t block>
    size_t Deque<T, block>::count() const
    {
        return length;
    }

    template<class T, size_t block>
    class Deque<T, block>::Iterator Deque<T, block>::begin() const
    {
        return Deque::Iterator(left, offset);
    }

    template<class T, size_t block>
    class Deque<T, block>::Iterator Deque<T, block>::end() const
    {
        return Deque::Iterator(right, offset, offset.right);
    }

    template<class T, size_t block>
    class Deque<T, block>::Iterator Deque<T, block>::leftStart() const
    {
        return Deque::Iterator(left, offset);
    }

    template<class T, size_t block>
    class Deque<T, block>::Iterator Deque<T, block>::rightStart() const
    {
        return Deque::Iterator(right, offset, 0);
    }

    template<class T, size_t block>
    class Deque<T, block>::Iterator Deque<T, block>::leftStop() const
    {
        return Deque::Iterator(left, offset, offset.left);
    }

    template<class T, size_t block>
    class Deque<T, block>::Iterator Deque<T, block>::rightStop() const
    {
        return Deque::Iterator(right, offset, offset.right);
    }

    template<class T, size_t block>
    T &Deque<T, block>::get(size_t index) const
    {
        T *element = getElement(index);

        return *element;
    }

    template<class T, size_t block>
    void Deque<T, block>::set(size_t index, const T &value)
    {
        T *element = getElement(index);

        *element = value;
    }

    template<class T, size_t block>
    void Deque<T, block>::pushRight(const T *array, size_t length)
    {
        for (size_t index = 0; index < length; index++)
        {
            if (offset.right == block)
                expandRight();

            right->cache[offset.right++] = array[index];
        }

        this->length += length;
    }

    template<class T, size_t block>
    template<size_t ablock>
    void Deque<T, block>::pushRight(const Deque<T, ablock> &deque)
    {
        auto dequeIterator = deque.leftStart();
        auto dequeEnd = deque.rightStop();

        for (; dequeIterator != dequeEnd; dequeIterator.next(false))
            pushRight(dequeIterator.address(), dequeIterator.rest());
    }

    template<class T, size_t block>
    void Deque<T, block>::pushRight(const Deque<T, block> &deque)
    {
        auto dequeIterator = deque.leftStart();
        auto dequeEnd = deque.rightStop();

        for (; dequeIterator != dequeEnd; dequeIterator.next(false))
            pushRight(dequeIterator.address(), dequeIterator.rest());
    }

    template<class T, size_t block>
    void Deque<T, block>::pushRight(Deque::Values values)
    {
        const T *begin = values.begin();

        pushRight(begin, values.end() - begin);
    }

    template<class T, size_t block>
    void Deque<T, block>::pushRight(const T &instance)
    {
        if (offset.right == block)
            expandRight();

        right->cache[offset.right++] = instance;

        length++;
    }

    template<class T, size_t block>
    void Deque<T, block>::pushLeft(const T *array, size_t length)
    {
        for (size_t index = length - 1; index != SIZE_MAX; index--)
        {
            if (offset.left == SIZE_MAX)
                expandLeft();

            left->cache[offset.left--] = array[index];
        }

        this->length += length;
    }

    template<class T, size_t block>
    template<size_t ablock>
    void Deque<T, block>::pushLeft(const Deque<T, ablock> &deque)
    {
        auto dequeIterator = deque.rightStart();
        auto dequeEnd = deque.leftStop();

        for (; dequeIterator != dequeEnd; dequeIterator.prev(false))
            pushLeft(dequeIterator.address(), dequeIterator.rest());
    }

    template<class T, size_t block>
    void Deque<T, block>::pushLeft(const Deque<T, block> &deque)
    {
        auto dequeIterator = deque.rightStart();
        auto dequeEnd = deque.leftStop();

        for (; dequeIterator != dequeEnd; dequeIterator.prev(false))
            pushLeft(dequeIterator.address(), dequeIterator.rest());
    }

    template<class T, size_t block>
    void Deque<T, block>::pushLeft(Deque::Values values)
    {
        const T *begin = values.begin();

        pushLeft(begin, values.end() - begin);
    }

    template<class T, size_t block>
    void Deque<T, block>::pushLeft(const T &instance)
    {
        if (offset.left == SIZE_MAX)
            expandLeft();

        left->cache[offset.left--] = instance;

        length++;
    }

    template<class T, size_t block>
    template<size_t rblock>
    Deque<T, rblock> Deque<T, block>::popRight(size_t length)
    {
        if (length > this->length)
            length = this->length;

        Deque<T, rblock> result;

        for (size_t index = 0; index < length; index++)
        {
            if (offset.right == 0) // offset.right - 1 == SIZE_MAX
                shrinkRight();

            result.pushLeft(right->cache[--offset.right]);
        }

        this->length -= length;

        return result;
    }

    template<class T, size_t block>
    T Deque<T, block>::popRight()
    {
        if (offset.right == 0) // offset.right - 1 == SIZE_MAX
            shrinkRight();

        T result = right->cache[--offset.right];

        length--;

        return result;
    }

    template<class T, size_t block>
    template<size_t rblock>
    Deque<T, rblock> Deque<T, block>::popLeft(size_t length)
    {
        if (length > this->length)
            length = this->length;

        Deque<T, rblock> result;

        for (size_t index = 0; index < length; index++)
        {
            if (offset.left == block - 1) // offset.left + 1 == block
                shrinkLeft();

            result.pushRight(left->cache[++offset.left]);
        }

        this->length -= length;

        return result;
    }

    template<class T, size_t block>
    T Deque<T, block>::popLeft()
    {
        if (offset.left == block - 1) // offset.left + 1 == block
            shrinkLeft();

        T result = left->cache[++offset.left];

        length--;

        return result;
    }

    template<class T, size_t block>
    T &Deque<T, block>::operator[](size_t index)
    {
        return get(index);
    }


    template<class T, size_t block>
    const T &Deque<T, block>::operator[](size_t index) const
    {
        return get(index);
    }

    template<class T, size_t block>
    template<class AT, size_t ablock>
    bool Deque<T, block>::operator==(const Deque<AT, ablock> &another) const
    {
        if (length != another.count())
            return false;

        auto thisIterator = begin();
        auto anotherIterator = another.begin();

        auto thisEnd = end();

        for (; thisIterator != thisEnd; ++thisIterator, ++anotherIterator)
            if ((AT) * thisIterator != *anotherIterator)
                return false;

        return true;
    }

    template<class T, size_t block>
    template<size_t ablock>
    bool Deque<T, block>::operator==(const Deque<T, ablock> &another) const
    {
        if (length != another.count())
            return false;

        auto thisIterator = begin();
        auto anotherIterator = another.begin();

        auto thisEnd = end();

        for (; thisIterator != thisEnd; ++thisIterator, ++anotherIterator)
            if (*thisIterator != *anotherIterator)
                return false;

        return true;
    }

    template<class T, size_t block>
    template<class AT, size_t ablock>
    bool Deque<T, block>::operator!=(const Deque<AT, ablock> &another) const
    {
        if (length != another.count())
            return true;

        auto thisIterator = begin();
        auto anotherIterator = another.begin();

        auto thisEnd = end();

        for (; thisIterator != thisEnd; thisIterator++, anotherIterator++)
            if ((AT) * thisIterator != *anotherIterator)
                return true;

        return false;
    }

    template<class T, size_t block>
    template<size_t ablock>
    bool Deque<T, block>::operator!=(const Deque<T, ablock> &another) const
    {
        if (length != another.count())
            return true;

        auto thisIterator = begin();
        auto anotherIterator = another.begin();

        auto thisEnd = end();

        for (; thisIterator != thisEnd; thisIterator++, anotherIterator++)
            if (*thisIterator != *anotherIterator)
                return true;

        return false;
    }

    template<class T, size_t block>
    template<size_t ablock>
    Deque<T, block> &Deque<T, block>::operator<<(const Deque<T, ablock> &another)
    {
        pushRight(another);

        return *this;
    }

    template<class T, size_t block>
    Deque<T, block> &Deque<T, block>::operator<<(const T &instance)
    {
        pushRight(instance);

        return *this;
    }

    template<class T, size_t block>
    T Deque<T, block>::operator--()
    {
        return popLeft();
    }

    template<class T, size_t block>
    T Deque<T, block>::operator--(int)
    {
        return popRight();
    }

    template<class T, size_t block>
    template<class AT, size_t rblock>
    Deque<T, block>::operator Deque<AT, rblock>() const
    {
        Deque<AT, rblock> result;

        auto iterator = begin();
        auto stop = end();

        for (; iterator != stop; iterator++)
            result.pushRight((AT) * iterator);

        return result;
    }

    template<class T, size_t block>
    template<size_t rblock>
    Deque<T, block>::operator Deque<T, rblock>() const
    {
        Deque<T, rblock> result;

        result.pushRight(*this);

        return result;
    }

    template<class T, size_t block>
    Deque<T, block>::operator bool() const
    {
        return length != 0;
    }

    template<class T, size_t block>
    void Deque<T, block>::expandRight()
    {
        right = new Block(right);

        offset.right = 0;

        size += block;
        blocks++;
    }

    template<class T, size_t block>
    void Deque<T, block>::expandLeft()
    {
        left = new Block(nullptr, left);

        offset.left = block - 1;

        size += block;
        blocks++;
    }

    template<class T, size_t block>
    void Deque<T, block>::shrinkRight()
    {
        if (blocks == 1)
            return;

        right = right->left;
        delete right->right;
        right->right = nullptr;

        offset.right = block;

        size -= block;
        blocks--;
    }

    template<class T, size_t block>
    void Deque<T, block>::shrinkLeft()
    {
        if (blocks == 1)
            return;

        left = left->right;
        delete left->left;
        left->left = nullptr;

        offset.left = SIZE_MAX;

        size -= block;
        blocks--;
    }

    template<class T, size_t block>
    T *Deque<T, block>::getElement(size_t index) const
    {
        Block *current = left;

        size_t currentLength = block - (offset.left + 1);

        while (index >= currentLength)
        {
            index -= currentLength;

            current = current->right;

            if (current == nullptr)
                return nullptr;

            if (currentLength != block)
                currentLength = block;
        }

        if (current == left)
            return current->cache + (offset.left + 1) + index;

        return current->cache + index;
    }

    template<class T, size_t block, size_t ablock>
    inline Deque<T, block> &operator>>(const Deque<T, ablock> &another, Deque<T, block> &deque)
    {
        deque.pushLeft(another);

        return deque;
    }

    template<class T, size_t block>
    inline Deque<T, block> &operator>>(const T &instance, Deque<T, block> &deque)
    {
        deque.pushLeft(instance);

        return deque;
    }
}