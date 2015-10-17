import java.util.ArrayList;
import java.util.Enumeration;
import gnu.io.CommPortIdentifier;


public class PortsEnumerator
{
	 public static ArrayList<String> enumerate()
	 {
		 ArrayList<String> ret = new ArrayList<String>();
		 Enumeration ports = CommPortIdentifier.getPortIdentifiers();
		 int i = 1;
		 while (ports.hasMoreElements())
		 {
			 CommPortIdentifier port = (CommPortIdentifier) ports.nextElement();
			 System.out.println("Port n"+i++);
			 System.out.println("\tNom\t:\t"+port.getName());
			 ret.add(port.getName());
			 String type = null;
			 if(port.getPortType() == CommPortIdentifier.PORT_SERIAL)
				 type = "Serie";
			 else
				 type = "Parallele";
			 System.out.println("\tType\t:\t"+type);
			 String etat = null;
			 if(port.isCurrentlyOwned())
				 etat = "Possede par "+port.getCurrentOwner();
			 else
				 etat = "Libre";
			 System.out.println("\tEtat\t:\t"+etat+"\n");
		 }
		 return ret;
	 }
}
