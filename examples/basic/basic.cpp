#include <iostream>
#include "viface/viface.hpp"

using namespace std;

class MyDispatcher
{
    private:

        int count = 0;

    public:

        void handler(string const& name, uint id, vector<uint8_t>& packet) {
            cout << "+++ Received packet " << count;
            cout << " from interface " << name;
            cout << " (" << id << ") of size " << packet.size() << endl;
            this->count++;
        }
};

int main(int argc, const char* argv[])
{
    string name = "viface0";
    if (argc > 1) {
        name = string(argv[1]);
    }

    string ip = "192.168.25.46";
    if (argc > 2) {
        ip = string(argv[2]);
    }

    string mac = "ec:f1:f8:d5:47:6b";
    if (argc > 3) {
        mac = string(argv[3]);
    }

    try {
        // Create interface
        viface::VIface iface(name);

        // Configure interface
        iface.setIPv4(ip);
        iface.setMAC(mac);

        // Bring up interface
        iface.up();

        // Call dispatch
        cout << "Starting packet printer for " << name << " ..." << endl;

        set<viface::VIface*> myifaces = {&iface};

        MyDispatcher printer;
        viface::dispatcher_cb mycb = bind(
            &MyDispatcher::handler,
            &printer,
            placeholders::_1,
            placeholders::_2,
            placeholders::_3
            );

        viface::dispatch(myifaces, mycb);
    } catch(exception const & ex) {
        cerr << ex.what() << endl;
        return -1;
    }

    return 0;
}