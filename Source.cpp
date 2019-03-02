#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <camera.h>
#include <model.h>
#include <iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(vector<std::string> faces);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float positionx = 0.0f;
float positiony = -2.5f;
float positionz = 1.0f;
float ball_positionx = 0;
float ball_positiony = 0;
float ball_positionz = 0;
float valuex = -20, valuey = 0, valuez = -20;
int keyframe1 = 1;
int keyframe2 = 0, keyframe3 = 0,keyframe4 = 0,keyframe5 = 0,keyframe6 = 0, keyframe7 = 0;
int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "The last freekick", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	float ball_totalx, ball_totaly, ball_totalz;
	int shooterKeyFrames[2] = { 1, 0 };

	string goal_animation[5] = {"../Game assets/obj_files/Goal.obj",""};
	Shader shader("vs.vs", "fs.fs");
	Model newModel("../Game assets/obj_files/soccer_ball.obj");
	Model player("../Game assets/obj_files/escapetwo.obj");
	Model playertwo("../Game assets/obj_files/escapeone.obj");
	Model playerthree("../Game assets/obj_files/chartwo.obj");
	Model playerfour("../Game assets/obj_files/charthree.obj");
	Model playerfive("../Game assets/obj_files/charfour.obj");
	Model playersix("../Game assets/obj_files/charfive.obj");
	Model goal(goal_animation[0]);
	Shader skyboxShader("lmp.vs", "lmp.fs");
	Shader ourShader("vs.vs", "fs.fs");
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	float skyboxVertices[] = {
		// positions          
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};
	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// load textures
	// -------------
	//unsigned int cubeTexture = loadTexture("../Game assets/obj_files/right.jpg");
	vector<std::string> faces
	{ "../Game assets/obj_files/xp.jpg",
	"../Game assets/obj_files/xn.jpg",
	"../Game assets/obj_files/yp.jpg",
	"../Game assets/obj_files/yn.jpg",
	"../Game assets/obj_files/zp.jpg",
	"../Game assets/obj_files/zn.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);

	// shader configuration
	// --------------------
	shader.use();
	shader.setInt("texture1", 0);
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		
	
		// per-frame time logic
		// --------------------
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
			keyframe2 = 1;
		}
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			processInput(window);
			glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
			shader.use();
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = camera.GetViewMatrix();
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			shader.setMat4("model", model);
			shader.setMat4("view", view);
			shader.setMat4("projection", projection);
			// draw skybox as last
			glDepthFunc(GL_LEQUAL); 
			skyboxShader.use();
			view = glm::mat4(glm::mat3(camera.GetViewMatrix())); 
			skyboxShader.setMat4("view", view);
			skyboxShader.setMat4("projection", projection);
			// skybox cube
			glBindVertexArray(skyboxVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glDepthFunc(GL_LESS);
			//ball movement
			ball_totalx = 4.0f + ball_positionx;
			ball_totaly = -3.0f + ball_positiony;
			ball_totalz = 9.0f + ball_positionz;
			ourShader.use();
			glm::mat4 projection1 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			glm::mat4 view1;
			view1 = camera.GetViewMatrix();
			ourShader.use();
			ourShader.setMat4("projection", projection1);
			ourShader.setMat4("view", view1);
			glm::mat4 model1 = glm::mat4(1.0f);
			model1 = glm::rotate(model1, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model1 = glm::translate(model1, glm::vec3(ball_totalx, ball_totaly, ball_totalz)); 
			model1 = glm::scale(model1, glm::vec3(0.5f, 0.5f, 0.5f));
			
			ourShader.setMat4("model", model1);
			newModel.Draw(ourShader);
			//goal model
			ourShader.use();// view/projection transformations
			ourShader.setMat4("projection", projection1);
			ourShader.setMat4("view", view1);
			glm::mat4 model2 = glm::mat4(1.0f);
			model2 = glm::rotate(model2, glm::radians(-100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::translate(model2, glm::vec3(-10.0f + valuex, -3.0f + valuey, -25.0f + valuez)); 
			model2 = glm::scale(model2, glm::vec3(3.0f, 3.0f, 3.0f));
	
			ourShader.setMat4("model", model2);
			goal.Draw(ourShader);
			//Player animation
			if (keyframe2){
				//player model 1
				ourShader.use();
				ourShader.setMat4("projection", projection1);
				ourShader.setMat4("view", view1);
				glm::mat4 player1 = glm::mat4(1.0f);
				player1 = glm::rotate(player1, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				player1 = glm::translate(player1, glm::vec3(0.0f, -3.0f, 2.0f)); // translate it down so it's at the center of the scene
				player1 = glm::scale(player1, glm::vec3(0.5f, 0.5f, 0.5f));
				// it's a bit too big for our scene, so scale it down 
				ourShader.setMat4("model", player1);
				player.Draw(ourShader);
				keyframe3 = 1;
			}
			if (keyframe1){
				//player model 2
				ourShader.use();// view/projection transformations
				ourShader.setMat4("projection", projection1);
				ourShader.setMat4("view", view1);
				glm::mat4 player2 = glm::mat4(1.0f);
				player2 = glm::rotate(player2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				player2 = glm::translate(player2, glm::vec3(0.0f, -3.0f, 2.0f)); // translate it down so it's at the center of the scene
				player2 = glm::scale(player2, glm::vec3(0.5f, 0.5f, 0.5f));
				// it's a bit too big for our scene, so scale it down 
				ourShader.setMat4("model", player2);
				playertwo.Draw(ourShader);
			}
			//player model 3
		
	
			//player model 4 
		
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
				ball_positionx += 10;
				ball_positiony += 0;
				ball_positionz += 10;
				keyframe1 = 0;
				keyframe2 = 1;
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){

				ball_positionx -= 10;
				ball_positionz -= 10;

			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
			
				ball_positionx += 10;
				ball_positionz += 12;
			
			}
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
				ball_positiony += 13;
			}
			//player model 6
		
			//player model 7
			//player model 8
			//player model 9
			//player model 10
			//player model 11
			//player model 12

			
			std::cout << glfwGetTime()<<std::endl;
		
			if (ball_totalx < -15 && ball_totaly > 30){
				std::cout << "Goal";
			}
			else{
			
				std::cout << "missed";
			}


		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------

	glDeleteVertexArrays(1, &skyboxVAO);
	
	glDeleteBuffers(1, &skyboxVAO);

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
//	if (firstMouse)
	//{
		//lastX = xpos;
		//lastY = ypos;
		//firstMouse = false;
	//}
	if (xpos > 2220.0f){
		lastX = xpos;
	}
	else if (xpos< 906.0f){
		lastX = xpos;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;


	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}