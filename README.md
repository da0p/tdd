# Test-Driven Development

A repo created to practice test-driven development following the book TDD With Modern Cpp by Jeff Langr.

# How to use it
- Create a build directory
```
    mkdir build
```

- Install dependencies
```
    conan install conanfile.txt 
        --output-folder=build 
        --build=missing 
        -c tools.cmake.cmaketoolchain:generator=Ninja 
        -s build_type=Debug
```
- Run cmake
```
    cd build
    cmake -GNinja ..
```
- Build examples
```
    ninja
```

# Notes
- The cycle of TDD is to write a small test, ensure it fails, get it to pass, review and clean up the design (including that of the tests), and ensure the tests all still pass. Repeat the cycle throughout the day, keeping it short to maximize the feedback it gives you

- Use NiceMock when needed, but not habitually. Seek to fix your design if requiring it often.