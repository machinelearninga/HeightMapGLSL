//////////////////////////////////////////////////////////// 
// Headers 
//////////////////////////////////////////////////////////// 
#include "stdafx.h" 
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 
//#pragma comment(lib,"glew32.lib") 

  
//#include <GL/glew.h>
#include "SFML/Graphics.hpp" 
#include "SFML/OpenGL.hpp" 
#include <iostream>
#include <sstream> 
  

#include "gridMesh.h"
  
//#include "torus.h"
#define X 0.52573111f
#define Z 0.85065080f
 
GLfloat icosVertices[][3]={ {-X,0.0,Z},{X,0.0,Z},{-X,0.0,-Z},{X,0.0,-Z},
						{0.0, Z,X},{0.0,Z,-X},{0.0,-Z,X},{0.0,-Z,-X},
						{Z,X,0.0},{-Z,X,0.0 },{Z,-X,0.0 },{-Z,-X,0.0}
}; // coordinates for vertices an icosahedron
 
 
GLuint icosTriangles[][3]={ {1,4,0}, {4,9,0},   {4,5,9},   {8,5,4},   {1,8,4},{1,10,8},  {10,3,8},   {8,3,5},   {3,2,5},   {3,7,2},{3,10,7},  {10,6,7},   {6,11,7},  {6,0,11},  {6,1,0}, {10,1,6},  {11,0,9},   {2,11,9},   {5,2,9},   {11,2,7}};


//////////////////////////////////////////////////////////// 
/// Entry point of application 
//////////////////////////////////////////////////////////// 
int main() 
{ 
    // Create the main window 
    sf::RenderWindow App(sf::VideoMode(1200, 800, 32), "SFML OpenGL"); 
  
	// Create a clock for measuring time elapsed     
	sf::Clock Clock; 
	//create a font
	sf::Font font;

	// Load font from a file
	if (!font.loadFromFile("../sansation.ttf"))
		//find this file in the "pong" example in the SFML examples folder
	{
		std::cout << "Error loading font\n" ;
		exit(1);
	}


	//load texture image
	sf::Texture heightMap;
	//sf::String height_file=;
	if (!heightMap.loadFromFile("../HeightMap.png"))
	{
		std::cout << "Could not load ";// << height_file.getData();

	}
	
	//load texture image
	sf::Texture snowTex;
	if (!snowTex.loadFromFile("../snow.jpg"))
	{
		std::cout << "Could not load snow.jpg";

	}

	//load texture image
	sf::Texture grassTex;
	if (!grassTex.loadFromFile("../grass.jpg"))
	{
		std::cout << "Could not load grass.jpg";

	}
	
	//load texture image
	sf::Texture waterTex;
	if (!waterTex.loadFromFile("../water.jpg"))
	{
		std::cout << "Could not load water.jpg";

	}

    int curShader=0;
	const int NumShaders=1;
	sf::Shader shaders[NumShaders];
	std::string vertexShaders[NumShaders]={		"heightMap_vert.glsl"};
	std::string fragShaders[NumShaders]={		"heightMap_frag.glsl"};
	std::string shaderDir="..\\SampleShaders\\";
	
	for(int i=0;i<NumShaders;i++){
		if(!shaders[i].loadFromFile(shaderDir+vertexShaders[i],shaderDir+fragShaders[i])){
		  std::cout << "\nError:\n" << "loading shaders " << vertexShaders[i] << " & " <<fragShaders[i] ;
		};
	}	


	
	shaders[0].setParameter("snowTex",  snowTex); //set texture of 5th shader
	shaders[0].setParameter("heightMap",  heightMap); //set texture of 5th shader
	shaders[0].setParameter("grassTex",  grassTex); //set texture of 5th shader
	shaders[0].setParameter("waterTex",  waterTex); //set texture of 5th shader



	shaders[0].setParameter("gridWidth", (float)(3.0f/U_STEPS));
	shaders[0].setParameter("texWidth", (float)(1.0f/U_STEPS));

	//GLenum err = glewInit();
	//if (GLEW_OK != err)
	//{
	//	/* Problem: glewInit failed, something is seriously wrong. */
	//	std::cout << "Error:\n" << glewGetErrorString(err);
 //

	//}
    //prepare OpenGL surface for HSR 
    glClearDepth(1.f); 
    glClearColor(0.3f, 0.3f, 0.3f, 0.f); //background colour
    glEnable(GL_DEPTH_TEST); 
    glDepthMask(GL_TRUE); 
  
	enum drawModes{wireframe,outline,DRAWEND} drawMode=outline;
	enum Models{cube,icosagon,grid,MODELSEND} model=grid;

	int numGridPoints=createGrid();

    //// Setup a perspective projection & Camera position 
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    
	//set up a 3D Perspective View volume
	gluPerspective(30.f, 1.f, 1.f, 300.0f);//fov, aspect, zNear, zFar 

	//set up a  orthographic projection 4 uints high/wide centred on origin

	//glOrtho(-2,2,-2,2,0,10); 

	bool wasSpressed=false;
	bool shaderOn=true;

	while (App.isOpen()) 
    { 
        // Process events 
        sf::Event Event; 
        while (App.pollEvent(Event)) 
        { 
            // Close window : exit 
            if (Event.type == sf::Event::Closed) 
                App.close(); 
  
            // Escape key : exit 
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape)) 
                App.close(); 
            

						
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::W)){
				drawMode=(drawModes)(drawMode+1);
				if(drawMode==DRAWEND)
					drawMode=(drawModes)0;
			}
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::M)){
				model=(Models)(model+1);
				if(model==MODELSEND)
					model=(Models)0;
			}


			bool sPressed=false;

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::S)){
				sPressed=true;
				if(!wasSpressed)
					shaderOn=!shaderOn;
			}

			wasSpressed=sPressed;
  
            
        } 
          
		if(!App.isOpen()){
			continue;
		}
        //Prepare for drawing 

		
        // Clear color and depth buffer 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  
		if(shaderOn==true){
			sf::Shader::bind(&shaders[curShader]);
		}
		else{
			sf::Shader::bind(NULL);
		}
        // Apply some transformations 
        glMatrixMode(GL_MODELVIEW); 
        glLoadIdentity(); 
		
		double angle=Clock.getElapsedTime().asMilliseconds();
		gluLookAt(0,2.5,-5,  0,0.5,0,  0,1,0);

		GLfloat light_position[]={0.0, 2.0, 0.0, 1.0};
 
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glRotated(angle/100, 0, 1, 0); // rotate
		//glRotated(30.f, 1, 1, 1); // rotate
		
		
		switch(drawMode){
		case wireframe:
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			break;
		
		case outline:
		default:
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			break;
  
		}

		switch(model){
		 default:
		case cube:
			glBegin(GL_QUADS);//draw some squares
			glColor3d(0,1,1);
            glTexCoord2d(0,0);glVertex3f(-1.0f, -1.0f, -1.0f);
            glTexCoord2d(0,1);glVertex3f(-1.0f,  1.0f, -1.0f);
            glTexCoord2d(1,1);glVertex3f( 1.0f,  1.0f, -1.0f);
            glTexCoord2d(1,0);glVertex3f( 1.0f, -1.0f, -1.0f);

			glColor3f(0,0,1);
            glVertex3f(-1.0f, -1.0f, 1.0f);
            glVertex3f(-1.0f,  1.0f, 1.0f);
            glVertex3f( 1.0f,  1.0f, 1.0f);
            glVertex3f( 1.0f, -1.0f, 1.0f);

			glColor3f(1,0,1);
            glVertex3f(-1.0f, -1.0f, -1.0f);
            glVertex3f(-1.0f,  1.0f, -1.0f);
            glVertex3f(-1.0f,  1.0f,  1.0f);
            glVertex3f(-1.0f, -1.0f,  1.0f);

			glColor3f(0,1,0);
            glVertex3f(1.0f, -1.0f, -1.0f);
            glVertex3f(1.0f,  1.0f, -1.0f);
            glVertex3f(1.0f,  1.0f,  1.0f);
            glVertex3f(1.0f, -1.0f,  1.0f);

			glColor3f(1,1,0);
            glVertex3f(-1.0f, -1.0f,  1.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f);
            glVertex3f( 1.0f, -1.0f, -1.0f);
            glVertex3f( 1.0f, -1.0f,  1.0f);

			glColor3f(1,1,1);
            glTexCoord2d(0,1);glVertex3f(-1.0f, 1.0f,  1.0f);
            glTexCoord2d(0,0);glVertex3f(-1.0f, 1.0f, -1.0f);
            glTexCoord2d(1,0);glVertex3f( 1.0f, 1.0f, -1.0f);
            glTexCoord2d(1,1);glVertex3f( 1.0f, 1.0f,  1.0f);

        glEnd();
			break;
		case icosagon:
			glEnableClientState(GL_VERTEX_ARRAY); // we want to use vertex arrays for coordinate info
			glVertexPointer(3,GL_FLOAT,0,(GLvoid*)icosVertices);// give openGL our array of vertices
			glDrawElements(GL_TRIANGLES, 20*3, GL_UNSIGNED_INT,icosTriangles);
			break;
		case grid://draw grid
			//// we want to use a vertex array
			//glEnableClientState(GL_VERTEX_ARRAY);
			//glVertexPointer(3,GL_FLOAT,0,(GLvoid*)gridVertices);
 
			////we want to use a normal array
			//glEnableClientState(GL_NORMAL_ARRAY);
			//glNormalPointer(GL_FLOAT,0,(GLvoid*)torusNormals);

			//glDrawElements(GL_POINTS, numGridPoints, GL_UNSIGNED_INT,gridVertices);
			/*GLfloat* verts=(GLfloat*)gridVertices;
			GLfloat* text=(GLfloat*)gridTexCoords;
			glBegin(GL_POINTS);
			for(int i=0;i<numGridPoints;i++){
					glTexCoord2fv(text+i*2);
					glVertex3fv(verts+i*3);
			}
			glEnd();*/
			GLfloat* verts=(GLfloat*)gridVertices;
			GLfloat* text=(GLfloat*)gridTexCoords;
			glEnableClientState(GL_VERTEX_ARRAY); // we want to use vertex arrays for coordinate info
			glVertexPointer(3,GL_FLOAT,0,(GLvoid*)verts);// give openGL our array of vertices
			glEnableClientState(GL_TEXTURE_COORD_ARRAY); // we want to use vertex arrays for coordinate info
			glTexCoordPointer(2,GL_FLOAT,0,(GLvoid*)text);
			GLint* tris=(GLint*)gridTriangles;
			glDrawElements(GL_TRIANGLES, numGridTriangles*3, GL_UNSIGNED_INT,tris);
			break;
			
			break;
		}
	
  

		//Display info
	    sf::Shader::bind(NULL);
		App.pushGLStates();
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		//set up text properties
		sf::Text atext;
		atext.setFont(font);
		atext.setCharacterSize(20);
		atext.setStyle(sf::Text::Bold);
		atext.setColor(sf::Color::White);
		atext.setPosition(0,0);


		std::ostringstream ss; //string buffer to convert numbers to string
		ss << "[Space] to change shader" ;// 
		
		std::ostringstream ms; //string buffer to convert numbers to string
		ms << "[m] to change model:" ;// 
		
		std::ostringstream ws; //string buffer to convert numbers to string
		ws << "[w] wireframe/solid";// 
		

		std::ostringstream vs; //string buffer to convert numbers to string
		vs << "VertexShader is: " << vertexShaders[curShader];// 
		
		std::ostringstream fs; //string buffer to convert numbers to string
		fs << "FragmentShader is: " << fragShaders[curShader];// 

		atext.setString(ss.str()); //ss.str() converts the string buffer into a regular string 
		App.draw(atext);

		atext.setPosition(0,20);
		atext.setString(ms.str()); //ss.str() converts the string buffer into a regular string 
		App.draw(atext);

		atext.setPosition(0,40);
		atext.setString(ws.str()); //ss.str() converts the string buffer into a regular string 
		App.draw(atext);

		atext.setPosition(300,0);
		atext.setString(vs.str()); //ss.str() converts the string buffer into a regular string 
		App.draw(atext);

		atext.setPosition(300,20);
		atext.setString(fs.str()); //ss.str() converts the string buffer into a regular string 
		App.draw(atext);

		//restore OpenGL setting that were saved earlier
		App.popGLStates();
		
        // Finally, display rendered frame on screen 
        App.display(); 
    } 
  
    return EXIT_SUCCESS; 
} 