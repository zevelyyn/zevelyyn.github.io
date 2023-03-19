package graph;
import graph.WeightedGraph;
import maze.Juncture;
import maze.Maze;

// The MazeGraph is an extension of WeightedGraph of junctures. The constructor converts a Maze into a graph.
public class MazeGraph extends WeightedGraph<Juncture> {

    // Takes the maze parameter to construct the MazeGraph using the vertices (Junctures) and weighted edges 
    // from the maze. For every pair of adjacent junctures which are not blocked by a wall, two edges from A to B, and 
    // another from B to A.  The weight is provided by the Maze. 
    public MazeGraph(Maze maze) {
	for (int row = 0; row < maze.getMazeHeight(); row++) {
	    for (int col = 0; col < maze.getMazeWidth(); col++) {
		Juncture j = new Juncture(col, row); // create a new juncture to add to the maze graph
		if (containsVertex(j) == false) { // avoid duplicates or replacing already added vertices
		    addVertex(j);
		}
		
		// check if there is a wall above, below, left, or right of the current vertex & make a new vertex with
		// the weight that connects the two if there is no wall. Avoids duplicates vertices
		
		if (maze.isWallAbove(j) == false && row >= 0) {
		    Juncture jAbove = new Juncture(col, row - 1);
		    if (containsVertex(jAbove) == false) {
			addVertex(jAbove);
		    }
		    addEdge(j, jAbove, maze.getWeightAbove(j));
		}
		
		if (maze.isWallBelow(j) == false && row < maze.getMazeHeight()) {
		    Juncture jBelow = new Juncture(col, row + 1);
		    if (containsVertex(jBelow) == false) {
			addVertex(jBelow);
		    }
		    addEdge(j, jBelow, maze.getWeightBelow(j));
		}
		
		if (maze.isWallToLeft(j) == false && col >= 0) {
		    Juncture jLeft = new Juncture(col - 1, row);
		    if (containsVertex(jLeft) == false) {
			addVertex(jLeft);
		    }
		    addEdge(j, jLeft, maze.getWeightToLeft(j));
		}
		
		if (maze.isWallToRight(j) == false && col < maze.getMazeWidth()) {
		    Juncture jRight = new Juncture(col + 1, row);
		    if (containsVertex(jRight) == false) {
			addVertex(jRight);
		    }
		    addEdge(j, jRight, maze.getWeightToRight(j));
		}
	    }
	}
    }
}
