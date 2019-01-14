#include <iostream>

#include <CL/cl.hpp>

#include <vector>

int main()
{
    std::cout << "Hello OpenCL" << std::endl;

    // Get all platforms (drivers)
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    if (platforms.empty())
    {
        std::cout << "No platforms found. Check OpenCL installation" << std::endl;
        return 1;
    }

    cl::Platform default_platform = platforms.front();
    std::cout << "OpenCL platform: " << default_platform.getInfo<CL_PLATFORM_NAME>() << std::endl;

    // Get all devices of the default platform
    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);

    if (all_devices.empty())
    {
        std::cout << "No devices found. Check OpenCL installation" << std::endl;
        return 1;
    }

    cl::Device default_device = all_devices.front();
    std::cout << "OpenCL device: " << default_device.getInfo<CL_DEVICE_NAME>() << std::endl;
	std::cout << "Device cores: " << default_device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << std::endl; 

    cl::Context context({ default_device });
    cl::Program::Sources src;

    std::string kernel_code
        = "   void kernel simple_add(global const int* A, global const int* B, global int* C){       "
          "       C[get_global_id(0)]=A[get_global_id(0)]+B[get_global_id(0)];                 "
          "   }                 ";


    

    src.push_back({ kernel_code.c_str(), kernel_code.length() });
    cl::Program program(context, src);

    if (program.build({ default_device }) != CL_SUCCESS)
    {
        std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << std::endl;
        return 1;
    }

    // create buffers on the device

    cl::Buffer buffer_A(context, CL_MEM_READ_WRITE, sizeof(int) * 10);
    cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, sizeof(int) * 10);
    cl::Buffer buffer_C(context, CL_MEM_READ_WRITE, sizeof(int) * 10);

	//Arrays on host which will be copied to device
    int A[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int B[] = { 0, 1, 2, 0, 1, 2, 0, 1, 2, 0 };
    
    // Create queue to which we will push commands for the devices
	cl::CommandQueue queue(context, default_device);

	// Copy data from A to buffer_A and B to buffer_B (host to devices)
	queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof(int)*10, A);
	queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, sizeof(int)*10, B);

	//cl::detail::KernelFunctorGlobal< simple_add(cl::Kernel(program, "simple_add"), queue, cl::NullRange, cl::NDRange(19), cl::NullRange);

	cl::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer> func(cl::Kernel(program, "simple_add"));
	cl::EnqueueArgs eargs(queue, cl::NullRange, cl::NDRange(10), cl::NullRange);
	func(eargs, buffer_A, buffer_B, buffer_C).wait();

	int C[10];

	queue.enqueueReadBuffer(buffer_C,CL_TRUE,0,sizeof(int)*10,C);
    

	
	    //std::cout << " result: \n";
     //   for (int i = 0; i < 10; i++)
     //   {
     //       std::cout << C[i] << " ";
     //   }

	
    return 0;
}