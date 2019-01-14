#include <iostream>

#define CL_TARGET_OPENCL_VERSION 110
#include <CL/cl.hpp>

#include <vector>

int main()
{
	std::cout << "Hello OpenCL" << std::endl;

	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	if(platforms.empty())
	{
		std::cout << "No platforms found. Check OpenCL installation" << std::endl;
	}
    else
    {
        cl::Platform default_platform = platforms.front();
		std::cout << "OpenCL platform: " << default_platform.getInfo<CL_PLATFORM_NAME>() << std::endl;
	}


	//prevent default exit
	int i = 0;
	std::cin >> i;

	return 0;
}