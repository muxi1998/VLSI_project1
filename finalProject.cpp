# include <iostream>
# include <stdio.h>
# include <string>
# include <vector>
# include <fstream>
# include <stdlib.h>
# include <algorithm>
# include <queue>
# include <iomanip>

using namespace std ;

struct Edge {
	string name ;
	int weight ;
	string from ;
	string to ;
	
	Edge() {
		name = "" ;
		weight = 0 ;
		from = "" ;
		to = "" ;
	}  // constructor
};

struct Vertex {
	string name ;
	string type ;
    string color ;
	int d ; // distance to source
	string parent ;
	int finishTime ; 
	vector<Edge> edges ; // only record the out edges
	vector<Vertex> neighbor ;
	
	Vertex() {
	  name = "" ;
	  type = "" ;
      color = "white" ;
      d = 0 ;
      parent = "" ;
      finishTime = 0 ;
	} // constructor()
};

bool mycompare( Edge a, Edge b ) {
  return a.weight > b.weight ;
} // mycompare()

bool mycompare2( Vertex a, Vertex b ) {
  return a.d < b.d ; 
} // mycompare2()

struct G {
	vector<Vertex> V ;
	vector<Edge> E ;
};

class Graph {
  private:
  	G g ;
  	G g_dijkstra ;
  	vector<Vertex> dijkstraResult ;
  	int time ;
  	
  	void DFS_visit( string nextVectexName ) {
  	  int indexOfThisVertex = findVertexIndex( nextVectexName ) ;
  	  time += 1 ;
  	  g.V[ indexOfThisVertex ].d = time ;
  	  g.V[ indexOfThisVertex ].color = "gray" ;
  	  for ( int i = 0 ; i < g.V[ indexOfThisVertex ].neighbor.size() ; i ++ ) { // explore edge (u, v)
  	    string neighborName = g.V[ indexOfThisVertex ].neighbor[ i ].name ;
  	    if ( g.V[ findVertexIndex( neighborName ) ].color == "white" ) {
  	      g.V[ findVertexIndex( neighborName ) ].parent = g.V[ indexOfThisVertex ].name ;
  	      DFS_visit( g.V[ findVertexIndex( neighborName ) ].name ) ;
	    } // if()
	  } // for()
	  g.V[ indexOfThisVertex ].color = "black" ;
	  time += 1 ;
	  g.V[ indexOfThisVertex ].finishTime = time ;
	} // DFS_visit()
		
  public:
  
    Graph() {
  	  time = 0 ;
    } // constructor
    
    void init( vector<Vertex> v, vector<Edge> e ) {
      g.V.assign( v.begin(), v.end() ) ;
      g.E.assign( e.begin(), e.end() ) ;
	} // init()
	
	void clear() {
	  g.E.clear() ;
	  g.V.clear() ;
	  g_dijkstra.E.clear() ;
	  g_dijkstra.V.clear() ;
	  dijkstraResult.clear() ;
	} // clea()
    
    void showVertex( vector<Vertex> vList ) {
      for ( int i = 0 ; i < vList.size() ; i ++ ) {
        cout << "Name: " << vList[ i ].name << ", Neighbors: " << vList[ i ].neighbor.size() << ", Distance: " << vList[ i ].d << ", FinishTime: " << vList[ i ].finishTime << ", Parent: " << vList[ i ].parent << endl ; 
	  } // for()
	} // showVertex()
	
	void showEdge( vector<Edge> eList ) {
		for ( int i = 0 ; i < eList.size() ; i ++ ) {		
			cout << "Edge: " << eList[ i ].name << ", Weight:" << eList[ i ].weight << ",  From: " << eList[ i ].from << ", To: " << eList[ i ].to << endl ; 	
		} // for()
	} // showEdge()
	
	vector<Vertex> getV() {
	  return g.V ;
	} // getV()
	
	vector<Edge> getE() {
	  return g.E ;
	} // getE()
	
	Edge findEdge( string name ) {
      Edge e ;
      for ( int i = 0 ; i < g.E.size() ; i ++ ) {
        if ( name == g.E[ i ].name ) {
          e = g.E[ i ] ;
          return e ;
		} // if()
	  } // for()
	  
	  return e ;
	} // findEdge()
	
	int findVertexIndex( string name ) {
	  int target ;
	  for ( int i = 0 ; i < g.V.size() ; i ++ ) {
        if ( name == g.V[ i ].name ) {
        	target = i ;
        	return target ;
		} // if()
	  } // for()
	  return target ;
	} // findVertex()
    
    void buildAdjList() {
      for ( int i = 0 ; i < g.V.size() ; i ++ ) {
      	sort( g.V[ i ].edges.begin(), g.V[ i ].edges.end(), mycompare ) ;

        for ( int j = 0 ; j < g.V[ i ].edges.size() ; j ++ ) {
          Edge e = findEdge( g.V[ i ].edges[ j ].name ) ;
          Vertex v = g.V[ findVertexIndex( e.to ) ] ;
          g.V[ i ].neighbor.push_back( v ) ;
		} // for()
	  } // for() 
	  // copy the list to g_dijkstra
	  g_dijkstra.E.assign( g.E.begin(), g.E.end() ) ;
	  g_dijkstra.V.assign( g.V.begin(), g.V.end() ) ;
	} // buildAdjList()
	
	void DFS() {
      for ( int i = 0 ; i < g.V.size() ; i ++ ) {
      	if ( g.V[ i ].color == "white" ) {
      		DFS_visit( g.V[ i ].name ) ;
		} // if() 
	  } // for()
	} // DFS()
	
	void printDFSResult() {
	  // Print the result
	  vector<Vertex> vList = g.V ;
	  sort( vList.begin(), vList.end(), mycompare2 ) ;
	  cout << "[DFS traverse order]: " << endl << endl ;
	  for ( int i = 0 ; i < vList.size() ; i ++ ) {
	    cout << vList[ i ].name ;
	    if ( i < vList.size() - 1 ) {
	      cout << " -> " ; 
		} // if()
	  } // for()
	  cout << endl << endl ;
	  vList.clear() ;
	} // printDFSResult()
	
	int getEdgeWeight( string from, string to ) {
	  int weight = 0 ;
	  for ( int i = 0 ; i < g_dijkstra.E.size() ; i ++ ) {
	  	if ( g_dijkstra.E[ i ].from == from && g_dijkstra.E[ i ].to == to ) {
	  	  weight = g_dijkstra.E[ i ].weight ;
		  return weight ;	
	    } // if()
	  } // for()
	  return weight ;
	} // getEdgeWeight()
	
	void initialSingleSource() {
	  for ( int i = 0 ; i < g_dijkstra.V.size() ; i ++ ) {
	    g_dijkstra.V[ i ].d = 99999999 ;
	    g_dijkstra.V[ i ].parent = "" ;
	  } // for()
	  
	  int sourceIndex = findVertexIndex( "S" ) ;
	  g_dijkstra.V[ sourceIndex ].d = 0 ;
	} // initialSingleSource()
	
	void updateQueue( vector<Vertex> &queue, Vertex v ) {
	  for ( int i = 0 ; i < queue.size() ; i ++ ) {
	    if ( v.name == queue[ i ].name ) {
	      queue[ i ] = v ;
	      return ;
		} // if()  
	  } // for()
	} // updateQueue()
	
	//  string uName, string vName, int weight 
	void relax( string uName, string vName, vector<Vertex> &queue ) {
		int uIndex = findVertexIndex( uName ) ;
		int vIndex = findVertexIndex( vName ) ;
		int edgeWeight = getEdgeWeight( uName, vName ) ;
		if ( g_dijkstra.V[ vIndex ].d > g_dijkstra.V[ uIndex ].d + edgeWeight ) {
			g_dijkstra.V[ vIndex ].d = g_dijkstra.V[ uIndex ].d + edgeWeight ;
			g_dijkstra.V[ vIndex ].parent = uName ;
			
			updateQueue( queue, g_dijkstra.V[ vIndex ] ) ;
		} // if()
		sort( queue.begin(), queue.end(), mycompare2 ) ;
	} // relax()
	
	struct cmp {
	  bool operator() ( Vertex left, Vertex right ) {
	    return left.d > right.d ;	
	  } // operator()
	};
	
	void printDistance( vector<Vertex> list ) {
	  for ( int i = 0 ; i < list.size() ; i ++ ) {
	  	cout << "Vertex: " << list[ i ].name << "  Distance: " << list[ i ].d << endl ;
	  } // for()
	  cout << endl ;		
	} // printDistance()
	
	void Dijkstra() {
	  initialSingleSource() ;
	  vector<Vertex> s ; // the vertexes that had visited
	  vector<Vertex> q ; // implement min priority queue
	  for ( int i = 0 ; i < g_dijkstra.V.size() ; i ++ ) {
	  	q.push_back( g_dijkstra.V[ i ] ) ;
	  } // for()
	  sort( q.begin(), q.end(), mycompare2 ) ;

	  while ( !q.empty() ) {
	  	Vertex u = q[ 0 ] ; // get the smallest one
	  	q.erase( q.begin() ) ;
	  	s.push_back( u ) ; // add to the visited list
	  	
	  	int uIndex = findVertexIndex( u.name ) ;
	  	for ( int i = 0 ; i < g_dijkstra.V[ uIndex ].neighbor.size() ; i ++ ) { // explore the neighbors
	  		relax( u.name, g_dijkstra.V[ uIndex ].neighbor[ i ].name, q ) ;
	  		// printDistance( q ) ;
		} // for()
	  } // while()
	  
	  dijkstraResult.assign( s.begin(), s.end() ) ;
	  s.clear() ;
	  q.clear() ;
	} // Dijkstra()
	
	int getSingleSourceDistance( string name ) {
	  int distance = 0 ;
	  for ( int i = 0 ; i < dijkstraResult.size() ; i ++ ) {
	    if ( name == dijkstraResult[ i ].name ) {
	      distance = dijkstraResult[ i ].d ;
	      return distance ;
		} // if()	
	  } // for()
	  return distance ;
	} // getSingleSourceDistance()
	
	void printDijkstraResult() {
	  cout << "[Distance from Source to each vertex]:" << endl << endl ;
	  for ( int i = 0 ; i < g_dijkstra.V.size() ; i ++ ) {
	  	string vertexName = g_dijkstra.V[ i ].name ;
	    cout << "Source - " << setw( 3 ) << left << vertexName << ": " << getSingleSourceDistance( vertexName ) << endl ;
	  } // for()
	} // printDijkstraResult()
};

Graph gr ;

class Tool {

  private:
    fstream file ;
    string circuitName ;
    vector<Vertex> vList ;
    vector<Edge> eList ;
    
    bool pureStr( string str ) {
    	for ( int i = 0 ; i < str.length() ; i ++ ) {
    		if ( isDel( str[ i ] )) {
    			return false ;
			} // if()
		} // for()
		return true ;
	} // pureStr()
	
	void setEdgeDest( string edgeName, string vertexName ) {
		for ( int i = 0 ; i < eList.size() ; i ++ ) {
			if ( edgeName == eList[ i ].name ) {
				eList[ i ].to = vertexName ;
				return ;
			} // if()
		} // for()
		
		Edge e ;
		e.name = edgeName ;
		e.to = vertexName ;
		eList.push_back( e ) ;
	} // setEdgeDest()
	
	void setEdgeSource( Edge edge ) {
	  int index = -1 ;
	  for ( int i = 0 ; i < eList.size() && index == -1 ; i ++) {
	  	if ( edge.name == eList[ i ].name ) {
	  	  index = i ;	
		} // if()
	  }  // for()
	  
	  if ( index == -1 ) { // haven't exist
	  	eList.push_back( edge ) ;
	  } // if()
	  else {
	  	eList[ index ].from = edge.from ;
	  	eList[ index ].weight = edge.weight ;
	  } // else()
	} // setEdgeSource()
	
	Edge getEdge( string originStr, string vertexName ) {
		string str = "" ;
		int leftParIndex = 0 ;
		int commaIndex = 0 ;
		int rightParIndex = 0 ;
		Edge edge ;
		Edge empty ;

		for ( int i = 0 ; i < originStr.length() ; i ++ ) {
			 if ( originStr[ i ] == '(' ) leftParIndex = i ;
			 else if ( originStr[ i ] == ',' ) commaIndex = i ;
			 else if ( originStr[ i ] == ')' ) rightParIndex = i ;
		} // for()
		
		if ( leftParIndex == 0 || commaIndex == 0 || rightParIndex == 0 || !( ( leftParIndex < commaIndex ) && ( commaIndex < rightParIndex ) ) ) {
            cout << "### Error: Some information is missing in the edge! ###" << endl << endl ;
			return empty ;
		} // if()
		
		for ( int i = 0 ; i < originStr.length() ; i ++ ) {
			if ( i < leftParIndex ) {
				str += originStr[ i ] ;
			} // if()
			else if ( i == leftParIndex ) {
				edge.name = str ;
				str = "" ;
			} // else if()
			else if ( i > leftParIndex && i < commaIndex ) {
				str += originStr[ i ] ;
			} // else if()
			else if ( i == commaIndex ) {
				edge.weight = atoi( str.c_str() ) ;
				str = "" ;
			} // else if()
			else if ( i > commaIndex && i < rightParIndex ) {
				str += originStr[ i ] ;
			} // else if()
		} // for()
		
		if ( str == "i" ) {
			setEdgeDest( edge.name, vertexName ) ;
			edge = empty ;
		} // if()
		else if ( str == "o" ) {
			edge.from = vertexName ;
			setEdgeSource( edge ) ; // put this edge into our edge list
			return edge ;
		} // else if()
		else {
			cout << "### Error: Some infromation is wrong in the edge! ###" << endl << endl ;
			return empty ;
		} // else()
		
		return edge ;
	} // getEdge()
	
	string Instance() {
		// expected the vertex instance's name
		string str = "" ;
		Vertex v ;
		
		str = GetToken() ;
		if ( pureStr( str ) ) {
			v.name = str ;
		} // if()
		
		str = GetToken() ;
		if ( pureStr( str ) ) {
			v.type = str ;
		} // if()
		
		// assert: expect edges
		str = GetToken() ;
<<<<<<< HEAD
        while( str != "INSTANCE" && str != "ENDCIRCUIT" && str != "" ) {
=======
        while( str != "INSTANCE" && str != "ENDCIRCUIT" ) {
>>>>>>> 47358c517164b6bada808aa2b3279c92d4ee5f5d
			Edge edge = getEdge( str, v.name ) ;
			if ( edge.name != "" ) {
				v.edges.push_back( edge ) ;
			} // if()
			
			str = GetToken() ;
		} // while()
		
		vList.push_back( v ) ;
		
		return str ;
		
	} // Instance()
    
    bool Circuit() {
    	// first token should be "CIRCUIT
    	string str = "" ;
    	str = GetToken() ;
    	if ( str == "CIRCUIT" ) {
    		str = GetToken() ; // the next token after "CIRCUIT" must be the name
<<<<<<< HEAD
    		if ( str == "INSTANCE" ) {
    			cout << "### Error: Missing circuit name! ###" << endl << endl ;
    			return false ;
		    } // if()
	     	else {
			    if ( pureStr( str ) ) {
				    circuitName = str ;
			    } // if()
		    } // else()
		
		    str = GetToken() ; // the next token after the circuit name must be "INSTANCE"
		    do {
		        if ( str != "INSTANCE" ) {
		            cout << "### Error: Missing keyword <INSTANCE>! """ << endl << endl ;
			    } // if()
		        str = Instance() ;
		    } while( str == "INSTANCE" ) ;
    	    if ( str == "ENDCIRCUIT" ) {
    	    	return true ;
		    } // if()
		    else {
		        cout << "### Error: Missing keyword <ENDCIRCUIT>! ###" << endl << endl ;
		        return false ;
			} // else()
    		
	    } // if()
	    else {
		    cout << "### Error: Missing token <CIRCUIT> in the input file! ###" << endl << endl ;
		    return false ;
	    } // else()
=======
    		if ( str == "INSTANCE") {
    			cout << "### Error: Missing circuit name! ###" << endl << endl ;
    			return false ;
		} // if()
		else {
			if ( pureStr( str ) ) {
				circuitName = str ;
			} // if()
		} // else()
			
		str = GetToken() ; // the next token after the circuit name must be "INSTANCE"
		while ( str == "INSTANCE" ) {
			str = Instance() ;
		} // while()
    		
    		if ( str == "ENDCIRCUIT" ) {
    			return true ;
		} // if()
    		
	} // if()
	else {
		cout << "### Error: Missing token <CIRCUIT> in the input file! ###" << endl << endl ;
		return false ;
	} // else()
>>>>>>> 47358c517164b6bada808aa2b3279c92d4ee5f5d
    } // Circuit()
    
    bool checkEdges() {
      bool noProblem = true ; 
      for ( int i = 0 ; i < eList.size() ; i ++ ) {
        if ( eList[ i ].name == "" ) {
          cout << "### Error: Edge missing name! ###" << endl << endl ;
          noProblem = false ;
		} // if()
		if ( eList[ i ].from == "" ) {
		  cout << "### Error: Edge missing source! ###" << endl << endl ;
		  noProblem = false ;
		} // if()
		if ( eList[ i ].to == "" ) {
		  cout << "### Error: Edge missing destination! ###" << endl << endl ;
		  noProblem = false ;
		} // if()
	  } // for()
	  return noProblem ;
	} // checkEdges()

  public:
  	
  	void clear() {
	  vList.clear() ;
	  eList.clear() ; 
    } // initialGraph()
    
    bool isSpace( char ch ) {
      if ( ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' ) {
        return true ;
      } // if()

      return false ;
    } // isSpace()

    bool isDel( char ch ) {
      if ( ch == '(' || ch == ')' || ch == ',' ) {
        return true ;
      } // if()

      return false ;
    } // isDel()
    
    char skipSpace() {
      char ch = '\0' ;
      ch = file.peek() ;
      while ( isSpace( ch ) ) {
        ch = file.get() ; // get the space char
        ch = file.peek() ; // peek the next char after the space
      } // while()
      return ch ; // return the non-space char
    } // skipSpace()  	

    void openFile() {
      string inputName = "input.txt" ; // Default file name is "input.txt"
      cout << "Please enter the testing input file name (ex: input.txt): " ;
      cin >> inputName ;

      file.open( inputName.c_str(), ios :: in ) ;
      while ( !file ) {
        cout << endl << "### Error: < " << inputName << " > doesn't exist! ###" << endl << endl ;
        cout << "Please enter the testing input file name (ex: input.txt): " ;
        cin >> inputName ;

        file.open( inputName.c_str(), ios :: in ) ;
      } // if()

      cout << endl << "Successfully open < " << inputName << " > !" << endl << endl ;
    } // openFile()
    
    void closeFile() {
      file.close() ;
    } // closeFile()

    void readFile() {
      char ch = '\0' ;
      string str = "" ;
      
      if ( Circuit() && checkEdges() ) {
      	gr.init( vList, eList ) ;
      	gr.buildAdjList() ;
      	clear() ;
      	gr.DFS() ;
      	gr.Dijkstra() ;
<<<<<<< HEAD
      	
      	cout << "*************  Result - start *************" << endl << endl ;
   	    gr.printDFSResult() ;
        cout << "-------------------------------------------" << endl << endl ;
        gr.printDijkstraResult() ;
        cout << endl << "**************  Result - end  **************" << endl << endl ;
        gr.clear() ;  	
        clear() ;
	  } // if()
	  else {
	  	cout << "### Error: Something is wrong in the file! ###" << endl << endl ;
	  	gr.clear() ;
	  	clear() ;
=======
	  } // if()
	  else {
	  	cout << "### Error: Something is wrong in the file! ###" << endl << endl ;
>>>>>>> 47358c517164b6bada808aa2b3279c92d4ee5f5d
	  	closeFile() ;
	  } // else()
      
    } // readFile()

    string GetToken() {
      char ch = '\0' ;
      string str = "" ;
      bool finish = false ;

      ch = skipSpace() ;
      
<<<<<<< HEAD
      while ( !finish && ch != EOF ) {
=======
      while ( !finish ) {
>>>>>>> 47358c517164b6bada808aa2b3279c92d4ee5f5d
        if ( !isSpace( ch ) ) {
          ch = file.get() ;
          str += ch ;
        } // if() 
        else { // encounter a space
          finish = true ;
        } // else()

        ch = file.peek() ;
      } // while()

      return str ;
    } // GetToken()
    
    void showVertice( vector<Vertex> vList ) {
    	for ( int i = 0 ; i < vList.size() ; i ++ ) {
    		cout << "Vertex: " << vList[ i ].name << ", Edges: " ;
    		for ( int j = 0 ; j < vList[ i ].edges.size() ; j ++ ) {
    			cout << vList[ i ].edges[ j ].name << " " ;
			} // for()
			cout << ", Neighbor: " ;
			for ( int j = 0 ; j < vList[ i ].neighbor.size() ; j ++ ) {
			  cout <<  vList[ i ].neighbor[ j ].name ;
			} // for()
			cout << ", Finish: " << vList[ i ].finishTime << ", Parent: " << vList[ i ].parent ;
			cout << endl ;
		} // for()
	} // showVertice()

} ; // Tool()

int main() {

  Tool tool ;
  string command = "" ;
  cout << "Please enter a number (0)Quit (1)Continue: " ;
  cin >> command ;
  while ( command != "0" && command != "1" ) {
  	cout << "Please enter a number (0)Quit (1)Continue: " ;
  	cin >> command ;
  } // while()
  
  while ( command == "1" ) {
    tool.openFile() ;
    tool.readFile() ;
    tool.closeFile() ;
<<<<<<< HEAD
=======
  
    cout << "*************  Result - start *************" << endl << endl ;
    gr.printDFSResult() ;
    cout << "-------------------------------------------" << endl << endl ;
    gr.printDijkstraResult() ;
    cout << endl << "**************  Result - end  **************" << endl << endl ;
    gr.clear() ;  	
>>>>>>> 47358c517164b6bada808aa2b3279c92d4ee5f5d
    
    cout << "Please enter a number (0)Quit (1)Continue: " ;
    cin >> command ;
    while ( command != "0" && command != "1" ) {
  	  cout << "Please enter a number (0)Quit (1)Continue: " ;
  	  cin >> command ;
    } // while()   
  } // while()

  cout << endl << "Quit the program!" << endl ;

  return 0 ;
  
} // main()
