import socket 

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR,1)

port = 3000
host = "localhost"

server.bind((host, port))
server.listen()

def handle_request(req):
    lines = req.split("\n")
    req_line = lines[0]
    method, path, _ = req_line.split()

    status = "HTTP/1.1 200 OK"

    if (method != "GET"):
        status = "HTTP/1.1 404 Not Found"
        body = "<h1>Not found </h1>"
    elif path == "/":
        body = "<h1>Path: /</h1>"
    elif path == "/page1":
        body = "<h1>Path; /page1</h1>" 
    elif path == "/page2":
        body = "<h1>Path: /page2</h1>"
    else:
        status = "HTTP/1.1 404 Not Found"
        body = "<h1>Not found </h1>"

    body_bytes = body.encode("utf-8")
    content_type = "text/html; charset=utf-8"
    content_len = len(body_bytes)
    headers = f"Content-Type: {content_type}\r\nContent-Length: {content_len}"

    # CRLF carriage return + line feed as line terminators
    res = f"{status}\r\n{headers}\r\n\r\n{body}" 

    return res

def get_connection():
    print(f"Server listenting on host {host} port {port}")
    print(f"running on {host}:{port}")
    
    try:
        while True:
            print("Server: waiting for connection")
            try:
                # client
                client_socket, client_address = server.accept()
                print("Server: connection from ", client_address)
            
                req = client_socket.recv(1024).decode("utf-8").rstrip()
                print(f"Request received: \n{req}")
                    
                if not req:
                    print("Empty?")
                    client_socket.close()
                    continue

                res = handle_request(req)
                client_socket.sendall(res.encode("utf-8"))
                
                client_socket.shutdown(socket.SHUT_WR)
                client_socket.close()
                print("Connectio closed")
            except Exception as error:
                print(f"error: {error}")
    except KeyboardInterrupt:
        print("\n\n Server shutting down by ctrl + C")
        server.close()
        print("\n server.close()")

if __name__ == "__main__":
    get_connection()
