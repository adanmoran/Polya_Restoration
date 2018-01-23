class PUPFilter {
  
  private Image source;
  private Image result;
  private int iterations;
  private int distance;
  private int colorSet;
  
  public PUPFilter( Image source, int colorSet, int iterations, int distance ) {
    this.source = source;
    this.colorSet = colorSet;
    this.iterations = iterations;
    this.distance = distance;
    result = source.clone();
  }
  
  public void reset() {
    this.result = source.clone();
  }
  
  public void setParams(){}
    
}