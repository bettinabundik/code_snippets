# code_snippets
Individual files for my code snippets from bigger projects

--- Marker.h, Marker.cpp
- Topic: Computer vision, Image and video processing and analysis
- Problem: Follow or approximate the movements of a human face on every frame of a 2D input video using coloured, circular markers placed on characteristic locations of the face (eyes, forehead, cheeks, mouth)
- Solution: After validating input data, execute a first detection of markers on the first few frames of an input video, and given these detected marker locations, execute an optical flow algorithm for the next input frame, correct results if necessary by checking colours of the pixels or executing detection algorithm again if some points were lost (number of markers on a face is previously given)
- Using C++17 and OpenCV 3.2.0
- Additional runtime files: opencv_world320.dll, opencv_ffmpeg320_64.dll, xml configuration files


--- Program.cs, KnightsModel.cs
- Problem: Solve the knight's tour (Hamiltonian-path problem, NP-Hard) by generating a DIMACS format of CNF file list based on the knight's tour constraint and use a SAT solver to solve the CNF list
- Solution: Define 4 constraints: 1) At time i, the knight is at one square only and not at two or more positions at the same time. This constraint will result in a list of 2-CNF variables.   2) Considering the knight’s valid movements on the chessboard, there are maximum 8 options. If the knight at time i is at position (j,k), then at time i+1, its next position is decided according to these 8 possible moves. This constraint can generate a list of variables of 3-CNF to 9-CNF.   3) At each time, every square is visited only once, meaning that we need to iterate the time in this case, not the position. This will result in a list of 2-CNF variables again.   4) Another important rule is that no square is left unvisited so we have to exclude the case where there is no knight and the sequence of moves is empty. This will generate a list of N^2-CNF-variables. For constraints 2 and 4, we transform the CNF variables into 3-CNF, where necessary, with Tseytin Transformation.
- Using C# .NET Framework 4.5.2, SAT solver MiniSat 2.2


--- BitStringDictionary.hs
- A dictionary and its methods, created between Char and Bitstring types, using Haskell functions only.
- Using Haskell 2010

