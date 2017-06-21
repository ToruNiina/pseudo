#define CATCH_CONFIG_MAIN
#include <extlib/Catch/single_include/catch.hpp>
#include <pseudo/zip_iterator.hpp>
#include <pseudo/make_zip.hpp>
#include <vector>
#include <list>
#include <forward_list>

TEST_CASE("zip several iterators to one iterator", "[zip_iterator]")
{
    std::vector<int>    v1{1, 2,  3,   4,    5};
    std::vector<double> v2{3.,3.1,3.14,3.141,3.1415};
    std::list<char>     l1{'a', 'b', 'c', 'd', 'e'};
    std::forward_list<char> l2{'a', 'b', 'c', 'd', 'e'};

    REQUIRE(v1.size() == 5);
    REQUIRE(v1.size() == v2.size());
    REQUIRE(v1.size() == l1.size());

    SECTION("category")
    {
        const auto random  = psd::make_zip(v1.begin(), v2.begin());
        const auto bidirc  = psd::make_zip(l1.begin(), v2.begin());
        const auto forward = psd::make_zip(l2.begin(), v2.begin());

        const bool random_access =
            std::is_same<typename decltype(random)::iterator_category,
                         std::random_access_iterator_tag>::value;
        const bool bidirectional =
            std::is_same<typename decltype(bidirc)::iterator_category,
                         std::bidirectional_iterator_tag>::value;
        const bool forward_linked =
            std::is_same<typename decltype(forward)::iterator_category,
                         std::forward_iterator_tag>::value;

        REQUIRE(random_access);
        REQUIRE(bidirectional);
        REQUIRE(forward_linked);
    }

    SECTION("comparing operators")
    {
              auto begin1 = psd::make_zip(v1.begin(), v2.begin());
        const auto begin2 = psd::make_zip(v1.begin(), v2.begin());
        const auto end    = psd::make_zip(v1.end(),   v2.end());
        REQUIRE(begin1 == begin2);
        REQUIRE(begin1 != end);
        REQUIRE(begin2 != end);
        REQUIRE(begin1 < end);
        REQUIRE(begin2 < end);

        ++begin1;

        REQUIRE(begin1 != begin2);
        REQUIRE(begin1 >  begin2);
        REQUIRE(begin1 <  end);
    }

    SECTION("accesser")
    {
        auto begin = psd::make_zip(v1.begin(), v2.begin(), l1.begin(), l2.begin());
        auto end   = psd::make_zip(v1.end(),   v2.end(),   l1.end(),   l2.end());

        REQUIRE(begin.ref<0>()  == 1);
        REQUIRE(begin.ref<1>()  == 3.);
        REQUIRE(begin.ref<2>()  == 'a');
        REQUIRE(begin.ref<3>()  == 'a');
        REQUIRE(*begin.ptr<0>() == 1);
        REQUIRE(*begin.ptr<1>() == 3.);
        REQUIRE(*begin.ptr<2>() == 'a');
        REQUIRE(*begin.ptr<3>() == 'a');
        REQUIRE(std::get<0>(*begin) == 1);
        REQUIRE(std::get<1>(*begin) == 3.);
        REQUIRE(std::get<2>(*begin) == 'a');
        REQUIRE(std::get<3>(*begin) == 'a');

        ++begin;
        REQUIRE(begin.ref<0>() == 2);
        REQUIRE(begin.ref<1>() == 3.1);
        REQUIRE(begin.ref<2>() == 'b');
        REQUIRE(begin.ref<3>() == 'b');
        REQUIRE(*begin.ptr<0>() == 2);
        REQUIRE(*begin.ptr<1>() == 3.1);
        REQUIRE(*begin.ptr<2>() == 'b');
        REQUIRE(*begin.ptr<3>() == 'b');
        REQUIRE(std::get<0>(*begin) == 2);
        REQUIRE(std::get<1>(*begin) == 3.1);
        REQUIRE(std::get<2>(*begin) == 'b');
        REQUIRE(std::get<3>(*begin) == 'b');

        ++begin;
        REQUIRE(begin.ref<0>() == 3);
        REQUIRE(begin.ref<1>() == 3.14);
        REQUIRE(begin.ref<2>() == 'c');
        REQUIRE(begin.ref<3>() == 'c');
        REQUIRE(*begin.ptr<0>() == 3);
        REQUIRE(*begin.ptr<1>() == 3.14);
        REQUIRE(*begin.ptr<2>() == 'c');
        REQUIRE(*begin.ptr<3>() == 'c');
        REQUIRE(std::get<0>(*begin) == 3);
        REQUIRE(std::get<1>(*begin) == 3.14);
        REQUIRE(std::get<2>(*begin) == 'c');
        REQUIRE(std::get<3>(*begin) == 'c');

        ++begin;
        REQUIRE(begin.ref<0>() == 4);
        REQUIRE(begin.ref<1>() == 3.141);
        REQUIRE(begin.ref<2>() == 'd');
        REQUIRE(begin.ref<3>() == 'd');
        REQUIRE(*begin.ptr<0>() == 4);
        REQUIRE(*begin.ptr<1>() == 3.141);
        REQUIRE(*begin.ptr<2>() == 'd');
        REQUIRE(*begin.ptr<3>() == 'd');
        REQUIRE(std::get<0>(*begin) == 4);
        REQUIRE(std::get<1>(*begin) == 3.141);
        REQUIRE(std::get<2>(*begin) == 'd');
        REQUIRE(std::get<3>(*begin) == 'd');

        ++begin;
        REQUIRE(begin.ref<0>() == 5);
        REQUIRE(begin.ref<1>() == 3.1415);
        REQUIRE(begin.ref<2>() == 'e');
        REQUIRE(begin.ref<3>() == 'e');
        REQUIRE(*begin.ptr<0>() == 5);
        REQUIRE(*begin.ptr<1>() == 3.1415);
        REQUIRE(*begin.ptr<2>() == 'e');
        REQUIRE(*begin.ptr<3>() == 'e');
        REQUIRE(std::get<0>(*begin) == 5);
        REQUIRE(std::get<1>(*begin) == 3.1415);
        REQUIRE(std::get<2>(*begin) == 'e');
        REQUIRE(std::get<3>(*begin) == 'e');

        ++begin;
        REQUIRE(begin == end);
    }

    SECTION("traverse")
    {
        auto begin = psd::make_zip(v1.begin(), v2.begin());
        auto end   = psd::make_zip(v1.end(),   v2.end());

        begin += 3;
        REQUIRE(begin.ref<0>()  == 4);
        REQUIRE(begin.ref<1>()  == 3.141);
        REQUIRE(*begin.ptr<0>() == 4);
        REQUIRE(*begin.ptr<1>() == 3.141);
        begin -= 3;
        REQUIRE(begin.ref<0>()  == 1);
        REQUIRE(begin.ref<1>()  == 3.);
        REQUIRE(*begin.ptr<0>() == 1);
        REQUIRE(*begin.ptr<1>() == 3.);
    }

    SECTION("modifier")
    {
        auto begin = psd::make_zip(v1.begin(), v2.begin(), l1.begin(), l2.begin());
        auto end   = psd::make_zip(v1.end(),   v2.end(),   l1.end(),   l2.end());
        REQUIRE(std::get<0>(*begin) == 1);
        REQUIRE(std::get<1>(*begin) == 3.);
        REQUIRE(std::get<2>(*begin) == 'a');
        REQUIRE(std::get<3>(*begin) == 'a');

        std::get<0>(*begin) = 5;
        std::get<1>(*begin) = 2.;
        std::get<2>(*begin) = 'e';
        std::get<3>(*begin) = 'e';

        REQUIRE(std::get<0>(*begin) == 5);
        REQUIRE(std::get<1>(*begin) == 2.);
        REQUIRE(std::get<2>(*begin) == 'e');
        REQUIRE(std::get<3>(*begin) == 'e');

        REQUIRE(v1.front() == 5);
        REQUIRE(v2.front() == 2.);
        REQUIRE(l1.front() == 'e');
        REQUIRE(l2.front() == 'e');
    }
}
