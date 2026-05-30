# SOCKETmessanger
Simple bi-directional messanger on localhost (Client-Server architecture)
Created with C++ and Windows Sockets API (Winsock2).

# Compiling:
First of all, you have to **compile** it.
  Windows (MSVC - Visual Studio):
  cl /EHsc main.cpp /link ws2_32.lib
  
  Linux:
  git clone https://github.com/xlg0o/SOCKETmessanger/ 
  g++ -o SOCKETmessanger main.cpp

# Usage:
Since the server and client logic are combined into a single executable, you can test it on a single machine using the local loopback address (`127.0.0.1`).
1. **Start the Server:** Run the app in the first terminal, type `s`, and press **Enter** to start listening.
2. **Start the Client:** Run the app in a second terminal, type `c`, and press **Enter** to connect automatically.
3. **Chat:** Type your message in the client window and hit **Enter** to send. Type `exit` to c
