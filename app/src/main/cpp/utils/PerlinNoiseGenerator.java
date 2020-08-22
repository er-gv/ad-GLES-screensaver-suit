import java.util.Random;

import android.graphics.Bitmap;


public class PerlinNoiseGenerator {

	private static Random rnd = new Random();
	public static void seed(long s){
		rnd.setSeed(s);
	}
	
	private static float[][] makeWhiteNoise(int n){
		
		float[][] result = new float[n][n];
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				result[i][j] = rnd.nextFloat();
			}
		}
		return result;
	}
	
	private static final float interpolate(float a, float b, float percent){
		return a*(1.0f - percent) +b*percent;
	}

	
	private static float[][] makeOctaveNoise(int octave, final float[][] whiteNoise, int size){
		
		float[][] smoothNoise = new float[size][size];
		int samplePeriod = 1 << octave; // calculates 2 ^ octave
		float sampleFrequency = 1.0f / samplePeriod;
		
		for (int i = 0; i <size; i++){
		      //calculate the horizontal sampling indices
		      int sample_i0 = (i / samplePeriod) * samplePeriod;
		      int sample_i1 = (sample_i0 + samplePeriod) % size; //wrap around
		      float horizontal_blend = (i - sample_i0) * sampleFrequency;
		 
		      for (int j = 0; j < size; j++)
		      {
		         //calculate the vertical sampling indices
		         int sample_j0 = (j / samplePeriod) * samplePeriod;
		         int sample_j1 = (sample_j0 + samplePeriod) % size; //wrap around
		         float vertical_blend = (j - sample_j0) * sampleFrequency;
		 
		         //blend the top two corners
		         float top = interpolate(whiteNoise[sample_i0][sample_j0],
		        		 whiteNoise[sample_i1][sample_j0], horizontal_blend);
		 
		         //blend the bottom two corners
		         float bottom = interpolate(whiteNoise[sample_i0][sample_j1],
		        		 whiteNoise[sample_i1][sample_j1], horizontal_blend);
		 
		         //final blend
		         smoothNoise[i][j] = interpolate(top, bottom, vertical_blend);
		      }
		   }
		 
		return smoothNoise;
	}//Method smoothNoise
	
	
	public static Bitmap make2Dtexture(int size, float[] octaveWeights){
		
		
		int nOctaves = octaveWeights.length;
		
		float[][][] smothOctaves = new float[nOctaves][size][size];
		float[][] whiteNoise = makeWhiteNoise(size);
		for(int i=0; i<nOctaves; i++){
			smothOctaves[i] = makeOctaveNoise(i, whiteNoise, size);
			
		}
		
		Bitmap texture = Bitmap.createBitmap(size, size, Bitmap.Config.ARGB_8888);
		for(int i=0; i<size; i++){
			for(int j=0; j<size; j++){
				float samplesSum = 0.0f;
				float f = 0.0f;
				for(int freq=0; freq<nOctaves; freq++){
			
					f += smothOctaves[freq][i][j] * octaveWeights[freq];
					samplesSum += smothOctaves[freq][i][j];
				}
				f/=samplesSum; //normalize the signal to [0.0 , 1.0]
				byte k = (byte) (256*f);
				int c = ((k)|(k<<8)|(k<<16)|0xff<<24);
				
				texture.setPixel(i, j, c);		
			}
		}
		
		return texture;
	}
	
	
}	
