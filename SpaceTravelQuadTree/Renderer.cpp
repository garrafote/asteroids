#include "Renderer.h"

Renderer::~Renderer()
{
	glfwTerminate();
}

int Renderer::start()
{
	// Initialize the library 
	if (!glfwInit())
		return -1;

	// Create a windowed mode window and its OpenGL context 
	window = glfwCreateWindow(WINDOW_X, WINDOW_Y, "spaceTravelFrustumCulled.cpp", nullptr, nullptr);
	resize(WINDOW_X, WINDOW_Y);
	glfwSetWindowSizeCallback(window, _resizeCallback);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make the window's context current 
	glfwMakeContextCurrent(window);

	// Init GLEW 
	glewInit();

	// initialize the graphics
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glViewport(0, 0, (GLsizei)WINDOW_X, (GLsizei)WINDOW_Y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 250.0);
	glMatrixMode(GL_MODELVIEW);
}

void Renderer::createLine()
{
	// create the line for the middle of the screen
	points[line_index].x = 0;
	points[line_index].y = -5;
	points[line_index].z = -6;
	points[line_index + 1].x = 0;
	points[line_index + 1].y = 5;
	points[line_index + 1].z = -6;
}

void Renderer::createCone()
{
	// create the cone for a spaceship
	glm::vec3 direction(0, 1, 0);
	glm::vec3 apex(0, 10, 0);
	createConeMesh(direction, apex, 10, 5, 10, cone_index);
}

void Renderer::createSphere()
{
	createSphereMesh(SPHERE_SIZE, 0, 0, 0, sphere_index);
}

void Renderer::createBuffers()
{

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object for each circle
	GLuint aBuffer;
	glGenBuffers(1, &aBuffer);
	myBuffer = aBuffer;
	glBindBuffer(GL_ARRAY_BUFFER, myBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// Load shaders and use the resulting shader program
	GLuint program = initShaders("vshader.glsl", "fshader.glsl");
	myShaderProgram = program;
	glUseProgram(myShaderProgram);

	// Initialize the vertex position attribute from the vertex shader
	GLuint loc = glGetAttribLocation(myShaderProgram, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

// function obtained from tutorial at:
// http://www.freemancw.com/2012/06/opengl-cone-function/
// used in drawing a cone
glm::vec3 Renderer::perp(const glm::vec3 &v) {
	float min = fabs(v.x);
	glm::vec3 cardinalAxis(1, 0, 0);

	if (fabs(v.y) < min) {
		min = fabs(v.y);
		cardinalAxis = glm::vec3(0, 1, 0);
	}

	if (fabs(v.z) < min) {
		cardinalAxis = glm::vec3(0, 0, 1);
	}

	return glm::cross(v, cardinalAxis);
}

// function derived from tutorial at:
// http://www.freemancw.com/2012/06/opengl-cone-function/
void Renderer::createConeMesh(const glm::vec3 &d, const glm::vec3 &a,
	const float h, const float rd, const int n, int offset) {
	glm::vec3 c;
	c.x = a.x + (-d.x * h);
	c.y = a.y + (-d.y * h);
	c.z = a.z + (-d.z * h);
	glm::vec3 e0 = perp(d);
	glm::vec3 e1 = glm::cross(e0, d);
	float angInc = 360.0 / n * (PI / 180.0f);

	// calculate points around directrix
	vector <glm::vec3> pts;
	for (int i = 0; i < n; ++i) {
		float rad = angInc * i;
		glm::vec3 p = c + (((e0 * cos(rad)) + (e1 * sin(rad))) * rd);
		pts.push_back(p);
	}

	// draw cone top
	int i = 0;
	int o = offset + i;
	points[o].x = a.x;
	points[o].y = a.y;
	points[o].z = a.z;

	for (i = 1; i < n + 1; ++i) {
		o = i + offset;
		points[o].x = pts[i - 1].x;
		points[o].y = pts[i - 1].y;
		points[o].z = pts[i - 1].z;
	}

	o = i + offset;
	points[o].x = pts[0].x;
	points[o].y = pts[0].y;
	points[o].z = pts[0].z;

	// original tutorial has cone bottom
	// not necessary when cone is a spaceship!
}

// function derived from tutorial at:
// http://www.swiftless.com/tutorials/opengl/sphere.html
void Renderer::createSphereMesh(const float R, const float H, const float K, const float Z, int offset) {
	int n;
	float a;
	float b;
	n = 0;
	const int space = 30;

	// draw the front half
	for (b = 0; b <= 90 - space; b += space){
		for (a = 0; a <= 360 - space; a += space){

			points[n + offset].x = R * sin((a) / 180 * PI) * sin((b) / 180 * PI) - H;
			points[n + offset].y = R * cos((a) / 180 * PI) * sin((b) / 180 * PI) + K;
			points[n + offset].z = R * cos((b) / 180 * PI) - Z;
			n++;

			points[n + offset].x = R * sin((a) / 180 * PI) * sin((b + space) / 180 * PI) - H;
			points[n + offset].y = R * cos((a) / 180 * PI) * sin((b + space) / 180 * PI) + K;
			points[n + offset].z = R * cos((b + space) / 180 * PI) - Z;
			n++;
			points[n + offset].x = R * sin((a + space) / 180 * PI) * sin((b) / 180 * PI) - H;
			points[n + offset].y = R * cos((a + space) / 180 * PI) * sin((b) / 180 * PI) + K;
			points[n + offset].z = R * cos((b) / 180 * PI) - Z;
			n++;
			points[n + offset].x = R * sin((a + space) / 180 * PI) * sin((b + space) / 180 * PI) - H;
			points[n + offset].y = R * cos((a + space) / 180 * PI) * sin((b + space) / 180 * PI) + K;
			points[n + offset].z = R * cos((b + space) / 180 * PI) - Z;
			n++;
		}
	}

	// draw the back half
	for (b = 0; b <= 90 - space; b += space){
		for (a = 0; a <= 360 - space; a += space){

			points[n + offset].x = R * sin((a) / 180 * PI) * sin((b) / 180 * PI) - H;
			points[n + offset].y = R * cos((a) / 180 * PI) * sin((b) / 180 * PI) + K;
			points[n + offset].z = -1 * (R * cos((b) / 180 * PI)) - Z;
			n++;

			points[n + offset].x = R * sin((a) / 180 * PI) * sin((b + space) / 180 * PI) - H;
			points[n + offset].y = R * cos((a) / 180 * PI) * sin((b + space) / 180 * PI) + K;
			points[n + offset].z = -1 * (R * cos((b + space) / 180 * PI)) - Z;
			n++;
			points[n + offset].x = R * sin((a + space) / 180 * PI) * sin((b) / 180 * PI) - H;
			points[n + offset].y = R * cos((a + space) / 180 * PI) * sin((b) / 180 * PI) + K;
			points[n + offset].z = -1 * (R * cos((b) / 180 * PI)) - Z;
			n++;
			points[n + offset].x = R * sin((a + space) / 180 * PI) * sin((b + space) / 180 * PI) - H;
			points[n + offset].y = R * cos((a + space) / 180 * PI) * sin((b + space) / 180 * PI) + K;
			points[n + offset].z = -1 * (R * cos((b + space) / 180 * PI)) - Z;
			n++;
		}
	}

}

GLFWwindow* Renderer::getWindow()
{
	return window;
}

GLuint Renderer::initShaders(const char* vShaderFile, const char* fShaderFile)
{
	struct Shader {
		const char*  filename;
		GLenum       type;
		GLchar*      source;
	}  shaders[2] = {
		{ vShaderFile, GL_VERTEX_SHADER, NULL },
		{ fShaderFile, GL_FRAGMENT_SHADER, NULL }
	};

	GLuint program = glCreateProgram();

	for (int i = 0; i < 2; ++i) {
		Shader& s = shaders[i];
		s.source = readShaderSource(s.filename);
		if (shaders[i].source == NULL) {
			std::cerr << "Failed to read " << s.filename << std::endl;
			exit(EXIT_FAILURE);
		}

		GLuint shader = glCreateShader(s.type);
		glShaderSource(shader, 1, (const GLchar**)&s.source, NULL);
		glCompileShader(shader);

		GLint  compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			std::cerr << s.filename << " failed to compile:" << std::endl;
			GLint  logSize;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
			char* logMsg = new char[logSize];
			glGetShaderInfoLog(shader, logSize, NULL, logMsg);
			std::cerr << logMsg << std::endl;
			delete[] logMsg;

			exit(EXIT_FAILURE);
		}

		delete[] s.source;

		glAttachShader(program, shader);
	}

	/* link  and error check */
	glLinkProgram(program);

	GLint  linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
		std::cerr << "Shader program failed to link" << std::endl;
		GLint  logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetProgramInfoLog(program, logSize, NULL, logMsg);
		std::cerr << logMsg << std::endl;
		delete[] logMsg;

		exit(EXIT_FAILURE);
	}

	/* use program object */
	glUseProgram(program);

	return program;
}

// Create a NULL-terminated string by reading the provided file
char* Renderer::readShaderSource(const char* shaderFile)
{
	//FILE* fp = fopen(shaderFile, "r"); // VS gives warning about being insecure :-)

	// one way to deal with warning: http://stackoverflow.com/questions/2575116/fopen-fopen-s-and-writing-to-files:
	FILE* fp;
#ifdef WIN32
	errno_t err;
	if ((err = fopen_s(&fp, shaderFile, "r")) != 0) {
#else
	if ((fopen_s(&fp, shaderFile, "r")) != 0) {
#endif
		fprintf(stderr, "Cannot open config file %s!\n", shaderFile);
	}

	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);

	buf[size] = '\0';
	fclose(fp);

	return buf;
}

bool Renderer::isDisposed()
{
	return glfwWindowShouldClose(window);
}

void Renderer::_resizeCallback(GLFWwindow* window, int w, int h)
{
	Renderer::getInstance().resize(w, h);
}

void Renderer::draw(Asteroid** asteroids, float x, float z, float angle)
{
	int i, j;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the buffer and shader for each circle.
	glUseProgram(myShaderProgram);
	glBindBuffer(GL_ARRAY_BUFFER, myBuffer);

	// Initialize the vertex position attribute from the vertex shader.
	GLuint loc = glGetAttribLocation(myShaderProgram, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// copy over global points array in case anything has changed
	// very slow when points is just HUGE
	glBindBuffer(GL_ARRAY_BUFFER, myBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// Begin left viewport.
	glViewport(0, 0, width / 2.0, height);
	glLoadIdentity();

	// Write text in isolated (i.e., before gluLookAt) translate block.
	// DOES NOT WORK WITHOUT GLUT 
	//glPushMatrix();
	//glColor3f(1.0, 1.0, 1.0);
	//glRasterPos3f(5.0, 25.0, -30.0);
	//if (isFrustumCulled) writeBitmapString((void*)font, "Frustum culling on!");
	//else writeBitmapString((void*)font, "Frustum culling off!");
	//glColor3f(1.0, 0.0, 0.0);
	//glRasterPos3f(-28.0, 25.0, -30.0);
	//if (isCollision) writeBitmapString((void*)font, "Cannot - will crash!");
	//glPopMatrix();

	// Fixed camera 
	lookAt(0.0, 10.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//if (!isFrustumCulled)
	//{
		// Draw all the asteroids in arrayAsteroids.
		for (i = 0; i < ROWS; i++)
			for (j = 0; j < COLUMNS; j++)
				asteroids[i][j].draw();
	//}
	//else
	//{
		// Draw only asteroids in leaf squares of the quadtree that intersect the fixed frustum
		// with apex at the origin.
		//asteroidsQuadtree.drawAsteroids(-5.0, -5.0, -250.0, -250.0, 250.0, -250.0, 5.0, -5.0);
	//}

	// off is white spaceship and on it red
	//if (isFrustumCulled)
		glColor3f(1.0, 0.0, 0.0);
	//else
	//	glColor3f(1.0, 1.0, 1.0);

	// spacecraft moves and so we translate/rotate according to the movement
	glPushMatrix();
	glTranslatef(x, 0, z);
	glRotatef(angle, 0.0, 1.0, 0.0);

	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0); // To make the spacecraft point down the $z$-axis initially.

	// Turn on wireframe mode
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLE_FAN, cone_index, CONE_VERTEX_COUNT);
	// Turn off wireframe mode
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	glPopMatrix();
	// End left viewport.

	// Begin right viewport.
	glViewport(width / 2.0, 0, width / 2.0, height);
	glLoadIdentity();

	// Write text in isolated (i.e., before gluLookAt) translate block.
	// DOES NOT WORK WITHOUT GLUT
	//glPushMatrix();
	//glColor3f(1.0, 1.0, 1.0);
	//glRasterPos3f(5.0, 25.0, -30.0);
	//if (isFrustumCulled)  writeBitmapString((void*)font, "Frustum culling on.");
	//else writeBitmapString((void*)font, "Frustum culling off.");
	//glColor3f(1.0, 0.0, 0.0);
	//glRasterPos3f(-28.0, 25.0, -30.0);
	//if (isCollision)  writeBitmapString((void*)font, "Cannot - will crash!");
	//glPopMatrix();

	// draw the line in the middle to separate the two viewports
	glPushMatrix();
	glTranslatef(-6, 0, 0);
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(2.0);
	glDrawArrays(GL_LINE_STRIP, line_index, LINE_VERTEX_COUNT);
	glLineWidth(1.0);
	glPopMatrix();

	// Locate the camera at the tip of the cone and pointing in the direction of the cone.
	lookAt(x - 10 * sin((PI / 180.0) * angle),
		0.0,
		z - 10 * cos((PI / 180.0) * angle),
		x - 11 * sin((PI / 180.0) * angle),
		0.0,
		z - 11 * cos((PI / 180.0) * angle),
		0.0,
		1.0,
		0.0);

	//if (!isFrustumCulled)
	//{
		// Draw all the asteroids in arrayAsteroids.
		for (j = 0; j < COLUMNS; j++)
			for (i = 0; i < ROWS; i++)
				asteroids[i][j].draw();
	//}
	//else
	//{
		// Draw only asteroids in leaf squares of the quadtree that intersect the frustum
		// "carried" by the spacecraft with apex at its tip and oriented with its axis
		// along the spacecraft's axis.
		//asteroidsQuadtree.drawAsteroids(xVal - 7.072 * sin((PI / 180.0) * (45.0 + angle)),
		// zVal - 7.072 * cos((PI / 180.0) * (45.0 + angle)),
		// xVal - 353.6 * sin((PI / 180.0) * (45.0 + angle)),
		// zVal - 353.6 * cos((PI / 180.0) * (45.0 + angle)),
		// xVal + 353.6 * sin((PI / 180.0) * (45.0 - angle)),
		// zVal - 353.6 * cos((PI / 180.0) * (45.0 - angle)),
		// xVal + 7.072 * sin((PI / 180.0) * (45.0 - angle)),
		// zVal - 7.072 * cos((PI / 180.0) * (45.0 - angle))
		// );
	//}
	// End right viewport.

	glfwSwapBuffers(window);

	// Poll for and process events 
	glfwPollEvents();
}

void Renderer::drawSphere(float x, float y, float z, unsigned char* color) 
{
	glPushMatrix();
	glTranslatef(x, y, z);
	//glScalef(0.0125 * radius, 0.0125 * radius, 0.0125 * radius);
	glColor3ubv(color);

	// Turn on wireframe mode
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);

	// create the sphere and place it in a global array
	//CreateSphere(SPHERE_SIZE, 0, 0, 0, index);

	// draw sphere
	glDrawArrays(GL_TRIANGLE_FAN, sphere_index, SPHERE_VERTEX_COUNT);

	// Turn off wireframe mode
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);

	glPopMatrix();
}

void Renderer::lookAt(
	float eyex,		float eyey,		float eyez, 
	float centerx,	float centery,	float centerz,
	float upx,		float upy,		float upz)
{
	int i;
	glm::vec3 forward, side, up;
	float m[4][4];

	// create identity matrix
	for (int i = 0; i < 4; i++) {
		m[i][0] = 0;
		m[i][1] = 0;
		m[i][2] = 0;
		m[i][3] = 0;
	}

	m[0][0] = 1.0f;
	m[1][1] = 1.0f;
	m[2][2] = 1.0f;
	m[3][3] = 1.0f;

	// glu code for lookat
	forward.x = centerx - eyex;
	forward.y = centery - eyey;
	forward.z = centerz - eyez;

	up.x = upx;
	up.y = upy;
	up.z = upz;

	forward = glm::normalize(forward);

	// Side = forward x up 
	side = glm::cross(forward, up);
	side = glm::normalize(side);

	// Recompute up as: up = side x forward 
	up = glm::cross(side, forward);

	m[0][0] = side[0];
	m[1][0] = side[1];
	m[2][0] = side[2];

	m[0][1] = up[0];
	m[1][1] = up[1];
	m[2][1] = up[2];

	m[0][2] = -forward[0];
	m[1][2] = -forward[1];
	m[2][2] = -forward[2];

	glMultMatrixf((const GLfloat *)m[0]);
	glTranslated(-eyex, -eyey, -eyez);
}


void Renderer::resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 250.0);
	glMatrixMode(GL_MODELVIEW);

	// Pass the size of the OpenGL window.
	width = w;
	height = h;
}