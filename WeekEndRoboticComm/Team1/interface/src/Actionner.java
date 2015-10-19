
public interface Actionner
{
	public void actOn(boolean[] isPressed, int keyConcerned);
	public void update(boolean[] isPressed);
	public void stop();
	public void attack();
}
