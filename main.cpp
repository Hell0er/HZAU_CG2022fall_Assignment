#include "misc.h"
#include "FBO.h"
#include "Param.h"
#include <vector>

#define DEBUG
#ifdef DEBUG
#pragma comment(lib, "soil2-debug.lib")
#pragma comment(lib, "glew32d.lib")
#pragma comment(lib, "freeglut_staticd.lib")
#pragma comment(lib, "freeglutd.lib")
#else
#pragma comment(lib, "soil2.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "freeglut_static.lib")
#pragma comment(lib, "freeglut.lib")
#endif

FrameBufferObject *mFbo;

bool bCameraRotate = false;	   //  摄像机是否允许旋转
float cameraXZRotateAngle = 0; // 摄像机xz平面内的旋转角度 即左右旋转
float cameraYRotateAngle = 0;  //  摄像机垂直平面内的旋转角度   上下旋转

int lastMousePosX, lastMousePosY; //  上次鼠标的位置

// 绘制的函数原型申明
void DrawItem();	  // 绘制物品
void DrawShadow();	  // 绘制阴影
void DrawSunShadow(); // 绘制太阳阴影
void DrawSun();		  // 新太阳

// 光照开关
int lightOn = 1; // 是否开启光照

int worldtime = 0;		 // 虚拟时间，控制太阳和时钟，单位s
#define DAY_SECOND 86400 // 一天秒数
float sunlight = 50.0;
float sunx = 0, suny = 0, sunz = 0; // 光源位置

/**
 * @方法:读取模型
 * @说明: VertexData: obj模型的全部顶点数据
 *        Vbo:顶点缓冲对象,存储顶点的各类属性信息
 *        IBO:索引缓冲对象,存储顶点索引,减少传输顶点数据,提高效率
 */
void readModel()
{
	// wall
	wallVertexData = LoadObjModel("res/wall.obj", &wallIndexes, wallVertexCount, wallIndexCount);
	wallVbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * wallVertexCount, GL_STATIC_DRAW, wallVertexData);
	wallIbo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * wallIndexCount, GL_STATIC_DRAW, wallIndexes);
	walls.push_back(CreateTexture("res/wall.png"));
	wallsSize = walls.size();

	// floor
	floorVertexData = LoadObjModel("res/floor.obj", &floorIndexes, floorVertexCount, floorIndexCount);
	floorVbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * floorVertexCount, GL_STATIC_DRAW, floorVertexData);
	floorIbo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * floorIndexCount, GL_STATIC_DRAW, floorIndexes);
	floors.push_back(CreateTexture("res/floor.png"));
	floorsSize = floors.size();

	// desk
	deskVertexData = LoadObjModel("res/desk.obj", &deskIndexes, deskVertexCount, deskIndexCount);
	deskVbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * deskVertexCount, GL_STATIC_DRAW, deskVertexData);
	deskIbo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * deskIndexCount, GL_STATIC_DRAW, deskIndexes);
	desks.push_back(CreateTexture("res/desk.png"));
	desksSize = desks.size();

	// lamp
	lampVertexData = LoadObjModel("res/lamp.obj", &lampIndexes, lampVertexCount, lampIndexCount);
	lampVbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * lampVertexCount, GL_STATIC_DRAW, lampVertexData);
	lampIbo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * lampIndexCount, GL_STATIC_DRAW, lampIndexes);
	lamps.push_back(CreateTexture("res/lamp.png"));
	lampsSize = lamps.size();

	// flower
	flowerVertexData = LoadObjModel("res/flower.obj", &flowerIndexes, flowerVertexCount, flowerIndexCount);
	flowerVbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * flowerVertexCount, GL_STATIC_DRAW, flowerVertexData);
	flowerIbo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * flowerIndexCount, GL_STATIC_DRAW, flowerIndexes);
	flowers.push_back(CreateTexture("res/flower.png"));
	flowersSize = flowers.size();

	// painting1
	painting1VertexData = LoadObjModel("res/painting1.obj", &painting1Indexes, painting1VertexCount, painting1IndexCount);
	painting1Vbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * painting1VertexCount, GL_STATIC_DRAW, painting1VertexData);
	painting1Ibo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * painting1IndexCount, GL_STATIC_DRAW, painting1Indexes);
	painting1s.push_back(CreateTexture("res/painting1.png"));
	painting1sSize = painting1s.size();

	// painting2
	painting2VertexData = LoadObjModel("res/painting2.obj", &painting2Indexes, painting2VertexCount, painting2IndexCount);
	painting2Vbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * painting2VertexCount, GL_STATIC_DRAW, painting2VertexData);
	painting2Ibo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * painting2IndexCount, GL_STATIC_DRAW, painting2Indexes);
	painting2s.push_back(CreateTexture("res/painting2.png"));
	painting2sSize = painting2s.size();

	// plant
	plantVertexData = LoadObjModel("res/plant.obj", &plantIndexes, plantVertexCount, plantIndexCount);
	plantVbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * plantVertexCount, GL_STATIC_DRAW, plantVertexData);
	plantIbo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * plantIndexCount, GL_STATIC_DRAW, plantIndexes);
	plants.push_back(CreateTexture("res/plant.png"));
	plantsSize = plants.size();

	// cat
	catVertexData = LoadObjModel("res/cat.obj", &catIndexes, catVertexCount, catIndexCount);
	catVbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * catVertexCount, GL_STATIC_DRAW, catVertexData);
	catIbo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * catIndexCount, GL_STATIC_DRAW, catIndexes);
	cats.push_back(CreateTexture("res/cat.png"));
	catsSize = cats.size();

	// sun
	sunVertexData = LoadObjModel("res/sun.obj", &sunIndexes, sunVertexCount, sunIndexCount);
	sunVbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * sunVertexCount, GL_STATIC_DRAW, sunVertexData);
	sunIbo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * sunIndexCount, GL_STATIC_DRAW, sunIndexes);
	suns.push_back(CreateTexture("res/sun.png"));
	sunsSize = suns.size();

	// table
	tableVertexData = LoadObjModel("res/table.obj", &tableIndexes, tableVertexCount, tableIndexCount);
	tableVbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * tableVertexCount, GL_STATIC_DRAW, tableVertexData);
	tableIbo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * tableIndexCount, GL_STATIC_DRAW, tableIndexes);
	tables.push_back(CreateTexture("res/table.png"));
	tablesSize = tables.size();

	// balloon
	balloonVertexData = LoadObjModel("res/balloon.obj", &balloonIndexes, balloonVertexCount, balloonIndexCount);
	balloonVbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * balloonVertexCount, GL_STATIC_DRAW, balloonVertexData);
	balloonIbo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * balloonIndexCount, GL_STATIC_DRAW, balloonIndexes);
	balloons.push_back(CreateTexture("res/balloon.png"));
	balloonsSize = balloons.size();
}

void init()
{
	// 读取模型
	readModel();

	// 初始化相机
	cameraPos = glm::vec3(0.0f, 10.0f, 40.0f);	 // 初始化摄像机的位置(x,y,z)
	cameraTarget = glm::vec3(0.0f, 10.0f, 0.0f); // 摄像机目标
	up = glm::vec3(0.0f, 1.0f, 0.0f);			 // 定义上向量

	cameraDirection = glm::normalize(cameraTarget - cameraPos);	   // 摄像机朝向
	cameraRight = glm::normalize(glm::cross(up, cameraDirection)); // 右轴
	cameraUp = glm::cross(cameraDirection, cameraRight);		   // 上轴
	cameraSpeed = 3.0f;											   // 设置摄像机速度

	viewMatrix = glm::lookAt(cameraPos, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));

	// 前两个参数指定了平截头体的左右坐标，第三和第四参数指定了平截头体的底部和顶部
	// 通过这四个参数我们定义了近平面和远平面的大小
	// 第五和第六个参数则定义了近平面和远平面的距离
	lightProjectionMatrix = glm::ortho(-520.0f, 520.0f, -520.0f, 520.0f, 0.1f, 1000.0f);
	// 第一个参数为视锥上下面之间的夹角，第二个参数为视窗的宽/高，第三第四个参数分别为近截面和远界面的深度
	lightViewMatrix = glm::lookAt(glm::vec3(150.0f, 150.0f, 100.0f), glm::vec3(0.0f, 0.0f, -50.0f), glm::vec3(0.0f, 0.0f, -1.0f));

	// 加载Vertex和Fragment渲染器
	renderProgram = CreateGPUProgram("res/light.vs", "res/light.fs");
	depthProgram = CreateGPUProgram("res/shadow.vs", "res/shadow.fs");

	// 向fs和vs传递参数
	renderPosLoc = 0;
	renderTexcoordLoc = 1;
	renderNormalLoc = 2;
	renderMLoc = glGetUniformLocation(renderProgram, "M");
	renderVLoc = glGetUniformLocation(renderProgram, "V");
	renderPLoc = glGetUniformLocation(renderProgram, "P");
	renderNMLoc = glGetUniformLocation(renderProgram, "NM");
	renderViewPosLocation = glGetUniformLocation(renderProgram, "ViewPos");
	renderTextureLoc = glGetUniformLocation(renderProgram, "U_MainTexture");
	shadowMapLoc = glGetUniformLocation(renderProgram, "U_ShadowMap");
	renderLightVMatrixLoc = glGetUniformLocation(renderProgram, "LightViewMatrix");
	renderLightPMatrixLoc = glGetUniformLocation(renderProgram, "LightProjectionMatrix");
	renderLightOnLoc = glGetUniformLocation(renderProgram, "lightOn");
	rendersunlight = glGetUniformLocation(renderProgram, "sunlight");
	rendersunx = glGetUniformLocation(renderProgram, "sunx");
	rendersuny = glGetUniformLocation(renderProgram, "suny");
	rendersunz = glGetUniformLocation(renderProgram, "sunz");

	depthPosLoc = 0;
	depthTexcoordLoc = 1;
	depthNormalLoc = 2;
	depthMLoc = glGetUniformLocation(depthProgram, "M");
	depthVLoc = glGetUniformLocation(depthProgram, "V");
	depthPLoc = glGetUniformLocation(depthProgram, "P");

	glEnable(GL_DEPTH_TEST);

	mFbo = new FrameBufferObject;
	mFbo->AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, 800, 600);
	mFbo->AttachDepthBuffer("depth", 800, 600);
	mFbo->Finish();

	mFbo->Bind();
	glClear(GL_DEPTH_BUFFER_BIT);
	DrawShadow();
	mFbo->Unbind();

	shadowMap = mFbo->GetBuffer("depth");

	glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
}

void display()
{
	mFbo->Bind();
	glClear(GL_DEPTH_BUFFER_BIT);
	DrawShadow(); // 绘制阴影
	DrawSunShadow();

	mFbo->Unbind();
	shadowMap = mFbo->GetBuffer("depth");

	glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawSun();	// 更新太阳
	DrawItem(); //  绘制物品

	glutSwapBuffers();
	glutPostRedisplay();
}

void keyFunc(GLubyte key, int x, int y)
{
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	switch (key)
	{
	case 'w':
	case 'W': // 前移
		cameraPos += cameraSpeed * cameraDirection;
		cameraTarget += cameraSpeed * cameraDirection;
		break;
	case 's':
	case 'S': // 后移
		cameraPos -= cameraSpeed * cameraDirection;
		cameraTarget -= cameraSpeed * cameraDirection;
		break;
	case 'a':
	case 'A': // 左移
		cameraPos += cameraSpeed * cameraRight;
		cameraTarget += cameraSpeed * cameraRight;
		break;
	case 'd':
	case 'D': // 右移
		cameraPos -= cameraSpeed * cameraRight;
		cameraTarget -= cameraSpeed * cameraRight;
		break;
	case ' ': // 飞天
		cameraPos += cameraSpeed * up;
		cameraTarget += cameraSpeed * up;
		break;
	case GLUT_KEY_SHIFT_L: // 遁地
		cameraPos -= cameraSpeed * up;
		cameraTarget -= cameraSpeed * up;
		break;
	case 'i':
	case 'I': // 时间加速10min
		worldtime += 600;
		worldtime %= DAY_SECOND;
		break;
	case 'o':
	case 'O': // 时间倒流10min
		worldtime -= 600;
		worldtime %= DAY_SECOND;
		break;
	case 'l':
	case 'L':
		lightOn = lightOn == 1 ? 0 : 1;
		break;
	}
	viewMatrix = glm::lookAt(cameraPos, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));
	lightViewMatrix = glm::lookAt(glm::vec3(150.0f, 150.0f, 100.0f), glm::vec3(0.0f, 0.0f, -50.0f) + glm::vec3(sunx, suny, sunz), glm::vec3(0.0f, 0.0f, -1.0f));
}

// 鼠标左键旋转视角操作函数
void MouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		bCameraRotate = true;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		bCameraRotate = false;
	}
}

// 视角缩放函数
void MouseWheelFunc(int wheel, int direction, int x, int y)
{
	if (direction > 0) // 缩进
	{
		fov += 1.0f;
	}
	else
	{ // 缩放
		fov -= 1.0f;
	}
}

void MotionFunc(int x, int y) // 鼠标移动函数，  右键摁下移动即摆动摄像头
{
	y = 600 - y;
	if (bCameraRotate)
	{
		if (x > lastMousePosX)
		{
			cameraXZRotateAngle += 0.02f;
			lastMousePosX = x;
		}
		else if (x < lastMousePosX)
		{
			cameraXZRotateAngle -= 0.02f;
			lastMousePosX = x;
		}
		if (y > lastMousePosY)
		{
			cameraYRotateAngle += 0.02f;
			lastMousePosY = y;
		}
		else if (y < lastMousePosY)
		{
			cameraYRotateAngle -= 0.02f;
			lastMousePosY = y;
		}
	}
	cameraDirection.x = sin(cameraXZRotateAngle);
	cameraDirection.z = -cos(cameraXZRotateAngle);
	cameraDirection.y = sin(cameraYRotateAngle);
	cameraDirection = glm::normalize(cameraDirection);
	cameraTarget = cameraPos + cameraDirection;
	viewMatrix = glm::lookAt(cameraPos, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("CG Assignment");
	glewInit();

	init();
	glutDisplayFunc(display); // 设置显示回调函数 -- 每帧执行
	glutKeyboardFunc(keyFunc);
	glutMouseFunc(MouseFunc);
	glutMouseWheelFunc(MouseWheelFunc);
	glutMotionFunc(MotionFunc);
	printf("------------Base Operation ------------\n");
	printf("# Key WASD control move，Key SPACE control fly, Key left-SHIFT control dive.\n");
	printf("# Mouse LEFT control angle of view, Mouse WHEEL controls view width.\n");
	printf("------------Light Operation------------\n");
	printf("# Key L open/close light.\n");
	printf("# Key I accelerate 10 minutes, Key O turn back 10minutes.\n");
	printf("---------------------------------------\n");

	glutMainLoop();
	return 0;
}

/*
 * @方法：绘制一个物体
 * @说明：传入初始方法中生成的vbo、ibo、indexcount来绘制单个物体
 */
void drawOne(GLuint vbo, GLuint ibo, int indexCount)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // 绑定vbo
	glVertexAttribPointer(renderPosLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)0);
	glVertexAttribPointer(renderTexcoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 3));
	glVertexAttribPointer(renderNormalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 5));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // 利用ibo中的index绘制图形
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void drawOneShadow(GLuint vbo, GLuint ibo, int indexCount)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(depthPosLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)0);
	glVertexAttribPointer(depthTexcoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 3));
	glVertexAttribPointer(depthNormalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(float) * 5));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // 利用ibo中的index绘制图形
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void DrawItem()
{
	//--------------------------------------------------------------------------------------------------------------
	// 准备绘制
	//--------------------------------------------------------------------------------------------------------------
	modelMatrix = glm::scale(2.0f, 2.0f, 2.0f);
	normalMatrix = glm::inverseTranspose(modelMatrix); // 模型矩阵，控制平移、旋转、缩放

	glUseProgram(renderProgram);
	// glUniformMatrix4fv想着色器程序传值，第一个参数是位置，2、3不变，第四个就是你要传的矩阵
	glUniformMatrix4fv(renderMLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix)); // 向gpu program中传值
	glUniformMatrix4fv(renderVLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));  // 这些参数都在47行赋值
	projectionMatrix = glm::perspective(fov, 800.0f / 600.0f, 0.1f, 800.0f);  // 投影矩阵，因滚轮修改了fov参数，所以需要实时传递矩阵给着色器程序
	glUniformMatrix4fv(renderPLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(renderNMLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glUniform3fv(renderViewPosLocation, 1, glm::value_ptr(cameraPos));
	// glUniform1i设置每个采样器的方式单纯告诉OpenGL每个着色器采样器属于哪个纹理单元。纹理单元总共有16个
	glUniformMatrix4fv(renderLightPMatrixLoc, 1, GL_FALSE, glm::value_ptr(lightProjectionMatrix));
	glUniformMatrix4fv(renderLightVMatrixLoc, 1, GL_FALSE, glm::value_ptr(lightViewMatrix));
	glUniform1i(renderLightOnLoc, lightOn);

	// 光源位置传值
	glUniform1f(rendersunlight, sunlight);
	glUniform1f(rendersunx, sunx);
	glUniform1f(rendersuny, suny);
	glUniform1f(rendersunz, sunz);

	// 绑定纹理单元
	glBindTextureUnit(1, shadowMap);

	glEnableVertexAttribArray(renderPosLoc);
	glEnableVertexAttribArray(renderTexcoordLoc);
	glEnableVertexAttribArray(renderNormalLoc);
	//--------------------------------------------------------------------------------------------------------------
	// 开始绘制
	//--------------------------------------------------------------------------------------------------------------
	glBindTextureUnit(0, floors[floorTexture]);
	drawOne(floorVbo, floorIbo, floorIndexCount);

	glBindTextureUnit(0, walls[wallTexture]);
	drawOne(wallVbo, wallIbo, wallIndexCount);

	glBindTextureUnit(0, lamps[lampTexture]);
	drawOne(lampVbo, lampIbo, lampIndexCount);

	glBindTextureUnit(0, tables[tableTexture]);
	drawOne(tableVbo, tableIbo, tableIndexCount);

	glBindTextureUnit(0, desks[deskTexture]);
	drawOne(deskVbo, deskIbo, deskIndexCount);

	glBindTextureUnit(0, balloons[balloonTexture]);
	drawOne(balloonVbo, balloonIbo, balloonIndexCount);

	glBindTextureUnit(0, lamps[lampTexture]);
	drawOne(balloonVbo, lampIbo, lampIndexCount);

	glBindTextureUnit(0, flowers[flowerTexture]);
	drawOne(flowerVbo, flowerIbo, flowerIndexCount);

	glBindTextureUnit(0, plants[plantTexture]);
	drawOne(plantVbo, plantIbo, plantIndexCount);

	glBindTextureUnit(0, cats[catTexture]);
	drawOne(catVbo, catIbo, catIndexCount);

	glBindTextureUnit(0, painting1s[painting1Texture]);
	drawOne(painting1Vbo, painting1Ibo, painting1IndexCount);

	glBindTextureUnit(0, painting2s[painting2Texture]);
	drawOne(painting2Vbo, painting2Ibo, painting2IndexCount);

	glUseProgram(0);
}

glm::vec3 getSunPosition()
{
	glm::vec3 ret;
	int step = int(worldtime * (100.0f * 4.0f / DAY_SECOND)) % 100;
	sunx = 20.0f;
	if (worldtime < DAY_SECOND / 4)
	{
		suny = -100.0f + step;
		sunz = step;
	}
	else if (worldtime < DAY_SECOND / 2)
	{
		suny = step;
		sunz = 100.0f - step;
	}
	else if (worldtime < DAY_SECOND / 4 * 3)
	{
		suny = 100.0f - step;
		sunz = -step;
	}
	else
	{
		suny = -step;
		sunz = step - 100.0f;
	}
	return glm::vec3(sunx, suny, sunz);
}

void DrawSun()
{
	//--------------------------------------------------------------------------------------------------------------
	// 准备绘制
	//--------------------------------------------------------------------------------------------------------------
	modelMatrix = glm::scale(2.0f, 2.0f, 2.0f);
	modelMatrix = glm::translate(modelMatrix, getSunPosition());
	normalMatrix = glm::inverseTranspose(modelMatrix); // 模型矩阵，控制平移、旋转、缩放

	glUseProgram(renderProgram);
	glUniformMatrix4fv(renderMLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(renderVLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	projectionMatrix = glm::perspective(fov, 800.0f / 600.0f, 0.1f, 800.0f);
	glUniformMatrix4fv(renderPLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(renderNMLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glUniform3fv(renderViewPosLocation, 1, glm::value_ptr(cameraPos));
	glUniformMatrix4fv(renderLightPMatrixLoc, 1, GL_FALSE, glm::value_ptr(lightProjectionMatrix));
	glUniformMatrix4fv(renderLightVMatrixLoc, 1, GL_FALSE, glm::value_ptr(lightViewMatrix));
	glUniform1i(renderLightOnLoc, lightOn);
	glUniform1f(rendersunlight, sunlight);
	glUniform1f(rendersunx, sunx);
	glUniform1f(rendersuny, suny);
	glUniform1f(rendersunz, sunz);

	glBindTextureUnit(1, shadowMap);

	glEnableVertexAttribArray(renderPosLoc);
	glEnableVertexAttribArray(renderTexcoordLoc);
	glEnableVertexAttribArray(renderNormalLoc);
	//--------------------------------------------------------------------------------------------------------------
	// 开始绘制
	//--------------------------------------------------------------------------------------------------------------
	glBindTextureUnit(0, suns[sunTexture]);
	drawOne(sunVbo, sunIbo, sunIndexCount);

	glUseProgram(0);
}

void DrawShadow()
{
	modelMatrix = glm::scale(2.0f, 2.0f, 2.0f);
	glUseProgram(depthProgram);
	glUniformMatrix4fv(depthMLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(depthVLoc, 1, GL_FALSE, glm::value_ptr(lightViewMatrix));
	glUniformMatrix4fv(depthPLoc, 1, GL_FALSE, glm::value_ptr(lightProjectionMatrix));

	glEnableVertexAttribArray(depthPosLoc);
	glEnableVertexAttribArray(depthTexcoordLoc);
	glEnableVertexAttribArray(depthNormalLoc);

	//--------------------------------------------------------------------------------------------------------------
	// 开始绘制
	//--------------------------------------------------------------------------------------------------------------
	drawOneShadow(tableVbo, tableIbo, tableIndexCount);
	drawOneShadow(floorVbo, floorIbo, floorIndexCount);
	drawOneShadow(wallVbo, wallIbo, wallIndexCount);
	drawOneShadow(deskVbo, deskIbo, deskIndexCount);
	drawOneShadow(balloonVbo, balloonIbo, balloonIndexCount);
	drawOneShadow(tableVbo, tableIbo, tableIndexCount);
	drawOneShadow(lampVbo, lampIbo, lampIndexCount);
	drawOneShadow(flowerVbo, flowerIbo, flowerIndexCount);
	drawOneShadow(painting1Vbo, painting1Ibo, painting1IndexCount);
	drawOneShadow(painting2Vbo, painting2Ibo, painting2IndexCount);
	drawOneShadow(plantVbo, plantIbo, plantIndexCount);
	drawOneShadow(catVbo, catIbo, catIndexCount);

	glUseProgram(0);
}

void DrawSunShadow()
{
	modelMatrix = glm::scale(2.0f, 2.0f, 2.0f);
	modelMatrix = glm::translate(modelMatrix, getSunPosition());
	glUseProgram(depthProgram);
	glUniformMatrix4fv(depthMLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(depthVLoc, 1, GL_FALSE, glm::value_ptr(lightViewMatrix));
	glUniformMatrix4fv(depthPLoc, 1, GL_FALSE, glm::value_ptr(lightProjectionMatrix));

	glEnableVertexAttribArray(depthPosLoc);
	glEnableVertexAttribArray(depthTexcoordLoc);
	glEnableVertexAttribArray(depthNormalLoc);

	drawOneShadow(sunVbo, sunIbo, sunIndexCount);

	glUseProgram(0);
}
