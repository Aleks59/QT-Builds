__kernel void vector_add(__global const int *A, __global const int *B, __global int *C)
{
    int num = get_global_id(0);
    C[i] = A[i]+B[i];
}
