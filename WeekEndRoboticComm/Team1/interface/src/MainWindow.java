import java.awt.*;
import java.awt.Desktop.Action;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.ArrayList;

import javax.swing.AbstractAction;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JToggleButton;

public class MainWindow extends JFrame implements ActionListener
{
	private PortCommunication comm = null;
	private ActionnerOnImageAndSerial actionner = null;
	private KeyRegisters keysRegisters;
	private ImagePanel[] images;
	
	public MainWindow(String name)
	{
		super(name);
        setFocusable(true);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        
        
        ArrayList<String> ports = PortsEnumerator.enumerate();
	    if(ports.size()<1)
	    {
	    	System.out.println();
	    	System.out.println("No port detected or port in use");
	    	System.exit(0);
	    }
	    System.out.println(ports.get(0));
	    comm = new PortCommunication(ports.get(0),9600,false);

	    
	    images = new ImagePanel[5];
	    images[0] = new ImagePanel("left.png",100,300);
	    images[1] = new ImagePanel("up.png",100+123+10,300-123-10);
	    images[2] = new ImagePanel("right.png",100+123+10+123+10,300);
	    images[3] = new ImagePanel("down.png",100+123+10,300+123+10);
	    images[4] = new ImagePanel("middle.png",100+123+10,300);
	    
	    
	    
	    Container pane = getContentPane();
        pane.setLayout(new GridLayout(7,3));
        
        
        pane.add(whitePanel());
		pane.add(whitePanel());
		pane.add(whitePanel());
		
		JButton temp = new JButton("Stop");
        JPanel tempPanel = new JPanel();
        tempPanel.setBackground(Color.WHITE);
		tempPanel.setLayout(new FlowLayout());
        temp.addActionListener(this);
        tempPanel.add(temp);
		pane.add(whitePanel());
		pane.add(tempPanel);
		pane.add(whitePanel());
		
        temp = new JButton("Launch attack");
        tempPanel = new JPanel();
        tempPanel.setBackground(Color.WHITE);
		tempPanel.setLayout(new FlowLayout());
        temp.addActionListener(this);
        tempPanel.add(temp);
		pane.add(whitePanel());
		pane.add(tempPanel);
		pane.add(whitePanel());
		
		tempPanel = new JPanel();
		tempPanel.setBackground(Color.WHITE);
		tempPanel.setLayout(new FlowLayout());
		JToggleButton tempToggleButton = new JToggleButton("Toggle attack");
		tempToggleButton.addActionListener(this);
        tempPanel.add(tempToggleButton);
		pane.add(whitePanel());
		pane.add(tempPanel);
		pane.add(whitePanel());
		
		pane.add(whitePanel());
		pane.add(images[1]);
		pane.add(whitePanel());
		
		pane.add(images[0]);
		pane.add(images[4]);
		pane.add(images[2]);
		
		pane.add(whitePanel());
		pane.add(images[3]);
		pane.add(whitePanel());

        
	    boolean nulls[] = new boolean[4];
	    for(int i=0;i<4;i++)
	    	nulls[i] = false;
        actionner = new ActionnerOnImageAndSerial(images,comm);
        actionner.update(nulls);
        keysRegisters = new KeyRegisters(actionner,50);
        KeyboardFocusManager manager = KeyboardFocusManager.getCurrentKeyboardFocusManager();
        manager.addKeyEventDispatcher(keysRegisters);
        
        getContentPane().setBackground(Color.WHITE);
	    setBackground(Color.WHITE);
	}

	static JPanel whitePanel()
	{
		JPanel whitePanel = new JPanel();
        whitePanel.setBackground(Color.WHITE);
        return whitePanel;
	}
	
	public void actionPerformed(ActionEvent arg0)
	{
		try
		{
			JButton t = ((JButton)arg0.getSource());
			if(t.getText()=="Stop")
				comm.send("5",false);
			else
				comm.send("4",false);
		}
		catch(Exception e)
		{
			if(((JToggleButton)arg0.getSource()).isSelected())
				comm.send("6",false);
			else
				comm.send("7",false);
		}
	}
}
