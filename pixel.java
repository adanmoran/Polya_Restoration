import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;

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
	
	public Pixel( int colorSet, int redWhite, int green, int blue, int alpha = 0 ) {
		this.colorSet = colorSet;
		switch ( colorSet ) {
			case Image.CS_BW:
			case Image.CS_BW:
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
		return new Pixel( colorSet, colorLevels );
	}
	
	public int[] getLevels() {
		return Arrays.copyOf( colorLevels );
	}
	
	public int[][] getSuperLevels( int distance, int direction ) {
		
		Pixel pixel = null;
		int[] level = null;
		Iterator<Pixel> it = neighbours.iterator();
		int[][] superLevels = new int[2][1];
		
		if ( distance == 0 ) { // this is an 'edge' pixel in our distance
			if ( colorSet > Image.CS_BW ) {
				superLevels = new int[2][4];
				level = this.getLevels;
				superLevels[0][RED] = level[RED];
				superLevels[0][GREEN] = level[GREEN];
				superLevels[0][BLUE] = level[BLUE];
				superLevels[0][ALPHA] = level[ALPHA];
				superLevels[1][0] = 1; // How many pixels were queried
			} else {
				pixel = it.Next();
				level = this.getLevels;
				superLevels[0][0] = level[0];
				superLevels[1][0] = 1; // How many pixels were queried
			}
		} else {
			if ( direction == CENTER ) { // this is the central pixel from which we radiate outwards
				if ( colorSet > Image.CS_BW ) {
					superLevels = new int[2][4];
					while ( it.hasNext() ) {
						pixel = it.next();
						if ( pixel ) {
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
					while ( it.hasNext() ) {
						pixel = it.next();
						pixel.getSuperLevels( ( distance - 1 ), neighbours.indexOf( pixel ) );
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
							if ( pixel ) {
								level = pixel.getSuperLevels( ( distance - 1 ), neighbours.indexOf( pixel ) );
								superLevels[0][RED] += level[0][RED];
								superLevels[0][GREEN] += level[0][GREEN];
								superLevels[0][BLUE] += level[0][BLUE];
								superLevels[0][ALPHA] += level[0][ALPHA];
								superLevels[1][0] += level[1][0]; // accumulation of number of pixels queried
							}
						} else {
							pixel = neighbours.get( direction );
							pixel.getSuperLevels( ( distance - 1 ), neighbours.indexOf( pixel ) );
							superLevels[0][WHITE] += level[0][WHITE];
							superLevels[1][0] += level[1][0]; // accumulation of number of pixels queried
						}
						break;
					case UPPERLEFT:
					case UPPERRIGHT:
					case LOWERRIGHT:
					case LOWERLEFT:
						if ( colorSet > Image.CS_BW ) {
							for ( int i=-1; i < 2; i++ ) { 		// This construct along with mod 8 arithmetic gets the three pixels
								superLevels = new int[2][4];	// adjacent to any corner in a general way
								pixel = neighbours.get( ( direction + i ) % 8 );
								if ( pixel ) {
									level = pixel.getSuperLevels( ( distance - 1 ), neighbours.indexOf( pixel ) );
									superLevels[0][RED] += level[0][RED];
									superLevels[0][GREEN] += level[0][GREEN];
									superLevels[0][BLUE] += level[0][BLUE];
									superLevels[0][ALPHA] += level[0][ALPHA];
									superLevels[1][0] += level[1][0]; // accumulation of number of pixels queried
								}
							}
						} else {
							for ( int i=-1; i < 2; i++ ) { 		// This construct along with mod 8 arithmetic gets the three pixels
								pixel = neighbours.get( ( direction + i ) % 8 );
								pixel.getSuperLevels( ( distance - 1 ), neighbours.indexOf( pixel ) );
								superLevels[0][WHITE] += level[0][WHITE];
								superLevels[1][0] += level[1][0]; // accumulation of number of pixels queried
							}
						}
						break;
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
	
	public void setNeighbour( int index, Pixel pixel ) {
		neighbours.set( index, pixel );
	}
	
	public void modifyLevel( int level, int value ) {
		colorLevels[level] += value;
	}
}
	