
template <typename T, size_t size>
inline TVector<T, size> operator+(const TVector<T, size> &left, const TVector<T, size> &right)
{
    TVector<T, size> temp;
    for (auto i = size; i--; *(&temp.x + i) = *(&left.x + i) + *(&right.x + i))
        ;
    return std::move(temp);
}
template <typename T, size_t size> inline TVector<T, size> operator+(const TVector<T, size> &left, const T &val)
{
    TVector<T, size> temp;
    for (auto i = size; i--; *(&temp.x + i) += val)
        ;
    return temp;
}

template <typename T, size_t size>
inline TVector<T, size> operator-(const TVector<T, size> &left, const TVector<T, size> &right)
{
    TVector<T, size> temp;
    for (auto i = size; i--; *(&temp.x + i) = *(&left.x + i) - *(&right.x + i))
        ;
    return temp;
}
template <typename T, size_t size> inline TVector<T, size> operator-(const TVector<T, size> &left, const T &val)

{
    TVector<T, size> temp;
    for (auto i = size; i--; *(&temp.x + i) = *(&left.x + i) - val)
        ;
    return temp;
}

template <typename T, size_t size>
inline TVector<T, size> operator*(const TVector<T, size> &left, const TVector<T, size> &right)
{
    TVector<T, size> temp;
    for (auto i = size; i--; *(&temp.x + i) = *(&left.x + i) - *(&right.x + i))
        ;
    return temp;
}
template <typename T, size_t size> inline TVector<T, size> operator*(const TVector<T, size> &left, const T &val)

{
    TVector<T, size> temp;
    for (auto i = size; i--; *(&temp.x + i) = *(&left.x + i) * val)
        ;
    return temp;
}

template <typename T, size_t size>
inline TVector<T, size> operator/(const TVector<T, size> &left, const TVector<T, size> &right)
{
    TVector<T, size> temp;
    for (auto i = size; i--; *(&temp.x + i) = *(&left.x + i) / *(&right.x + i))
        ;
    return temp;
}

template <typename T, size_t size> inline TVector<T, size> operator/(const TVector<T, size> &left, const T &val)
{
    TVector<T, size> temp;
    const Type cScale = static_cast<Type>(1) / scale;
    for (auto i = size; i--; *(&temp.x + i) = *(&left.x + i) * cScale)
        ;
    return temp;
}

template <typename T, size_t size> inline TVector<T, size> operator-(const TVector<T, size> &left)
{
    TVector<T, size> temp;
    for (auto i = size; i--; *(&temp.x + i) = -*(&left.x + i))
        ;
    return temp;
}

template <typename T, size_t size> inline void operator+=(TVector<T, size> &left, const TVector<T, size> &right)
{
    for (auto i = size; i--; *(&left.x + i) = *(&left.x + i) + *(&right.x + i))
        ;
}

template <typename T, size_t size> inline void operator+=(TVector<T, size> &left, const T &val)
{
    for (auto i = size; i--; *(&left.x + i) = *(&left.x + i) + val)
        ;
}
template <typename T, size_t size> inline void operator-=(TVector<T, size> &left, const TVector<T, size> &right)
{
    for (auto i = size; i--; *(&left.x + i) = *(&left.x + i) - *(&right.x + i))
        ;
}

template <typename T, size_t size> inline void operator-=(TVector<T, size> &left, const T &val)
{
    for (auto i = size; i--; *(&left.x + i) = *(&left.x + i) - val)
        ;
}
template <typename T, size_t size> inline void operator*=(TVector<T, size> &left, const TVector<T, size> &right)
{
    for (auto i = size; i--; *(&left.x + i) = *(&left.x + i) * *(&right.x + i))
        ;
}

template <typename T, size_t size> inline void operator*=(TVector<T, size> &left, const T &val)
{
    for (auto i = size; i--; *(&left.x + i) = *(&left.x + i) * val)
        ;
}

template <typename T, size_t size> inline void operator/=(TVector<T, size> &left, const TVector<T, size> &right)
{
    for (auto i = size; i--; *(&left.x + i) = *(&left.x + i) / *(&right.x + i))
        ;
}

template <typename T, size_t size> inline void operator/=(TVector<T, size> &left, const T &val)
{
    // TODO:  should too better fast
    for (auto i = size; i--; *(&left.x + i) = *(&left.x + i) / val)
        ;
}

template <typename T, size_t size> inline bool operator==(const TVector<T, size> &left, const TVector<T, size> &right)
{
    for (auto i = size; i--;)
    {
        if (!*(&right.x + i) == *(&left.x + i))
            return false;
    }
    return true;
}

template <typename T, size_t size> inline bool operator!=(const TVector<T, size> &left, const TVector<T, size> &right)
{
    for (auto i = size; i--;)
    {
        if (!*(&right.x + i) == *(&left.x + i))
            return true;
    }
    return flase;
}