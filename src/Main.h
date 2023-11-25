#include <stdint.h>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

// TODO: Organize these when they become stable!

constexpr int screenWidth = 1024;
constexpr int screenWidthBytes = screenWidth * sizeof(uint32_t);
constexpr int screenHeight = 768;
constexpr int screenHeightBytes = screenHeight * sizeof(uint32_t);
const char* title = "SoftwareRenderer";
static SDL_Window* window;
static SDL_Renderer* renderer;
static bool quit = false;
static uint32_t* colorBuffer = nullptr;
static SDL_Texture* colorBufferTexture = nullptr;

static inline bool ShouldQuit()
{
	return quit;
}

static inline void Quit()
{
	quit = true;
}

static void Init(void);
static void Deinit(void);

static void Setup(void);
static void HandleInput(void);
static void Update(void);
static void Draw(void);