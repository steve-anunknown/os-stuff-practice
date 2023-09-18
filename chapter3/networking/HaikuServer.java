import java.io.IOException;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class HaikuServer {
    public static void main(String[] args)  {
		try {
			ServerSocket sock = new ServerSocket(5575);

			// now listen for connections
			while (true) {
				Socket client = sock.accept();
				// we have a connection
				
				PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
				// write the Date to the socket
				pout.println("Omorfos Kairos\nKalo Fai Shmera\nTo Thelo Pali.");

				// close the socket and resume listening for more connections
				client.close();
			}
		}
		catch (IOException ioe) {
				System.err.println(ioe);
		}
	}
}
