#define CATCH_CONFIG_MAIN
#include <extlib/Catch/single_include/catch.hpp>
#include <pseudo/type_traits.hpp>
#include <vector>
#include <list>
#include <array>

TEST_CASE("traits about iterator", "[has_iterator]")
{
    SECTION("has_iterator")
    {
        const bool vector = psd::has_iterator<std::vector<double>>::value;
        const bool list   = psd::has_iterator<std::list<double>>::value;
        const bool array  = psd::has_iterator<std::array<int, 3>>::value;

        REQUIRE(vector);
        REQUIRE(list);
        REQUIRE(array);
    }

    SECTION("is_iterator")
    {
        const bool vector  = psd::is_iterator<std::vector<double>::iterator>::value;
        const bool cvector = psd::is_iterator<std::vector<double>::const_iterator>::value;
        const bool list    = psd::is_iterator<std::list<double>::iterator>::value;
        const bool clist   = psd::is_iterator<std::list<double>::const_iterator>::value;
        const bool array   = psd::is_iterator<std::array<int, 3>::iterator>::value;
        const bool carray  = psd::is_iterator<std::array<int, 3>::const_iterator>::value;

        REQUIRE(vector);
        REQUIRE(list);
        REQUIRE(array);

        REQUIRE(cvector);
        REQUIRE(clist);
        REQUIRE(carray);
    }

    SECTION("iterator_of")
    {
        static_assert(std::is_same<std::vector<int>::iterator,
                      psd::iterator_of<std::vector<int>>::iterator>::value,
                      "");
        static_assert(std::is_same<std::vector<int>::const_iterator,
                      psd::iterator_of<std::vector<int>>::const_iterator>::value,
                      "");
        static_assert(std::is_same<std::vector<int>::reverse_iterator,
                      psd::iterator_of<std::vector<int>>::reverse_iterator>::value,
                      "");
        static_assert(std::is_same<std::vector<int>::const_reverse_iterator,
                      psd::iterator_of<std::vector<int>>::const_reverse_iterator>::value,
                      "");

        int a[10];
        static_assert(std::is_same<decltype(std::begin(a)),
                      psd::iterator_of<decltype(a)>::iterator>::value, "");
        static_assert(std::is_same<decltype(std::end(a)),
                      psd::iterator_of<decltype(a)>::iterator>::value, "");
    }

    SECTION("meta_op")
    {
        typedef std::true_type  t;
        typedef std::false_type f;
        const bool and_t_t = psd::meta_and<t, t>::value;
        const bool and_t_f = psd::meta_and<t, f>::value;
        const bool and_f_t = psd::meta_and<f, t>::value;
        const bool and_f_f = psd::meta_and<f, f>::value;
        const bool or_t_t  = psd::meta_or<t, t>::value;
        const bool or_t_f  = psd::meta_or<t, f>::value;
        const bool or_f_t  = psd::meta_or<f, t>::value;
        const bool or_f_f  = psd::meta_or<f, f>::value;
        const bool not_t   = psd::meta_not<t>::value;
        const bool not_f   = psd::meta_not<f>::value;

        REQUIRE( and_t_t);
        REQUIRE(!and_t_f);
        REQUIRE(!and_f_t);
        REQUIRE(!and_f_f);

        REQUIRE( or_t_t);
        REQUIRE( or_t_f);
        REQUIRE( or_f_t);
        REQUIRE(!or_f_f);

        REQUIRE(!not_t);
        REQUIRE( not_f);

        const bool and_t_t_t_t = psd::meta_and<t, t, t, t>::value;
        const bool and_t_t_t_f = psd::meta_and<t, t, t, f>::value;
        const bool or_f_f_f_f = psd::meta_or<f, f, f, f>::value;
        const bool or_f_f_f_t = psd::meta_or<f, f, f, t>::value;

        REQUIRE( and_t_t_t_t);
        REQUIRE(!and_t_t_t_f);
        REQUIRE(!or_f_f_f_f);
        REQUIRE( or_f_f_f_t);
    }

    SECTION("is_all")
    {
        const bool is_all_integer = psd::is_all<std::is_integral, char, short, int, long, long long>::value;
        REQUIRE(is_all_integer);
    }

    SECTION("concated_tuple")
    {
        const bool concat1 = std::is_same<
            typename psd::concat_tuple<std::tuple<int>>::type, std::tuple<int>>::value;
        const bool concat2 = std::is_same<
            typename psd::concat_tuple<std::tuple<int, long>, std::tuple<float, double>>::type,
            std::tuple<int, long, float, double>>::value;
        const bool concat3 = std::is_same<
            typename psd::concat_tuple<std::tuple<int>, std::tuple<double>, std::tuple<char>>::type,
            std::tuple<int, double, char>>::value;
    }

    SECTION("partial_tuple")
    {
        const bool partial0 = std::is_same<
            typename psd::partial_tuple<0, std::tuple<int, long, unsigned>>::type,
            std::tuple<int>>::value;
        const bool partial1 = std::is_same<
            typename psd::partial_tuple<1, std::tuple<int, long, unsigned>>::type,
            std::tuple<int, long>>::value;
        const bool partial2 = std::is_same<
            typename psd::partial_tuple<2, std::tuple<int, long, unsigned>>::type,
            std::tuple<int, long, unsigned>>::value;
    }

}

