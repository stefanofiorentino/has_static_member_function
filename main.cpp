#include <iostream>

template< typename, typename = void >
struct has_static_member_function : std::false_type { };

template< typename T >
struct has_static_member_function<T,
        std::void_t<decltype(T::print())>
> : std::true_type { };

struct Base
{};

struct Struct : public Base
{
    static void print()
    {}
};

int main()
{
    static_assert(not has_static_member_function<int>::value, "has static print member");
    static_assert(has_static_member_function<Struct>::value, "hasn't static print member");
    static_assert(std::is_base_of<Base, Struct>::value, "is not base of Base");

    return 0;
}