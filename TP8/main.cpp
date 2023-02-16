#include "p6/p6.h"

int main()
{
    auto ctx = p6::Context{{.title = "TP2"}};
    ctx.maximize_window();

    // Declare your infinite update loop.
    ctx.update = [&]() {
        glClearColor(1.000f, 0.662f, 0.970f, 1.000f);
        glClear(GL_COLOR_BUFFER_BIT);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}