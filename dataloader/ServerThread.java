package dataloader;

import java.net.*;
import java.io.*;
import java.util.*;
import dbutils.*;
import java.sql.*;


public class ServerThread extends Thread {
    private Socket socket = null;
    private Hashtable hash = null;
    private Connection conn = null;
    private Server caller = null;

    public ServerThread(Socket socket, Hashtable _hash, Connection _conn, Server _caller) {
        super("ServerThread");
        this.socket = socket;
        hash = _hash;
        conn = _conn;	
        caller = _caller;
    }

    public void run() {

        try {
            BufferedReader in = new BufferedReader(
                                    new InputStreamReader(
                                    socket.getInputStream()));

            String inputLine;

            while ((inputLine = in.readLine()) != null) {
               if (inputLine.equals("KILLPROCESS")) 
               {
                 caller.setkillprocess("Kill Process Message received");
                 break;
               }
               Observation obs = new Observation(inputLine,hash,conn);
               obs.process();

              // System.out.println(inputLine);

            }
            in.close();
            socket.close();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
