#include "Renderer.h"
#include "Rasterizer.h"
Renderer::Renderer(const char* title, int width, int height)
	: mWidth(width)
	, mHeight(height)
	, mTicksCount(0)
	, mIsRunning(false)
	, pixelBuffer(width, height)
	, depthBuffer(width, height)
	, camera(vec3(0, 0, 2), vec3(0, 0, 0), vec3(0, 1, 0))
	, shader()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		exit(1);
	}
	mWindow = SDL_CreateWindow(title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		mWidth,
		mHeight,
		SDL_WINDOW_OPENGL
	);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		exit(1);
	}
	mSurface = SDL_GetWindowSurface(mWindow);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_ShowCursor(SDL_ENABLE);
}

Renderer::~Renderer()
{
	for (auto& mesh : meshes) {
		delete mesh;
	}
	meshes.clear();
}

void Renderer::Run()
{
	mIsRunning = true;
	Init();
	while (mIsRunning) {
		ProcessInput();
		auto delta = GetDelta();
		Update();
		Clear();
		Draw();
		Display();
	}
	ShutDown();
}

void Renderer::Display() {
	SDL_LockSurface(mSurface);
	memcpy(mSurface->pixels, pixelBuffer.GetPixels(), pixelBuffer.GetHeight() * pixelBuffer.GetWidth() * sizeof(Uint32));
	SDL_UnlockSurface(mSurface);
	SDL_UpdateWindowSurface(mWindow);
}

void Renderer::Init()
{
	auto head = new Mesh("Assets/african_head", "african_head");
	meshes.push_back(head);

	auto eyeInner = new Mesh("Assets/african_head", "african_head_eye_inner");
	meshes.push_back(eyeInner);

	/*auto eyeOuter = new Mesh("Assets/african_head", "african_head_eye_outer");
	meshes.push_back(eyeOuter);*/

}


void Renderer::ShutDown()
{
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Renderer::Update()
{
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 proj = camera.GetProjectionMatrix();
	auto mv = view * model;
	shader.MVP = proj * view * model;
	vec3 lightPos = view * glm::vec4(0, 0, 1, 0);
	shader.lightPos = lightPos;
	shader.MV = mv;
	shader.N = glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2]));
}

float Renderer::GetDelta()
{
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
	//	;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	//if (deltaTime > 0.05f)
	//{
	//	deltaTime = 0.05f;
	//}
	std::cout << deltaTime << std::endl;
	mTicksCount = SDL_GetTicks();
	return deltaTime;
}



void Renderer::Draw() {
	vec3 vertices[3];
	vec3 normals[3];
	vec2 uvs[3];
	vec3 faceNormal;
	vec4 clipCoords[3];
	
	for (auto& mesh : meshes) {
		shader.diffuseTexture =  mesh->GetDiffuse();
		shader.specTexture = mesh->GetSpecular();
		for (int i = 0; i < mesh->GetFaceNumbers(); i++) {
			
			for (int j = 0; j < 3; j++) {
				vertices[j] = mesh->vertice(i, j);
				uvs[j] = mesh->uv(i, j);
				normals[j] = mesh->normal(i, j);
			}
			for (int j = 0; j < 3; j++) {
				clipCoords[j] = shader.Vertex(vertices[j], normals[j], uvs[j], j);
			}

			if (Rasterizer::Clipping(clipCoords)) continue;

			Rasterizer::PerspectiveDivide(clipCoords);
			Rasterizer::ViewPortTransform(pixelBuffer, clipCoords);
			if (Rasterizer::FaceCulling(clipCoords)) continue;
			Rasterizer::Rasterize(pixelBuffer, depthBuffer, &shader, clipCoords);
		}
	}
}

void Renderer::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		case SDL_MOUSEMOTION:
			if (!mouseFirst) {
				camera.OnMouseMove(event.motion.xrel, event.motion.yrel);
			}
			else {
				mouseFirst = false;
				camera.OnMouseMove(0, 0);
			}
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
}

void Renderer::Clear()
{
	pixelBuffer.Clear();
	depthBuffer.Clear();
}



