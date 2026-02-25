// Node js TCP module
const net = require('net');

// TCP server port 3000. Bare levere HTML. Når client (nettleser) åpner localhost:3000 sendes automatisk HTTP req.  
const httpServer = net.createServer((connection) => {
  connection.on("error", ()=>{})

  connection.on('data', () => {
    // js: oppretter websocket connection til port 3001 og sender 'hello' når koblingen er klar
    // Viser alert når mottar melding fra server ws.onmessage
    let content = `<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8" />
  </head>
  <body>
    <h1>WebSocket test page</h1>
    <input type="text" id="input">
    <button id="sendBtn">Send to server</button>
      
    <script>
      let ws = new WebSocket('ws://localhost:3001');
      ws.onmessage = (event) => {
        console.log("Server: " + event.data);
        alert('Message from server: ' + event.data);
      };

      ws.onopen = () => {
        ws.send('client: hello');
        console.log("Websocket connected");
      }
      
      ws.onclose = () =>{
        console.log("Websocket closed");  
      }


      function sendMessage(){
        const inputElement = document.getElementById("input");
        const inputValue = inputElement.value;
        if(ws.readyState === WebSocket.OPEN){
          ws.send(inputValue);
          inputElement.value = Math.random()*10;
        }else{
          alert("Websocket not connected");  
        }
      }
      document.getElementById("sendBtn").addEventListener("click", sendMessage);
      document.getElementById("input").addEventListener("keydown", (e)=>{
        if(e.key === "enter") sendMessage();
      })



    </script>
  </body>
</html>
`;
    connection.write('HTTP/1.1 200 OK\r\nContent-Length: ' + content.length + '\r\n\r\n' + content);
  });
});
httpServer.listen(3000, () => {
  console.log('HTTP server listening on port 3000');
});

// SERVER

// Alle websocket connections
const clients = new Set();

// nettleser sender req med sec-websocket-key og 101. 
function handshake(socket, req){

  const crypto = require("crypto")
  const magicString = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"


  const [reqLine, ...headerLines] = req.split("\n").map(item=> item.trim()) 
  // heeaders to object
  const headers = Object.fromEntries(
    headerLines.map(line =>{
      const [key, ...rest] = line.split(":");
      return [key.trim().toLowerCase(),rest.join(":").trim()];
    })
  )
 
  const {upgrade, connection, "sec-websocket-key":secWebsocketKey,"sec-websocket-version":secWebsocketVersion}= headers;

  const sec_websocket_accept = crypto.createHash('sha1').update(secWebsocketKey+magicString).digest('base64');


  return ["HTTP/1.1 101 Switching Protocols",
"Upgrade: websocket",
"Connection: Upgrade",
`Sec-WebSocket-Accept: ${sec_websocket_accept}`,"\r\n",].join("\r\n");
}

// Nettleser maskerer alltid data med XOR. dekoder til string
function decode(buffer){
  let bytes = buffer;
  let length = bytes[1] & 127;      // Maskerer bort MASK-biten, får lengde = 3
  let maskStart = 2;                  // Masken starter på byte 2
  let dataStart = maskStart + 4;      // Data starter etter 4 maske-bytes
  let result = "";

  for (let i = dataStart; i < dataStart + length; i++) {
    let byte = bytes[i] ^ bytes[maskStart + ((i - dataStart) % 4)];
    result += String.fromCharCode(byte);
  }
  return result;
}

// Serveren kan ikke sende raw string da client forventer websocket frames. 
function createFrame(message){
  const payload = Buffer.from(message, "utf-8");
//  Pakkes tekst inn i frame 
  const header = Buffer.from([0x81, payload.length]);
  return Buffer.concat([header,payload])
}


function broadcast(message, sender){
  let frame;
  clients.forEach(client =>{
      client.write(createFrame(message));
  })
}


// Selve WebSocket server. Initailiseres som en vanlig TCP. Websocket lages av hanshake og oppgraderer protokollen til client med res. callback hver gang client connection
const wsServer = net.createServer((connection) => {
  console.log('Client connected to websocket');
  let handshakeDone = false
  let res = "";

  // Hver gang client sender data
  connection.on('data', (data) => {
    console.log("Server: Data from client: ", data.toString());
    if (!handshakeDone){
      res = handshake(connection, data.toString())
      console.log("res from hanshake:\n", res)
      connection.write(res); // sender oppgradert protokollen til client
      clients.add(connection)
      handshakeDone = true;
    } else {
      res = decode(data) // WebSocket frame til string
      console.log("res (websocket frames) from client:\n", res)
      broadcast(res, connection)
    }
  });

  // Når client kobler fra
  connection.on('end', () => {
    clients.delete(connection)
    console.log('Client disconnected from websocket');
  });
});
// Ved error som port allerede i bruk
wsServer.on('error', (error) => {
  console.error('Error:', error);
});
// Starter server i port 3001
wsServer.listen(3001, () => {
  console.log('WebSocket server listening on port 3001');
});

