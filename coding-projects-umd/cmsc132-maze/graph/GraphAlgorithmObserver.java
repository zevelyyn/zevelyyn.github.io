package graph;

import java.util.List;

/**
 * A GraphAlgorithmObserver will register with a WeightedGraph 
 * to let it know that it is "watching".  As algorithms are 
 * carried out on the WeightedGraph (BFS, DFS, Dijkstra), the 
 * graph will notify the Observer to let it know how the 
 * algorithms are progressing.
 * 
 * @author Fawzi Emad (C) 2022
 *
 * @param <V>
 */
public interface GraphAlgorithmObserver<V> {
	
    /** Called by the graph to notify this Observer that
     * a Depth-First-Search has been initiated.
     */
    public void notifyDFSHasBegun();
    
    /** Called by the graph to notify this Observer that
     * a Breadth-First Search has been initiated.
     */
    public void notifyBFSHasBegun();
    
    /** Called by the graph to notify this Observer that
     * a vertex is being "visited" during either DFS or BFS.
     * 
     * @param vertexBeingVisited
     */
    public void notifyVisit(V vertexBeingVisited);
    
    /** Called by the graph to notify this observer that
     * the search (either DFS or BFS) is over.
     */
    public void notifySearchIsOver();
	
    /** Called by the graph to notify this observer that
     * Dijkstra's algorithm has begun. 
     */
    public void notifyDijkstraHasBegun();
    
    /** Called by the graph to notify this observer that
     * a vertex has been added to the "Finished Set"
     * during Dijkstra's algorithm.  The second parameter
     * is the "cost" (total weight) of the best path
     * leading from the starting vertex to the one referenced
     * by the first parameter.
     * 
     * @param vertexAddedToFinishedSet
     * @param costOfPath
     */
    public void notifyDijkstraVertexFinished(V vertexAddedToFinishedSet, Integer costOfPath);
    
    /** 
     * <P>Called by the graph to notify this observer that
     * Dijkstra's algorithm is over.</P>
     * 
     * @param path A list of Vertices that are connected along edges,
     * beginning with the "starting vertex" and ending with the
     * "finishing vertex".  This will be the optimal (lowest cost)
     * path from start to finish.
     */
    public void notifyDijkstraIsOver(List<V> path);
}
