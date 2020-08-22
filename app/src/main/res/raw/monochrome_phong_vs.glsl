uniform mat4 u_MVPMatrix;		// A constant representing the combined model/view/projection matrix.      		             		
		  			
attribute vec3 a_Position;		// Per-vertex mPosition information we will pass in.
		       		          		    		
		  
// The entry point for our vertex shader.  
void main()                                                 	
{                                                         	           		      
	// gl_Position is a special variable used to store the final mPosition.
	// Multiply the vertex by the matrix to get the final point in normalized screen coordinates.
	vec4 pos = vec4(a_Position, 1.0);
	gl_Position = u_MVPMatrix * pos;                       		  
}   

                                                                      