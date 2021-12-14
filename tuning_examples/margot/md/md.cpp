#include <margot/margot.hpp>
#include <iostream>
#include <string>
#include <cuda_runtime_api.h>
#include <math.h>
#include <chrono>
#include <thread>

std::string command_compile(int, std::string, int, int);

int main()
{
    margot::init();
    int BLOCK_SIZE = 1;
    int PRECISION_BITS = 32;
    std::string PRECISION = (PRECISION_BITS == 32 ? "float" : "double");
    int TEXTURE_MEMORY = 0;
    int WORK_PER_THREAD = 1;

    //Input features due to hw spec

    int return_code = system(command_compile(BLOCK_SIZE, PRECISION, TEXTURE_MEMORY, WORK_PER_THREAD).c_str());
    assert(!return_code && "failed starting compilation");

    int repetitions = 100;
    margot::block1::context().manager.wait_for_knowledge(10);
    for(int i = 0; i < repetitions || margot::block1::context().manager.in_design_space_exploration(); i++)
    {
        if(margot::block1::update(BLOCK_SIZE, PRECISION_BITS, TEXTURE_MEMORY, WORK_PER_THREAD))
        {
            PRECISION = (PRECISION_BITS == 32 ? "float" : "double");
            int return_code = system(command_compile(BLOCK_SIZE, PRECISION, TEXTURE_MEMORY, WORK_PER_THREAD).c_str());
            assert(!return_code && "failed mid compilation");
            margot::block1::context().manager.configuration_applied();
            printf("< < < CHANGED VERSION > > >\n");
        }
        margot::block1::start_monitors();
        int return_code = system((char*)"./MD -s 1");
        assert(!return_code && "failed execution");
        margot::block1::stop_monitors();
        margot::block1::push_custom_monitor_values();
        margot::block1::log();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

std::string command_compile(int BLOCK_SIZE,
                            std::string PRECISION,
                            int TEXTURE_MEMORY,
                            int WORK_PER_THREAD)
{
    std::string start_path = "../../../src/programs";
    std::string start_compile = "nvcc -I " + start_path + "/common/ -I " + start_path + "/cuda-common/ -g -O2 -c -o main.o " + start_path + "/cuda-common/main.cpp \n";
    std::string end_compile = "nvcc -L " + start_path + "/cuda-common -L " + start_path + "/common -o MD main.o md.o -lSHOCCommon";
    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, 0);
    std::string cc = std::to_string(prop.major) + std::to_string(prop.minor);
    std::string program = "nvcc -gencode=arch=compute_" + cc + ",code=sm_" + cc + " -I " + start_path + "/common -I " + start_path + "/cuda-common/ -g -O2 -c " + start_path + "/md/md.cu";
    program += " -DTEXTURE_MEMORY=" + std::to_string(TEXTURE_MEMORY);
    program += " -DWORK_PER_THREAD=" + std::to_string(WORK_PER_THREAD);
    program += " -DPRECISION=" + PRECISION;
    program += " -DBLOCK_SIZE=" + std::to_string(BLOCK_SIZE) + " \n";
    std::cout << start_compile + program + end_compile << std::endl;
    return start_compile + program + end_compile;
}
