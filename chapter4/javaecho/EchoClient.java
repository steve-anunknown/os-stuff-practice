import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

public class EchoClient {
    public static void main(String[] args)
    {
        try
        {
            Socket sock = new Socket("127.0.0.1", 5575);
            InputStream sockIn = sock.getInputStream();
            OutputStream sockOut = sock.getOutputStream();

            int userByte = 0;
            int sockByte = 0;
            while (true)
            {
                if ((userByte = System.in.read()) != -1)
                {
                    System.out.println("Client: " + (char) userByte);
                    sockOut.write(userByte);
                }
                if ((sockByte = sockIn.read()) != -1 && (sockByte == userByte))
                {
                    System.out.println("Successful echo");
                }
            }
        }
        catch (IOException ioe)
        {
            System.err.println(ioe);
        }
    }
}
