import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;
import javax.swing.JPanel;

public class ImagePanel extends JPanel
{
	private static final long serialVersionUID = 1L;
	private BufferedImage image;

    public ImagePanel(String path, int originX, int originY)
    {
       try
       {                
           image = ImageIO.read(new File(path));
           Dimension size = new Dimension(image.getWidth(null), image.getHeight(null));
           setPreferredSize(size);
           setMinimumSize(size);
           setMaximumSize(size);
           setSize(size);
           setLocation(originX,originY);
           setLayout(null);
           setBackground(Color.WHITE);
       }
       catch (IOException ex)
       {
            System.out.println("Error during image loading "+path);
       }
    }

    protected void paintComponent(Graphics g)
    {
        super.paintComponent(g);
        g.drawImage(image, 0, 0, null);            
    }
}