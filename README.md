# SoftwareRenderer
This is a renderer I'm making as a part of Pikuma's "3D Graphics Programming form Scratch" course.

### Usage
1. Generate
2. Build
3. Move resulting SDL3.dll file to executable directory
4. ???????
5. Profit!

### TODO
- Finish the renderer, duh
- Testing
- Platforms other than Windoze/VS
- Organize static stuff inside Main.cpp
- Build/Install procedure improvements
- OOP where it makes sense
- Make sure color formats are consistent

### FAQ

Q: You seem to have some strong opinions, don't you?\
A: "Strong opinions, weakly held."

Q: Why use C-style casts?\
A: They're more readable, and readability is the #1 most important thing about code. I use C++-style casts when doing something shady, and C-style casts when it's natural code flow.

Q: Why not OOP/TDD/whatever?\
A: I've never done a renderer and I have 0 users, so I'm not sure what its interface should be.

Q: Why not Rust, Zig or some other language?\
A: I don't really care about any programming languages/frameworks/toolchains. The game industry only cares about C++, that's why I'm using C++. If you pay me to use the Brainf\*ck, I'll use Brainf\*ck.

Q: Are you afraid of `if` statements?\
A: You shouldn't have ifs at the domain level unless they're part of the domain logic.

Q: Errors or exceptions?\
A: Neither. In small, non-critical apps, you want your software to crash, preferably giving you some useful information before that. In big (say >100KLOC) apps, you want to report the problem and keep going.

Q: You've enabled GPU acceleration. How's that a software renderer?\
A: I'm building the pipeline in code, using exclusively CPU instructions.
