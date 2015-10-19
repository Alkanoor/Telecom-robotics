
public class ActionnerOnImageAndSerial implements Actionner
{
	private boolean state;
	private boolean middle;
	private boolean[] prev;
	private ImagePanel[] images;
	private PortCommunication comm;
	
	public ActionnerOnImageAndSerial(ImagePanel[] images, PortCommunication comm)
	{
		this.comm = comm;
		this.images = images;
		prev = new boolean[4];
		for(int i=0;i<4;i++)
			prev[i] = true;
		middle = true;
	}
	
	public void actOn(boolean[] isPressed, int keyConcerned)
	{
		if(isPressed[keyConcerned])
			comm.send(String.valueOf(keyConcerned), false);
		update(isPressed);
	}
	
	public void update(boolean[] isPressed)
	{
		boolean oneImageOk = false;
		for(int i=0;i<4;i++)
		{
			if(prev[i]&&!isPressed[i])
				images[i].setVisible(false);
			else if(!prev[i]&&isPressed[i])
				images[i].setVisible(true);
			if(isPressed[i])
				oneImageOk = true;
			prev[i] = isPressed[i];
		}
		if(oneImageOk&&middle)
		{
			middle = false;
			images[4].setVisible(false);
		}
		else if(!oneImageOk&&!middle)
		{
			middle = true;
			images[4].setVisible(true);
		}
	}
	
	public void stop()
	{
		comm.send("5", false);
	}
	
	public void attack()
	{
		if(state)
			comm.send("6", false);
		else
			comm.send("7", false);
		state = !state;
	}
}
