import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Arrays;

class Pixel {
  
  public static final int WHITE = 0;
  public static final int RED = 0;
  public static final int GREEN = 1;
  public static final int BLUE = 2;
  public static final int ALPHA = 3;
  // neighbour key
  public static final int UPPERLEFT = 0;
  public static final int UPPER = 1;
  public static final int UPPERRIGHT = 2;
  public static final int RIGHT = 3;
  public static final int LOWERRIGHT = 4;
  public static final int LOWER = 5;
  public static final int LOWERLEFT = 6;
  public static final int LEFT = 7;
  // this last 'direction' is used in the collection of superlevel (superurn) values
  public static final int CENTER = 8;
  
  private int colorSet;
  private int[] colorLevels;
  private List<Pixel> neighbours; // Array list containing no more than 8 entries of immediate neighbours.
  
  public Pixel( int colorSet, int redWhite, int green, int blue ) {
    this( colorSet, redWhite, green, blue, 255 );
  }
  
  public Pixel( int colorSet, int redWhite, int green, int blue, int alpha ) {
    this.colorSet = colorSet;
    switch ( colorSet ) {
      case Image.CS_BW:
      case Image.CS_GS:
        colorLevels = new int[1];
        colorLevels[ WHITE ] = redWhite;
        break;
      case Image.CS_RGB:
        colorLevels = new int[4];
        colorLevels[ RED ] = redWhite;
        colorLevels[ GREEN ] = green;
        colorLevels[ BLUE ] = blue;
        colorLevels[ ALPHA ] = alpha;
    }
    neighbours = new ArrayList<Pixel>();
    for ( int i=0; i < 8; i++ ) {
      neighbours.add( null );
    }
  }
  
  private Pixel( int colorSet, int[] colorLevels ) {
    this.colorSet = colorSet;
    this.colorLevels = colorLevels;
    neighbours = new ArrayList<Pixel>();
    for ( int i=0; i < 8; i++ ) {
      neighbours.add( null );
    }
  }
  
  public Pixel clone() {
    return new Pixel( colorSet, getLevels() );
  }
  
  public int[] getLevels() {
    return Arrays.copyOf( colorLevels, colorLevels.length );
  }

  public int[][] getSuperLevels( int distance, int direction ) {
    return getSuperLevels( distance, direction, 2 ); // if no norm is specified, default to 2 norm
  }
  
  public int[][] getSuperLevels( int distance, int direction, int norm ) {
    
    Pixel pixel = null;
    Iterator<Pixel> it = neighbours.iterator();
    int[][] superLevels = new int[2][1];
    
    if ( norm == 2 ) {
      if ( distance == 0 ) { // this is an 'edge' pixel in our distance
        if ( colorSet > Image.CS_BW ) {
          superLevels = new int[2][4];
          int [] level = this.getLevels();
          superLevels[0][RED] = level[RED];
          superLevels[0][GREEN] = level[GREEN];
          superLevels[0][BLUE] = level[BLUE];
          superLevels[0][ALPHA] = level[ALPHA];
          superLevels[1][0] = 1; // How many pixels were queried
        } else {
          pixel = it.next();
          int[] level = this.getLevels();
          superLevels[0][0] = level[0];
          superLevels[1][0] = 1; // How many pixels were queried
        }
      } else {
        if ( direction == CENTER ) { // this is the central pixel from which we radiate outwards
          if ( colorSet > Image.CS_BW ) {
            int[][] level;
            superLevels = new int[2][4];
            while ( it.hasNext() ) {
              pixel = it.next();
              if ( pixel != null ) {
                level = pixel.getSuperLevels( ( distance - 1 ), neighbours.indexOf( pixel ) );
                superLevels[0][RED] += level[0][RED];
                superLevels[0][GREEN] += level[0][GREEN];
                superLevels[0][BLUE] += level[0][BLUE];
                superLevels[0][ALPHA] += level[0][ALPHA];
                superLevels[1][0] += level[1][0]; // accumulation of number of pixels queried
              }
            }
            // Add the center pixels values after querying surroundings
            superLevels[0][RED] += getLevels()[RED];
            superLevels[0][GREEN] += getLevels()[GREEN];
            superLevels[0][BLUE] += getLevels()[BLUE];
            superLevels[0][ALPHA] += getLevels()[ALPHA];
            superLevels[1][0] ++;
          } else {
            int[][] level;
            while ( it.hasNext() ) {
              pixel = it.next();
              level = pixel.getSuperLevels( ( distance - 1 ), neighbours.indexOf( pixel ) );
              superLevels[0][WHITE] += level[0][WHITE];
              superLevels[1][0] += level[1][0]; // accumulation of number of pixels queried
            }
            // Add the center pixels values after querying surroundings
            superLevels[0][WHITE] += getLevels()[WHITE];
            superLevels[1][0] ++;
          }
        } else {
          switch ( direction ) {
            case UPPER:
            case RIGHT:
            case LOWER:
            case LEFT:
              if ( colorSet > Image.CS_BW ) {
              superLevels = new int[2][4];
              pixel = neighbours.get( direction );
              if ( pixel != null ) {
                int[][] level = pixel.getSuperLevels( ( distance - 1 ), neighbours.indexOf( pixel ) );
                superLevels[0][RED] += level[0][RED];
                superLevels[0][GREEN] += level[0][GREEN];
                superLevels[0][BLUE] += level[0][BLUE];
                superLevels[0][ALPHA] += level[0][ALPHA];
                superLevels[1][0] += level[1][0]; // accumulation of number of pixels queried
              }
            } else {
              pixel = neighbours.get( direction );
              int[][] level = pixel.getSuperLevels( ( distance - 1 ), neighbours.indexOf( pixel ) );
              superLevels[0][WHITE] += level[0][WHITE];
              superLevels[1][0] += level[1][0]; // accumulation of number of pixels queried
            }
            break;
            case UPPERLEFT:
            case UPPERRIGHT:
            case LOWERRIGHT:
            case LOWERLEFT:
              if ( colorSet > Image.CS_BW ) {
              int[][] level;
              for ( int i=-1; i < 2; i++ ) {   // This construct along with mod 8 arithmetic gets the three pixels
                superLevels = new int[2][4]; // adjacent to any corner in a general way
                pixel = neighbours.get( ( direction + i ) % 8 );
                if ( pixel != null ) {
                  level = pixel.getSuperLevels( ( distance - 1 ), neighbours.indexOf( pixel ) );
                  superLevels[0][RED] += level[0][RED];
                  superLevels[0][GREEN] += level[0][GREEN];
                  superLevels[0][BLUE] += level[0][BLUE];
                  superLevels[0][ALPHA] += level[0][ALPHA];
                  superLevels[1][0] += level[1][0]; // accumulation of number of pixels queried
                }
              }
            } else {
              int[][] level;
              for ( int i=-1; i < 2; i++ ) {   // This construct along with mod 8 arithmetic gets the three pixels
                pixel = neighbours.get( ( direction + i ) % 8 );
                level = pixel.getSuperLevels( ( distance - 1 ), neighbours.indexOf( pixel ) );
                superLevels[0][WHITE] += level[0][WHITE];
                superLevels[1][0] += level[1][0]; // accumulation of number of pixels queried
              }
            }
            break;
          }
        }
      }
    }
    /*
     if ( colorSet > Image.CS_BW ) {
     superLevels = new int[2][4];
     while ( it.hasNext() ) {
     pixel = it.next();
     level = pixel.getLevels;
     superLevels[0][RED] += level[RED];
     superLevels[0][GREEN] += level[GREEN];
     superLevels[0][BLUE] += level[BLUE];
     superLevels[0][ALPHA] += level[ALPHA];
     superLevels[1][0]++;
     }
     } else {
     while ( it.hasNext() ) {
     pixel = it.next();
     level = pixel.getLevels;
     superLevels[0][0] += level[0];
     superLevels[1][0]++;
     }
     }
     */
    return superLevels;
  }
  
  public void removeNeighbour( Pixel pixel ) {
    removeNeighbour( pixel, false );
  }
  
  public void removeNeighbour( Pixel pixel, boolean mutual ) {
    if ( mutual ) {
      pixel.removeNeighbour( this );
      neighbours.remove( pixel );
    } else {
      neighbours.remove( pixel );
    }
  }
  
  public void removeAllNeighbours() {
    removeAllNeighbours( false );
  }
  
  public void removeAllNeighbours( boolean mutual ) {
    Iterator<Pixel> it = neighbours.iterator();
    Pixel pixel;
    while ( it.hasNext() ) {
      pixel = it.next();
      if ( mutual ) {
        removeNeighbour( pixel, true );
      } else {
        removeNeighbour( pixel, false );
      }
    }
  }
  
  public void setNeighbour( int index, Pixel pixel ) {
    neighbours.set( index, pixel );
  }
  
  public void modifyLevel( int level, int value ) {
    colorLevels[level] += value;
  }
}
