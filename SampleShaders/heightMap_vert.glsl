#version 120


uniform sampler2D heightMap;

void main()
{
	vec4 pos=gl_Vertex;

	float f;

	if(gl_MaxVertexTextureImageUnits>0){
		vec4 h=texture2D(heightMap,gl_MultiTexCoord0.st);

		f=h.r;
	
	}
   
 
    pos.y=f;

	gl_FrontColor=vec4(1);

	gl_TexCoord[0]=gl_MultiTexCoord0;

	gl_Position = gl_ProjectionMatrix*gl_ModelViewMatrix*pos;
}