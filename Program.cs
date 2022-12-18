using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace GopherServer
{
    class Program
    {
        static void Main(string[] args)
        {
            // create a listening socket
            var listener = new TcpListener(IPAddress.Any, 12345);  // listen on port 70
            listener.Start();

            // accept incoming connections
            while (true)
            {
                Console.WriteLine("Waiting for a connection...");
                var client = listener.AcceptTcpClient();
                Console.WriteLine("Connected!");

                // read the request
                var request = new byte[1024];
                int bytesRead = client.GetStream().Read(request, 0, 1024);
                string requestString = Encoding.ASCII.GetString(request, 0, bytesRead);
                Console.WriteLine("Received request: " + requestString);

                // construct the response
                string responseString = "Hello, Gopher!\r\n";
                byte[] response = Encoding.ASCII.GetBytes(responseString);

                // send the response
                client.GetStream().Write(response, 0, response.Length);

                // close the connection
                client.Close();
            }
        }
    }
}
