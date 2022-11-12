#include "playground.h"


// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>                                         
using namespace glm;
#include <iostream>
#include <common/shader.hpp>

//new inclued
#define STB_IMAGE_IMPLEMENTATION
#include <common/stb_image.h>
#include <chrono>

 
std::chrono::steady_clock::time_point lastUpdate;

float x = 0;
float y = 0;

float xRes = 1920;
float yRes = 1080;
bool state = false;

glm::mat2 myR;

GLuint uvbuffer;
GLuint textureSamplerID;
GLuint texture;
unsigned char* data;



int main( void )
{
   
  //Initialize window
  bool windowInitialized = initializeWindow();
  if (!windowInitialized) return -1;

  //Initialize vertex buffer
  bool vertexbufferInitialized = initializeVertexbuffer();
  if (!vertexbufferInitialized) return -1;

  // Create and compile our GLSL program from the shaders
  programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

  int width, height;
  int nrChannels = 4;
  data = stbi_load("../s1.png", &width, &height, &nrChannels, 4);
  

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  //glTexSubImage2D(GL_TEXTURE0, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data );
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  initializeVertexbuffer();

  lastUpdate = std::chrono::steady_clock::now();
 
	//start animation loop until escape key is pressed
	do{
        if ((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastUpdate).count()) > 30) {
            lastUpdate = std::chrono::steady_clock::now();
            updateAnimationLoop();
        }
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );
    
	
  //Cleanup and close window
  cleanupVertexbuffer();
  glDeleteProgram(programID);
	
  closeWindow();
  
	return 0;
}

void updateAnimationLoop()
{



    /* glGenTextures(1, &texture);
     glBindTexture(GL_TEXTURE_2D, texture);
     // set the texture wrapping/filtering options (on the currently bound texture object)
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     // load and generate the texture
     int width, height, nrChannels;
     unsigned char* data = stbi_load("../container.jpg", &width, &height, &nrChannels, 0);
     if (data)
     {

         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

         glGenerateTextureMipmap(GL_TEXTURE_2D);

         std::cout << "image loades sucessfully";
     }
     else
     {
         std::cout << "Failed to load texture" << std::endl;
     }
     stbi_image_free(data);
     */


    if (glfwGetKey(window, GLFW_KEY_SPACE)) {

    }
    if (glfwGetKey(window, GLFW_KEY_W)) {
        y += 0.001f;
        myR = glm::mat2(1, 0,
            0, 1);

        int width, height;
        int nrChannels = 4;
        data = stbi_load("../sprites/front_1.png", &width, &height, &nrChannels, 4);


        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        //glTexSubImage2D(GL_TEXTURE0, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data );
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    if (glfwGetKey(window, GLFW_KEY_S)) {
        myR = glm::mat2(-1, 0,
            0, -1);
        y -= 0.001f;

        int width, height;
        int nrChannels = 4;
        data = stbi_load("../sprites/front_2.png", &width, &height, &nrChannels, 4);


        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        //glTexSubImage2D(GL_TEXTURE0, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data );
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    if (glfwGetKey(window, GLFW_KEY_A)) {
        x -= 0.001f;
        myR = glm::mat2(0, 1,
            -1, 0);
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        x += 0.001f;
        myR = glm::mat2(0, -1,
            1, 0);
    }

    float ver = xRes / yRes;
    float size = 0.2f;

    glm::mat4 mv = glm::mat4(
      -1 * size, 0, 0, x,
        0, ver * -1 *size, 0, y,
        0, 0, 1, 0,
        0, 0, 0, 1);

   
    GLfloat matrix = glGetUniformLocation(programID, "mv");
    glUniformMatrix4fv(matrix, 1, GL_FALSE, &mv[0][0]);

  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT);

  // Use our shader
  glUseProgram(programID);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glUniform1i(textureSamplerID, 0);
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glVertexAttribPointer(
      2,
      2,
      GL_FLOAT,
      GL_FALSE,
      0,
      (void*)0
  );
  // Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, vertexbuffer_size); // 3 indices starting at 0 -> 1 triangle

  glDisableVertexAttribArray(0);

  // Swap buffers
  glfwSwapBuffers(window);
  glfwPollEvents();
  
 
}

bool initializeWindow()
{
  // Initialise GLFW
  if (!glfwInit())
  {
    fprintf(stderr, "Failed to initialize GLFW\n");
    getchar();
    return false;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(xRes  , yRes, "Tutorial 02 - RED triangle", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
    getchar();
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
    return false;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
  return true;
}

bool initializeVertexbuffer()
{
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    vertexbuffer_size = 6;

    glm::vec2 triangleVertice1 = glm::vec2(0.0f, 0.0f);
    glm::vec2 triangleVertice2 = glm::vec2(0.0f, 1.0f);
    glm::vec2 triangleVertice3 = glm::vec2(1.0f, 1.0f);

    glm::vec2 secTriangleVertice1 = glm::vec2(0.0f,0.0f);
    glm::vec2 secTriangleVertice2 = glm::vec2(1.0f, 1.0f);
    glm::vec2 secTriangleVertice3 = glm::vec2(1.0f, 0.0f);

    /* glm::mat2 mySResize = glm::mat2(1, 0,
         0, xRes/yRes);


     glm::mat2 myS = glm::mat2(0.1, 0,
                               0, 0.1);

     myS = myS * mySResize;

     glm::vec2 myT = glm::vec2(x,
                               y);

     myT = myT * mySResize;

     triangleVertice1 = myR * triangleVertice1 * myS  + myT;
     triangleVertice2 = myR * triangleVertice2 * myS  + myT;
     triangleVertice3 = myR * triangleVertice3 * myS  + myT;

     */
    static GLfloat g_vertex_buffer_data[18];
    g_vertex_buffer_data[0] = triangleVertice1[0];
    g_vertex_buffer_data[1] = triangleVertice1[1];
    g_vertex_buffer_data[2] = 0.0f;
    g_vertex_buffer_data[3] = triangleVertice2[0];
    g_vertex_buffer_data[4] = triangleVertice2[1];
    g_vertex_buffer_data[5] = 0.0f;
    g_vertex_buffer_data[6] = triangleVertice3[0];
    g_vertex_buffer_data[7] = triangleVertice3[1];
    g_vertex_buffer_data[8] = 0.0f;
    g_vertex_buffer_data[9] = secTriangleVertice1[0];
    g_vertex_buffer_data[10] = secTriangleVertice1[1];
    g_vertex_buffer_data[11] = 0.0f;
    g_vertex_buffer_data[12] = secTriangleVertice2[0];
    g_vertex_buffer_data[13] = secTriangleVertice2[1];
    g_vertex_buffer_data[14] = 0.0f;
    g_vertex_buffer_data[15] = secTriangleVertice3[0];
    g_vertex_buffer_data[16] = secTriangleVertice3[1];
    g_vertex_buffer_data[17] = 0.0f;

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


   

    textureSamplerID = glGetUniformLocation(programID, "myTextureSampler");

    static const GLfloat g_uv_buffer_date[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_date), g_uv_buffer_date, GL_STATIC_DRAW);

  return true;
}

bool cleanupVertexbuffer()
{
  // Cleanup VBO
  glDeleteBuffers(1, &vertexbuffer);
  glDeleteVertexArrays(1, &VertexArrayID);
  return true;
}

bool closeWindow()
{
  glfwTerminate();
  return true;
}

