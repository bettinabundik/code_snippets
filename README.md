# code_snippets
Individual files for my code snippets from bigger projects

--- Marker.h, Marker.cpp
- Topic: Computer vision, Image and video processing and analysis
- Problem: Follow or approximate the movements of a human face on every frame of a 2D input video using coloured, circular markers placed on characteristic locations of the face (eyes, forehead, cheeks, mouth)
- Solution: After validating input data, execute a first detection of markers on the first few frames of an input video, and given these detected marker locations, execute an optical flow algorithm for the next input frame, correct results if necessary by checking colours of the pixels or executing detection algorithm again if some points were lost (number of markers on a face is previously given)
- Using C++17 and OpenCV 3.2.0
- Additional runtime files: opencv_world320.dll, opencv_ffmpeg320_64.dll, xml configuration files


--- Program.cs, KnightsModel.cs, BundikBettinaVivien_Assignment3.pdf
- Problem: Solve the knight's tour (Hamiltonian-path problem, NP-Hard) by generating a DIMACS format of CNF file list based on the knight's tour constraint and use a SAT solver to solve the CNF list
- Solution: Details in the pdf
- Using C# .NET Framework 4.5.2, SAT solver MiniSat 2.2
