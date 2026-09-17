// Custom includes
#include "engine.h"
#include "trexel.h"

// C++ includes
#include <memory>


int main(int argc, char *argv[]) {
    Engine engine(std::make_unique<Trexel>());
    engine.run();
    return 0;
}
