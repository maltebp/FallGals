#define CATCH_CONFIG_RUNNER
#include <External/catch2.hpp>


// Test files ------------------------------------------------------------------

#include "Core/EntityTests.hpp"

// -----------------------------------------------------------------------------


int main(int argc, const char** argv) {

    std::vector<const char*> args;
    for (int i = 0; i < argc; i++) {
        args.push_back(argv[i]);
    }
	
	// Test arguments:
	//args.push_back("-b"); // Break when assertion failed

    return Catch::Session().run((int)args.size(), args.data());
}