namespace arrays
{
    template<class T, size_t block>
    void Array<T, block>::move(size_t index, int offset)
    {
        if (index >= length)
            return;

        size_t length = this->length - index;

        if (offset == 0)
            return;
        else if (offset < 0)
            if (index == 0)
                this->offset -= offset;
            else if (this->offset + index < -offset)
                memmove(cache, cache + this->offset + index, sizeof(T) * length);
            else
                memmove(cache + this->offset + index + offset, cache + this->offset + index, sizeof(T) * length);
        else // offset > 0
        if (this->offset + index + offset + length > size)
        {
            size_t minSize = this->offset + index + offset + length;

            size = minSize - minSize % block + block;

            T* temp = new T[size];

            memcpy(temp, cache + offset + index, length);

            delete[] cache;

            cache = temp;
        }
        else
            memmove(cache + this->offset + index + offset, cache + this->offset + index, sizeof(T) * length);
    }

    template<class T, size_t block>
    void Array<T, block>::resize(size_t length)
    {
        if (offset <= block && offset + length <= size && offset + length + block * 2 >= size + 1)
        {
            this->length = length;

            return;
        }

        size = length - length % block + block;

        T* temp = new T[size];

        memcpy(temp, cache + offset, sizeof(T) * std::min(this->length, length));

        offset = 0;
        this->length = length;

        delete[] cache;

        cache = temp;
    }

    template<class T, size_t block>
    Array<T, block>::Array(const T *array, size_t length)
    {
        push(array, length);
    }

    template<class T, size_t block>
    template<size_t ablock>
    Array<T, block>::Array(const Array<T, ablock> &array)
    {
        push(array);
    }

    template<class T, size_t block>
    Array<T, block>::Array(const Array<T, block> &array)
    {
        push(array);
    }

    template<class T, size_t block>
    Array<T, block>::Array(Array::Values values)
    {
        push(values);
    }

    template<class T, size_t block>
    Array<T, block>::Array() :
        size(block),
        cache(new T[size])
    { }

    template<class T, size_t block>
    Array<T, block>::~Array()
    {
        delete[] cache;
    }

    template<class T, size_t block>
    bool Array<T, block>::empty() const
    {
        return length == 0;
    }

    template<class T, size_t block>
    bool Array<T, block>::reaches(size_t length) const
    {
        return this->length >= length;
    }

    template<class T, size_t block>
    bool Array<T, block>::has(size_t index, size_t length, const T &instance) const
    {
        if (index < this->length)
            for (size_t end = std::min(length, this->length - index); index < end; index++)
                if (cache[offset + index] == instance)
                    return true;

        return false;
    }

    template<class T, size_t block>
    bool Array<T, block>::has(size_t index, const T &instance) const
    {
        if (index < length)
            for (size_t end = length - index; index < end; index++)
                if (cache[offset + index] == instance)
                    return true;

        return false;
    }

    template<class T, size_t block>
    bool Array<T, block>::some(size_t index, size_t length, Array::Checker checker) const
    {
        if (index < this->length)
            for (size_t end = std::min(length, this->length - index); index < end; index++)
                if (checker(*this, index, cache[offset + index]))
                    return true;

        return false;
    }

    template<class T, size_t block>
    bool Array<T, block>::some(size_t index, Array::Checker checker) const
    {
        if (index < length)
            for (size_t end = length - index; index < end; index++)
                if (checker(*this, index, cache[offset + index]))
                    return true;

        return false;
    }

    template<class T, size_t block>
    T *Array<T, block>::find(size_t index, size_t length, Array::Checker checker) const
    {
        if (index < length)
            for (size_t end = length - index; index < end; index++)
                if (checker(*this, index, cache[offset + index]))
                    return cache + offset + index;

        return nullptr;
    }

    template<class T, size_t block>
    T *Array<T, block>::find(size_t index, Array::Checker checker) const
    {
        if (index < this->length)
            for (size_t end = std::min(length, this->length - index); index < end; index++)
                if (checker(*this, index, cache[offset + index]))
                    return true;

        return false;
    }

    template<class T, size_t block>
    template<size_t rblock>
    Array<T, rblock> Array<T, block>::filter(size_t index, size_t length, Array::Checker checker) const
    {
        Array<T, rblock> result;

        if (index < this->length)
            for (size_t end = std::min(length, this->length - index); index < end; index++)
                if (checker(*this, index, cache[offset + index]))
                    result.push(cache[offset + index]);

        return result;
    }

    template<class T, size_t block>
    template<size_t rblock>
    Array<T, rblock> Array<T, block>::filter(size_t index, Array::Checker checker) const
    {
        Array<T, rblock> result;

        if (index < length)
            for (size_t end = length - index; index < end; index++)
                if (checker(*this, index, cache[offset + index]))
                    result.push(cache[offset + index]);

        return result;
    }

    template<class T, size_t block>
    template<class AT, size_t rblock>
    Array<AT, rblock> Array<T, block>::map(size_t index, size_t length, Array::Convertor<AT> convertor) const
    {
        Array<AT, rblock> result;

        if (index < this->length)
            for (size_t end = std::min(length, this->length - index); index < end; index++)
                result.push(convertor(*this, index, cache[offset + index]));

        return result;
    }

    template<class T, size_t block>
    template<class AT, size_t rblock>
    Array<AT, rblock> Array<T, block>::map(size_t index, Array::Convertor<AT> convertor) const
    {
        Array<AT, rblock> result;

        if (index < length)
            for (size_t end = length - index; index < end; index++)
                result.push(convertor(*this, index, cache[offset + index]));

        return result;
    }

    template<class T, size_t block>
    template<size_t rblock>
    Array<T, rblock> Array<T, block>::slice(size_t index, size_t length) const
    {
        Array<T, rblock> result;

        if (index < this->length)
            return Array<T, rblock>(cache + offset + index, std::min(length, this->length - index));

        return Array<T, rblock>();
    }

    template<class T, size_t block>
    T &Array<T, block>::get(size_t index) const
    {
        return cache[offset + index];
    }

    template<class T, size_t block>
    void Array<T, block>::set(size_t index, const T &value)
    {
        if (index < length)
            cache[offset + index] = value;
    }

    template<class T, size_t block>
    const T *Array<T, block>::begin() const
    {
        return cache + offset;
    }

    template<class T, size_t block>
    const T *Array<T, block>::end() const
    {
        return cache + offset + length;
    }

    template<class T, size_t block>
    T *Array<T, block>::begin()
    {
        return cache + offset;
    }

    template<class T, size_t block>
    T *Array<T, block>::end()
    {
        return cache + offset + length;
    }

    template<class T, size_t block>
    size_t Array<T, block>::firstIndex(size_t offset, const T &instance) const
    {
        for (size_t index = this->offset + offset, end = this->offset + length; index < end; index++)
            if (cache[index] == instance)
                return index - this->offset;

        return length;
    }

    template<class T, size_t block>
    size_t Array<T, block>::lastIndex(size_t offset, const T &instance) const
    {
        for (size_t index = this->offset + std::min(offset, length - 1), end = this->offset; index >= end; index--)
            if (cache[index] == instance)
                return index - this->offset;
            else if (index == 0)
                break;

        return length;
    }

    template<class T, size_t block>
    size_t Array<T, block>::count() const
    {
        return length;
    }

    template<class T, size_t block>
    void Array<T, block>::put(size_t index, const T *array, size_t length)
    {
        if (index > this->length)
            return;
        else if (index == this->length)
        {
            push(array, length);

            return;
        }

        bool free = false;

        if (
                array >= cache && array < cache + size ||
                array + length >= cache && array + length < cache + size ||
                cache >= array && cache < array + length ||
                cache + size >= array && cache + size < array + length
                )
        {
            T* temp = new T[length];

            memcpy(temp, array, sizeof(T) * length);

            array = temp;

            free = true;
        }

        move(index, length);

        memmove(cache + offset + index, array, sizeof(T) * length);

        resize(this->length + length);

        if (free)
            delete[] array;
    }

    template<class T, size_t block>
    template<size_t ablock>
    void Array<T, block>::put(size_t index, const Array<T, ablock> &array)
    {
        put(index, array.begin(), array.count());
    }

    template<class T, size_t block>
    void Array<T, block>::put(size_t index, Array::Values values)
    {
        const T* begin = values.begin();
        const T* end = values.end();

        size_t length = end - begin;

        size_t oldLength = this->length;

        move(index, length);

        memcpy(cache + offset + index, begin, sizeof(T) * length);

        resize(oldLength + length);
    }

    template<class T, size_t block>
    void Array<T, block>::put(size_t index, const T &instance)
    {
        if (index > length)
            return;
        else if (index == length)
        {
            push(instance);

            return;
        }

        move(index, 1);

        cache[offset + index] = instance;

        resize(length + 1);
    }

    template<class T, size_t block>
    void Array<T, block>::push(const T *array, size_t length)
    {
        bool free = false;

        if (
                array >= cache && array < cache + size ||
                array + length >= cache && array + length < cache + size ||
                cache >= array && cache < array + length ||
                cache + size >= array && cache + size < array + length
                )
        {
            T* temp = new T[length];

            memcpy(temp, array, sizeof(T) * length);

            array = temp;

            free = true;
        }

        resize(this->length + length);

        memmove(cache + offset + this->length - length, array, sizeof(T) * length);

        if (free)
            delete[] array;
    }

    template<class T, size_t block>
    template<size_t ablock>
    void Array<T, block>::push(const Array<T, ablock> &array)
    {
        push(array.begin(), array.count());
    }

    template<class T, size_t block>
    void Array<T, block>::push(Array::Values values)
    {
        const T* begin = values.begin();
        const T* end = values.end();

        size_t length = end - begin;

        size_t oldLength = this->length;

        resize(oldLength + length);

        memcpy(cache + offset + oldLength, begin, sizeof(T) * length);
    }

    template<class T, size_t block>
    void Array<T, block>::push(const T &instance)
    {
        resize(length + 1);

        cache[offset + length - 1] = instance;
    }

    template<class T, size_t block>
    void Array<T, block>::unshift(const T *array, size_t length)
    {
        bool free = false;

        if (
                array >= cache && array < cache + size ||
                array + length >= cache && array + length < cache + size ||
                cache >= array && cache < array + length ||
                cache + size >= array && cache + size < array + length
                )
        {
            T* temp = new T[length];

            memcpy(temp, array, sizeof(T) * length);

            array = temp;

            free = true;
        }

        move(0, length);

        memmove(cache + offset, array, sizeof(T) * length);

        resize(this->length + length);

        if (free)
            delete[] array;
    }

    template<class T, size_t block>
    template<size_t ablock>
    void Array<T, block>::unshift(const Array<T, ablock> &array)
    {
        unshift(array.begin(), array.count());
    }

    template<class T, size_t block>
    void Array<T, block>::unshift(Array::Values values)
    {
        const T* begin = values.begin();
        const T* end = values.end();

        size_t length = end - begin;

        move(0, length);

        memcpy(cache + offset, begin, sizeof(T) * length);

        resize(this->length + length);
    }

    template<class T, size_t block>
    void Array<T, block>::unshift(const T &instance)
    {
        move(0, 1);

        cache[offset] = instance;

        resize(length + 1);
    }

    template<class T, size_t block>
    template<size_t rblock>
    Array<T, rblock> Array<T, block>::pull(size_t index, size_t length)
    {
        Array<T, rblock> result = slice(index, length);

        remove(index, length);

        return result;
    }

    template<class T, size_t block>
    T Array<T, block>::pull(size_t index)
    {
        T result = get(index);

        remove(index, 1);

        return result;
    }

    template<class T, size_t block>
    template<size_t rblock>
    Array<T, rblock> Array<T, block>::pop(size_t length)
    {
        size_t popIndex = std::max(this->length - length, (size_t)0);
        size_t popLength = this->length - popIndex;

        Array<T, rblock> result = slice(popIndex, popLength);

        remove(popIndex, popLength);

        return result;
    }

    template<class T, size_t block>
    T Array<T, block>::pop()
    {
        T result = get(length - 1);

        resize(length - 1);

        return result;
    }

    template<class T, size_t block>
    template<size_t rblock>
    Array<T, rblock> Array<T, block>::shift(size_t length)
    {
        Array<T, rblock> result = slice(0, length);

        remove(0, length);

        return result;
    }

    template<class T, size_t block>
    T Array<T, block>::shift()
    {
        T result = get(0);

        move(1, -1);

        if (this->length != 0)
            resize(length - 1);

        return result;
    }

    template<class T, size_t block>
    void Array<T, block>::remove(size_t index, size_t length)
    {
        if (index < this->length)
        {
            move(index + length, -length);
            resize(this->length - std::min(length, this->length - index));
        }
    }

    template<class T, size_t block>
    void Array<T, block>::remove(size_t index)
    {
        if (index < this->length)
        {
            move(index + 1, -1);
            resize(this->length - std::min((size_t)1, this->length - index));
        }
    }

    template<class T, size_t block>
    void Array<T, block>::flip()
    {
        int amount = length / 2;

        for (int index = 0; index < amount; index++)
            std::swap(cache[offset + index], cache[offset + length - index - 1]);
    }

    template<class T, size_t block>
    void Array<T, block>::sort(Array::Comparator comparator)
    {
        qsort((void*)(cache + offset), length, sizeof(T), (int(*)(const void *, const void *))comparator);
    }

    template<class T, size_t block>
    void Array<T, block>::clear()
    {
        resize(0);
    }

    template<class T, size_t block>
    template<class AT, size_t ablock>
    bool Array<T, block>::operator==(const Array<AT, ablock> &another) const
    {
        if (length != another.count())
            return false;
        else if (cache == another.cache)
            return true;

        const AT *anotherBegin = another.begin();

        for (size_t index = 0; index < length; index++)
            if (*(cache + offset + index) != *(anotherBegin + index))
                return false;

        return true;
    }

    template<class T, size_t block>
    template<class AT, size_t ablock>
    bool Array<T, block>::operator!=(const Array<AT, ablock> &another) const
    {
        if (length != another.count())
            return true;
        else if (cache == another.cache)
            return false;

        const AT *anotherBegin = another.begin();

        for (size_t index = 0; index < length; index++)
            if (*(cache + offset + index) == *(anotherBegin + index))
                return false;

        return true;
    }

    template<class T, size_t block>
    T &Array<T, block>::operator[](size_t index)
    {
        return cache[offset + index];
    }

    template<class T, size_t block>
    const T &Array<T, block>::operator[](size_t index) const
    {
        return cache[offset + index];
    }

    template<class T, size_t block>
    template<size_t ablock>
    Array<T, block> &Array<T, block>::operator<<(const Array<T, ablock> &another)
    {
        push(another);

        return *this;
    }

    template<class T, size_t block>
    Array<T, block> &Array<T, block>::operator<<(Array::Values values)
    {
        push(values);

        return *this;
    }

    template<class T, size_t block>
    Array<T, block> &Array<T, block>::operator<<(const T &instance)
    {
        push(instance);

        return *this;
    }

    template<class T, size_t block>
    T Array<T, block>::operator--()
    {
        return shift();
    }

    template<class T, size_t block>
    T Array<T, block>::operator--(int)
    {
        return pop();
    }

    template<class T, size_t block>
    template<class AT, size_t rblock>
    Array<T, block>::operator Array<AT, rblock>() const
    {
        Array<AT, rblock> result;

        for (size_t index = 0; index < length; index++)
            result.push((AT)*(cache + offset + index));

        return result;
    }

    template<class T, size_t block>
    Array<T, block>::operator bool() const
    {
        return length != 0;
    }

    template<class T, size_t block, size_t ablock>
    inline Array<T, block> &operator>>(const Array<T, ablock> &another, Array<T, block> &array)
    {
        array.unshift(another);

        return array;
    }

    template<class T, size_t block>
    inline Array<T, block> &operator>>(class Array<T, block>::Values values, Array<T, block> &array)
    {
        array.unshift(values);

        return array;
    }

    template<class T, size_t block>
    inline Array<T, block> &operator>>(const T &instance, Array<T, block> &array)
    {
        array.unshift(instance);

        return array;
    }
}