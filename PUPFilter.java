class PUPFilter {
  
  private Image source;
  private Image result;
  private int iterations;
  private int distance;
  private int colorSet;
  
  public PUPFilter( Image source, int colorSet, int iterations, int distance ) {
    this.source = source;
    this.result = source.clone();
    this.colorSet = colorSet;
    this.iterations = iterations;
    this.distance = distance;
  }
  
  private 
    
}