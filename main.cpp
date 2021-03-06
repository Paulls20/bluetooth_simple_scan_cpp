#include <cstdlib>
#include <memory>
#include <iostream>
#include <cerrno>
extern "C"
{
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
}

int main(int argc, char **argv)
{
    int dev_id = hci_get_route(nullptr);
    int sock = hci_open_dev(dev_id);

    if (dev_id < 0 || sock < 0)
    {
        std::cerr << "opening socket:" << strerror(errno) << std::endl;
        std::_Exit(1);
    }

    constexpr int len  = 8;
    constexpr int max_rsp = 255;
    constexpr int flags = IREQ_CACHE_FLUSH;

    auto ii = std::make_unique<inquiry_info[]>(max_rsp);
    auto ii_ptr = ii.get();

    int num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii_ptr, flags);

    if (num_rsp < 0) std::cerr << "hci_inquiry:" << strerror(errno) << std::endl;

    std::string addr;
    addr.resize(19);
    std::string name;
    name.resize(250);

#ifdef _DEBUG
    std::cout << "sizeof(addr): " << addr.size() << "\n";
    std::cout << "sizeof(name): " << name.size() << "\n";
#endif

    for (int i = 0; i < num_rsp; ++i)
    {
        ba2str(&(ii_ptr[i].bdaddr), addr.data());
        if (hci_read_remote_name(sock, &(ii_ptr[i].bdaddr), name.size(),name.data(), 0) < 0)
        {
            name = "[unknown]";
        }
        std::cout << addr << " " << name << "\n";
    }

    close( sock );

    return 0;
}
