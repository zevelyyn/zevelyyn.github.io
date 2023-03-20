package graph;

import java.util.*;

// WeightedGraph represents a directed graph. It is a collection of vertices connected by weighted, directed edges.
// This graph will never store duplicate vertices and the weights will always be non-negative integers.

// The WeightedGraph will be capable of performing three algorithms:
// Depth-First-Search, Breadth-First-Search, and Djikatra's
 
// The Weighted Graph will also maintain a collection of "GraphAlgorithmObservers", which will be notified during the
// performance of the graph algorithms to update the observers on how the algorithms are progressing.
 
public class WeightedGraph<V> {

    // a graph implemented using a Map - key: V (vertex) & value: a Map with a key: neighboring vertex, and
    // value: the weight between the vertex and its neighbor as an Integer
    private Map<V, Map<V, Integer>> weightedGraph;
    
    // Collection of observers that will be notified during BFS, DFS, and Djisktra's 
    private Collection<GraphAlgorithmObserver<V>> observerList;
    
    
    // Initialize the data structures to "empty"
    public WeightedGraph() {
	weightedGraph = new HashMap<>();
	observerList = new HashSet<>();
    }
    
    // Add a GraphAlgorithmObserver to observerList
    public void addObserver(GraphAlgorithmObserver<V> observer) {
	observerList.add(observer);
    }
    
    // Add a vertex to the graph.  If the vertex is already in the graph, throw an IllegalArgumentException.
    public void addVertex(V vertex) {
	if (weightedGraph.containsKey(vertex)) {
	    throw new IllegalArgumentException("Vertex already present.");
	}
	weightedGraph.put(vertex, new HashMap<>());		
    }
    
    // Searches for a given vertex & returns a boolean
    public boolean containsVertex(V vertex) {
	if (weightedGraph.containsKey(vertex)) {
	    return true;
	}
	return false;
    }
    
    // Add an edge from one vertex of the graph to another, with the specified weight. This method throws an 
    // IllegalArgumentExeption if the "from" or "to" verties are not already in the graph or if the weight 
    // is less than 0.
    public void addEdge(V from, V to, Integer weight) {
	if (containsVertex(from) == false || containsVertex(to) == false || weight < 0) {
	    throw new IllegalArgumentException("Invalid arguements.");
	}
	
	if (from.equals(to) == false) {
	    weightedGraph.get(from).put(to, weight);
	}
    }
    
    // Returns weight of the edge connecting one vertex to another or returns null if the edge does not exist.
    // Throws an IllegalArgumentException if either of the vertices specified are not in the graph.
    public Integer getWeight(V from, V to) {
	if (containsVertex(from) == false || containsVertex(to) == false) {
	    throw new IllegalArgumentException("Invalid arguments.");
	}
	return weightedGraph.get(from).get(to);
    }
    
    // This method will perform a Breadth-First-Search on the graph. The search will begin at the "start" vertex and 
    // conclude once the "end" vertex has been reached. 
    // This method will go through the collection of Observers before and after the search.
    public void DoBFS(V start, V end) {
	for (GraphAlgorithmObserver<V> observer : observerList) {
	    observer.notifyBFSHasBegun();
	}
	
	BreadthFirst(start, end);
	
	for (GraphAlgorithmObserver<V> observer : observerList) {
	    observer.notifySearchIsOver();
	}
    }
    
    // A private method that conducts the Breadth First Search using a visitedSet to track the vertices that are
    // finished, and a discovered queue (implemented with a linked list) to determine which index will be visited next.
    // Once an element is added to the visitedSet, this method goes through the list of Observers and notifies the
    // visit of the element.
    private void BreadthFirst(V start, V end) {
	Set<V> visitedSet = new HashSet<>();
	LinkedList<V> discoveredQueue = new LinkedList<>();
	
	discoveredQueue.add(start);
	V element = start;
	
	while (element.equals(end) == false) { // while the end is not found, keep traversing
	    element = discoveredQueue.removeFirst();
	    
	    if (!visitedSet.contains(element)) {
		visitedSet.add(element);
		for (GraphAlgorithmObserver<V> observer : observerList) {
		    observer.notifyVisit(element);
		}
		
		// go through the neighbors and add it to the end of the queue if it is not already visited
		for (V neighbor : weightedGraph.get(element).keySet()) {
		    if (!visitedSet.contains(neighbor)) {
			discoveredQueue.addLast(neighbor);
		    }
		}
	    }
	}
    }
    
    // This method will perform a Depth-First-Search on the graph. The search will begin at the "start" vertex and 
    // conclude once the "end" vertex has been reached.
    // This method will go through the collection of Observers before and after the search.
    public void DoDFS(V start, V end) {
	for (GraphAlgorithmObserver<V> observer : observerList) {
	    observer.notifyDFSHasBegun();
	}
	
	DepthFirst(start, end);
	
	for (GraphAlgorithmObserver<V> observer : observerList) {
	    observer.notifySearchIsOver();
	}
    }
    
    // A private method that conducts the Depth First Search using a visitedSet to track the vertices that are
    // finished, and a discovered stack to determine which index will be visited next.
    // Once an element is added to the visitedSet, this method goes through the list of Observers and notifies the
    // visit of the element.
    private void DepthFirst(V start, V end) {
	Set<V> visitedSet = new HashSet<>();
	Stack<V> discoveredStack = new Stack<>();
	
	V element = discoveredStack.push(start); // push = add
	
	while (element.equals(end) == false) { // while the end is not found, keep traversing
	    element = discoveredStack.pop(); // removes and returns first element in stack
	    
	    if (!visitedSet.contains(element)) {
		visitedSet.add(element);
		for (GraphAlgorithmObserver<V> observer : observerList) {
		    observer.notifyVisit(element);
		}
		
		// go through the neighbors and add to discoveredStack if they are not already visited
		for (V neighbor : weightedGraph.get(element).keySet()) { 
		    if (!visitedSet.contains(neighbor)) {
			discoveredStack.push(neighbor);
		    }
		}
	    }
	}
    }
    
    // Perform Dijkstra's algorithm, beginning at the "start" vertex. The algorithm will continue until EVERY vertex in 
    // the graph has been added to the finished set.
    // After the graph has been fully traversed, the algorithm will calculate the "least cost" path of vertices from 
    // the starting vertex to the ending vertex.
    // This method will go through the collection of Observers before the search and after the lowest cost path is
    // found.
    public void DoDijsktra(V start, V end) {
	for (GraphAlgorithmObserver<V> observer : observerList) {
	    observer.notifyDijkstraHasBegun();;
	}
	
	Map<V, V> table = Dijsktra(start, end);
	List<V> path = lowestCostPath(table, start, end);
	
	for (GraphAlgorithmObserver<V> observer : observerList) {
	    observer.notifyDijkstraIsOver(path);
	}
    }
    
    
    // A private method that conducts Dijsktra's using a Map (table) with the vertex & cost to get there from the 
    // start, a map (predecessor) to keep track of the lowest cost predecessor, and a set to keep track of finished
    // vertices. This method returns the map of vertices and their predecessor.
    // Each time a vertex is added to the finishedSet, this  method goes through the collection of Observers
    private Map<V, V> Dijsktra(V start, V end) {	
	Map<V, Integer> table = new HashMap<>();
	Map<V, V> predecessor = new HashMap<>(); // <key, predecessor> - keeps track of the lowest cost pred
	Set<V> finishedSet = new HashSet<>();
	
	
	table = initializeTable(table);
	table.put(start, 0); // zero cost to go from start to start
	predecessor.put(start, start);
	
	while (table.isEmpty() == false) { // traverse until table is empty
	    V curr = getSmallest(table); // get the smallest cost vertex
	    
	    for (V neighbor : weightedGraph.get(curr).keySet()) {
		if (finishedSet.contains(neighbor) == false) {
		    // new cost from current to neighbor
		    int newCost = table.get(curr) + weightedGraph.get(curr).get(neighbor);
		    
		    if (newCost < table.get(neighbor)) { // if faster path is found
			table.put(neighbor, newCost); // update cost
			predecessor.put(neighbor, curr); // if the cost it updated, update the predecessor
		    }
		}
	    }			
	    finishedSet.add(curr);
	    for (GraphAlgorithmObserver<V> observer : observerList) {
		observer.notifyDijkstraVertexFinished(curr, table.get(curr));
	    }
	    
	    table.remove(curr);
	}
	return predecessor; // returns a map of vertices and their predecessor to find the shortest path
    }
    
    // a private method that takes a map with vertices and the cost to get there & returns the smallest cost vertex
    private V getSmallest(Map<V, Integer> map) {
	int lowest = Integer.MAX_VALUE;
	V smallest = null;
	for (V key : map.keySet()) {
	    if (map.get(key) <= lowest) {
		lowest = map.get(key);
		smallest = key;
	    }
	}
	return smallest;
    }
    
    // Returns a map with all the vertices and infinite cost
    private Map<V, Integer> initializeTable(Map<V, Integer> map) {
	for (V key : weightedGraph.keySet()) {
	    map.put(key, Integer.MAX_VALUE);
	}
	return map;
    }
    
    // Returns a list of the lowest cost path from start to end. This method takes a map with <key, predecessor>
    private List<V> lowestCostPath(Map<V, V> map, V start, V end) {
	LinkedList<V> path = new LinkedList<>();
	path.add(end);
	
	V key = end;
	while(path.contains(start) == false) {
	    key = map.get(key);
	    path.addFirst(key);			
	}
	return path;
    }
    
}
