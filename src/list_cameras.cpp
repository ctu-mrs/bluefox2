#include <iostream>
#include <mvIMPACT_CPP/mvIMPACT_acquire.h>

int main() {
    mvIMPACT::acquire::DeviceManager dev_mgr;
    const auto dev_cnt = dev_mgr.deviceCount();

    std::string devices = "";

    for (decltype(dev_mgr.deviceCount()) i = 0; i < dev_cnt; ++i) {
        auto device = dev_mgr.getDevice(i);
        devices += device->serial.read() + " ";
    }

    std::cout << devices << std::endl;
    
    return 0;
}