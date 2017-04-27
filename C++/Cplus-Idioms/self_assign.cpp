

/*

T::operator= that handles the case where the LHS and the RHS refer to the same object.



*/


T& operator= (const T& that)
{
    if (this == &that)
        return *this;

    // handle assignment here

    return *this;
}
