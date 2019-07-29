#ifndef MAYBEVALUE_H
#define MAYBEVALUE_H

template<class T>
struct MaybeValue {
   bool hasValue;
   T value;

   MaybeValue() : hasValue{false}, value{} {}
   explicit MaybeValue(T const &v) : hasValue{true}, value{v} {}
};

template<class T>
inline bool operator==(MaybeValue<T> const &lhs, MaybeValue<T> const &rhs)
{
   return (!lhs.hasValue && !rhs.hasValue) || lhs.value == rhs.value;
}

template<class T>
inline bool operator!=(MaybeValue<T> const &lhs, MaybeValue<T> const &rhs)
{
   return !(lhs == rhs);
}
#endif // MAYBEVALUE_H
