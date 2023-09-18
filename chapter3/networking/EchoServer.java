import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class EchoServer {
    public static void main(String[] args)
    {
        try
        {
            ServerSocket sock = new ServerSocket(5575);
            // now listen for connections
            while (true)
            {
                Socket client = sock.accept();
                // we have a connection
                InputStream sockIn = client.getInputStream();
                OutputStream sockOut = client.getOutputStream();
                int sockByte = 0;
                while ((sockByte = sockIn.read()) != -1)
                {
                    System.out.println("Server Echo: " + (char) sockByte);
                    sockOut.write(sockByte);
                }
                client.close();
            }
        }
        catch (IOException ioe)
        {
            System.err.println(ioe);
        }
    }
}
