#include "mac/core/matrix.hpp"
#include <cassert>
#include <stdexcept>

int main() {
    mac::Matrix a(3, 4);
    assert(a.rows() == 3 && a.cols() == 4);
    assert(a.data().size() == 12);
    a.at(2, 1) = 9.0;
    assert(a(2, 1) == 9.0);
    assert(a.data()[9] == 9.0); // row-major: 2*4+1
    const mac::Matrix& ca = a;
    assert(ca.at(2, 1) == 9.0);
    bool thrown = false;
    try { (void)a.at(0, 4); } catch (const std::out_of_range&) { thrown = true; }
    assert(thrown);
    mac::Matrix empty(0, 3);
    assert(empty.data().empty());
}
