#include <asio.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <stdexcept>


using namespace std;

const size_t max_udp_message_size = 0xffff-20-8; // 2^64-2-8

class Server {
  // UDP socket
  asio::ip::udp::socket socket;

  public:
    // constructor bind socket to port 3000
    Server(asio::io_context &io_context) : socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v6(), 3000)){}
    
    asio::awaitable<void> handle_request(asio::ip::udp::endpoint endpoint, string message){
      try{
        co_await socket.async_send_to(asio::buffer(message, message.length()), endpoint, asio::use_awaitable);

      } catch (const std::exception &e){
        cerr << "Server error: " << e.what() << endl;
      };
    };

    // cast string to int using stoi
    vector<int> parse_vector(const string& s){
      vector<int> v;
      stringstream ss(s);
      string token;
      while (getline(ss,token,',')){
        v.push_back(stoi(token));
      };
      return v;
    };
    // skalarprodukt
    int dot_product(vector<int>& v1, vector<int>& v2){
      if(v1.size() != v2.size()){
        throw runtime_error("vectors in dot product must have same dimention\n");
      };

      int result = 0;

      for (size_t i=0;i<v1.size();i++){
        result += v1[i] * v2[i];
      };

      return result;
    };

    asio::awaitable<void> start(){
      char buffer[max_udp_message_size];

      for (;;){
        try{
          asio::ip::udp::endpoint endpoint;
          auto bytes_transferred = co_await socket.async_receive_from(asio::buffer(buffer, max_udp_message_size), endpoint, asio::use_awaitable);
          auto message = string(buffer, bytes_transferred);

          cout << "Server: received: " << message << endl;

          size_t sep = message.find(";");
          if(sep == string::npos){
            cout << "wrong vector format" << endl;
          }

          vector<int> v1 = parse_vector(message.substr(0,sep));
          vector<int> v2 = parse_vector(message.substr(sep+1));
          

          int result = dot_product(v1,v2);
          
          string res = to_string(result);

          cout << "sent: " << res << " to " << endpoint.address() << ":" << endpoint.port() << endl;

          co_spawn(socket.get_executor(), handle_request(endpoint,res),asio::detached);
        }catch(const std::exception &e){
          cerr << "server error: " << e.what() << endl;
        };
      };
    };
};

int main(){
  asio::io_context event_loop(1);

  Server server(event_loop);
  co_spawn(event_loop, server.start(), asio::detached);

  event_loop.run();
}

