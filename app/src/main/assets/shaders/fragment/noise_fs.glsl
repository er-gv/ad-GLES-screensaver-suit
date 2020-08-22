precision mediump float;       	// Set the default precision to medium. We don't need as high of a 
								// precision in the fragment shader.

uniform vec3 u_skyColor;      
uniform vec3 u_cloudColor;   
   
varying vec3  v_ModelPosition;		  // interpolation of spatial data in eye coords.       		
varying float v_LightIntensity;	  // interpolation of the light intensity.          	   		
   
          		
//////////////////////////////////
//
//  Noise generation code
//
int i, j, k;
vec3 A = vec3(0, 0, 0);
float u, v, w, s;
float ONE_THIRD = 0.333333333;
float ONE_SIXTH = 0.166666667;

int T[8] = int[8](0x15, 0x38, 0x32, 0x2c, 0x0d, 0x13, 0x07, 0x2a);

int fastfloor(float n){
	return n > 0.0 ?  int(n) : int(n-1.0);		 
}

int b(int N, int B) {
	while(B > 0){
		N = N /2;
		B--;
	}	
	return 2*int(N/2);
}

int b(int i, int j, int k, int B) {
	return T[(b(i, B) /4) | (b(j, B) /2) | b(k, B)];
}


   

void main()                    		
{                              
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);                                  		
}                                                                     	

