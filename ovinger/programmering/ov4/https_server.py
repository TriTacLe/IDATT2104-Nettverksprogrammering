import socket, ssl 

# Opprette socket
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# Socket opsjoner
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR,1)

port = 3000
host = "localhost"

server.bind((host, port))
server.listen()

def handle_request(req):
	# Parse req
    lines = req.splitlines() 
    req_line = lines[0] # første linja er HTTP/1.1 
    method, path, _ = req_line.split()

    status = "HTTP/1.1 200 OK"
	
	# Routing
    if (method != "GET"):
        status = "HTTP/1.1 405 Method Not Allowed"
        body = "<h1>Not found </h1>"
    elif path == "/":
        body = "<h1>GET: /</h1>"
    elif path == "/page1":
        body = "<h1>GET: /page1</h1>" 
    elif path == "/page2":
        body = "<h1>GET: /page2</h1>"
    else:
        status = "HTTP/1.1 404 Not Found"
        body = "<h1>Not found </h1>"

    body_bytes = body.encode("utf-8") 
    # Definerer type content, nå HTML kode som renders og definerer tegnsett. Andre typer er application/json
    content_type = "text/html; charset=utf-8"
    # len slik at client (browser) vet når den har mottatt hele respons. Når socket lukkes tror client at forbindelsen er brutt midt overføring
    content_len = len(body_bytes) 

    # CRLF carriage return + line feed as line terminators. HTTP standard
    headers = (f"{status}\r\n"
        f"Content-Type: {content_type}\r\n"
        f"Content-Length: {content_len}\r\n"
        f"Connection: close\r\n"
        f"\r\n")

    # hele res som bytes og ikke string. string til bytes med encode
    res = headers.encode("utf-8") + body_bytes
    return res

def get_connection():
    print(f"Server listenting on host {host} port {port}")
    print(f"running on {host}:{port}")
    
    try:
        # Lag sslkontekst
        context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
        context.load_cert_chain(certfile="server.crt", keyfile="server.key")
                
        while True:
            print("Server: waiting for connection")
            try:
                # client and 3WHS
                # wrapper socketen med TLS 
                client_socket, client_address = server.accept()
                tls_client_socket = context.wrap_socket(client_socket, server_side=True)

                print("Server: connection from ", client_address)
           
                # HTTP-req kan være større enn 1024, men hardkoed for øving da req er enkelt
                req = tls_client_socket.recv(1024).decode("utf-8").rstrip() # opptil 1024 bytes fra socket, convert (decode) bytes til string
                print(f"Request received: \n{req}")
                    
                if not req:
                    print("Empty?")
                    tls_client_socket.close()
                    continue

                res = handle_request(req)
                tls_client_socket.sendall(res) # send all bytes til client, 
                
                tls_client_socket.shutdown(socket.SHUT_WR) # venter for at alle data er sent og ACK
                tls_client_socket.close()
                print("Connectio closed")
            except Exception as error:
                print(f"error: {error}")
    # Så ctrl + C funker og ikke gir støgg error
    except KeyboardInterrupt:
        print("\n\n Server shutting down by ctrl + C")
        server.close()
        print("\n server.close()")

if __name__ == "__main__":
    get_connection()
