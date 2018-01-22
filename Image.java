import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.io.IOException;
import javax.imageio.ImageIO;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;

class Image {
	
	// Image colour set globals
	public static final int CS_BW = 0;
	public static final int CS_GS = 1;
	public static final int CS_RGB = 2;
	
	private int width;
	private int height;
	private int colorSet;
	private BufferedImage source;
	private Map<Integer, Map<Integer, Pixel>> pixelGrid;
	
	public Image( int colorSet, String path ) {
		
		imageFile = new File( path );
		source = ImageIO.read(file);
		width = image.getWidth();
		height = image.getHeight();
		pixelGrid = new Map<Integer, Map<Integer, Pixel>>();
		for ( i=0; i < height; i++ ) {
			pixelGrid.put( i, new HashMap<Integer, Pixel>() );
		}
		convertToPixelObjects( source );
		initializeNeighbourhood();
	}
	
	private Image( int width, int height, int colorSet, Map<Integer, Map<Integer, Pixel>> pixelGrid ) {
		this.width = width;
		this.height = height;
		this.colorSet = colorSet;
		this.pixelGrid = pixelGrid;
		initializeNeighbourhood();
	}
	
	public int[] getDimensions() {
		return new int{ height, width };
	}
	
	public int[] getLevels( int row, int col ) {
		pixelGrid.get(row).get(col).getLevels();
	}
	
	public int[][] getSuperLevels( int distance, int row, int col ) {
		pixelGrid.get(row).get(col).getSuperLevels( distance, Pixel.CENTER );
	}
	
	// This method was borrowed and modified from a solution found here:  
	// https://stackoverflow.com/questions/6524196/java-get-pixel-array-from-image
	private void convertToPixelObjects( BufferedImage image ) {

      final byte[] pixels = ((DataBufferByte) image.getRaster().getDataBuffer()).getData();
      final int width = image.getWidth();
      final int height = image.getHeight();
      final boolean hasAlphaChannel = image.getAlphaRaster() != null;

      int[][] result = new int[height][width];
      if (hasAlphaChannel) {
         final int pixelLength = 4;
         for (int pixel = 0, row = 0, col = 0; pixel < pixels.length; pixel += pixelLength) {
            int alpha = 0, red = 0, green = 0, blue = 0;
            //argb += (((int) pixels[pixel] & 0xff) << 24); // alpha
			alpha += (((int) pixels[pixel] & 0xff) << 24); // alpha
            //argb += (((int) pixels[pixel + 3] & 0xff) << 16); // red
			red += ((int) pixels[pixel + 3] & 0xff ); // red
			//argb += (((int) pixels[pixel + 2] & 0xff) << 8); // green
			green += ((int) pixels[pixel + 2] & 0xff) ; // green
			//argb += ((int) pixels[pixel + 1] & 0xff); // blue
			blue += ((int) pixels[pixel + 1] & 0xff); // blue
            
            //result[row][col] = argb;
			pixelGrid.get(row).put( col, new Pixel( colorSet, red, green, blue, alpha ) );
            col++;
            if (col == width) {
               col = 0;
               row++;
            }
         }
      } else {
         final int pixelLength = 3;
         for (int pixel = 0, row = 0, col = 0; pixel < pixels.length; pixel += pixelLength) {
            int alpha = 0, red = 0, green = 0, blue = 0;
            alpha += 255; // 255 alpha
             //argb += (((int) pixels[pixel + 3] & 0xff) << 16); // red
			red += ((int) pixels[pixel + 3] & 0xff ); // red
			//argb += (((int) pixels[pixel + 2] & 0xff) << 8); // green
			green += ((int) pixels[pixel + 2] & 0xff) ; // green
			//argb += ((int) pixels[pixel + 1] & 0xff); // blue
			blue += ((int) pixels[pixel + 1] & 0xff); // blue
            
			//result[row][col] = argb;
			pixelGrid.get(row).put( col, new Pixel( colorSet, red, green, blue, alpha ) );
            col++;
            if (col == width) {
               col = 0;
               row++;
            }
         }
      }
   }
   
	public Image clone() {
		cloneGrid = new Map<Integer, Map<Integer, Pixel>>();
		for ( i=0; i < height; i++ ) {
			cloneGrid.put( i, new HashMap<Integer, Pixel>() );
		}
		for ( int row=0; row < height; row++ ) {
			for ( int col=0; col < width; col++ ) {
				cloneGrid.get(row).put( col, pixelGrid.get(row).get(col).clone() );
			}
		}
		return new Image( width, height, colorSet, cloneGrid );
	}
   
	public void modifyLevel( int level, int value, int row, int col ) {
		pixelGrid.get(row).get(col).modifyLevel( level, value );
	}
   
	private void initializeNeighbourhood() {
	   
		for ( int row = 0; row < height; row++ ) {
			for ( int col = 0; col < width; col++ ) {
				if ( ( col - 1 ) < 0 ) {
					if ( ( row - 1 ) < 0 ) { // Top left corner
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.RIGHT, pixelGrid.get( row ).get( col + 1 ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.LOWERRIGHT, pixelGrid.get( row + 1 ).get( col + 1 ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.LOWER, pixelGrid.get( row + 1 ).get( col ) );
					} else if ( ( row + 1 ) == height ) { // Bottom left corner
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.UPPERLEFT, pixelGrid.get( row - 1 ).get( col - 1 ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.UPPER, pixelGrid.get( row - 1 ).get( col ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.LEFT, pixelGrid.get( row ).get( col - 1 ) );
					} else { // Left edge
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.UPPER, pixelGrid.get( row - 1 ).get( col ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.UPPERRIGHT, pixelGrid.get( row - 1 ).get( col + 1 ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.RIGHT, pixelGrid.get( row ).get( col + 1 ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.LOWERRIGHT, pixelGrid.get( row + 1 ).get( col + 1 ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.LOWER, pixelGrid.get( row + 1 ).get( col ) );
					}
				} else if ( ( col + 1 ) == width ) {
					if ( ( row - 1 ) < 0 ) { // Top right corner
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.LEFT, pixelGrid.get( row ).get( col - 1 ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.LOWER, pixelGrid.get( row + 1 ).get( col ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.LOWERLEFT, pixelGrid.get( row + 1 ).get( col - 1 ) );
					} else if ( ( row + 1 ) == height ) { // Bottom right corner
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.UPPERLEFT, pixelGrid.get( row - 1 ).get( col - 1 ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.UPPER, pixelGrid.get( row - 1 ).get( col ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.LEFT, pixelGrid.get( row ).get( col - 1 ) );
					} else { // Right edge
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.UPPERLEFT, pixelGrid.get( row - 1 ).get( col - 1 ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.UPPER, pixelGrid.get( row - 1 ).get( col ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.LOWER, pixelGrid.get( row + 1 ).get( col ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.LOWERLEFT, pixelGrid.get( row + 1 ).get( col - 1 ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.LEFT, pixelGrid.get( row ).get( col - 1 ) );
					}
				} else {
					if ( ( row - 1 ) < 0 ) { // Top edge
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.RIGHT, pixelGrid.get( row ).get( col + 1 ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.LOWERRIGHT, pixelGrid.get( row + 1 ).get( col + 1 ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.LOWER, pixelGrid.get( row + 1 ).get( col ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.LOWERLEFT, pixelGrid.get( row + 1 ).get( col - 1 ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.LEFT, pixelGrid.get( row ).get( col - 1 ) );
					} else if ( ( row + 1 ) == height ) { // Bottom edge
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.UPPERLEFT, pixelGrid.get( row - 1 ).get( col - 1 ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.UPPER, pixelGrid.get( row - 1 ).get( col ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.UPPERRIGHT, pixelGrid.get( row - 1 ).get( col + 1 ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.RIGHT, pixelGrid.get( row ).get( col + 1 ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.LEFT, pixelGrid.get( row ).get( col - 1 ) );
					} else { // somewhere in the middle
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.UPPERLEFT, pixelGrid.get( row - 1 ).get( col - 1 ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.UPPER, pixelGrid.get( row - 1 ).get( col ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.UPPERRIGHT, pixelGrid.get( row - 1 ).get( col + 1 ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.RIGHT, pixelGrid.get( row ).get( col + 1 ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.LOWERRIGHT, pixelGrid.get( row + 1 ).get( col + 1 ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.LOWER, pixelGrid.get( row + 1 ).get( col ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.LOWERLEFT, pixelGrid.get( row + 1 ).get( col - 1 ) );
						pixelGrid.get( row ).get( col ).setNeighbour( Pixel.LEFT, pixelGrid.get( row ).get( col - 1 ) );
					}
				}
			}
		}
	}
}