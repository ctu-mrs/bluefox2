#include <iostream>
#include <mvIMPACT_CPP/mvIMPACT_acquire.h>

int main() {
    mvIMPACT::acquire::DeviceManager dev_mgr;
    const auto dev_cnt = dev_mgr.deviceCount();
    
    if (dev_cnt == 0) {
        std::cout << "No cameras found!" << std::endl;
        return 1;
    }
    
    std::cout << "Available cameras:" << std::endl;
    for (decltype(dev_mgr.deviceCount()) i = 0; i < dev_cnt; ++i) {
        auto device = dev_mgr.getDevice(i);
        std::cout << "  Serial: " << device->serial.read() 
                  << ", Product: " << device->product.read() << std::endl;
    }
    
    // Auto-generate launch command for first camera
    if (dev_cnt > 0) {
        std::string first_serial = dev_mgr.getDevice(0)->serial.read();
        std::cout << "\nTo launch with first camera:" << std::endl;
        std::cout << "ros2 launch bluefox2 single.launch.py device:=" << first_serial << std::endl;
    }
    
    return 0;
}