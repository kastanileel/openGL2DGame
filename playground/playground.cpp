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
#include <vector>

 
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


class GameObject {
public:
    //some global variables for handling the vertex sbuffer
    GLuint vertexbuffer;

    //modified
    GLuint colorbuffer;


    GLuint VertexArrayID;
    GLuint vertexbuffer_size;
    glm::mat4 translation;
    float speed;
    float radius;
    bool isActive;



    virtual void update(glm::mat4* mvp) = 0;
    virtual void draw(float scale) {

        glm::mat4 mvp_matrix = translation;
        mvp_matrix = mvp_matrix * glm::mat4(
            scale, 0, 0, 0,
            0, scale, 0, 0,
            0, 0, scale, 0,
            0, 0, 0, 1
        );

        mvp_matrix[0][3] = mvp_matrix[0][3] / scale;
        mvp_matrix[1][3] = mvp_matrix[1][3] / scale;


        GLfloat matrix = glGetUniformLocation(programID, "Translation");
        glUniformMatrix4fv(matrix, 1, GL_FALSE, &mvp_matrix[0][0]);

        // Use our shader
        glUseProgram(programID);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

        //2nd attribute buffer : color
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );




        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, vertexbuffer_size); // 3 indices starting at 0 -> 1 triangle

        glDisableVertexAttribArray(0);
    }
    
    virtual bool initializeVAOs() = 0;
    virtual bool cleanupVAOs() = 0;
    virtual bool initializeColorbuffer()
    {
        static GLfloat g_color_buffer_data[18];
        g_color_buffer_data[0] = 1;
        g_color_buffer_data[1] = 0;
        g_color_buffer_data[2] = 0;

        g_color_buffer_data[3] = 1;
        g_color_buffer_data[4] = 0;
        g_color_buffer_data[5] = 0;

        g_color_buffer_data[6] = 1;
        g_color_buffer_data[7] = 0;
        g_color_buffer_data[8] = 0;

        g_color_buffer_data[9] = 0;
        g_color_buffer_data[10] = 1;
        g_color_buffer_data[11] = 0;

        g_color_buffer_data[12] = 0;
        g_color_buffer_data[13] = 1;
        g_color_buffer_data[14] = 0;

        g_color_buffer_data[15] = 0;
        g_color_buffer_data[16] = 1;
        g_color_buffer_data[17] = 0;




        glDeleteBuffers(1, &colorbuffer);
        glGenBuffers(1, &colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);



        return true;
    }

    virtual bool initializeVertexbuffer()
    {

        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);


        vertexbuffer_size = 6;
        glm::vec2 triangleVertice1 = glm::vec2(-1.0f, -1.0f);
        glm::vec2 triangleVertice2 = glm::vec2(1.0f, -1.0f);
        glm::vec2 triangleVertice3 = glm::vec2(0.0f, 1.0f);

        glm::vec2 triangleVertice4 = glm::vec2(2.0f, 1.0f);
        glm::vec2 triangleVertice5 = glm::vec2(1.0f, -1.0f);
        glm::vec2 triangleVertice6 = glm::vec2(0.0f, 1.0f);

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


        g_vertex_buffer_data[9] = triangleVertice4[0];
        g_vertex_buffer_data[10] = triangleVertice4[1];
        g_vertex_buffer_data[11] = 0.0f;

        g_vertex_buffer_data[12] = triangleVertice5[0];
        g_vertex_buffer_data[13] = triangleVertice5[1];
        g_vertex_buffer_data[14] = 0.0f;

        g_vertex_buffer_data[15] = triangleVertice6[0];
        g_vertex_buffer_data[16] = triangleVertice6[1];
        g_vertex_buffer_data[17] = 0.0f;






        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        return true;
    }

    bool cleanupVertexbuffer()
    {
        // Cleanup VBO
        glDeleteBuffers(1, &vertexbuffer);

        glDeleteVertexArrays(1, &VertexArrayID);
        return true;
    }

    bool cleanupColorbuffer() {
        glDeleteBuffers(1, &colorbuffer);
        glDeleteVertexArrays(1, &VertexArrayID);
        return true;
    }


    //detect collisions with other game objects and return all objects that are colliding
    std::vector<GameObject*> checkCollisions(std::vector<GameObject*> gameObjects) {
        std::vector<GameObject*> collidedObjects;

        if (isActive) {
            for each (GameObject * cur in gameObjects)
            {
                if (cur != this) {
                    float dx = translation[0][3] - cur->translation[0][3];
                    float dy = translation[1][3] - cur->translation[1][3];


                    float dist = sqrt(dx * dx + dy * dy);
                    float mindistance = radius + cur->radius;
                    if (dist < mindistance) {
                        collidedObjects.push_back(cur);
                        //std::cout << "Collision detected!" << std::endl;
                    }


                }
            }
        }

        return collidedObjects;
    }

};


class Player : public GameObject {
    int hitpoints;

public:
    Player(int hp, glm::mat4 translation_g) {
        hitpoints = hp;
        translation = translation_g;
        speed = 0.02f;
        radius = 0.1f;
        isActive = true;
    }
    void update(glm::mat4* mvp) override {
        glm::mat4 finished_mvp = *mvp;

        finished_mvp[0][3] = finished_mvp[0][3] * speed;
        finished_mvp[1][3] = finished_mvp[1][3] * speed;

        translation = translation * finished_mvp;

        glm::mat4 test = *mvp;

        //std::cout << test[0][3] << std::endl;
        //std::cout << test[1][3] << std::endl;
        draw(0.1f);


    }
    void draw(float scale) override{
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

        float ver = xRes / yRes;
        float size = 0.2f;

        glm::mat4 mv = glm::mat4(
            -1 * size, 0, 0, x,
            0, ver * -1 * size, 0, y,
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
    }
    bool initializeVertexbuffer() override {

        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);



        vertexbuffer_size = 3;
        glm::vec2 triangleVertice1 = glm::vec2(-1.0f, -1.0f);
        glm::vec2 triangleVertice2 = glm::vec2(1.0f, -1.0f);
        glm::vec2 triangleVertice3 = glm::vec2(0.0f, 1.0f);

        static GLfloat g_vertex_buffer_data[9];
        g_vertex_buffer_data[0] = triangleVertice1[0];
        g_vertex_buffer_data[1] = triangleVertice1[1];
        g_vertex_buffer_data[2] = 0.0f;

        g_vertex_buffer_data[3] = triangleVertice2[0];
        g_vertex_buffer_data[4] = triangleVertice2[1];
        g_vertex_buffer_data[5] = 0.0f;

        g_vertex_buffer_data[6] = triangleVertice3[0];
        g_vertex_buffer_data[7] = triangleVertice3[1];
        g_vertex_buffer_data[8] = 0.0f;

        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        return true;

    }
    bool initializeColorbuffer() override {
        static GLfloat g_color_buffer_data[9];
        g_color_buffer_data[0] = 1;
        g_color_buffer_data[1] = 1;
        g_color_buffer_data[2] = 1;

        g_color_buffer_data[3] = 1;
        g_color_buffer_data[4] = 1;
        g_color_buffer_data[5] = 1;

        g_color_buffer_data[6] = 1;
        g_color_buffer_data[7] = 1;
        g_color_buffer_data[8] = 1;

        glDeleteBuffers(1, &colorbuffer);
        glGenBuffers(1, &colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

        return true;
    }

    bool initializeVAOs() override{
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        vertexbuffer_size = 6;

        glm::vec2 triangleVertice1 = glm::vec2(0.0f, 0.0f);
        glm::vec2 triangleVertice2 = glm::vec2(0.0f, 1.0f);
        glm::vec2 triangleVertice3 = glm::vec2(1.0f, 1.0f);

        glm::vec2 secTriangleVertice1 = glm::vec2(0.0f, 0.0f);
        glm::vec2 secTriangleVertice2 = glm::vec2(1.0f, 1.0f);
        glm::vec2 secTriangleVertice3 = glm::vec2(1.0f, 0.0f);

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
    bool cleanupVAOs() override{
        // Cleanup VBO
        glDeleteBuffers(1, &vertexbuffer);
        glDeleteVertexArrays(1, &VertexArrayID);
        return true;
    }


};

class Enemy : public GameObject {
    int hitpoints;
    glm::vec2 position;


public:

    glm::vec2 playerposition_enemy;

    bool initializeVertexbuffer() override {
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);



        vertexbuffer_size = 18;

        std::vector<glm::vec2*> vertices;



        glm::vec2 triangleVertice1 = glm::vec2(6.0f, 0.0f);
        glm::vec2 triangleVertice2 = glm::vec2(3.0f, 3 * sqrt(3.0f));
        glm::vec2 triangleVertice3 = glm::vec2(0.0f, 0.0f);

        glm::vec2 triangleVertice4 = glm::vec2(3.0f, 3 * sqrt(3.0f));
        glm::vec2 triangleVertice5 = glm::vec2(-3.0f, 3 * sqrt(3.0f));
        glm::vec2 triangleVertice6 = glm::vec2(0.0f, 0.0f);

        glm::vec2 triangleVertice7 = glm::vec2(-3.0f, 3 * sqrt(3.0f));
        glm::vec2 triangleVertice8 = glm::vec2(-6.0f, 0);
        glm::vec2 triangleVertice9 = glm::vec2(0.0f, 0.0f);

        glm::vec2 triangleVertice10 = glm::vec2(-6.0f, 0);
        glm::vec2 triangleVertice11 = glm::vec2(-3.0f, -3 * sqrt(3.0f));
        glm::vec2 triangleVertice12 = glm::vec2(0.0f, 0.0f);

        glm::vec2 triangleVertice13 = glm::vec2(-3.0f, -3 * sqrt(3.0f));
        glm::vec2 triangleVertice14 = glm::vec2(3.0f, -3 * sqrt(3.0f));
        glm::vec2 triangleVertice15 = glm::vec2(0.0f, 0.0f);

        glm::vec2 triangleVertice16 = glm::vec2(3.0f, -3 * sqrt(3.0f));
        glm::vec2 triangleVertice17 = glm::vec2(6.0f, 0.0f);
        glm::vec2 triangleVertice18 = glm::vec2(0.0f, 0.0f);
        vertices.push_back(&triangleVertice1);
        vertices.push_back(&triangleVertice2);
        vertices.push_back(&triangleVertice3);
        vertices.push_back(&triangleVertice4);
        vertices.push_back(&triangleVertice5);
        vertices.push_back(&triangleVertice6);
        vertices.push_back(&triangleVertice7);
        vertices.push_back(&triangleVertice8);
        vertices.push_back(&triangleVertice9);
        vertices.push_back(&triangleVertice10);
        vertices.push_back(&triangleVertice11);
        vertices.push_back(&triangleVertice12);
        vertices.push_back(&triangleVertice13);
        vertices.push_back(&triangleVertice14);
        vertices.push_back(&triangleVertice15);
        vertices.push_back(&triangleVertice16);
        vertices.push_back(&triangleVertice17);
        vertices.push_back(&triangleVertice18);




        static GLfloat g_vertex_buffer_data[54];

        for (int i = 0; i < vertices.size(); i++) {
            glm::vec2 cur = *vertices[i];
            g_vertex_buffer_data[3 * i] = float(cur[0]);
            g_vertex_buffer_data[3 * i + 1] = float(cur[1]);
            g_vertex_buffer_data[3 * i + 2] = 0.0f;
            std::cout << "x: " << g_vertex_buffer_data[i] << " y: " << g_vertex_buffer_data[i + 1] << std::endl;
        }


        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        return true;

    }

    bool initializeColorbuffer() override {
        static GLfloat g_color_buffer_data[54];

        for (int i = 0; i < 54; i++) {
            g_color_buffer_data[i] = 1;
        }

        glDeleteBuffers(1, &colorbuffer);
        glGenBuffers(1, &colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

        return true;

    }

    Enemy(int hp, glm::mat4 translation_g) {
        hitpoints = hp;
        translation = translation_g;
        speed = 0.01f;
        isActive = false;
        radius = 0.1f;

    }
    void update(glm::mat4* mvp) override {
        glm::mat4 playertranslation = *mvp;


        if (!isActive) {
            translation = glm::mat4(
                1, 0, 0, 10,
                0, 1, 0, 10,
                0, 0, 1, 0,
                0, 0, 0, 1
            );
        }

        if (
            translation[0][3] > 2.0f || translation[0][3] < -2.0f ||
            translation[1][3] > 2.0f || translation[1][3] < -2.0f
            ) {
            isActive = false;
        }


        if (isActive)
        {
            translation[0][3] = translation[0][3] + (playerposition_enemy[0] - translation[0][3]) * speed;

            if (translation[1][3] > playerposition_enemy[1]) {
                translation[1][3] = translation[1][3] - speed;
                //std::cout << "moving to: y= " << playerposition_enemy[1] << std::endl;
            }
            //std::cout << "moving to: x= " << playerposition_enemy[0] << std::endl;




        }

        draw(0.02f);
    }
};


Player* p;

int main( void )
{
   
  //Initialize window
  bool windowInitialized = initializeWindow();
  if (!windowInitialized) return -1;

  glm::mat4 trans = glm::mat4(
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
      );

  p = &Player(10, trans);

  p->initializeVAOs();

  // Create and compile our GLSL program from the shaders
  programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

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
  p->cleanupVAOs();
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

        
    }
    if (glfwGetKey(window, GLFW_KEY_S)) {
        myR = glm::mat2(-1, 0,
            0, -1);
        y -= 0.001f;

       
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

    p->draw(1);

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

/*bool initializeVertexbuffer()
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
*/
bool closeWindow()
{
  glfwTerminate();
  return true;
}

