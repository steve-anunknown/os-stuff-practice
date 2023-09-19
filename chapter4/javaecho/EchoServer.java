import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;

class EchoTask implements Runnable
{
    private Socket client;
    public EchoTask(Socket client)
    {
        this.client = client;
    }
    public void run()
    {
        try
        {
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
        catch (IOException ioe)
        {
            System.err.println(ioe);
        }
    }
}

public class EchoServer {
    public static void main(String[] args)
    {
        try
        {
            ServerSocket sock = new ServerSocket(5575);
            while (true)
            {
                Socket client = sock.accept();
                Thread worker = new Thread(new EchoTask(client));
                worker.start();
                // i think that there is no need to join
                // since that would block the main thread
                // and the server would not be able to accept
                // any more clients.    
            }
        }
        catch (IOException ioe)
        {
            System.err.println(ioe);
        }
    }
}
