// TODO: Organize these when they become stable!

int screenWidth = 0;
int screenHeight = 0;
int screenWidthBytes = 0;
int screenHeightBytes = 0;

const char* title = "SoftwareRenderer";
static bool quit = false;

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