#include "kernel.h"

kernel::kernel()
{

}

void kernel::GetParam(int **Ar1, int **Ar2)
{    
    A = new int*[ROW_SIZE];
    B = new int*[ROW_SIZE];

     for(int i=0;i<ROW_SIZE;i++)
     {
         A[i]=new int[COL_SIZE];
         B[i]=new int[COL_SIZE];
         for(int j =0;j<COL_SIZE;j++)
         {
             A[i][j]=Ar1[i][j];
             B[i][j]=Ar2[i][j];
         }
     }
}

bool kernel::Initialize(char* file_name, char* func_name)
{
    fp = fopen(file_name, "r");
        if (!fp) {
            fprintf(stderr, "Failed to load kernel.\n");
            return 0;
            exit(1);
        }

    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);
    // Get platform and device information
    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
    //create an OpenCL context
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    //create a command queue
    command_queue = clCreateCommandQueue(context, device_id, 0,&ret);
    //create a memory buffers on the device for each vector
    mem_obj_a = clCreateBuffer(context, CL_MEM_READ_ONLY, ROW_SIZE*COL_SIZE*sizeof(int), NULL, &ret);
    mem_obj_b = clCreateBuffer(context, CL_MEM_READ_ONLY, ROW_SIZE*COL_SIZE*sizeof(int), NULL, &ret);
    mem_obj_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY, ROW_SIZE*COL_SIZE*sizeof(int), NULL, &ret);
    //copy the lists A and B to ther resprective memory buffers
    ret = clEnqueueWriteBuffer(command_queue, mem_obj_a, CL_TRUE, 0, ROW_SIZE*COL_SIZE*sizeof(int), A, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, mem_obj_b, CL_TRUE, 0, ROW_SIZE*COL_SIZE*sizeof(int), B, 0, NULL, NULL);
    //create a program from the kernel source
    program = clCreateProgramWithSource(context,1,(const char **)&source_str, (const size_t*)&source_size,&ret);
    //build the program
    ret = clBuildProgram(program, 1, &device_id, NULL,NULL, NULL);
    if (ret == CL_BUILD_PROGRAM_FAILURE) {
        // Determine the size of the log
        size_t log_size;
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

        // Allocate memory for the log
        char *log = (char *) malloc(log_size);

        // Get the log
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

        // Print the log
        printf("%s\n", log);
    }

    //create the OpenCL kernel
    main_kernel = clCreateKernel(program, func_name, &ret);


    //set the arguments of the kernel
    ret = clSetKernelArg(main_kernel, 0, sizeof(cl_mem),(void*)&mem_obj_a);
    ret = clSetKernelArg(main_kernel, 1, sizeof(cl_mem),(void*)&mem_obj_b);
    ret = clSetKernelArg(main_kernel, 2, sizeof(cl_mem),(void*)&mem_obj_c);
    //ret = clSetKernelArg(main_kernel, 3, sizeof())

    //execture the OpenCL kernel on the list
    global_item_size = ROW_SIZE*COL_SIZE;//process the entire lists
    local_item_size = 64;//divide work items into groups of 64
    return 1;

}

void kernel::Start()
{
    ret = clEnqueueNDRangeKernel(command_queue, main_kernel, 1, NULL,&global_item_size, &local_item_size,0,NULL,NULL);

    //read the memory buffer C on the device to the local variable C
    //C = (int*)malloc(sizeof(int)*ROW_SIZE*COL_SIZE);

    C = new int*[ROW_SIZE];
    for(int j = 0;j<ROW_SIZE;j++)
    {
        C[i] = new int[COL_SIZE];
    }
    ret = clEnqueueReadBuffer(command_queue, mem_obj_c, CL_TRUE,0,ROW_SIZE*COL_SIZE*sizeof(int),C,0,NULL,NULL);
}

void kernel::EndWork()
{
    //clean up
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(main_kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(mem_obj_a);
    ret = clReleaseMemObject(mem_obj_b);
    ret = clReleaseMemObject(mem_obj_c);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    //free(A);
    //free(B);
    //free(C);
    delete A;
    delete B;
    delete C;
}
