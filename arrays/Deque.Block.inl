namespace arrays
{
    template<class T, size_t block>
    Deque<T, block>::Block::Block()
    = default;

    template<class T, size_t block>
    Deque<T, block>::Block::Block(Deque::Block *left, Deque::Block *right) :
            left(left),
            right(right)
    {
        if (left != nullptr)
            left->right = this;

        if (right != nullptr)
            right->left = this;
    }

    template<class T, size_t block>
    Deque<T, block>::Block::~Block()
    {
        delete[] cache;
    }
}