#ifndef KERNEL_H
#define KERNEL_H
#include <QString>
#include <CL/opencl.h>
#include <CL/cl.hpp>
#define MAX_SOURCE_SIZE (0x100000)


class kernel
{
public:
    kernel();
    void Start(), EndWork();
    void GetParam(int **Ar1, int **Ar2);
    bool Initialize(char* file_name, char* func_name);
    int **C;
    const int ROW_SIZE = 1024;
    const int COL_SIZE = 1024;
private:
    int i;
    int **A,**B;
    FILE *fp;
    char *source_str;
    size_t source_size;
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret;
    cl_context context;
    cl_command_queue command_queue;
    cl_mem mem_obj_a;
    cl_mem mem_obj_b;
    cl_mem mem_obj_c;
    cl_program program;
    cl_kernel main_kernel;
    size_t global_item_size;
    size_t local_item_size;
};

#endif // KERNEL_H
