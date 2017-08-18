#include "CL/cl.h"
#include "aopencl.h"
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

using namespace std;

#define N 20	//这里的N是sobel滤波的阈值
#define  KERNEL_SRC "\n" \
	"			__kernel void Sobel(__global char *array1, __global char *array2, __global int *array3)		\n "\
	"			{																							\n "\
	"				size_t gidx = get_global_id(0);															\n "\
	"				size_t gidy = get_global_id(1);															\n "\
	"				unsigned char a00, a01, a02;															\n "\
	"				unsigned char a10, a11, a12;															\n "\
	"				unsigned char a20, a21, a22;															\n "\
	"				int width=array3[0];																	\n "\
	"				int heigh=array3[1];																	\n "\
	"				int widthStep=array3[2];																\n "\
	"				if(gidy>0&&gidy<heigh-1&&gidx>0&&gidx<width-1)											\n "\
	"				{																						\n "\
	"					a00 = array1[gidx-1+widthStep*(gidy-1)];											\n "\
	"					a01 = array1[gidx+widthStep*(gidy-1)];												\n "\
	"					a02 = array1[gidx+1+widthStep*(gidy-1)];											\n "\
	"					a10 = array1[gidx-1+widthStep*gidy];												\n "\
	"					a11 = array1[gidx+widthStep*gidy];													\n "\
	"					a12 = array1[gidx+1+widthStep*gidy];												\n "\
	"					a20 = array1[gidx-1+widthStep*(gidy+1)];											\n "\
	"					a21 = array1[gidx+widthStep*(gidy+1)];												\n "\
	"					a22 = array1[gidx+1+widthStep*(gidy+1)];											\n "\
	"					float ux=a20+2*a21+a22-a00-2*a01-a02;												\n "\
	"					float uy=a02+2*a12+a22-a00-2*a10-a20;												\n "\
	"					//array2[gidx+width*gidy]=sqrt(ux*ux + uy*uy);										\n "\
	"					float u=sqrt(ux*ux + uy*uy);														\n "\
	"					if(u>255) {																			\n "\
	"						u=-1;																			\n "\
	"					} else if(u<20) {																	\n "\
	"						u=0;																			\n "\
	"					}																					\n "\
	"					array2[gidx+widthStep*gidy]=u;														\n "\
	"				}																						\n "\
"}"



int GPU_Sobel()
{
	/*這裡一定要初始化各个函数*/

	initFns();
	/*宿主机变量*/
	cl_uint numPlatforms; //the NO. of platforms
	cl_platform_id platform = NULL; //the chosen platform
	cl_int status;
	cl_platform_id* platforms;
	cl_uint numDevices = 0;
	cl_device_id *devices;
	cl_context context;
	cl_command_queue commandQueue;
	cl_program program;
	cl_kernel kernel;
	//size_t global;
	cl_mem a1, a2, a3;

	int IMG_WIDTH = 400;
	int IMG_HEIGHT = 400;

	long IN_DATA_SIZE = 4 * IMG_HEIGHT;
	long OUT_DATA_SIZE = 4 * IMG_HEIGHT;
	//unsigned char *inputData1;
	//inputData1=(unsigned char *)malloc(DATA_SIZE*sizeof(unsigned char));
	char *inputData1 = new char[400 * 400 * 4];

	//float *outputData;
	//outputData=(float *)malloc(DATA_SIZE*sizeof(float));
	char *outputData = new char[400 * 400 * 4];

	int inputData2[3] = { IMG_WIDTH, IMG_HEIGHT, 4 };

	/*Step1: Getting platforms and choose an available one.*/
	status = clGetPlatformIDs(0, NULL, &numPlatforms);

	/*For clarity, choose the first available platform. */
	if (numPlatforms > 0) {
		platforms = (cl_platform_id*) malloc(
				numPlatforms * sizeof(cl_platform_id));
		status = clGetPlatformIDs(numPlatforms, platforms, NULL);
		platform = platforms[0];
		free(platforms);
	}

	/*Step 2:Query the platform and choose the first GPU device if has one.Otherwise use the CPU as device.*/
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
//	if (numDevices == 0) //no GPU available.
//			{
//		printf("No GPU device available.\n");
//		printf("Choose CPU as default device.\n");
//		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 0, NULL,
//				&numDevices);
//		devices = (cl_device_id*) malloc(numDevices * sizeof(cl_device_id));
//		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, numDevices,
//				devices, NULL);
//	} else {
//		devices = (cl_device_id*) malloc(numDevices * sizeof(cl_device_id));
//		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, numDevices,
//				devices, NULL);
//	}
	devices = (cl_device_id*) malloc(numDevices * sizeof(cl_device_id));
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, numDevices, devices,
			NULL);

	/*Step 3: Create context.*/
	context = clCreateContext(NULL, 1, devices, NULL, NULL, &status);

	/*Step 4: Creating command queue associate with the context.*/
	commandQueue = clCreateCommandQueue(context, devices[0], 0, &status);

	/*Step 5: Create program object */
	const char *source = KERNEL_SRC;
	size_t sourceSize[] = { strlen(source) };
	program = clCreateProgramWithSource(context, 1, &source, sourceSize,
			&status);

	/*Step 6: Build program. */
	status = clBuildProgram(program, 1, devices, NULL, NULL, NULL);

	a1 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
			sizeof(unsigned char) * IN_DATA_SIZE, inputData1, &status);

	a2 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
			sizeof(unsigned char) * OUT_DATA_SIZE, outputData, &status);

	a3 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
			sizeof(int) * 3, inputData2, &status);

	/*Step 8: Create kernel object */
	kernel = clCreateKernel(program, "Sobel", &status);

	// set the argument list for the kernel command
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &a1);
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &a2);
	status = clSetKernelArg(kernel, 2, sizeof(cl_mem), &a3);

	size_t local[] = { 11, 11 };
	size_t global[2];
	global[0] = (
			IMG_WIDTH % local[0] == 0 ?
					IMG_WIDTH : (IMG_WIDTH + local[0] - IMG_WIDTH % local[0]));
	global[1] =
			(IMG_HEIGHT % local[1] == 0 ?
					IMG_HEIGHT : (IMG_HEIGHT + local[1] - IMG_HEIGHT % local[1]));
//	size_t local[] = { 2, 2 };
//		size_t global[]={10,10};

	// enqueue the kernel command for execution
	status = clEnqueueNDRangeKernel(commandQueue, kernel, 2, NULL, global,
			local, 0, NULL, NULL);
	if (status != 0)
		return status;
	clFinish(commandQueue);

	clEnqueueReadBuffer(commandQueue, a2, CL_TRUE, 0,
			sizeof(unsigned char) * OUT_DATA_SIZE, outputData, 0, NULL, NULL);

	clReleaseMemObject(a1);
	clReleaseMemObject(a2);
	clReleaseMemObject(a3);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(commandQueue);
	clReleaseContext(context);

	return 0;
}


