package dataloader;

import java.net.*;
import java.io.*;
import java.util.*;
import dbutils.*;
import java.sql.*;


public class ObservationThread extends Thread {
    private Hashtable hash = null;
    private Connection conn = null;
    private String inputLine = null;
    private Logger log = null;

    public ObservationThread (String _line, Hashtable _hash, Connection _conn) {
        super("ObservationThread");
        inputLine = _line;
        hash = _hash;
        conn = _conn;
        log = Logger.getInstance();	
    }

    public void run() 
    {

        Observation obs = new Observation(inputLine,hash,conn);
        obs.process();

        // System.out.println(inputLine);

    }
}
