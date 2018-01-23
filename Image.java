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
import java.io.File;

class Image {
  
  // Image colour set globals
  public static final int CS_BW = 0;
  public static final int CS_GS = 1;
  public static final int CS_RGB = 2;
  // Mask types
  // MASK_ISOSING: Completely isolate indivdual (singular) pixels
  // MASK_ISOBLOB: Completely isolate masked pixels from surroundings but group adjacent pixels as neighbours
  // MASK_OWMOUTWARD: 'One Way Mirror' looking from the inside out, i.e. isolated pixels see non-masked pixels but
  // the reverse is not true. Masked pixels are neighbours if adjacent.
  // MASK_OWMINWARD: Opposite effect of OWMOUTWARD except masked pixels are neighbours if adjacent.
  public static final int MASK_ISOSING = 0;
  public static final int MASK_ISOBLOB = 1;
  public static final int MASK_OWMOUTWARD = 2;
  public static final int MASK_OWMINWARD = 3;
  private int width;
  private int height;
  private int colorSet;
  private BufferedImage source;
  private Map<Integer, Map<Integer, Pixel>> pixelGrid;
  
  public Image( int colorSet, String path ) {
    
    File imageFile = new File( path );
    try {
      source = ImageIO.read( imageFile );
    } catch ( IOException e ) {
      System.out.println( e );
    }
    width = source.getWidth();
    height = source.getHeight();
    pixelGrid = new HashMap<Integer, Map<Integer, Pixel>>();
    for ( int i=0; i < height; i++ ) {
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
    return new int[]{ height, width };
  }
  
  public int[] getLevels( int row, int col ) {
    return pixelGrid.get(row).get(col).getLevels();
  }
  
  public int[][] getSuperLevels( int distance, int row, int col ) {
    return pixelGrid.get(row).get(col).getSuperLevels( distance, Pixel.CENTER );
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
        alpha += ((int) pixels[pixel] & 0xff ); // alpha
        //argb += (((int) pixels[pixel + 3] & 0xff) << 16); // red
        red += ((int) pixels[pixel + 3] & 0xff ); // red
        //argb += (((int) pixels[pixel + 2] & 0xff) << 8); // green
        green += ((int) pixels[pixel + 2] & 0xff ) ; // green
        //argb += ((int) pixels[pixel + 1] & 0xff); // blue
        blue += ((int) pixels[pixel + 1] & 0xff ); // blue
        
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
    
    Map<Integer, Map<Integer, Pixel>> cloneGrid = new HashMap<Integer, Map<Integer, Pixel>>();
    for ( int i=0; i < height; i++ ) {
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
  
  public void applyMask( int[][] mask, int maskType ) {
    
    boolean result = false;
    Pixel pixel = null;
    Pixel currentPixel = null;
    if ( mask.length < height && mask[0].length < width ) {
      for ( int row = 0; row < mask.length; row++ ) {
        for ( int col = 0; col < mask[0].length; col++ ) {
          currentPixel = pixelGrid.get( row ).get( col );
          if ( mask[row][col] == 1 ) {
            if ( maskType == MASK_ISOSING ) {
              currentPixel.removeAllNeighbours( true ); // mutual 'unfriending'
            }
            if ( maskType == MASK_ISOBLOB ) {
              for ( int rOffset = -1; rOffset < 2; rOffset++ ) {
                for ( int cOffset = -1; cOffset < 2; cOffset++ ) {
                  if ( ( row - rOffset ) != 0 && ( col - cOffset ) != 0 ) { // i.e. not on the 'centered' pixel
                    pixel = pixelGrid.get( row - rOffset ).get( col - cOffset );
                    if ( mask[ row - rOffset ][ col - cOffset] == 0 && pixel != null )
                      // mutually remove all pixels not part of the mask
                      // and ask that they do the same in return.
                      currentPixel.removeNeighbour( pixel, true ); 
                  }
                }
              }
            }
            if ( maskType == MASK_OWMOUTWARD ) {
              for ( int rOffset = -1; rOffset < 2; rOffset++ ) {
                for ( int cOffset = -1; cOffset < 2; cOffset++ ) {
                  if ( ( row - rOffset ) != 0 && ( col - cOffset ) != 0 ) { // i.e. not on the 'centered' pixel
                    pixel = pixelGrid.get( row - rOffset ).get( col - cOffset );
                    if ( mask[ row - rOffset ][ col - cOffset] == 0 && pixel != null )
                      // ask all pixels not part of the mask that see this pixel as a neighbour
                      // remove this pixel from their list.
                      pixel.removeNeighbour( currentPixel, false );
                  }
                }
              }
            }
            if ( maskType == MASK_OWMINWARD ) {
              for ( int rOffset = -1; rOffset < 2; rOffset++ ) {
                for ( int cOffset = -1; cOffset < 2; cOffset++ ) {
                  if ( ( row - rOffset ) != 0 && ( col - cOffset ) != 0 ) { // i.e. not on the 'centered' pixel
                    pixel = pixelGrid.get( row - rOffset ).get( col - cOffset );
                    if ( mask[ row - rOffset ][ col - cOffset] == 0 && pixel != null )
                      // remove all pixels not part of the mask from this pixels list
                      // but allow them to see this pixel as a neighbour.
                      currentPixel.removeNeighbour( pixel, false );
                  }
                }
              }
            }
          }
        }
      }
    }
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