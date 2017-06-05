#define CATCH_CONFIG_MAIN
#include <extlib/Catch/single_include/catch.hpp>
#include <pseudo/xorshift.hpp>

TEST_CASE("xorshift random number generator engine", "[xorshift]")
{
    SECTION("discard")
    {
        const std::size_t N=10;
        psd::xorshift32 x32, y32;
        for(std::size_t i=0; i<N; ++i)
            x32();
        y32.discard(N);

        REQUIRE(x32() == y32());


        psd::xorshift64 x64, y64;
        for(std::size_t i=0; i<N; ++i)
            x64();
        y64.discard(N);

        REQUIRE(x64() == y64());

        psd::xorshift96 x96, y96;
        for(std::size_t i=0; i<N; ++i)
            x96();
        y96.discard(N);

        REQUIRE(x96() == y96());

        psd::xorshift128 x128, y128;
        for(std::size_t i=0; i<N; ++i)
            x128();
        y128.discard(N);

        REQUIRE(x128() == y128());

        psd::xorshift160 x160, y160;
        for(std::size_t i=0; i<N; ++i)
            x160();
        y160.discard(N);

        REQUIRE(x160() == y160());
    }
}
