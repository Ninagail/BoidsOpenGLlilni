int calcul()
{
}

#include "doctest/doctest.h"

// This is just an example of how to use Doctest in order to write tests.
// To learn more about Doctest, see https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
TEST_CASE("Calcul renvoie toujoruse 5")
{
    CHECK(calcul() == 5);
}