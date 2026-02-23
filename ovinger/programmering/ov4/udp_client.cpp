#include <asio.hpp>
#include <iostream>
#include <string>

using namespace std;

const size_t max_udp_message_size = 0xffff-20-8;

class Client{
  asio::ip::udp::socket socket;

  public:
    Client(asio::io_context& io_context) : socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v6(),0)){}

    asio::awaitable<void> start(){
      auto executor = co_await asio::this_coro::executor;
      
      auto endpoint = (co_await asio::ip::udp::resolver(executor)
          .async_resolve(asio::ip::udp::v6(), "localhost", to_string(3000), asio::use_awaitable))
          .begin()
          ->endpoint();

      string vector = "1,2,3;1,2,3";
      
      auto byte_transferred = co_await socket.async_send_to(asio::buffer(vector), endpoint, asio::use_awaitable);
      
      cout << "Client: sent vector: " << vector << " to " << endpoint.address() << ":" << endpoint.port() << endl;
      
      char buffer[max_udp_message_size];
      byte_transferred = co_await socket.async_receive_from(asio::buffer(buffer, max_udp_message_size), endpoint, asio::use_awaitable);
      cout << "Client: received: " << string(buffer, byte_transferred) << ", from " << endpoint.address() << ":" << endpoint.port() << endl;
  };
};

int main(){
  asio::io_context event_loop(1);
  Client client(event_loop);

  co_spawn(event_loop, client.start(), asio::detached);

  event_loop.run();
};



