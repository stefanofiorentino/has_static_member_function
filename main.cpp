#include <iostream>

template<typename, typename = void>
struct has_static_member_function : std::false_type
{
};

template<typename T>
struct has_static_member_function<T, std::void_t<decltype(T::print())> > : std::true_type
{
};

template<typename D, typename B>
struct has_print_and_is_base_of : std::integral_constant<bool,
        std::is_base_of<B, D>::value && has_static_member_function<D>::value>
{
};

struct Base
{
    static void print()
    {
        std::puts(__PRETTY_FUNCTION__);
    }
};

struct Struct : public Base
{
    static void print()
    {
        std::puts(__PRETTY_FUNCTION__);
    }
};

struct NotBasedStruct
{
    static void print()
    {
        std::puts(__PRETTY_FUNCTION__);
    }
};

namespace lib
{
    template<typename T>
    void print()
    {
        static_assert(has_print_and_is_base_of<T, Base>::value,
                      "T neither has a static print member or is based on Base");
        T::print();
    }
}

int main()
{
    static_assert(not has_static_member_function<int>::value, "has static print member");
    static_assert(has_static_member_function<Struct>::value, "hasn't static print member");
    static_assert(has_static_member_function<NotBasedStruct>::value, "hasn't static print member");
    static_assert(std::is_base_of<Base, Struct>::value, "is not base of Base");
    static_assert(not std::is_base_of<Base, NotBasedStruct>::value, "is base of Base");
    static_assert(has_print_and_is_base_of<Struct, Base>::value,
                  "is not derived by Base and/or hasn't a static print member");
    static_assert(not has_print_and_is_base_of<NotBasedStruct, Base>::value,
                  "is derived by Base and/or hasn't a static print member");

    lib::print<Base>();
    lib::print<Struct>();

//    lib::print<NotBasedStruct>(); // compilation error

    return 0;
}