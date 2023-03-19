package maze;
/** 
 * Represents a Juncture (intersection) in
 * a Maze. The Juncture is merely a pair
 * of integers representing X and Y coordinates.
 * 
 * @author Fawzi Emad (C)2022
 *
 */
public class Juncture {
    
    private int x, y;
    
    public Juncture(int x, int y) {
	this.x = x;
	this.y = y;
    }
    
    public int getX() {
	return x;
    }
    
    public int getY() {
	return y;
    }
    
    public boolean equals(Object other) {
	if (this == other) {
	    return true;
	}
	if ( !(other instanceof Juncture)) {
	    return false;
	}
	Juncture p = (Juncture) other;
	return x == p.x && y == p.y;
    }
	
    public int hashCode() {
	return x + 10000 * y;
    }
    
    public String toString() {
	return "(" + x + ", " + y + ")";
    }
}
