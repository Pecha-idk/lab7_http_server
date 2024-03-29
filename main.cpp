#include "iostream"
#include "prepareSuggests.hpp"
#include "server.hpp"
int main(int argc, char* argv[]) {
  try {
    if (argc != 3) {
      std::cerr << "Usage: " << argv[0] << " <address> <port>\n";
      std::cerr << "  For IPv4, try:\n";
      std::cerr << "    receiver 0.0.0.0 80\n";
      std::cerr << "  For IPv6, try:\n";
      std::cerr << "    receiver 0::0 80\n";
      return EXIT_FAILURE;
    }

    auto const address = net::ip::make_address(argv[1]);
    unsigned short port = static_cast<unsigned short>(std::atoi(argv[2]));

    net::io_context ioc{1};

    preparerSug prepSug =
        preparerSug("/home/boris/lab07_http_server/suggestions.json");

    std::thread thrSug([&prepSug] { prepSug.serveSuggestions(); });
    thrSug.detach();

    tcp::acceptor acceptor{ioc, {address, port}};
    tcp::socket socket{ioc};

    http_server(acceptor, socket, prepSug);

    ioc.run();
  } catch (std::exception const& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}