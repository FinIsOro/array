namespace arrays::utils
{
    template<class T>
    constexpr size_t blockSize()
    {
        size_t size = 128 / sizeof(T);

        return size < 8 ? 8 : size;
    }
}