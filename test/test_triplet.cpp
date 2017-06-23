#define CATCH_CONFIG_MAIN
#include <extlib/Catch/single_include/catch.hpp>
#include <pseudo/triplet.hpp>

TEST_CASE("triplet", "[triplet]")
{
    SECTION("piecewise_construct")
    {
        psd::triplet<psd::triplet<bool, int, double>,
                     psd::triplet<bool, int, double>,
                     psd::triplet<bool, int, double>
            > t(std::piecewise_construct,
                std::make_tuple(true, 42, 3.14),
                std::make_tuple(true, 69, 2.71),
                std::make_tuple(true,  2, 1.41));

        REQUIRE(t.first  == psd::make_triplet(true, 42, 3.14));
        REQUIRE(t.second == psd::make_triplet(true, 69, 2.71));
        REQUIRE(t.third  == psd::make_triplet(true,  2, 1.41));
    }

    SECTION("make_triplet")
    {
        bool   b = true;
        int    i = 42;
        double d = 3.14;

        auto t = psd::make_triplet(b, i, d);

        REQUIRE(t.first  == b);
        REQUIRE(t.second == i);
        REQUIRE(t.third  == d);
    }

    SECTION("tuple_interface")
    {
        bool   b = true;
        int    i = 42;
        double d = 3.14;
        auto   t = psd::make_triplet(b, i, d);

        REQUIRE(psd::get<0>(t) == b);
        REQUIRE(psd::get<1>(t) == i);
        REQUIRE(psd::get<2>(t) == d);
        REQUIRE(psd::tuple_size<decltype(t)>::value == 3);

        std::tuple<bool, int, double> tpl = psd::make_tuple(t);
        REQUIRE(psd::get<0>(tpl) == b);
        REQUIRE(psd::get<1>(tpl) == i);
        REQUIRE(psd::get<2>(tpl) == d);
    }
}
