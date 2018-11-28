#include <cstdlib>
extern "C"
{
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
}
#include <memory>
#include <vector>
#include <iostream>

int main(int argc, char **argv)
{
    std::string addr;
    addr.resize(19);
    std::cout << "sizeof(addr): " << addr.size() << std::endl;
    std::string name;
    name.resize(250);
    std::cout << "sizeof(name): " << name.size() << std::endl;

    int dev_id = hci_get_route(nullptr);
    int sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) {
        std::cerr << "opening socket\n";
        std::_Exit(1);
    }

    constexpr int len  = 8;
    constexpr int max_rsp = 255;
    constexpr int flags = IREQ_CACHE_FLUSH;

    auto ii = std::make_unique<inquiry_info[]>( max_rsp );

    auto ii_ptr = ii.get();
    int num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii_ptr, flags);

    if( num_rsp < 0 ) std::cerr << "hci_inquiry\n";

    for(int i = 0; i < num_rsp; ++i) {
        ba2str(&(ii_ptr[i].bdaddr), addr.data());
        if (hci_read_remote_name(sock, &(ii_ptr[i].bdaddr), name.size(),name.data(), 0) < 0) {
            name = "[unknown]";
        }

        std::cout << addr << " " << name << "\n";
    }

    close( sock );

    return 0;
}
