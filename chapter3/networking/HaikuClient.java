import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.Socket;

public class HaikuClient {
    
    public static void main(String[] args)
    {
        try
        {
            Socket sock = new Socket("127.0.0.1", 5575);
            InputStream in = sock.getInputStream();
            BufferedReader bin = new BufferedReader(new InputStreamReader(in));
            String line;
            while ((line = bin.readLine()) != null)
                System.out.println(line);
            sock.close();
        }           
        catch (IOException ioe)
        {
            System.err.println(ioe);
        }
    }
}
