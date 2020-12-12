# include <iostream>
# include <stdio.h>
# include <string>
# include <vector>
# include <fstream>
# include <stdlib.h>

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

struct VertexAndWeight {
	string vertex ;
	int weight ;
	
	VertexAndWeight() {
	  vertex = "" ;
	  weight = 0 ;
	} // constructor
};

struct Vertex {
	string name ;
	string type ;
    string color ;
	int d ; // distance to source
	string parent ;
	int finishTime ; 
	vector<string> edges ; // only record the out edges
	vector<VertexAndWeight> neighbor ;
	
	Vertex() {
	  name = "" ;
	  type = "" ;
      color = "white" ;
      d = 0 ;
      parent = "" ;
      finishTime = 0 ;
	} // constructor()
};

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
	} // setEdgeDest()
	
	string getEdge( string originStr, string vertexName ) {
		string str = "" ;
		int leftParIndex = 0 ;
		int commaIndex = 0 ;
		int rightParIndex = 0 ;
		Edge edge ;

                if ( originStr == "ENDCIRCUIT" || originStr == "INSTANCE" ) return "" ;
		
		for ( int i = 0 ; i < originStr.length() ; i ++ ) {
			 if ( originStr[ i ] == '(' ) leftParIndex = i ;
			 else if ( originStr[ i ] == ',' ) commaIndex = i ;
			 else if ( originStr[ i ] == ')' ) rightParIndex = i ;
		} // for()
	        // cout << "here" << endl ; // test
		if ( leftParIndex == 0 || commaIndex == 0 || rightParIndex == 0 || !( ( leftParIndex < commaIndex ) && ( commaIndex < rightParIndex ) ) ) {
			cout << "RIGHT HERE: " << originStr << endl ; //test
                        cout << "### Error: Some information is missing in the edge! ###" << endl << endl ;
			return "" ;
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
			str = "" ;
		} // if()
		else if ( str == "o" ) {
			edge.from = vertexName ;
			eList.push_back( edge ) ; // put this new edge into our edge list
			str = edge.name ;
		} // else if()
		else {
			cout << "### Error: Some infromation is wrong in the edge! ###" << endl << endl ;
			return "" ;
		} // else()
		
		return str ;
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
                while( str != "INSTANCE" && str != "ENDCIRCUIT" ) {
			str = getEdge( str, v.name ) ;
			if ( str != "" ) {
				v.edges.push_back( str ) ;
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
    			cout << "Work done!!!" << endl << endl ;
    			return true ;
		} // if()
    		
	} // if()
	else {
		cout << "### Error: Missing token <CIRCUIT> in the input file! ###" << endl << endl ;
		return false ;
	} // else()
    } // Circuit()
    
    Edge findEdge( string name ) {
      Edge e ;
      for ( int i = 0 ; i < eList.size() ; i ++ ) {
        if ( name == eList[ i ].name ) {
          e = eList[ i ] ;
          return e ;
		} // if()
	  } // for()
	  
	  return e ;
	} // findEdge()
    
    void buildAdjList() {
      for ( int i = 0 ; i < vList.size() ; i ++ ) {
        for ( int j = 0 ; j < vList[ i ].edges.size() ; j ++ ) {
          VertexAndWeight unit ;
          Edge e = findEdge( vList[ i ].edges[ j ] ) ;
          unit.vertex = e.to ;
          unit.weight = e.weight ;
          vList[ i ].neighbor.push_back( unit ) ;
		} // for()
	  } // for() 
	} // buildAdjList()

  public:
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
        cout << "### Error: < " << inputName << " > doesn't exist! ###" << endl << endl ;
        cout << "Please enter the testing input file name (ex: input.txt): " ;
        cin >> inputName ;

        file.open( inputName.c_str(), ios :: in ) ;
      } // if()

      cout << "Successfully open < " << inputName << " > !" << endl << endl ;
    } // openFile()
    
    void closeFile() {
      file.close() ;
      cout << "Close file!" << endl << endl ;
    } // closeFile()

    void readFile() {
      char ch = '\0' ;
      string str = "" ;
      
      if ( Circuit() ) {
      	buildAdjList() ;
      	cout << "YES!!!" << endl << endl ;
	  } // if()
      
    } // readFile()

    string GetToken() {
      char ch = '\0' ;
      string str = "" ;
      bool finish = false ;

      ch = skipSpace() ;
      
      while ( !finish ) {
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
    
    void showVertice() {
    	for ( int i = 0 ; i < vList.size() ; i ++ ) {
    		cout << "Vertex: " << vList[ i ].name << ", Edges: " ;
    		for ( int j = 0 ; j < vList[ i ].edges.size() ; j ++ ) {
    			cout << vList[ i ].edges[ j ] << " " ;
			} // for()
			cout << ", Neighbor: " ;
			for ( int j = 0 ; j < vList[ i ].neighbor.size() ; j ++ ) {
			  cout <<  vList[ i ].neighbor[ j ].vertex << "(" << vList[ i ].neighbor[ j ].weight << ") " ;
			} // for()
			cout << endl ;
		} // for()
	} // showVertice()
	
	void showEdge() {
		for ( int i = 0 ; i < eList.size() ; i ++ ) {		
			cout << "Edge: " << eList[ i ].name << ", Weight:" << eList[ i ].weight << ",  From: " << eList[ i ].from << ", To: " << eList[ i ].to << endl ; 	
		} // for()
	} // showEdge()

} ; // Tool()

int main() {

  Tool tool ;

  tool.openFile() ;
  tool.readFile() ;
  tool.showVertice() ;
  cout << endl ;
  tool.showEdge() ;
  tool.closeFile() ;

  return 0 ;
  
} // main()
