import java.awt.KeyEventDispatcher;
import java.awt.event.KeyEvent;
import java.util.Timer;
import java.util.TimerTask;


public class KeyRegisters implements KeyEventDispatcher
{
	private Actionner toActOn;
	private boolean[] isPressed;
	private Timer[] timers;
	private int millis;
	
	public KeyRegisters(Actionner toActOn, int millis)
	{
		isPressed = new boolean[4];
		timers = new Timer[4];
		for(int i=0;i<4;i++)
			isPressed[i] = false;
		this.millis = millis;
		this.toActOn = toActOn;
	}

	public boolean dispatchKeyEvent(KeyEvent e)
	{
		if(e.getID() == KeyEvent.KEY_PRESSED)
        {
			final int code = e.getKeyCode()-37;
			if(code>=0&&code<=3&&!isPressed[code])
			{
				isPressed[code] = true;
				timers[code] = new Timer();
	            timers[code].scheduleAtFixedRate(
	            	new TimerTask() {
	            	  public void run() {
	            		  toActOn.actOn(isPressed,code);
	            	  }
	            	}, 0L, (long)millis);
			}
        }
        else if(e.getID() == KeyEvent.KEY_RELEASED)
        {
        	final int code = e.getKeyCode()-37;
        	if(code>=0&&code<=3)
        	{
	        	isPressed[code] = false;
	        	try
	        	{timers[code].cancel();}
	        	catch(Exception ex)
	        	{}
	        	toActOn.actOn(isPressed,code);
        	}
        	else if(e.getKeyCode()==10)
				toActOn.stop();
        	else if(e.getKeyCode()==32)
        		toActOn.attack();
        }
		return false;
	}
}
