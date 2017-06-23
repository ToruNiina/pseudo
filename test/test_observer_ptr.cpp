#define CATCH_CONFIG_MAIN
#include <extlib/Catch/single_include/catch.hpp>
#include <pseudo/memory.hpp>

struct X
{
    virtual ~X() noexcept = default;
    virtual int obtain() const = 0;
};

struct Y : public X
{
    Y(int i) : y(i){}
    int obtain() const override {return y;}
    int y;
};

TEST_CASE("handling pointer and resource", "[memory]")
{
    SECTION("ctor")
    {
        {
            psd::observer_ptr<int> op;
            REQUIRE(op.get() == nullptr);
        }
        {
            psd::observer_ptr<int> op(nullptr);
            REQUIRE(op.get() == nullptr);
        }
        {
            Y* y = new Y{42};
            {
                psd::observer_ptr<Y> yp(y);
                REQUIRE(yp->obtain() == 42);

                psd::observer_ptr<X> xp(yp);
                REQUIRE(xp->obtain() == 42);
            }
            delete y;
        }
    }

    SECTION("release")
    {
        {
            int* ip = new int{42};
            auto op = psd::make_observer(ip);
            REQUIRE(*op == 42);
            op.release();
            REQUIRE(op.get() == nullptr);
            delete ip;
        }
    }
    SECTION("reset")
    {
        {
            int* ip = new int{42};
            auto op = psd::make_observer(ip);
            REQUIRE(*op == 42);
            op.reset();
            REQUIRE(op.get() == nullptr);
            delete ip;
        }
        {
            int* ip1 = new int{42};
            int* ip2 = new int{54};
            auto op = psd::make_observer(ip1);
            REQUIRE(*op == 42);
            op.reset(ip2);
            REQUIRE(*op == 54);
            delete ip1;
            delete ip2;
        }
    }

    SECTION("swap")
    {
        {
            int* ip1 = new int{42};
            int* ip2 = new int{54};
            auto op1 = psd::make_observer(ip1);
            auto op2 = psd::make_observer(ip2);
            REQUIRE(*op1 == 42);
            REQUIRE(*op2 == 54);
            op1.swap(op2);
            REQUIRE(*op2 == 42);
            REQUIRE(*op1 == 54);
            delete ip1;
            delete ip2;
        }

        {
            int* ip1 = new int{42};
            int* ip2 = new int{54};
            auto op1 = psd::make_observer(ip1);
            auto op2 = psd::make_observer(ip2);
            REQUIRE(*op1 == 42);
            REQUIRE(*op2 == 54);
            psd::swap(op1, op2);
            REQUIRE(*op2 == 42);
            REQUIRE(*op1 == 54);
            delete ip1;
            delete ip2;
        }
    }

    SECTION("compare")
    {
        {
            int* ip1 = new int{42};
            auto op1 = psd::make_observer(ip1);
            auto op2 = psd::make_observer(ip1);
            REQUIRE(op1 == op2);
            delete ip1;
        }

        {
            int* ip1 = new int{42};
            int* ip2 = new int{54};
            auto op1 = psd::make_observer(ip1);
            auto op2 = psd::make_observer(ip2);
            REQUIRE(op1 != op2);
            delete ip1;
            delete ip2;
        }

        {
            int ia[2] = {42, 54};
            int* ip1 = static_cast<int*>(ia);
            int* ip2 = ip1 + 1;

            auto op1 = psd::make_observer(ip1);
            auto op2 = psd::make_observer(ip2);
            REQUIRE(op1 != op2);
            REQUIRE(op1 <  op2);
            REQUIRE(op1 <= op2);
            REQUIRE(op2 >  op1);
            REQUIRE(op2 >= op1);
        }
    }
}
