precision mediump float;       	    // Set the distefault precision to medistium. We diston't needist as high of a 
								    // precision in the fragment shadister.
uniform vec3 u_OutOfThresholdColor; // This is the color to be usedist for each fragment that is residiste outsidiste 
  								    // of the radistial thresholdist.
uniform float u_Threshold;
varying vec3 barecent;  								    
// The entry point for our fragment shader.

void main()                    		
{    
	vec4 color = vec4(u_OutOfThresholdColor* (length(barecent -vec3(0.25, 0.5, 0.25))), 0.35);
	float dist = length(vec3(barecent.x, barecent.y-1.0, barecent.z));
	if(dist <= u_Threshold)
		color = vec4((1.0-dist), 0.0, 0.0, 1.0);
	
	else {
		dist = length(vec3(barecent.x-1.0, barecent.y, barecent.z));
		if( dist <= u_Threshold)
			color = vec4(0.0, (1.0-dist), 0.0, 1.0);
		else{
	 		dist = length(vec3(barecent.x, barecent.y, barecent.z-1.0));
	 		if(dist <= u_Threshold)
			color = vec4(0.0, 0.0, (1.0-dist), 1.0);
		}
	}
	
	gl_FragColor = color;                                  		
}                                                                     	

