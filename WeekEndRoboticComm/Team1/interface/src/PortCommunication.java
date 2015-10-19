import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;

import gnu.io.CommPortIdentifier;
import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;
import gnu.io.UnsupportedCommOperationException;


public class PortCommunication implements SerialPortEventListener
{
	StringBuffer data;
	String name;
	int baudrate;
	SerialPort port = null;
	CommPortIdentifier portId = null;
	
	InputStream in = null;
	OutputStream out = null;
	
	public PortCommunication(String name, int baudrate, boolean parity)
	{
		this.name = name;
		this.baudrate = baudrate;
		try
		{
			portId = CommPortIdentifier.getPortIdentifier(name);
		}
		catch(NoSuchPortException e)
		{
			e.printStackTrace();
			System.exit(0);
		}
		try
		{
			port = (SerialPort) portId.open("PortCommunication", 2000);
		}
		catch(PortInUseException e)
		{
			e.printStackTrace();
			System.exit(0);
		}
		
		try
		{
			port.setSerialPortParams(baudrate, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
			port.setFlowControlMode(SerialPort.FLOWCONTROL_NONE);
			port.addEventListener(this);
			in = port.getInputStream();
			out = port.getOutputStream();
		}
		catch(UnsupportedCommOperationException e)
		{
			e.printStackTrace();
			System.exit(0);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		
		data = new StringBuffer();
	}
	
	public boolean send(String str, boolean carriageReturn)
	{
		try
		{
			if(carriageReturn)
				out.write((str+"\r\n").getBytes("US-ASCII"));
			else
				out.write(str.getBytes("US-ASCII"));
			return true;
		}
		catch(IOException e)
		{
			e.printStackTrace();
			return false;
		}
	}
	
	public void serialEvent(SerialPortEvent evt)
	{
		if(evt.getEventType()==SerialPortEvent.DATA_AVAILABLE)
			updateBuffer();
	}
	
	public void updateBuffer()
	{
		int a = 0;
		try
		{
			while(in.available()>0)
			{
				a = in.read();
				if(a!=13)
					data.append((char)a);
			}
		}
		catch(IOException e){}
	}
	
	public ArrayList<String> read()
	{
		updateBuffer();
		ArrayList<String> ret = new ArrayList<String>();
		int currentPos = 0, nextPos = 0;
		while(currentPos<=nextPos)
		{
			nextPos = data.indexOf("\n",currentPos);
			if(nextPos<0)
				break;
			ret.add(data.substring(currentPos,nextPos));
			currentPos = nextPos;
		}
		ret.add(data.substring(currentPos));
		data.setLength(0);
		return ret;
	}
}
