#include <emscripten.h>
extern "C" struct Axi{
public:
    Axi() = default;
    ~Axi() = default;
    int a_ = 0;
    EMSCRIPTEN_KEEPALIVE static void TestFunction();
};
