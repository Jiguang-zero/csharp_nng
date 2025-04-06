//
// Created by 86158 on 2025/4/6.
//

#include "client_test.h"
#include "nng_package.h"

#include <iostream>
#include <string>
#include <chrono>
#include <thread>

int main() {
    std::thread f1(testWithPort, 3001, 100000, 0);
    std::thread f2(testWithPort, 3002, 34533, 520);
    std::thread f3(testWithPort, 3003, 353134, 1200);
    f1.join();
    f2.join();
    f3.join();

    return 0;
}

void testWithPort(const int port, const int times, const int interval) {
    const std::string url = "tcp://127.0.0.1:" + std::to_string(port);
    auto * nng = Nng::NewNngRequest(url.c_str());
    if (!nng) {
        std::cerr << port << " Cannot set up connection" << std::endl;
        return;
    }
    for (int i = 0; i < times; i ++ ) {
        const auto * response = reinterpret_cast<char*>(GetResponseWithRequest(nng, reinterpret_cast<const unsigned char *>("test"), 4));
        std::cout << port << ": ";
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        if (!response) {
            std::cerr << "response is nullptr" << std::endl;
            continue;
        }
        std::cout << response << std::endl;
        delete response;
    }
    delete nng;
}
