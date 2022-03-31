#include "application.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaders/shader.h"
#include "performanceMonitor.h"

#include <stb_image.h>

using namespace std;

namespace Journey {

Application::Application(const std::string GameName)
{
    mName = GameName;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void Application::Start() const
{
        // settings
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    float x = 0.0;
    float y = -0.5;
    float z = 1.2;
    float speed = 1.6f;

    glm::vec3 camDir = glm::vec3(0.0f, 0.6, -0.3);

    // lighting
    glm::vec3 lightPos(1.5f, 1.0f, 2.5f);


    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    string title = "Journey Engine - " + mName;
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title.c_str(), NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("../../../source/shaders/lightMVPShader.vs", "../../../source/shaders/lightMVPShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // Load the image and 
    // build and compile our shader zprogram
    // ------------------------------------
    Shader mpvTexShader("../../../source/shaders/MVPTexShader.vs", "../../../source/shaders/MVPTexShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float texvertices[] = {
        -0.5f, 0.0f, -0.5f,  0.0f, 1.0f,
         0.5f, 0.0f, -0.5f,  1.0f, 1.0f,
         0.5f, 0.0f, 0.5f,  1.0f, 0.0f,
         0.5f,  0.0f, 0.5f,  1.0f, 0.0f,
        -0.5f,  0.0f, 0.5f,  0.0f, 0.0f,
        -0.5f, 0.0f, -0.5f,  0.0f, 1.0f
    };

    unsigned int texVBO, texVAO;
    glGenVertexArrays(1, &texVAO);
    glGenBuffers(1, &texVBO);

    glBindVertexArray(texVAO);

    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texvertices), texvertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // load and create a texture 
    // -------------------------
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char* data = stbi_load("../../../assets/sprites/carpincho.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Load a second texture

    float texvertices2[] = {
        -0.5f, 0.0f, -0.5f,  0.0f, 1.0f,
         0.5f, 0.0f, -0.5f,  1.0f, 1.0f,
         0.5f, 0.0f, 0.5f,  1.0f, 0.0f,
         0.5f,  0.0f, 0.5f,  1.0f, 0.0f,
        -0.5f,  0.0f, 0.5f,  0.0f, 0.0f,
        -0.5f, 0.0f, -0.5f,  0.0f, 1.0f
    };

    unsigned int texVBO2, texVAO2;
    glGenVertexArrays(1, &texVAO2);
    glGenBuffers(1, &texVBO2);

    glBindVertexArray(texVAO2);

    glBindBuffer(GL_ARRAY_BUFFER, texVBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texvertices2), texvertices2, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width2, height2, nrChannels2;
    unsigned char* data2 = stbi_load("../../../assets/sprites/tree.png", &width2, &height2, &nrChannels2, 0);
    if (data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data2);

    PerformanceMonitor pMonitor(glfwGetTime(), 0.5f);

    //Enabling transparencies
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // transforms 
    glm::mat4 tree1 = glm::mat4(1.0f); 
    tree1 = glm::translate(tree1, glm::vec3(4.2f, -0.4, 0.8));
    tree1 = glm::rotate(tree1, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    tree1 = glm::scale(tree1, glm::vec3(1.8f, 1.0f, 2.2f));

    // TREE 2
    glm::mat4 tree2 = glm::mat4(1.0f); 
    tree2 = glm::translate(tree2, glm::vec3(2.4f, 3.0, 0.7));
    tree2 = glm::rotate(tree2, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    tree2 = glm::scale(tree2, glm::vec3(2.2f, 1.0f, 2.6f));

    // TREE 3
    glm::mat4 tree3 = glm::mat4(1.0f); 
    tree3 = glm::translate(tree3, glm::vec3(-2.0f, 1.8, 0.5));
    tree3 = glm::rotate(tree3, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    tree3 = glm::scale(tree3, glm::vec3(0.9f, 1.0f, 1.4f));

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        pMonitor.update(glfwGetTime());
        stringstream ss;
        ss << title << " " << pMonitor;
        glfwSetWindowTitle(window, ss.str().c_str());

        // input
        // -----
        ;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            x -= speed * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            x += speed * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            y += speed * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            y -= speed * deltaTime;
        }

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("light.position", lightPos);
        lightingShader.setVec3("viewPos", glm::vec3(x, y , z) - camDir * 2.0f);

        glm::vec3 diffuseColor = glm::vec3(0.6f); // decrease the influence
        glm::vec3 ambientColor = glm::vec3(0.3f); // low influence
        lightingShader.setVec3("light.ambient", ambientColor);
        lightingShader.setVec3("light.diffuse", diffuseColor);
        lightingShader.setVec3("light.specular", 0.6f, 0.6f, 0.6f);

        // material properties
        lightingShader.setVec3("material.ambient", 0.1f, 0.6f, 0.05f);
        lightingShader.setVec3("material.diffuse", 0.25f, 0.9f, 0.01f);
        lightingShader.setVec3("material.specular", 0.1f, 0.1f, 0.1f); // specular lighting doesn't have full effect on this object's material
        lightingShader.setFloat("material.shininess", 64.0f);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(x, y , z) - camDir * 2.0f, glm::vec3(x, y , z), glm::vec3(0.0, 0.0, 1.0f));
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);


        // plane transformation 
        glm::mat4 transform1 = glm::mat4(1.0f); 
        transform1 = glm::translate(transform1, glm::vec3(-0.0f, 0.0f, 0.0f));
        transform1 = glm::rotate(transform1, glm::radians(35.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        transform1 = glm::scale(transform1, glm::vec3(10.0f, 7.0f, 0.3f));
        lightingShader.setMat4("model", transform1);

        // render the cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // activate shader
        mpvTexShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        mpvTexShader.setMat4("projection", projection);

        // camera/view transformation
        mpvTexShader.setMat4("view", view);

        
        // bind textures on corresponding texture units
        glBindTexture(GL_TEXTURE_2D, texture2);

        // TREE 1
        // render the texture
        glBindVertexArray(texVAO2);
        mpvTexShader.setMat4("model", tree1);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // TREE 2
        // render the texture
        glBindVertexArray(texVAO2);
        mpvTexShader.setMat4("model", tree2);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // TREE 3S
        // render the texture
        glBindVertexArray(texVAO2);
        mpvTexShader.setMat4("model", tree3);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        
        // bind textures on corresponding texture units
        glBindTexture(GL_TEXTURE_2D, texture1);
        // tex transformation carpincho 
        glm::mat4 transform2 = glm::mat4(1.0f); 
        transform2 = glm::translate(transform2, glm::vec3(x, y, z+0.0f));
        transform2 = glm::rotate(transform2, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        transform2 = glm::scale(transform2, glm::vec3(0.4f, 1.0f, 0.3f));
        // render the texture
        glBindVertexArray(texVAO);
        mpvTexShader.setMat4("model", transform2);

        glDrawArrays(GL_TRIANGLES, 0, 6);



        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

}
