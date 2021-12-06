#include <margot/margot.hpp>
#include <iostream>
#include <string>
#include <cuda_runtime_api.h>
#include <math.h>
#include <chrono>
#include <thread>

std::string command_compile(int, int, int, int);

int main()
{
    margot::init();
    int BLOCK_EXP = 0;
    int BLOCK_SIZE = 1 << BLOCK_EXP;
    int CHUNK_EXP = 0;
    int CHUNK_FACTOR = 1 << CHUNK_EXP;
    int TEXTURE_MEMORY_EA1 = 0;
    int TEXTURE_MEMORY_EAA = 0;

    //Input features due to hw spec

    int return_code = system(command_compile(BLOCK_SIZE, CHUNK_FACTOR, TEXTURE_MEMORY_EA1, TEXTURE_MEMORY_EAA).c_str());
    assert(!return_code && "failed starting compilation");

    int repetitions = 100;
    margot::block1::context().manager.wait_for_knowledge(10);
    for(int i = 0; i < repetitions || margot::block1::context().manager.in_design_space_exploration(); i++)
    {
        if(margot::block1::update(BLOCK_SIZE, CHUNK_EXP, TEXTURE_MEMORY_EA1, TEXTURE_MEMORY_EAA))
        {
            CHUNK_FACTOR = 1 << CHUNK_EXP;
            BLOCK_SIZE = 1 << BLOCK_EXP;
            int return_code = system(command_compile(BLOCK_SIZE, CHUNK_FACTOR, TEXTURE_MEMORY_EA1, TEXTURE_MEMORY_EAA).c_str());
            assert(!return_code && "failed mid compilation");
            margot::block1::context().manager.configuration_applied();
            printf("< < < CHANGED VERSION > > >\n");
        }
        margot::block1::start_monitors();
        int return_code = system((char*)"./BFS -s 1");
        assert(!return_code && "failed execution");
        margot::block1::stop_monitors();
        margot::block1::push_custom_monitor_values();
        margot::block1::log();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

std::string command_compile(int BLOCK_SIZE,
                            int CHUNK_FACTOR,
                            int TEXTURE_MEMORY_EA1,
                            int TEXTURE_MEMORY_EAA)
{
    std::string start_path = "../../../src/programs";
    std::string start_compile = "nvcc -I " + start_path + "/common/ -I " + start_path +"/cuda-common/ -g -O2 -c -o Graph.o " + start_path + "/common/Graph.cpp \n";
    start_compile += "nvcc -I " + start_path + "/common/ -I " + start_path + "/cuda-common/ -g -O2 -c -o main.o " + start_path + "/cuda-common/main.cpp \n";
    std::string end_compile = "nvcc -L " + start_path + "/cuda-common -L " + start_path + "/common -o BFS Graph.o main.o BFS.o -lSHOCCommon";
    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, 0);
    std::string cc = std::to_string(prop.major) + std::to_string(prop.minor);
    std::string program = "nvcc -gencode=arch=compute_" + cc + ",code=sm_" + cc + " -I " + start_path + "/common -I " + start_path + "/cuda-common/ -g -O2 -c " + start_path + "/bfs/BFS.cu";
    program += " -DTEXTURE_MEMORY_EA1=" + std::to_string(TEXTURE_MEMORY_EA1);
    program += " -DTEXTURE_MEMORY_EAA=" + std::to_string(TEXTURE_MEMORY_EAA);
    program += " -DCHUNK_FACTOR=" + std::to_string(CHUNK_FACTOR);
    program += " -DBLOCK_SIZE=" + std::to_string(BLOCK_SIZE) + " \n";
    std::cout << start_compile + program + end_compile << std::endl;
    return start_compile + program + end_compile;
}
