#include "SysInfo.h"

const int SysInfo::getLCpuNum(){
	const std::string target = "processor";
	const std::string cpuInfoPath = "/proc/cpuinfo";
	
	std::ifstream ifile(cpuInfoPath.c_str());
	if(!ifile.is_open()) return -1;
	std::stringstream buffer;  
	buffer << ifile.rdbuf();  
	std::string str(buffer.str());
	std::string temp(str);
	int sum = 0;
	while (1){
 		int loc = temp.find(target);
		if (-1 != loc){
			temp=temp.substr(loc + target.length(), temp.length() - target.length());
            sum++;
		}
        else break;
	}
	return sum;
}

const int SysInfo::getGpuNum(){
  std::string cmd = "num=`nvidia-smi -a|grep \"Attached GPUs\"|awk '{print $NF}'`; echo $num";
  std::string result = getShellResult(cmd);
  const int num = atoi(result.c_str());
  return num;  
}

std::string SysInfo::getShellResult(const std::string & cmd) {
  FILE * fp = NULL;
  char temp[64];
  if ((fp = popen(cmd.c_str(), "r")) != NULL)
  {
    fgets(temp, sizeof(cmd), fp);
    pclose(fp);
  }
  return temp;  
}