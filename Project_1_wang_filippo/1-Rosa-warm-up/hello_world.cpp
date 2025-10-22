#include <iostream>
#include <unistd.h>

int main() {
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    std::cout << "Hello World! from: " << hostname << std::endl;
    return 0;
}