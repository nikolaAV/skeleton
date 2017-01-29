#include <string>
#include <iostream>
#include "tut.h"

using namespace tut;

int main(int, char**)
{
    auto& test_engine = runner.get();
    reporter reporter{};
    test_engine.set_callback(&reporter);

    test_engine.run_tests();

    std::cin.get();
    return !reporter.all_ok();
}

/*
#include <tut/tut_main.hpp>
#include <tut/tut_xml_reporter.hpp>

int main(int argc, const char *argv[])
{
    try
    {
        xml_reporter reporter(std::cout);
        runner.get().set_callback(&reporter);

        tut_main(argc, argv);
        return !reporter.all_ok();
    }
    catch(const no_such_group &ex)
    {
        std::cerr << "No such group: " << ex.what() << std::endl;
    }
    catch(const no_such_test &ex)
    {
        std::cerr << "No such test: " << ex.what() << std::endl;
    }
    catch(const tut_error &ex)
    {
        std::cout << "General error: " << ex.what() << std::endl;
    }
    return 1;
}
*/
