
// Select which precision that are used in the calculations
#if PRECISION == 32
    #define DATA_TYPE float
#elif PRECISION == 64
    #define DATA_TYPE double
#endif

// ****************************************************************************
// Function: triad
//
// Purpose:
//   A simple vector addition kernel
//   C = A + s*B
//
// Arguments:
//   A,B - input vectors
//   C - output vectors
//   s - scalar
//
// Returns:  nothing
//
// Programmer: Kyle Spafford
// Creation: December 15, 2009
//
// Modifications:
//
// ****************************************************************************
extern "C" __global__ void triad_f(float* A, float* B, float* C, float s, int numberOfElements)
{
    int gid = threadIdx.x + (blockIdx.x * blockDim.x);
    
    // Ensure that the current thread id is less than total number of elements
    if (gid < numberOfElements) {
        C[gid] = A[gid] + s*B[gid];
    }
}

extern "C" __global__ void triad_d(double* A, double* B, double* C, double s, int numberOfElements)
{
    int gid = threadIdx.x + (blockIdx.x * blockDim.x);
    
    // Ensure that the current thread id is less than total number of elements
    if (gid < numberOfElements) {
        C[gid] = A[gid] + s*B[gid];
    }
}

extern "C" __global__ void triad_helper(float* Af, float* Bf, float* Cf, float sf, double* Ad, double* Bd, double* Cd, double sd, int numberOfElements) {
    #if PRECISION == 32
        triad_f(Af, Bf, Cf, sf, numberOfElements);
    #elif PRECISION == 64
        triad_d(Ad, Bd, Cd, sd, numberOfElements);
    #endif
}